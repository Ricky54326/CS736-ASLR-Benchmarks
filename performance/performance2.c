#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "bench.h"

/*
	Benchmark for ASLR performance across HardenedBSD and FreeBSD
	This benchmark specifically measures pthread creation time.

	@author John Detter <jdetter@wisc.edu>
	@author Riccardo Mutschlechner <riccardo@cs.wisc.edu>
*/



unsigned long long start;
unsigned long long end;

uint high;
uint low;

void *pthread_func(void *argument){
	RDTSC(end); //get the cycle counter right away here
	pthread_exit(NULL);
	return NULL;
}

void time_pthread(){
	pthread_t thr;

	uint val;
	unsigned long diff;
	unsigned long best = (unsigned long) -1;

	int x;

	for(x = 0;x < 10000;x++){
		/* Timing section */
		RDTSC(start);
		pthread_create(&thr, NULL, pthread_func, NULL);	
		pthread_join(thr, (void **)NULL);
		diff = end - start;

		/* End section */
		if(diff < best) best = diff;
	}

	diff = best;
	printf("%lu\n", diff);

	int file = open(CHILD_FILE, O_APPEND | O_RDWR | O_CREAT, 0644);

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
