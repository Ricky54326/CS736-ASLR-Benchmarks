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
	for(x = 0;x < 100000;x++)
	{
		/* Page size on x86 is 4096 */
		ull start;
		ull end;
		ull diff;
		RDTSCP(start);
		void* ptr = mmap(NULL, PGSIZE, PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		RDTSCP(end);

		if(!ptr)
		{
			printf("MMAP failed!\n");
			return -1;
		}

		diff = end - start;
		if(diff < min) 
			min = diff;
	}

	int file = open(OUTPUT_FILE, O_RDWR | O_CREAT | O_APPEND, 0664);
	if(file < 0)
	{
		printf("Open failed!\n");
		return -1;
	}

	char num[512];
	snprintf(num, 512, "%d\n", min);
	int written = write(file, num, strlen(num));
	if(written != strlen(num))
	{
		printf("Write failed!\n");
		return -1;
	}

	return 0;
}
