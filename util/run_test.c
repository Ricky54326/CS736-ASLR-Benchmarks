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
	char* program_name = NULL;
	if(argc != 2)
	{
		printf("run_test <program>\n");
		return -1;
	}

	program_name = argv[1];
	char output_file[512];
	strncpy(output_file, program_name, 512);
	strncat(output_file, "-output.txt", 512);	

	unlink(output_file);

	int x;
	for(x = 0; x < 10000; x++)
	{
		int pid = fork();

		if(pid == 0)
		{
			int fd = open(output_file, 
				O_RDWR | O_CREAT | O_APPEND, 0644);
			/* Bind the file */
			dup2(fd, 1);
			char *args[2];
			args[0] = program_name;
			args[1] = NULL;

		  	execvp(program_name, args); 	
			printf("run_test: EXEC FAILED\n");
			return -1;
		} else if(pid > 0)
		{
			//parent
			if(waitpid(pid, NULL, 0) != pid)
			{
				printf("run_test: WAITPID FAILED!\n");
				return -1;
			}
		} else {
			printf("run_test: FORK FAILED!\n");
			return -1; 
		}
	}
	
	return 0;
}
