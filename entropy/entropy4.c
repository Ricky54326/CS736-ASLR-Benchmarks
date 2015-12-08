#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

#include <sqlite3.h>

/* TUNABLES */
#define MAX_QUERY 	(8192 << 2)
#define BATCH_SIZE 	0x1F0
#define MAX_THREADS 	4
#define DB_COUNT	1

/* Set this if you want a custom amount of runs done */
#define SET_RUNS 10000

#define CBLACK  "\x1B[0m"
#define CGREEN  "\x1B[32m"
#define CBLUE   "\x1B[34m"

int callback_result;
static int callback(void *unused, int argc, char **argv, char **cols)
{
	int x;
	for(x = 0;x < argc;x++)
	{
		if(!strncmp(cols[x], "result", 6))
		{
			callback_result = atoi(argv[x]);
			// printf("Result: %d\n", callback_result);
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
	for(x = 0;x < 1000;x++)
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

void execute_insert(uint64_t* vals, int count, sqlite3* db)
{
	char* err_msg = NULL;
	char* query = malloc(MAX_QUERY);
	strncpy(query, "INSERT INTO \'addrs\' (addr) ", MAX_QUERY);

	int x;
	for(x = 0;x < count;x++)
	{
		char sub_query[128];
		if(x == 0)
		{
			snprintf(sub_query, 128, 
				"SELECT \'0x%llx\' AS \'addr\' ",
				vals[0]);
		} else {
			snprintf(sub_query, 128, 
                                "UNION ALL SELECT \'0x%llx\' ",
                                vals[x]);
		}

		strncat(query, sub_query, MAX_QUERY - 1);
	}

	if(strlen(query) >= MAX_QUERY - 10)
		printf("INCREASE MAX QUERY\n");

	/* Execute the insert */
	if(sqlite3_exec(db, query, callback, 0,
				&err_msg)
			!= SQLITE_OK)
	{
		printf("Couldn't add to db! "
				"\n%s\n%s\n",
				query,
				err_msg);
	}

	free(query);
}

/**
 * Run the program and put the output into the given database.
 * Returns 0 on success.
 */
int _run_test(char* program, sqlite3* db, uint64_t iterations, int prog)
{
	/* Where the result of the program will go */
	char result[128];
	memset(result, 0, 128);

	/* String for insert query */
	char* err_msg = NULL;
	uint64_t vals[BATCH_SIZE];

	uint64_t x;
	for(x = 0;x < iterations;x++)
	{
		if(prog && (x % (BATCH_SIZE / 2)) == 0)
		{
			int percent = (x * 100) / iterations;
			printf("["CGREEN"%2d"CBLACK"%%] Run "
					CBLUE"%ld"CBLACK
					" out of %ld",
					percent, x, iterations);
			printf("\r");
		}

		int fds[2]; /* Pipe pair */
		if(pipe(fds))
		{
			printf("PIPE FAILED\n");
			return 0;
		}

		int pid = fork();

		if(pid > 0)
		{
			/* Parent process */
			close(fds[1]); /* Close the write end */

			/* Wait for the process to finish */
			waitpid(pid, NULL, 0);
			memset(result, 0, 128);
			if(read(fds[0], result, 128) <= 0)
			{
				printf("READ FAILED!\n");
				return -1;
			}

			close(fds[0]); /* Close the read end */

			/* Convert to long int */
			uint64_t generated = convert_to_u64(result + 2);

			/* Insert this value into our batch */
			vals[x % BATCH_SIZE] = generated;

			if(x && (x % BATCH_SIZE == 0))
				execute_insert(vals, BATCH_SIZE, db);
		} else if(pid == 0)
		{
			/* Close the read end */
			close(fds[0]);
			close(1); /* Close stdout */

			/* Bind stdout to write end */
			dup2(fds[1], 1);

			/* Child function */
			char* arguments[2];
			arguments[0] = program;
			arguments[1] = NULL;

			/* Run the program */
			execv(program, arguments);
			printf("EXECVE FAILED!\n");
			return -1;
		} else {
			printf("FORK FAILED!\n");
			return -1;
		}
	}

	return 0; /* SUCCESS */
}

struct run_test_params
{
	char* program; 
	sqlite3* db; 
	uint64_t iterations;
	int prog;
};

void* run_test(void* args)
{
	struct run_test_params* p = args;

	_run_test(p->program, p->db, p->iterations, p->prog);

	free(args);
	return NULL;
}

sqlite3* setup_database(char* name, int verbose)
{
	sqlite3* db = NULL;
        char* err_msg = NULL;
        char* insert_str = "INSERT INTO addrs VALUES (%llu);";

        if(verbose) printf("Opening database %s\t\t\t\t\t", name);
        if(sqlite3_open_v2(name, &db,
                                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                NULL) != SQLITE_OK)
        {
                if(verbose) printf("[FAIL]\n");
                return NULL;
        }
        if(verbose) printf("[ OK ]\n");

        if(verbose) printf("Dropping table...\t\t\t\t\t\t\t");
        const char* drop_str = "DROP TABLE IF EXISTS addrs";
        if(sqlite3_exec(db, drop_str, callback, 0, &err_msg)
                        != SQLITE_OK)
        {
                if(verbose) printf("[FAIL]\n");
                return NULL;
        }
        if(verbose) printf("[ OK ]\n");

	if(verbose) printf("Creating table...\t\t\t\t\t\t\t");
        const char* create_str = "CREATE TABLE addrs(addr UNSIGNED BIGINT);";
        if(sqlite3_exec(db, create_str, callback, 0, &err_msg)
                        != SQLITE_OK)
        {
                if(verbose) printf("[FAIL]\n");
                return NULL;
        }
        if(verbose) printf("[ OK ]\n");

	return db;
}

int get_bits_of_entropy(char* program, uint64_t* map, 
		uint64_t* distinct, uint64_t* possible)
{
	/* Precalculate the amount of bits we need: */
	int bits =  estimate_bits_of_entropy(program, map);
	printf("Estimated bits: %d\n", bits);
	if(possible) *possible = 1 << bits;

	sqlite3* databases[DB_COUNT];
	char* db_names[DB_COUNT];

	printf("Opening databases...\t\t\t\t\t\t\t");
	/* Initilize the databases */
	int db;
	for(db = 0;db < DB_COUNT;db++)
	{
		char* db_name = malloc(128);
		char num[16];
		strncpy(db_name, program, 128);
		snprintf(num, 16, "%d", (db + 1));
		strncat(db_name, "-", 128);
		strncat(db_name, num, 128);
		strncat(db_name, ".sql", 128);

		databases[db] = setup_database(db_name, 0);

		if(!databases[db])
		{
			printf("[FAIL]\n");
			return -1;
		}
		db_names[db] = db_name;
	}

	printf("[ OK ]\n");

	/* How many threads should we run at once? */
	int thread_count = MAX_THREADS;
	uint64_t runs = (*possible);

	runs >>= 10;

	int gigs = ((runs) >> (30 - sizeof(uint64_t)));
	runs /= thread_count;

#ifdef SET_RUNS
	runs = SET_RUNS;
#endif

	printf("Possible values: 0x%lx\n", *possible);
	printf("Runs:            0x%lx\n", runs);

	if(gigs)
	{
		printf("Warning: This test will use %uGB of disk space.\n",
				gigs);
	}

	printf("Running with %d threads max\n", thread_count);

	/* Spawn all of the threads. */
	pthread_t threads[thread_count];
	int x;
	for(x = 0;x < thread_count;x++)
	{
		struct run_test_params* p = malloc(
				sizeof(struct run_test_params));
		p->program = program;
		p->db = databases[x % DB_COUNT];
		p->iterations = runs;
		p->prog = x == 0;

		if(pthread_create(&threads[x], NULL, run_test, (void*)p))
		{
			printf("Failed to create thread!\n");
			return -1;
		}

	}

	/* Wait for all of the threads */
	for(x = 0;x < thread_count;x++)
		pthread_join(threads[x], NULL);

	printf(CGREEN"\nRuns Complete!\n"CBLACK);


	printf("Merging all databases...\n");
	char* err_msg = NULL;

	for(x = 1;x < DB_COUNT;x++)
	{
		sqlite3* db = databases[0];
		/* Close other database */
		sqlite3_close(databases[x]);
		databases[x] = NULL;

		char query[512];
		/* Attach this Database */
		char* attach = "ATTACH DATABASE \'%s\' AS data%d;";
		snprintf(query, 512, attach, db_names[x], x);

		if(sqlite3_exec(db, query, callback, 0, &err_msg)
				!= SQLITE_OK)
		{
			printf("Couldn't attach to db! "
					"\n%s\n%s\n",
					query,
					err_msg);
			continue;
		}

		char* insert = "INSERT INTO addrs (addr) "
				"SELECT addr FROM data%d.addrs;";
		snprintf(query, 512, insert, x);

		if(sqlite3_exec(db, query, callback, 0, &err_msg)
                                != SQLITE_OK)
                {
                        printf("Couldn't insert into db! "
                                        "\n%s\n%s\n",
                                        query,
                                        err_msg);
                        continue;
                }

		char* detach = "DETACH DATABASE data%d;";
                snprintf(query, 512, detach, x);

                if(sqlite3_exec(db, query, callback, 0, &err_msg)
                                != SQLITE_OK)
                {
                        printf("Couldn't detach db! "
                                        "\n%s\n%s\n",
                                        query,
                                        err_msg);
                        continue;
                }

		unlink(db_names[x]);
	}

	/* How many distnct addresses did we see? */
	char* select_query = "SELECT COUNT(DISTINCT addr) "
		"AS result FROM addrs;";
	if(sqlite3_exec(databases[0], select_query, callback, 0,
				&err_msg)
			!= SQLITE_OK)
		printf("SELECT QUERY FAILED!\n");
	sqlite3_close(databases[0]);

	if(distinct) *distinct = callback_result;

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

	uint64_t tmp = distinct - 1;
	int pow_2 = 0;
	while(tmp)
	{
		tmp >>= 1;
		pow_2++;
	}
	printf("Patterns encountered: %ld\n", distinct);
	printf("Highest power of 2:   %d\n", pow_2);
	printf("Free bits:\n");
	printf("%s\n", measure);
	printf("%s\n", map);
	printf("LSB                                                     "
			"     MSB\n\n");
}

int main(int argc, char** argv)
{
	uint64_t map;
	uint64_t distinct;
	uint64_t possible;
	int start;
	printf("****** STACK ANALYSIS ******\n");
	start = time(NULL);
	int stack_bits = get_bits_of_entropy("./entropy1", &map, 
			&distinct, &possible);
	printf("Elapsed time: %d seconds\n", time(NULL) - start);
	print_analysis(map, distinct, possible);
	printf("****** HEAP ANALYSIS ******\n");
	start = time(NULL);
	int heap_bits = get_bits_of_entropy("./entropy2", &map,
			&distinct, &possible);
	printf("Elapsed time: %d seconds\n", time(NULL) - start);
	print_analysis(map, distinct, possible);
	printf("****** CODE ANALYSIS ******\n");
	start = time(NULL);
	int code_bits = get_bits_of_entropy("./entropy3", &map,
			&distinct, &possible);
	printf("Elapsed time: %d seconds\n", time(NULL) - start);
	print_analysis(map, distinct, possible);

	return 0;
}
