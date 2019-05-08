#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<ctype.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#ifndef buffer_size
#define buffer_size 40 //declared buffer size macro = 40 throughout the code
#endif

void main()
{
	char buffer[40];
	int j=1;
	ssize_t x;
	char *y = (char *) malloc(40); //allocating memory to each entry in the buffer with size 40 

	while((x = read(0, buffer, buffer_size)) > 0) //reading from buffer 
	{
		if(x != 40)
		{
			continue; //unless the command line receives correct input format(size 40), it will continue to go to the next line
		}
		else
		{            //Below snippet will check the input for the correct format as per the guidelines of the question in the assignment
		if (((buffer[0] == 'D') || (buffer[0] == 'C') || (buffer[0] == 'X')) && (buffer[1] 	== ',') && (buffer[12] == ',') && (buffer[23] == ',') && (buffer[29] == ',') &&	(buffer[26] == 	':') && (buffer[15] == '/') && ( buffer[18] == '/' ) 
		&& isdigit(buffer[13]) && isdigit(buffer[14]) && isdigit(buffer[16]) && isdigit	(buffer[17]) && isdigit(buffer[19]) && isdigit(buffer[20]) 
		&& isdigit(buffer[21]) && isdigit(buffer[22]) && isdigit(buffer[24]) && isdigit (buffer[25]) && isdigit(buffer[27]) && isdigit(buffer[28]))
		{
				j = j+1;
				y = strcat(y, buffer); //appends string pointed by buffer to the end of string pointed by b
				y= realloc(y, j*(40)); //reallocating memory of size (j*40)		
		}
		else
			{
			continue;
			}
		} 
	}
	for (int i =1 ; i<=((j-1)*40); i++)
	{
		printf("%c", y[i-1] );	//printing the input to standard out
		if(i%40 == 0)
			{	
				printf("\n");	//after every line reaches 40 characters, a new line will be inserted 
				
			}
	}

}
