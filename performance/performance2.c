#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "bench.h"

ull start;
ull end;

void *pthread_func(void *argument){
	RDTSC(end); //get the cycle counter right away here
	pthread_exit(NULL);
	return NULL;
}

void time_pthread(){
	pthread_t thr;

	uint val;
	ull high;
	ull low;
	unsigned long diff;
	unsigned long best = (unsigned long) -1;

	int x;
	for(x = 0;x < 10000;x++){
		/* Timing section */
		RDTSC(start);
		pthread_create(&thr, NULL, pthread_func, NULL);	
		pthread_join(thr, (void**)NULL);
		diff = end - start;

		printf("%d\n", start);
		printf("%d\n", end)

		/* End section */
		if(diff < best) best = diff;
	}

	diff = best;
	int file = open("output.txt", O_APPEND | O_RDWR | O_CREAT, 0644);

	if(file < 0) {
		printf("BAD FILE!\n");
		exit(EXIT_FAILURE);
	}

	char numbuffer[512];
	snprintf(numbuffer, 512, "%lu\n", diff);
	write(file, numbuffer, strlen(numbuffer));
	close(file);
}

int main(int argc, char *argv[])
{
	time_pthread();
	return 0;

}
