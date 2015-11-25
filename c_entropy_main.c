#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>



/*
	Main benchmark loader (in C versus Python)

	@author John Detter <jdetter@wisc.edu>
	@author Riccardo Mutschlechner <riccardo@cs.wisc.edu>
*/



int main(int argc, char *argv[])
{

	int x;
	for(x = 0; x < 1000000; x++)
	{

		int pid = fork(); /** << This is what we are measuring */

		if(pid == 0)
		{
			//child
			char *file = "./entropy1";
			char *args[2];
			args[0] = file;
			args[1] = NULL;

		  	execvp(file, args); 	
			exit(0);
			return -1;
		} 

		else if(pid > 0)
		{
			//parent
			
		}
	}
	
	long long unsigned int best = 0;
	#define OUTPUT_FILE "test.out.ignore"

        int file = open(OUTPUT_FILE, O_RDWR | O_CREAT | O_APPEND, 0664);
        if(file < 0)
        {
                printf("Open failed!\n");
                return -1;
        }

        char num[512];
        snprintf(num, 512, "%llu\n", best);
        int written = write(file, num, strlen(num));
        if(written != strlen(num))
        {
                printf("Write failed!\n");
                return -1;
        }

	return 0;
}
