#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <errno.h>

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

int get_bits_of_entropy(char* program)
{
	char result[128];
	uint64_t orig = 0;
	uint64_t bitmap = 0;
	int x;
	for(x = 0;x < 1000000;x++)
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

	int bits = 0;
	while(bitmap)
	{
		if(bitmap & 0x01)
			bits++;
		bitmap >>= 1;
	}

	return bits;
}

int main(int argc, char** argv)
{
	int stack_bits = get_bits_of_entropy("./entropy1");
	printf("Stack bits: %d\n", stack_bits);
	int heap_bits = get_bits_of_entropy("./entropy2");
	printf("Heap bits: %d\n", heap_bits);
	int code_bits = get_bits_of_entropy("./entropy3");
	printf("Code bits: %d\n", code_bits);

	return 0;
}
