#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <errno.h>

#include <sqlite3.h>

#define RUNS 1000

int callback_result;
static int callback(void *unused, int argc, char **argv, char **cols)
{
	int x;
	for(x = 0;x < argc;x++)
	{
		if(!strncmp(cols[x], "result", 6))
		{
			callback_result = atoi(argv[x]);
			printf("Result: %d\n", callback_result);
			return 0;
		}
	}

	callback_result = 0;
        return 0;
}

uint64_t convert_to_u64(char* str)
{
	uint64_t result = 0;

	int x;
	for(x = 0;x < strlen(str);x++)
	{
		int val;
		if(str[x] >= '0' && str[x] <= '9')
		{
			val = str[x] - '0';
			result <<= 4;
			result |= val;
		} else if(str[x] >= 'a' && str[x] <= 'f')
		{
			val = str[x] - 'a' + 10;
			result <<= 4;
			result |= val;
		} else if(str[x] >= 'A' && str[x] <= 'F')
		{
			val = str[x] - 'A' + 10;
			result <<= 4;
			result |= val;
		} else {
			return result;
		}
	}

	return result;
}

int estimate_bits_of_entropy(char* program, uint64_t* map)
{
	char result[128];
	uint64_t orig = 0;
	uint64_t bitmap = 0;
	int x;
	for(x = 0;x < RUNS;x++)
	{
		memset(result, 0, 128);
		int fds[2];
		if(pipe(fds))
		{
			printf("PIPE FAILED\n");
			return 0;
		}

		int pid = fork();

		if(pid > 0)
		{
			/* Parent function */
			close(fds[1]); /* Close the write end */

			/* Wait for the process to finish */
			waitpid(pid, NULL, 0);
			if(read(fds[0], result, 128) <= 0)
				return -1;

			/* Convert to long int */
			uint64_t generated = convert_to_u64(result + 2);
			close(fds[0]);

			/* Has orig been assigned? */
			if(!orig)
			{
				orig = generated;
				continue;
			}

			/* How many bits have changed? */
			uint64_t diff = generated ^ orig;
			bitmap |= diff;
		} else if(pid == 0)
		{
			close(fds[0]); /* Close the read end */
			close(1); /* Close stdout */
			dup2(fds[1], 1); /* Bind stdout */

			/* Child function */
			char* arguments[2];
			arguments[0] = program;
			arguments[1] = NULL;

			execv(program, arguments);
			printf("EXECVE FAILED!\n");
			return 0;
		} else {
			printf("FORK FAILED!\n");
			return 0;
		}
	}

	/* The experiment is done, how many bits have changed? */
	if(map) *map = bitmap;

	int bits = 0;
	while(bitmap)
	{
		if(bitmap & 0x01)
			bits++;
		bitmap >>= 1;
	}

	return bits;
}

int get_bits_of_entropy(char* program, uint64_t* map, 
		uint64_t* distinct, uint64_t* possible)
{
	/* Precalculate the amount of bits we need: */
	int bits =  estimate_bits_of_entropy(program, map);
	if(possible) *possible = 1 << bits;

	sqlite3* db = NULL;
	char* err_msg = NULL;
	char* insert_str = "INSERT INTO addrs VALUES (%llu);";

	char db_name[512];
	strncpy(db_name, program, 512);
	strncat(db_name, ".sql", 512);
	printf("Opening database %s\t\t\t\t\t\t", db_name);
	if(sqlite3_open_v2("entropy4-1.sql", &db,
                                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                NULL) != SQLITE_OK)
        {
                printf("[FAIL]\n");
                return -1;
	}
	printf("[ OK ]\n");

	printf("Dropping table...\t\t\t\t\t\t\t");
	const char* drop_str = "DROP TABLE IF EXISTS addrs";
	if(sqlite3_exec(db, drop_str, callback, 0, &err_msg)
			!= SQLITE_OK)
	{
		printf("[FAIL]\n");
		return -1;
	}
	printf("[ OK ]\n");

	printf("Creating table...\t\t\t\t\t\t\t");
	const char* create_str = "CREATE TABLE addrs(addr UNSIGNED BIGINT);";
        if(sqlite3_exec(db, create_str, callback, 0, &err_msg)
                        != SQLITE_OK)
        {
                printf("[FAIL]\n");
                return -1;
        }
	printf("[ OK ]\n");

	int spawn_count = 8;
	uint64_t runs = (*possible << 2) / spawn_count;
	int warn = ((runs * spawn_count) >> (30 - 4));
	int progress = 0;

	if(warn)
	{
		printf("Warning: This test will use %uGB of disk space.\n",
				warn);
		progress = 1;
	}
	char result[128];
	uint64_t x;
	for(x = 0;x < runs;x++)
	{
		if(x && progress && (x % 10240 == 0))
		{
			int percent = (x * 100) / runs;
			printf("[%02d%%] Run %ld out of %ld\n",
					percent, x, runs);
		}

		int pids[spawn_count];
		int pipes[spawn_count][2];
		int spawn;
		for(spawn = 0;spawn < spawn_count;spawn++)
		{
			printf("Running.\n");
			if(pipe(pipes[spawn_count]))
			{
				printf("PIPE FAILED\n");
				return 0;
			}
			int pid = fork();

			if(pid == 0)
			{
				printf("Child\n");
				/* Close the read end */
				close(pipes[spawn_count][0]);
				close(1); /* Close stdout */
				/* Bind stdout */
				dup2(pipes[spawn_count][1], 1);

				/* Child function */
				char* arguments[2];
				arguments[0] = program;
				arguments[1] = NULL;

				execv(program, arguments);
				printf("EXECVE FAILED!\n");
				return 0;
			} else if(pid > 0)
			{
				printf("Parent\n");
				pids[spawn] = pid;
				close(pipes[spawn][1]);
			} else {
				printf("FORK FAILED!\n");
				return 0;
			}
		}

		/* Collect the results */
		for(spawn = 0;spawn < spawn_count;spawn++)
		{
			/* Parent function */
			/* Wait for the process to finish */
			waitpid(pids[spawn], NULL, 0);
			memset(result, 0, 128);
			if(read(pipes[spawn][0], result, 128) <= 0)
			{
				printf("READ FAILED!\n");
				return -1;
			}

			/* Convert to long int */
			uint64_t generated = convert_to_u64(result + 2);
			close(pipes[spawn][0]);

			char query[512];
			snprintf(query, 512, insert_str, generated);

			if(sqlite3_exec(db, query, callback, 0,
						&err_msg)
					!= SQLITE_OK)
				printf("Couldn't add to db!\n");
		}
	}

	/* How many distnct addresses did we see? */

	char* select_query = "SELECT COUNT(DISTINCT addr) "
		"AS result FROM addrs;";
	if(sqlite3_exec(db, select_query, callback, 0,
				&err_msg)
			!= SQLITE_OK)
		printf("SELECT QUERY FAILED!\n");

	if(distinct) *distinct = callback_result;

	sqlite3_close(db);

	return bits;
}

void print_analysis(uint64_t bitmap, uint64_t distinct, uint64_t possible)
{
	char map[65];
	map[64] = 0;
	int x;
	for(x = 0;x < 64;x++)
		map[x] = ((bitmap >> x) & 0x01) + '0';

	int set = 0;
	char measure[65];
	measure[64] = 0;
	for(x = 0;x < 64;x++)
	{
		if(map[x] == '1' && !set)
		{
			set = 1;
			measure[x] = '|';
		} else if(map[x] == '1' && set)
		{
			measure[x] = '-';
		} else if(map[x] == '0' && set)
		{
			measure[x - 1] = '|';
			measure[x] = ' ';
			set = 0;
		} else {
			measure[x] = ' ';
		}
	}

	unsigned int tmp = distinct - 1;
	int pow_2 = 0;
	while(tmp)
	{
		tmp <<= 1;
		pow_2++;
	}
	printf("Patterns encountered: %ld\n", distinct);
	printf("Highest power of 2:   %d\n", pow_2);
	printf("Free bits:\n");
	printf("%s\n", measure);
	printf("%s\n", map);
	printf("LSB                                                     "
			"     MSB\n");
}

int main(int argc, char** argv)
{
	uint64_t map;
	uint64_t distinct;
	uint64_t possible;
	int stack_bits = get_bits_of_entropy("./entropy1", &map, 
			&distinct, &possible);
	printf("Estimated stack bits: %d\n", stack_bits);
	print_analysis(map, distinct, possible);
	int heap_bits = get_bits_of_entropy("./entropy2", &map,
			&distinct, &possible);
	printf("Estimated heap bits: %d\n", heap_bits);
	print_analysis(map, distinct, possible);
	int code_bits = get_bits_of_entropy("./entropy3", &map,
			&distinct, &possible);
	printf("Estimated code bits: %d\n", code_bits);
	print_analysis(map, distinct, possible);

	return 0;
}
