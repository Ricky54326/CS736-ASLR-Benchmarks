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
	{
		/* NOTE: this pipe should not impact performance */
		int fds[2]; /* Create a pipe for IPC */
		if(pipe(fds))
		{
			printf("PIPE FAILED (probably out of fds)\n");
			return -1;
		}

		RDTSCP(start); /* Start the timer */
		int pid = fork(); /** << This is what we are measuring */

		if(pid == 0)
		{
			//child
			RDTSCP(end); /* End the timer */

			/* Close the read end of the pipe */
			close(fds[0]);

			/* Calculate the entire fork time */
			diff = end - start;

			/* Send back to parent */
			int wr = write(fds[1], &diff, sizeof(ull));
			if(wr != sizeof(ull))
			{
				printf("PIPE WRITE FAILED!\n");
				return -1;
			}

			/* Return success for child */
			exit(0);
			return -1;
		} else if(pid > 0)
		{
			/* Close write end of the pipe */	
			close(fds[1]);
			
			/* Read the response */
			int rd = read(fds[0], &diff, sizeof(ull));
			if(rd != sizeof(ull))
			{
				printf("Read failed!\n");
				return -1;
			}

			int st; /* status of the returning child */
			if(waitpid(pid, &st, NULL) != pid)
			{
				printf("WAITPID failed!\n");
				return -1;
			}

			/* Was the return status okay? */
			if(st)
			{
				printf("Child returned failure.\n");
				return -1;
			}

			/* Was the last run significant?  */
			if(diff < best)
				best = diff;

			/* That run went okay, lets do another. */
		} else {
			printf("FORK FAILURE!\n");
			return -1;
		}

		/* SAFETY: clean up the file descriptors */
		close(fds[0]);
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
