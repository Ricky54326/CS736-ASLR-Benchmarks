#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

#include "bench.h"
#define PGSIZE 0x1000

int main(int argc, char** arv)
{
	ull start;
	ull end;
	ull diff;

	RDTSCP(start);
	void* ptr = mmap(NULL, PGSIZE, PROT_READ | PROT_WRITE,
                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	RDTSCP(end);

	diff = end - start;
	write(1, &diff, sizeof(ull));

	return 0;
}
