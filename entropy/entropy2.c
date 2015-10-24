/** 
 * Stack entropy test
 *
 * Authors: John Detter <jdetter@wisc.edu>
 * 		Riccardo Mutschlechner <riccardo@cs.wisc.edu>
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(char** argv, int argc)
{
	/* We are considering the start of the stack the address of argv */
	unsigned long heap_start = (unsigned long)malloc(sizeof(void*));
	printf("0x%lx\n", heap_start);

	return 0;
}
