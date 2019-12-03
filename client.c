#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 128
pthread_mutex_t lock;
typedef struct {
	char buf[MAXLINE];
	int fd;
	int sp;
	int count;
	pthread_mutex_t lock;
	pthread_cond_t notfull;
	pthread_cond_t notempty;
} data;
void *pthread_send(void *fd);
void *pthread_write(void *fd);

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
	pthread_t psend,pwrite;
	if (connect(cfd,(struct sockaddr*)&saddr,sizeof(saddr)) < 0) {
		printf("connect() failed.\n");
		exit(3);
	}else {
 	//send message

		while((n = read(0,buf,MAXLINE))>0){
			n = write(cfd,buf,n);
			memset(buf,'\0',sizeof(buf));
			n = read(cfd,buf,MAXLINE);
			if(n<0) break;
			n = write(1,buf,n);
			if(!strcmp(buf,"BYE\n")){
				//eturn 0; // exit
				break;
			} else if(!strcmp(buf,"Too many clients\n")){
				break;
			}
			memset(buf,'\0',sizeof(buf));				
			

		}
	}
	close(cfd);
}	