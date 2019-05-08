//Name : Kshitij Raj
//UFID : 13584965

#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<fcntl.h>
#include<stdlib.h>
#include<ctype.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/wait.h>

struct iteration 
				{
					char iteration_name[10];
					char iteration_date[10];
					int x_count;
				};
struct node
					{
					pthread_mutex_t lock;
					pthread_cond_t empty,full;
					char **buff;
					int in,buf_out,buff_size,buffer_filled,value,valid_value,calender_count,counter,date_counter,time_count,count,flag;
					};

int main(int argc, char*argv[])
				{
					struct node * nodeobj;
					//Creating shared region using mmap
					nodeobj = (struct node *) mmap(NULL, sizeof(struct node), PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS , -1 , 0);
					nodeobj->buff_size = atoi(argv[1]);
					nodeobj->in =0;
					nodeobj->buf_out =-1;
					nodeobj->value =0;
					nodeobj->valid_value=0;
					nodeobj->counter =0;
					nodeobj->date_counter=0;
					nodeobj->count=0;
					nodeobj->flag=0;
					nodeobj->time_count=0;
					nodeobj->calender_count=0;
					nodeobj->buffer_filled =0;
					nodeobj->buff = (char **) malloc(sizeof(char*) * nodeobj->buff_size);

					for(int i =0; i<nodeobj->buff_size; i++)
						{
							*(nodeobj->buff+i)= (char*)mmap(NULL, 50*sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
							**(nodeobj->buff+i) = '\0';
						}

pthread_mutexattr_t psharedm;
pthread_condattr_t psharedf;
//pthread_mutexattr_setpshared(&psharedm, PTHREAD_PROCESS_SHARED);
pthread_condattr_t psharede;
pthread_mutexattr_init(&psharedm);
pthread_mutexattr_setpshared(&psharedm, PTHREAD_PROCESS_SHARED);
pthread_mutex_init(&nodeobj->lock, &psharedm);
//pthread_mutex_init(&nodeobj->lock, &psharedf);
pthread_condattr_init(&psharedf);
pthread_condattr_init(&psharede);
//pthread_mutexattr_init(&psharedm);
//pthread_mutexattr_setpshared(&psharedm, PTHREAD_PROCESS_SHARED);
pthread_condattr_setpshared(&psharedf, PTHREAD_PROCESS_SHARED);
pthread_condattr_setpshared(&psharede, PTHREAD_PROCESS_SHARED);
pthread_cond_init(&nodeobj->empty, &psharedf);
pthread_cond_init(&nodeobj->full, &psharedf);
pid_t a,b;
a = fork();   //Creating the first child
if(a == -1)
	{
		printf("\nCannot create child process");
	}
else if (a == 0) 
	{
		int x =0;
		char *line;
		char buf[49];
		FILE * fp , * fp1;
		fp = fopen(argv[2], "r");
		fp1 = fopen(argv[2], "r");
		int value =0;
		int handle, sign;
		size_t len =0;
		while((handle= getline(&line, &len, fp)) != -1)
			{
					nodeobj->counter += 1;
			}
		while((sign= getline(&line, &len, fp1)) != -1)
        {
					for(int q =0; q< 49; q++)
						{
								buf[q] = line[q];
						}
				nodeobj->value += 1; 
				nodeobj->flag=0;                //email filter input format entry check below
				if (((buf[9] == 'D') || (buf[9] == 'C') || (buf[9] == 'X')) && (buf[10] == ',') && (buf[21] == ',') && (buf[32] == ',') &&
        (buf[38] == ',') && (buf[35] == ':') && (buf[24] == '/') && ( buf[27] == '/' ) && isdigit(buf[22]) && isdigit(buf[23])
        && isdigit(buf[25]) &&     isdigit    (buf[26]) && isdigit(buf[28]) && isdigit(buf[29]) && isdigit(buf[30]) && isdigit(buf[31]) && isdigit(buf[33]) && isdigit(buf[34]) && isdigit(buf[36]) && isdigit(buf[37]) && (buf[0]=='S') && (buf[1]=='u') && (buf[2]=='b')
        && (buf[3]=='j') && (buf[4]=='e') && (buf[5]=='c') && (buf[6]=='t') && (buf[7]==':') && (buf[8]=' '))
        {	
					nodeobj->valid_value++;
					pthread_mutex_lock(&(nodeobj->lock));
							while((nodeobj->buffer_filled) == (nodeobj->buff_size))
									{
											pthread_cond_wait(&(nodeobj->full),&(nodeobj->lock) );
									}
							for(int k =0; k<49; k++)
									{	
											nodeobj->buff[nodeobj->in][k] = buf[k];
									}
							nodeobj->in = (nodeobj->in + 1) % (nodeobj->buff_size);
							nodeobj->buffer_filled += 1;
							if((nodeobj->buffer_filled) == 1)
									{
										pthread_cond_signal(&(nodeobj->empty));
									}
							pthread_mutex_unlock(&(nodeobj->lock));
				}

		}
	nodeobj->flag=1;
}

else if ( a > 0)
{
	//wait(NULL);	  
	b = fork();   //Creating the second child
	  if (b == -1)
			{
				printf("Cannot create second child");
			}
	  else if (b == 0)
			{
					struct iteration iteration_arr[nodeobj->counter];
					int s=0;
					char buf[49];
					int k=0;
					char **arr;
					char **date_array;
					int a;
					int value=0;
					for(int i =0; i<nodeobj->counter; i++)
					{
						date_array[i]=(char*)malloc(10*sizeof(char));
					}
					int dateflag=0, datecount=0,date_counter=0;
					int flag=1;
					int i,j,flag1,flag2;
					int var1[2],var2[2],var3[2],var4[2],var5[2],var6[2],var7[2],var8[2]; 
					char inp_buffer2[10],inp_buffer3[10],inp_buffer4[10];
					char str[28],name[10],date[10],tag[27];
	while(1)
	{
			pthread_mutex_lock(&(nodeobj->lock));
			while((nodeobj->buffer_filled) == 0)
				{
					pthread_cond_wait(&(nodeobj->empty),&(nodeobj->lock) );
				}
			nodeobj->buf_out = (nodeobj->buf_out + 1) % (nodeobj->buff_size);
			for(int k =0; k<49; k++)
					{	
							buf[k]=nodeobj->buff[nodeobj->buf_out][k];
					}	
			nodeobj->buffer_filled -= 1;
			nodeobj->calender_count++;
		if(buf[9] == 'C')	
		{
		nodeobj->count=nodeobj->count+1;
		if(nodeobj->count==1) 
			{
				for(i=0;i<10;i++)
					{	
						name[i]=buf[i+11];
					}
				for(i=0;i<10;i++)
						date[i]=buf[i+22];
			iteration_arr[nodeobj->count-1].x_count=0;
			for(i=0;i<10;i++)
				{
					if(isalpha(buf[i+11])!=0)			
						iteration_arr[nodeobj->count-1].x_count++;			
					name[i]=buf[i+11];
			}
		
		for(i=0;i<iteration_arr[nodeobj->count-1].x_count;i++)
			iteration_arr[nodeobj->count-1].iteration_name[i]=name[i];
		for(i=0;i<10;i++)
			iteration_arr[nodeobj->count-1].iteration_date[i]=date[i];
		for(i=0;i<27;i++)
			tag[i]=buf[i+22];		
		}

	else
		{			
			iteration_arr[nodeobj->count-1].x_count=0;
			for(i=0;i<10;i++)
			{
			if(isalpha(buf[i+11])!=0)			
				iteration_arr[nodeobj->count-1].x_count++;			
			name[i]=buf[i+11];
			}	
			for(i=0;i<10;i++)
			date[i]=buf[i+22];
		for(i=0;i<iteration_arr[nodeobj->count-1].x_count;i++)
			iteration_arr[nodeobj->count-1].iteration_name[i]=name[i];
		for(i=0;i<10;i++)
			iteration_arr[nodeobj->count-1].iteration_date[i]=date[i];
		for(i=0;i<2;i++)   //MOnth Comparison
			{
				var1[i]=(int)(tag[i]-'0');
				var2[i]=(int)(buf[i+22]-'0');

			}
			if((var1[0]==var2[0])&&(var1[1]==var2[1]))
				flag=1;
			else
			{
				if(var1[0]>var2[0])
					flag=2;
				else if(var1[0]==var2[0])
					{
					if(var1[1]>var2[1])
					flag=2;
					else
					flag=3;
					}
				else
					flag=3;
				}

		if(flag==1)
			{
			for(i=0;i<2;i++)
				{
					var3[i]=(int)(tag[i+3]-'0');
					var4[i]=(int)(buf[i+25]-'0');
			}

			if((var3[0]==var4[0])&&(var3[1]==var4[1]))
				flag1=1;
			
			else
			{
				if(var3[0]>var4[0])
					flag1=2;
				else if(var3[0]==var4[0])
					{
					if(var3[1]>var4[1])
					flag1=2;
					else
					flag1=3;
					}
				else
					flag1=3;
				}
			
			if(flag1==1)
			{
				
				for(i=0;i<2;i++)
				{
					var5[i]=(int)(tag[i+11]-'0');
					var6[i]=(int)(buf[i+33]-'0');
				}
				
				if((var5[0]==var6[0])&&(var5[1]==var6[1]))
				{
					
					for(i=0;i<2;i++)
				{
					var7[i]=(int)(tag[i+14]-'0');
					var8[i]=(int)(buf[i+36]-'0');
				}
				
				if((var7[0]==var8[0])&&(var7[1]==var8[1]))
					{	
					}	
				
				else
				{
				if(var7[0]>var8[0])
					{
				      for(i=0;i<27;i++)
					  {
						tag[i]=buf[i+22];
					  }
					}	
				
			else if(var7[0]==var8[0])
					{
					if(var7[1]>var8[1])
					{
				      for(i=0;i<27;i++)
					  {
						tag[i]=buf[i+22];
					  }
					}					
				}	
			}
		}
			else
			{
				if(var5[0]>var6[0])
					{
				      for(i=0;i<27;i++)
					  {
						tag[i]=buf[i+22];
					  }
			}	
				else if(var5[0]==var6[0])
					{
					if(var5[1]>var6[1])
					{
				      for(i=0;i<27;i++)
					  {
						tag[i]=buf[i+22];
					  }
					}	
					else
						{}
					}
				else
					{}
				}
			}
			else 
			{
			if(nodeobj->time_count==0)
			{
			for(i=0;i<27;i++)
			printf("%c",tag[i]);
			printf("\n");
		}

			nodeobj->time_count=0;
		
				for(i=0;i<27;i++)
				{
					tag[i]=buf[i+22];
				}
			}
		}
	else
	{
	if(nodeobj->time_count==0)
	{for(i=0;i<27;i++)
		printf("%c",tag[i]);
	printf("\n");
		}

		nodeobj->time_count=0;		
			for(i=0;i<27;i++)
				{
				tag[i]=buf[i+22];
				}
		}
	}
}


	else if(buf[9]=='D')
		{			
		nodeobj->date_counter=nodeobj->date_counter+1;
		if(nodeobj->count==1)
	{
		for(i=0;i<27;i++)
			printf("%c",tag[i]);
		printf("\n");
		nodeobj->time_count=1;
		for(i=0;i<10;i++)             //Printing for Delete case event
			inp_buffer4[i]=date[i];		
			inp_buffer4[10]=',';			
			inp_buffer4[11]='-';
			inp_buffer4[12]='-';
			inp_buffer4[13]=':';
			inp_buffer4[14]='-';
			inp_buffer4[15]='-';
			inp_buffer4[16]=',';
			inp_buffer4[17]='N';
			inp_buffer4[18]='A';
			for(i=0;i<8;i++)
			inp_buffer4[i+19]=' ';
	for(i=0;i<27;i++)
		printf("%c",inp_buffer4[i]);
	printf("\n");
	}
		else
		{		
		for(i=0;i<nodeobj->count;i++)
		{
		for(j=0;j<10;j++)
		{
			if(buf[j+22]== iteration_arr[i].iteration_date[j])
				dateflag=1;		
			else
			{	
				dateflag=0;			
				break;				
			}
		}		
	if(dateflag==1)
	datecount++;
}
		if(nodeobj->date_counter==datecount)
			{
			nodeobj->time_count=1;
		datecount=0;
		date_counter=0;
		//row++;			
		for(i=0;i<10;i++)
		
				inp_buffer4[i]=iteration_arr[nodeobj->count-1].iteration_date[i];		
		
			inp_buffer4[10]=',';
			inp_buffer4[11]='-';
			inp_buffer4[12]='-';
			inp_buffer4[13]=':';
			inp_buffer4[14]='-';
			inp_buffer4[15]='-';
			inp_buffer4[16]=',';
			inp_buffer4[17]='N';
			inp_buffer4[18]='A';
			for(i=0;i<8;i++)
			inp_buffer4[i+19]=' ';
		for(i=0;i<27;i++)
			printf("%c",inp_buffer4[i]);
		printf("\n");
		}
	else
		{
		nodeobj->time_count=0;		
		}			
		datecount=0;
		} 
	}
	else if(buf[9]=='X')
	{			
		nodeobj->time_count=1;
		for(i=0;i<27;i++)
			printf("%c",tag[i]);
		printf("\n");
		for(i=0;i<27;i++)
			str[i]=buf[i+22];
	str[27]='\n';		
	if(nodeobj->count==1)
		{

	for(i=0;i<27;i++)
			printf("%c",str[i]);
		printf("\n");		
	}
		
		else
		{
		for(i=1;i<nodeobj->count;i++)
		{
			for(j=0;j<iteration_arr[i].x_count;j++)
		{
			if(buf[j+11]==iteration_arr[i].iteration_name[j])
				flag2=1;		
			else
				{
				flag2=0;
        break;				
				}			
				}
		if(flag2==1)
		{
		
		for(i=0;i<27;i++)
			printf("%c",str[i]);
		printf("\n");		
		break;
		}
		}
	}			
}
if((nodeobj->buffer_filled) == ((nodeobj->buff_size) -1))
		pthread_cond_signal(&(nodeobj->full));

	pthread_mutex_unlock(&(nodeobj->lock));
	
if(nodeobj->calender_count==nodeobj->valid_value)
	break;
}
if(nodeobj->time_count==0)
{
for(i=0;i<27;i++)
	printf("%c",tag[i]);
printf("\n");
}
	}
	else if (b > 0)
	{
		wait(NULL);
		wait(NULL);
	}
}
/*if(nodeobj->time_count==0)
{
for(i=0;i<27;i++)
	printf("%c",tag[i]);
	wait(NULL);*/
return 0;
//wait(NULL);;
}

