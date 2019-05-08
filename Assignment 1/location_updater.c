#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<ctype.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#ifndef buff_size
#define buff_size 40
#endif

int main()
	{
		int pipefd[2];
		pid_t x,y;
		pipe(pipefd);                                   //creating pipe
		x = fork();                                     //creating child process 1
	
		if(x == -1)                                     //checking if child process 1 has been created or not (not created in this case)
		{
			printf("\n Child process not created"); 
		}
		else if (x == 0)                               //checking if child process 1 has been created or not (created in this case)
		{
			printf("\n Inside Child process 1");
			close(pipefd[0]);                          //closing read end of pipe
			dup2(pipefd[1], 1);
			close(pipefd[1]);                          //closing write end of pipe
			execl("./email_filter.out","./email_filter.out",(char *) NULL);
		}
		else if ( x > 0)
		{
			y = fork();                                //creating child process 2
			if (y == -1)                               //checking if child process 2 has been created or not (not created in this case)
				{
					printf("Child process not created");
				}
			else if (y == 0)                           //checking if child process 2 has been created or not (created in this case)
				{
					printf("\n Inside Child process 2");
					close(pipefd[1]);                  //closing write end of pipe
					dup2(pipefd[0], 0);
					close(pipefd[0]);                  //closing read end of pipe
					execl("./calender_filter.out"	,"./calender_filter.out",(char *) NULL);
				}
			else if (y > 0)
				{
					close(pipefd[0]);                  //closing read end of pipe
					close(pipefd[1]);                  //closing read end of pipe
					printf("\n Inside parent process");
					wait(NULL);
					wait(NULL);
				}
		}
	return 0;
}
