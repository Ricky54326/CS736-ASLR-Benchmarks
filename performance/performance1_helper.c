#include "bench.h"
#define PGSIZE 0x1000

void sys_write(long int fd, void* buffer, long int size);
void sys_read(long int fd, void* buffer, long int size);
void sys_exit(unsigned long int);

ull end;
ull start;
ull diff;

void __memmove(void* dst, const void* src, unsigned int s)
{
	int x;
	for(x = 0;x < s;x++)
		((char*)dst)[x] = ((char*)src)[x];
}

int _start(char** arg, int argc)
{
	RDTSCP(end);

	sys_write(1, &end, sizeof(ull));

	sys_exit(0);
	
	return 0;
}
