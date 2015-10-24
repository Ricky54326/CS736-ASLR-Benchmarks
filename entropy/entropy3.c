/** 
 * Stack entropy test
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
	unsigned long stack_start = (unsigned long)argv;

	int fd = open("./entropy3");
	if(fd < 0)
	{
		printf("Open failed!\n");
		return -1;
	}

	Elf64_Ehdr header;

	if(read(fd, &header, sizeof(Elf64_Ehdr)) !=
		sizeof(Elf64_Ehdr))
	{
		printf("Read failure!\n");
		return -1;
	}	

	printf("0x%x\n", stack_start);

	return 0;
}
