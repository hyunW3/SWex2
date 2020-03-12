#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <netdb.h>
#include <sys/socket.h>
#include <time.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#define MAXLINE 128

typedef struct {
	char* datas;
	int max;
	int count;
	int* fd;
	pthread_mutex_t lock;
	pthread_cond_t send;
	//pthread_cond_t ;
} data;
data *qinit(int max);
void qdelete(data *q);
void *pthread_main(void *args);
int main(int argc, char** argv){
	data *q;
	int max = atoi(argv[2]);
	q = qinit(max);
	int connfd, listenfd, caddrlen ;
	struct hostent *h;
	struct sockaddr_in saddr, caddr ;
	int port =atoi ( argv [1]);
	int num=0;
	time_t t;
	pthread_t client[max]; // client number
	if (( listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf ("socket() failed.\n");
		exit(1);
	}
	bzero ((char *)& saddr, sizeof ( saddr ));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr =	htonl (INADDR_ANY);
	saddr.sin_port =htons (port);
	
	if (bind( listenfd , ( struct sockaddr *)& saddr, sizeof ( saddr )) < 0) {
		printf ("bind() failed.\n");
		exit(2);
	}


	if (listen( listenfd , max) < 0) {
		printf ("listen() failed.\n");
		exit(3);
	}	
	while(1){
		caddrlen = sizeof(caddr);
		if (( connfd = accept( listenfd , ( struct sockaddr *)& caddr , & caddrlen )) < 0) {
			printf ("accept() failed.\n");
			continue;
		}
		q->count++;
		q->fd[q->count] = connfd; 
		pthread_create(&client[q->count], NULL, pthread_main,(void *)q);
		pthread_detach(client[q->count]);
	}
	

	qdelete(q);
}
void *pthread_main(void *args){
	int n; 
	char buf[MAXLINE];
	data *q = (data*)args;
	int num = q->count;
while(1){
	while((n = read(q->fd[num], buf, MAXLINE)) >0)
	{	
		int i=0;
		for(int ans=0; i<n; i++){
			if(buf[i] >= '0' && buf[i] <= '9'){
				ans *= 10;
				ans += (int)(buf[i])-48;
			}

		}
		pthread_mutex_lock(&(q->lock));
		i=0;
		for(i; i<q->max; i++){
printf("fd[%d]:%d\n",i,q->fd[i]);
			
			if(q->fd[i] == -1) continue;
			if(i == num) continue;
			printf("i:%d,num:%d\n",i,num);
			write(q->fd[i],buf,MAXLINE);
			
		}
		printf("\n");
		pthread_mutex_unlock(&(q->lock));
	}
}
q->fd[num] =-1;
}
data *qinit(int max){
	data *q;
	q = (data *) malloc(sizeof(data));
	q->max = max;
	q->count = -1;
	q->fd = (int*)malloc(sizeof(int)*max);
	for(int i=0; i<max; i++){
		q->fd[i] =-1;
	}
	pthread_mutex_init(&q->lock, NULL);
	pthread_cond_init(&q->send, NULL);
	return q;
}
void qdelete(data *q){
	pthread_mutex_destroy(&q->lock);
	pthread_cond_destroy(&q->send);
	free(q);
}


