#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "bench.h"

/*
	Benchmark for ASLR performance across HardenedBSD and FreeBSD
	This benchmark specifically measures process fork/creation time.

	@author John Detter <jdetter@wisc.edu>
	@author Riccardo Mutschlechner <riccardo@cs.wisc.edu>
*/


unsigned long long start;
unsigned long long end;

uint high;
uint low;

int main(int argc, char *argv[])
{
	ull diff;
	ull best = (unsigned long)-1;
	uint val;

	int x;
	for(x = 0; x < 1000000; x++)
	// for(x = 0; x < 10; x++)
	{
		int to_child[2]; /* Create a pipe to tell child the start */
		if(pipe(to_child))
		{
			printf("PIPE FAILED (probably out of fds)\n");
			return -1;
		}

		int back[2]; /* Get pipe for child's result */
		if(pipe(back))
		{
			printf("START PIPE FAILED\n");
			return -1;
		}	

		/* Start the timer */
		RDTSCP(start);
		int pid = fork();

		if(pid == 0)
		{
			/* Close the read end of the write pipe */
			close(back[0]);
			/* Close write end of child pipe */
			close(to_child[1]);

			close(0);
			close(1);

			/* Our input comes from this pipe */
			if(dup2(to_child[0], 0) != 0)
			{
				fprintf(stderr, "dup failed!\n");
				exit(5);
			}

			/* Bind the pipe */
			if(dup2(back[1], 1) != 1)
			{
				perror("Error: ");
				fprintf(stderr, "dup failed! %d\n", back[1]);
				exit(7);
			}

			char* args[2];
			args[0] = "./performance1_helper";
			args[1] = NULL;

			execv(args[0], args);

			fprintf(stderr, "Failed to run helper program!!\n");
			exit(2);
		} else if(pid > 0)
		{
			/* Close write end of the result pipe */	
			close(back[1]);

			/* Close read end of write pipe */
			close(to_child[0]);

			/* Give the program the start value */
			if(write(to_child[1], &start, sizeof(ull)) != sizeof(ull))
			{
				printf("Write failed!\n");
				exit(1);
			}

			int status = 0;
			/* Wait for the child to finish */
			if(waitpid(-1, &status, 0) != pid)
			{
				printf("Child failed: %d\n", status);
				return 0;
			}

			/* Read the response */
			int rd = read(back[0], &diff, sizeof(ull));
			if(rd != sizeof(ull))
			{
				printf("Read failed! %d\n", rd);
				return -1;
			}

			/* Was the last run significant?  */
			if(diff < best)
				best = diff;

			/* That run went okay, lets do another. */
		} else {
			printf("FORK FAILURE!\n");
			exit(1);
		}

		/* SAFETY: clean up the file descriptors */
		close(back[0]);
		close(to_child[1]);
	}

	int file = open(OUTPUT_FILE, O_RDWR | O_CREAT | O_APPEND, 0664);
	if(file < 0)
	{
		printf("Open failed!\n");
		return -1;
	}

	char num[512];
	snprintf(num, 512, "%llu\n", best);
	int written = write(file, num, strlen(num));
	if(written != strlen(num))
	{
		printf("Write failed!\n");
		return -1;
	}

	return 0;
}
