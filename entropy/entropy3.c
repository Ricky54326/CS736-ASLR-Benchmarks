/** 
 * Code entropy test
 *
 * Authors: John Detter <jdetter@wisc.edu>
 * 		Riccardo Mutschlechner <riccardo@cs.wisc.edu>
 */

#include <unistd.h>
#include <stdio.h>
#include <elf.h>

int main(char** argv, int argc)
{
	/* We are considering the start of the stack the address of argv */
	unsigned long code_start = (unsigned long)main;

	printf("0x%lx\n", code_start);

	return 0;
}
