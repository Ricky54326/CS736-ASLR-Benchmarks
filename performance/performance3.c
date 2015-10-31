/**
 * Authors:
 * 	John Detter <jdetter@wisc.edu>
 * 	Riccardo Mutschlechner <riccardo@cs.wisc.edu> 
 */

#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

#include "bench.h"
	
/* Page size on x86 is 4096 */
#define PGSIZE 0x1000

int main(int argc, char** argv)
{
	ull min = (ull)-1;

	int x;
	for(x = 0;x < 1000000;x++)
	{
		/* Page size on x86 is 4096 */

		ull diff;

		int fds[2];
		if(pipe(fds))
		{
			printf("Pipe failure!\n");
			return -1;
		}

		int pid = fork();

		if(pid == 0)
		{
			if(dup2(fds[1], 1) == -1)
			{
				printf("Dup2 failure!\n");
				return -1;	
			}

			char* new_argv[2] = {"./performance3_helper", NULL};
			execve(new_argv[0], new_argv);
			printf("Execve failed!\n");
			return -1;
		} else {
			read(fds[0], &diff, sizeof(ull));
			int ret_val;
			waitpid(pid, &ret_val, 0);

			if(ret_val)
			{
				printf("Some error occurred.\n");
				return -1;
			}


			if(diff < min) 
				min = diff;

			close(fds[0]);
			close(fds[1]);
		}
	}

	int file = open(OUTPUT_FILE, O_RDWR | O_CREAT | O_APPEND, 0664);
	if(file < 0)
	{
		printf("Open failed!\n");
		return -1;
	}

	char num[512];
	snprintf(num, 512, "%llu\n", min);
	int written = write(file, num, strlen(num));
	if(written != strlen(num))
	{
		printf("Write failed!\n");
		return -1;
	}

	return 0;
}
