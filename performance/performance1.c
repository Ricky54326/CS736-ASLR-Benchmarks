#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "bench.h"

/*
	Benchmark for ASLR performance across HardenedBSD and FreeBSD
	This benchmark specifically measures process fork/creation time.

	@author John Detter <jdetter@wisc.edu>
	@author Riccardo Mutschlechner <riccardo@cs.wisc.edu>
*/


unsigned long long start;
unsigned long long end;

uint high;
uint low;

void *pthread_func(void *argument){
	RDTSC(end);
	return NULL;
}

void time_proc(){
	pthread_t thr;

	unsigned long diff;
	unsigned long best = (unsigned long)-1;
	uint val;

	int x;
	 // for(x = 0; x < 100; x++)
	 // {
		RDTSC(start);

		//child
		if(fork() == 0){
			//write the end time to a file
			RDTSC(end);
			int file = open(CHILD_FILE, O_TRUNC | O_RDWR | O_CREAT, 0644);
			
			if(file < 0){
				printf("BAD FILE!\n");
				exit(EXIT_FAILURE);
			}

			//@TODO: Write RDTSC val to file.
			int childFile = open(CHILD_FILE, O_RDWR, 0644);

			if(childFile < 0){
				printf("BAD FILE!\n");
				exit(EXIT_FAILURE);
			}

			char *buf = malloc(512);
			snprintf(buf, 512, "%llu", end);

			write(childFile, buf, 512);
			
			exit(EXIT_SUCCESS);
		}

		//parent
		else{
			wait();
		}

	 // }


	//read val from child file
	int childFile = open(CHILD_FILE, O_RDONLY, 0644);
	if(childFile < 0){
		printf("BAD FILE!\n");
		exit(EXIT_FAILURE);
	}

	char *buf = malloc(512);
	read(childFile, buf, 512);
	end = strtoul(buf, NULL, 10);

	printf("%llu\n", end-start);

	int file = open(PARENT_FILE, O_TRUNC | O_RDWR | O_CREAT, 0644);
	
	if(file < 0){
		printf("BAD FILE!\n");
		exit(EXIT_FAILURE);
	}

	// diff = end-start;
	// snprintf(numbuffer, 512, "%lu\n", diff);
	// write(file, numbuffer, strlen(numbuffer));
	close(file);

	return;
}

int main(int argc, char *argv[])
{
	time_proc();
	return 0;
}
