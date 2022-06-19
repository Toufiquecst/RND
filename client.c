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

int main(int argc,char **argv)
{
  	int sock_fd;
  	struct sockaddr_in serv_addr;
	char send_line[MAXLINE],recv_line[MAXLINE];
  	struct hostent *hptr;
	char *temp,*ptr,**pptr;
	char str[MAXLINE];
		

	if(argc<2)
    	{
       	 	fprintf(stderr,"\n input format error\n");
        	exit(0);
    	 }

	ptr= *++argv;
	if ((hptr=gethostbyname(ptr)) == NULL) 
	{ 
    		fprintf(stderr,"\n error for the hostname %s\n",ptr);
    		exit(1);
	}
/*
	if(inet_pton(AF_INET,argv[2],&serv_addr.sin_addr)<=0)
	{
		fprintf(stderr,"\n inet_pton error \n");
		exit(1);
	}
*/


	if( (sock_fd=socket(AF_INET,SOCK_STREAM,0)) == -1)
       	{
       	 	fprintf(stderr,"\n\n Socket Error..\n\n");
        	exit(0);
       	}
	bzero(&serv_addr,sizeof(serv_addr));
       	serv_addr.sin_family = AF_INET;
       	serv_addr.sin_port   = htonl(SERV_PORT);

	pptr=hptr->h_addr_list;
       	bcopy(*pptr,&serv_addr.sin_addr,hptr->h_length);
	printf("\taddress: %s\n",inet_ntop(serv_addr.sin_family,&serv_addr.sin_addr,str,sizeof(str)));
	fprintf(stderr,"\n222222\n");

       	if(connect(sock_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))!=0)
       	{
  	    	close(sock_fd); 
	       	fprintf(stderr,"\n\n Unable to connect..\n\n");
       		exit(0); 
       	}

	fprintf(stderr,"[client]:");
 	if( (fgets(send_line,MAXLINE,stdin))!=NULL)
 	{
         	if( (strcasecmp(send_line,"login\n")) == 0)        //fgets puts the new line in buffer send_line
		{
		fprintf(stderr,"\n11111\n");
                    	  

			send(sock_fd,send_line,strlen(send_line)+1,0);

			if (recv(sock_fd ,recv_line,MAXLINE,0) == -1 )
                  	{
		    		printf("\n\n Read Error...\n\n");
		    		exit(0);
		  	}
		  	fputs(recv_line,stdout);

	  		while( (fgets(send_line,MAXLINE,stdin)) != NULL)
	       		{
				bcopy(send_line,temp,4*sizeof(char));
				temp[4]='\0';
				if(strcasecmp(temp,"list")==0)
				{
				        while((strcasecmp(recv_line,"finished\n"))!=0 && (strcasecmp(recv_line,"no match found\n"))!=0)
					{
                             			//send(sock_fd ,"CONTINUE\n",strlen("CONTINUE\n")+1,0);
                             			recv(sock_fd,recv_line,MAXLINE,0);
			     			fputs(recv_line,stdout);
			    		}
				}
				
				send(sock_fd,send_line,strlen(send_line)+1,0); 
				if (recv(sock_fd ,recv_line,MAXLINE,0) == -1)
				{
		           		printf("\n\n Read Error...\n\n");
		           		exit(0);
	             		}
		     		fputs(recv_line,stdout);

				if( (strcasecmp(recv_line,"Connection Terminated\n")) == 0)
				{
					close(sock_fd);
					exit(0);
		     		}
			}
		}
	}
}
