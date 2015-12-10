#include "bench.h"

int main(char** arg, int argc)
{
	ull end;
	RDTSCP(end);

	write(1, &end, sizeof(end));

	return 0;
}
