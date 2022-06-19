#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define SERV_PORT 13
#define LISTENQ 5
#define MAXLINE 500

struct client_info
{
	char last_name[26];
	char first_name[21];
	char id_number[10];
	char location[31];
	struct client_info *next;
};
struct client_info *hdr_ptr=NULL;                 //header to the linked list.

int process_req(int sock_fd);
int total=0;


main()
{
	int listen_fd,conn_fd,yes=1;
	struct sockaddr_in cli_addr,serv_addr;
	socklen_t cli_len;

	if((listen_fd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
  		fprintf(stderr,"\n\n Socket Error..\n\n");
		exit(0);
        }
	setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes));

	bzero(&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port = htonl(SERV_PORT);
	if((bind(listen_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)))==-1)
   	{
    		perror("\n\n Could not bind\n\n");
    		exit(1);
   	} 
	listen(listen_fd,LISTENQ);
	fprintf(stdout,"\n listening completed \n\n");
	while(1)
    	{
      		cli_len=sizeof(cli_addr);
         	if((conn_fd = accept(listen_fd,(struct sockaddr*)&cli_addr,&cli_len))<0)
         	{
	fprintf(stdout,"\n accept completed \n\n");
              		if(errno==EINTR)
	        		continue;
	      		else
	      		{
	        		printf("\n error  in accept call \n");
	        		exit(1);
	      		}
	        }
		process_req(conn_fd);
	}
}

int process_req(int sock_fd)
{
	unsigned int n;
	int number,i=0,j=0,k=0;
	char buf[MAXLINE];
	char **command_arr;
	struct client_info temp_node;
	struct client_info *ptr_temp,*ptr_loop;

	while( ( n = recv(sock_fd,buf,MAXLINE,0)) > 0 ) 
   	{	
		bzero(&temp_node,sizeof(temp_node));
		ptr_temp=NULL;
		ptr_loop=NULL;
		for(i=0;i<6;i++)
			command_arr[i]=NULL;
		i=0;
		j=0;
		do
		{
			command_arr[i]=(char*)malloc(31* sizeof(char));
			k=0;
			while(buf[j]!=' ')
			{
				if(buf[j]=='\n')
					break;
				*(command_arr[i]+k)=buf[j];
				k++;
				j++;
			}
			*(command_arr[i]+k)='\0';
			i++;
		}
		while(buf[j]!='\n');
		command_arr[i]=NULL;
		number=i;

          	if( (strcasecmp(command_arr[0],"login")) == 0 )                       //Welcome on login 
              		send(sock_fd,"hello\n",(strlen("hello\n")+1 ),0);
		else  if(strcasecmp(command_arr[0],"quit") == 0 )                     //prompt on termination
	  	{
	     		send(sock_fd,"Connection Terminated\n",(strlen("Connection Terminated\n")+1),0);
 	  	}
		else if(strcasecmp(command_arr[0],"add") == 0)			//  add entry to the database
		{
			if(number!=5)
				send(sock_fd,"invalid\n",(strlen("invalid\n")+1),0);
			else
			{
				strcpy(temp_node.first_name,command_arr[1]);
               			strcpy(temp_node.last_name,command_arr[2]);
	        		strcpy(temp_node.id_number,command_arr[3]);
                              	strcpy(temp_node.location,command_arr[4]);
               			temp_node.next=NULL;

				if(strlen(temp_node.id_number)!=9)
					send(sock_fd,"invalid id\n",(strlen("invalid id\n")+1),0);

				ptr_temp=(struct client_info *)malloc(sizeof(struct client_info));
				if(ptr_temp==NULL)
				{
				  send(sock_fd,"memory not available\n",(strlen("memory not available\n")+1),0);
					continue;
				}
				ptr_temp->next=NULL;
				ptr_temp->first_name[0]='\0';
				ptr_temp->last_name[0]='\0';
				ptr_temp->id_number[0]='\0';
				ptr_temp->location[0]='\0';
				
				strncat(ptr_temp->first_name,temp_node.first_name,20);
				strncat(ptr_temp->last_name,temp_node.last_name,25);
				strncat(ptr_temp->id_number,temp_node.id_number,9);
				strncat(ptr_temp->location,temp_node.location,30);

				if((hdr_ptr==NULL)||(strcmp(temp_node.last_name,hdr_ptr->last_name)<=0))
				{
					ptr_temp->next=hdr_ptr;
					hdr_ptr=ptr_temp;
					send(sock_fd,"entry added\n",(strlen("entry added\n")+1),0);
					continue;
				}

				ptr_loop=hdr_ptr;
				while(ptr_loop->next!=NULL) 
				{
					if(strcmp(temp_node.last_name,ptr_loop->next->last_name)>0)
					{
						ptr_temp->next=ptr_loop->next;
						ptr_loop->next=ptr_temp;
					}
						ptr_loop=ptr_loop->next;
				}
				send(sock_fd,"entry added\n",(strlen("entry added\n")+1),0);
			}
		}
		else if(strcasecmp(command_arr[0],"remove") == 0)               //remove entry from the database
	  	{
			int success=0;
			ptr_temp=hdr_ptr;
			ptr_loop=hdr_ptr;

			if(hdr_ptr == NULL)
			{
				send(sock_fd,"nothing in list\n",(strlen("nothing in list\n")+1),0);
				continue;
			}
			if(strcmp(command_arr[1],ptr_temp->id_number)==0)
			{
				hdr_ptr=ptr_temp->next;
				send(sock_fd,"remove successful\n",strlen("remove successful\n")+1,0);
				continue;
			}
			while(ptr_loop->next!=NULL)
			{
				if(strcmp(command_arr[1],ptr_loop->next->id_number)==0)
				{
					ptr_temp=ptr_loop->next;
					ptr_loop->next=ptr_loop->next->next;
					free(ptr_temp);
					send(sock_fd,"remove successful\n",strlen("remove successful\n")+1,0);
					success=1;
				}
				ptr_loop=ptr_loop->next;
			}
			if(success==0)
				send(sock_fd,"id doesn't exist\n",strlen("id doesn't exist\n")+1,0);
			continue;

		}
		else if(strcasecmp(command_arr[0],"list") == 0)
		{
			char first,last,last_inr;
			struct client_info *first_ptr,*last_ptr;
			char info_buffer[100];
			char *space=" ";

			first=tolower(command_arr[1][0]);
			last=tolower(command_arr[2][0]);
			ptr_loop=hdr_ptr;
			for(;(ptr_loop!=NULL)&&(ptr_loop->last_name[0]!=first);ptr_loop=ptr_loop->next);
			if(ptr_loop==NULL)
			{
				send(sock_fd,"no match found\n",strlen("no match found\n")+1,0);
				continue;
			}
			first_ptr=ptr_loop;

			while(ptr_loop!=NULL && tolower(ptr_loop->last_name[0])<=last)
			{
				for(i=0;i<100;i++)
					info_buffer[i]='\0';
				sprintf(info_buffer,"%s %s %s %s\n",ptr_loop->first_name,ptr_loop->last_name, ptr_loop->id_number,ptr_loop->location);
				send(sock_fd,info_buffer,strlen(info_buffer),0);

			}
			send(sock_fd,"finished\n",strlen("finished\n")+1,0);
		}
		else 
			send(sock_fd,"invalid message\n",strlen("invalid message\n")+1,0);

	}
}
