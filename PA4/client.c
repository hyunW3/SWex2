#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 256

int main (int argc, char *argv[]) { // ./client 127.0.0.1 8888
	int n, cfd;
	char buf[MAXLINE];
	struct hostent *h;
	struct sockaddr_in saddr;

	char *host = argv[1]; //  127.0.0.1
	int port = atoi(argv[2]); // 8888

	if ((cfd= socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket() failed.\n");
		exit(1);
	}
	if ((h = gethostbyname(host)) == NULL) {
		printf("invalid hostname %s\n", host);
		exit(2);
	}

	bzero((char *)&saddr, sizeof(saddr));
	saddr.sin_family= AF_INET;
	bcopy((char *)h->h_addr, (char *)&saddr.sin_addr.s_addr, h->h_length);
	saddr.sin_port= htons(port);
	if (connect(cfd,(struct sockaddr*)&saddr,sizeof(saddr)) < 0) {
		printf("connect() failed.\n");
		exit(3);
	}else {
 	//send message
	//while(1){
	/*
		while((n = read(0,buf,MAXLINE))>0){ // something in stdin
			printf("\nbuf:%s",buf);
			n = write(cfd,buf,n); // send server from stdin
			memset(buf,'\0',sizeof(buf));
			n = read(cfd,buf,MAXLINE); // read from server
			if(n<0) return 0;
			n = write(1,buf,n); // 
			if(!strcmp(buf,"BYE\n")){
				//eturn 0; // exit
				return 0;
			} else if(!strcmp(buf,"Too many clients\n")){
				return 0;
			}
			memset(buf,'\0',sizeof(buf));				
		*/	

		
		
	int i=0;
		while((n = read(0,buf+i,1))>0) { // something in stdin
			if(buf[i++] != '\n') continue; // until get \n
			else buf[i] = '\0';
			//printf("\nbuf:%s",buf);			
			n = write(cfd,buf,i); // send server from stdin
			memset(buf,'\0',sizeof(buf));
			i=0;
			while((n = read(cfd,buf+i,1))>0){ // read from server
				if(buf[i++] != '\n') continue; // until get \n
				else break;		
			}
			//printf("buf_serv:%s",buf);
			n = write(1,buf,i); // 
			if(!strcmp(buf,"BYE\n")){
				//eturn 0; // exit
				return 0;
			} else if(!strcmp(buf,"Too many clients\n")){
				return 0;
			}
			fflush(stdout);
			memset(buf,'\0',sizeof(buf));
			i=0;	
		}	
	//}
	close(cfd);

	}
	
	
}	