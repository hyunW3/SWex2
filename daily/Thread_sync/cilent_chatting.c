#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unistd.h"
#include <pthread.h>
#define MAXLINE 80
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

int main (int argc, char *argv[]) {
	int n, cfd;
	struct hostent *h;
	struct sockaddr_in saddr;

	char *host = argv[1];
	int port = atoi(argv[2]);

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
	}
	pthread_create(&psend,NULL,pthread_send,(void*) &cfd);
	pthread_create(&pwrite,NULL,pthread_write,(void*) &cfd);
//	pthread_join(psend,NULL);
//	pthread_join(pwrite,NULL);
//        int cfd = *((int*)fd);
       char buf[MAXLINE];
//pthread_mutex_lock(&(lock));
        while ((n = read(0, buf, MAXLINE)) > 0) {
                write(cfd, buf, n);
        }

	close(cfd);
}	
void *pthread_send(void *fd){	
	int cfd = *((int*)fd);
	int n;
	char buf[MAXLINE];
pthread_mutex_lock(&(lock));
	while ((n = read(0, buf, MAXLINE)) > 0) {
		write(cfd, buf, n);
	}	
pthread_mutex_unlock(&(lock));
}

void *pthread_write(void *fd){	
	int cfd = *((int*)fd);
	int n;
	char buf[MAXLINE];
	while ((n = read(cfd, buf, MAXLINE)) > 0) {
		write(1, buf, n);
	}
}
