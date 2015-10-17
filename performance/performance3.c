/**
 * Authors:
 * 	John Detter <jdetter@wisc.edu>
 * 	Riccardo Mutschlechner <riccardo@cs.wisc.edu> 
 */

#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>

#include "bench.h"
	
/* Page size on x86 is 4096 */
#define PGSIZE 0x1000

int main(int argc, char** argv)
{

	int x;
	for(x = 0;x < 10000;x++)
	{
		/* Page size on x86 is 4096 */
		void* ptr = mmap(NULL, PGSIZE, PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

		if(!ptr)
		{
			printf("MMAP failed!\n");
			return -1;
		}
	}

	return 0;
}
