#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "bench.h"

/*
	Benchmark for ASLR performance across HardenedBSD and FreeBSD
	This benchmark specifically measures process fork/creation time.

	@author John Detter <jdetter@wisc.edu>
	@author Riccardo Mutschlechner <riccardo@cs.wisc.edu>
*/


ull* start;
ull end;
uint high;
uint low;

int main(int argc, char *argv[])
{
	ull diff;
	ull best = (unsigned long)-1;
	uint val;

	/* Create a shared mapping for the start value */
	void* mapping = mmap(NULL, 4096, PROT_READ | PROT_WRITE,
		MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	if(!mapping)
	{
		printf("Shared mapping failed!\n");
		return -1;
	}
	start = mapping;

	int x;
	// for(x = 0; x < 10; x++)
	for(x = 0; x < 1000000; x++)
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

			/* Start the timer */
			RDTSCP(*start);
			execv(args[0], args);

			fprintf(stderr, "Failed to run helper program!!\n");
			exit(2);
		} else if(pid > 0)
		{
			/* Close write end of the result pipe */	
			close(back[1]);

			/* Close read end of write pipe */
			close(to_child[0]);

			int status = 0;
			/* Wait for the child to finish */
			if(waitpid(-1, &status, 0) != pid)
			{
				printf("Child failed: %d\n", status);
				return 0;
			}

			/* Read end the response */
			int rd = read(back[0], &end, sizeof(ull));
			if(rd != sizeof(ull))
			{
				printf("Read failed! %d\n", rd);
				return -1;
			}

			diff = end - *start;

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
