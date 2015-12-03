#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bench.h"

void sys_write(long int, void* src, long int sz);

int main(int argc, char** argv)
{
	/* Get the input */

	ull start;
	ull diff = 0;
	RDTSCP(start);

	int to_child[2];
	int back[2];

	if(pipe(to_child) || pipe(back))
	{
		printf("Failed!\n");
		return -1;
	}

	int pid = fork();	

	if(pid == 0)
	{
		close(0);
		close(1);

		dup2(to_child[0], 0);
		dup2(back[1], 1);

		char* argv[2];

		argv[0] = "./performance1_helper";
		argv[1] = NULL;

		execv(argv[0], argv);
		fprintf(stderr, "Failed to exec!\n");
		exit(1);
	} else if(pid > 0)
	{
		close(to_child[0]);
		close(back[1]);

		/* Give the child the input */
		write(to_child[1], &start, sizeof(ull));

		/* Get the output */
		read(back[0], &diff, sizeof(ull));

		printf("Diff: %llu\n", diff);
	} else {
		printf("Fork failed!\n");
		return -1;
	}

	return 0;
}
