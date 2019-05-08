#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<ctype.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#ifndef Buffer_size
#define Buffer_size 40
#endif

void main()
{
ssize_t a;
char buf[Buffer_size];

int var1[2];
int var2[2];
int var3[2];
int var4[2];
int var5[2];
int var6[2];
int r =0;
int total=0;
int x_total=0;
int t_total=0;
char **arr = malloc(sizeof(*arr));
char early[40];
char buffer2[40];
char buffer3[40];

int flag;
int ack1;
int ack2;
int i,j;

char temp[10];
int m;

typedef struct event
	{ 
		char event_name[10];
		char time[5];
		char date[10];
		char location[10];
	} 
event_t;
event_t *event_arr;
event_t tag;
event_t ignore;
event_arr= malloc(sizeof(event_t));

	while((a = read(0, buf, Buffer_size)) != 0)
		{
			if(a != 40)
			{
				continue;
			}
			else
		{
		r=r+1;
		arr = realloc(arr,r* sizeof(*arr));
		arr[r-1]=malloc(40*sizeof(char*));
		if(buf[0] == 'C')
	{
		total=total+1;
		event_arr=realloc(event_arr,total*sizeof(event_t));
		for(i=0;i<10;i++)
		event_arr[total-1].event_name[i]=buf[i+2];			
		for(i=0;i<10;i++)
		event_arr[total-1].date[i]=buf[i+13];		
		for(i=0;i<5;i++)
		event_arr[total-1].time[i]=buf[i+24];
		for(i=0;i<10;i++)		
		event_arr[total-1].location[i]=buf[i+30];

		if(total==1) 
			{
				strcpy(arr[r-1],buf);
				tag=event_arr[total-1];			
			}
		else
			{
				for(i=0;i<2;i++)
				{
					var1[i]=(int)(tag.date[i+3]-'0');
					var2[i]=(int)(event_arr[total-1].date[i+3]-'0');
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
					var3[i]=(int)(tag.time[i]-'0');
					var4[i]=(int)(event_arr[total-1].time[i]-'0');
				}
			if((var3[0]==var4[0])&&(var3[1]==var4[1]))
				{
				for(i=0;i<2;i++)
				{
					var5[i]=(int)(tag.time[i+3]-'0');
					var6[i]=(int)(event_arr[total-1].time[i+3]-'0');
				}
				if(var5[0]>var6[0])
					ack1=2;
				else if(var5[0]==var6[0])
					{
					if(var5[1]>var5[1])
					ack1=2;
					else
					ack1=3;
					}
				else
					ack1=3;
				}
			else
				{
				if(var3[0]>var4[0])
					ack1=2;
				else if(var3[0]==var4[0])
					{
					if(var3[1]>var4[1])
					ack1=2;
					else
					ack1=3;
					}
				else
					ack1=3;
				}
				if(ack1==2)
				{ 
				strcpy(arr[x_total+t_total],buf);
				tag=event_arr[total-1];				
				}
			else if(ack1==3)
			{
					buffer2[0]='C';
					buffer2[1]=',';
				for(i=0;i<10;i++)		
					buffer2[i+2]=tag.event_name[i];			
			    	buffer2[12]=',';
				for(i=0;i<10;i++)
					buffer2[i+13]=tag.date[i];		
					buffer2[23]=',';
				for(i=0;i<5;i++)
					buffer2[i+24]=tag.time[i];
					buffer2[29]=',';
				for(i=0;i<10;i++)		
					buffer2[i+30]=tag.location[i];
					strcpy(arr[x_total+t_total],buffer2);
			}	
	}	
		else
				{ 		
					t_total=t_total+1;
					strcpy(arr[x_total+t_total],buf);
					tag=event_arr[total-1];				
				}
		}	
	}
	else if(buf[0]=='X')
	{
		x_total=x_total+2;	
		for(i=0;i<total;i++)
			{
				for(j=0;j<10;j++)
					{
						if(buf[j+2]== event_arr[i].event_name[j])
							ack2=1;
						else
							ack2=0;
					}
				if(ack2==1)
					{
						m=i;
						break;
					}
		
			}
	ignore=event_arr[m];
	
					buffer3[0]='C';
					buffer3[1]=',';
				for(i=0;i<10;i++)
					buffer3[i+2]=ignore.event_name[i];			
					buffer3[12]=',';
				for(i=0;i<10;i++)
					buffer3[i+13]=ignore.date[i];		
					buffer3[23]=',';
				for(i=0;i<5;i++)
					buffer3[i+24]=ignore.time[i];
					buffer3[29]=',';
				for(i=0;i<10;i++)		
					buffer3[i+30]=ignore.location[i];
					strcpy(arr[r-2],buffer3);
					strcpy(arr[r-1],buf);
			}
	else if(buf[0]=='D')
		{
				//Code for delete part
		}
	}
}
for(i=0;i<r;i++)
write(1,arr[i],40);
}

