/** 
 * Stack entropy test
 *
 * Authors: John Detter <jdetter@wisc.edu>
 * 		Riccardo Mutschlechner <riccardo@cs.wisc.edu>
 */

#include <unistd.h>
#include <stdio.h>

int main(char** argv, int argc)
{
	/* We are considering the start of the stack the address of argv */
	unsigned long stack_start = (unsigned long)&argv;
	printf("0x%lx\n", stack_start);

	return 0;
}
