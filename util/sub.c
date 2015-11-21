#include <stdio.h>

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		printf("USAGE: sub num1 num2\n");
		return -1;
	}

	int num1 = atoi(argv[1]);
	int num2 = atoi(argv[2]);

	printf("%d", num1 - num2);

	return 0;
}
