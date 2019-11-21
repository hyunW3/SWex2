#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAXLINE 80
int main (int argc, char *argv[]) {
	int n, listenfd, connfd, caddrlen;
	struct hostent *h;
	struct sockaddr_in saddr, caddr;
	char buf[MAXLINE];
	int port = atoi(argv[1]);
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket() failed.\n");
		exit(1);
	}
	bzero((char *)&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);
	if (bind(listenfd, (struct sockaddr *)&saddr,sizeof(saddr)) < 0) {
		printf("bind() failed.\n");
		exit(2);
	}
	if (listen(listenfd, 5) < 0) {
		printf("listen() failed.\n");
		exit(3);
	}
//	if(fork() == 0){
   while (1) { 
		caddrlen = sizeof(caddr);
		if ((connfd = accept(listenfd, (struct sockaddr *)&caddr,&caddrlen)) < 0) {
			printf ("accept() failed.\n");
			continue;
	}
       int random = rand() % 99+1;
//        int random = 20;
//        write(connfd,random,sizeof(int));

	h = gethostbyaddr((const char *)&caddr.sin_addr.s_addr, sizeof(caddr.sin_addr.s_addr), AF_INET);
	printf("server connected to %s (%s)\n",
	h->h_name,
	inet_ntoa(*(struct in_addr *)&caddr.sin_addr));
	

 char* g = "Guess? ";
	while(1){
		
		write(connfd,g,sizeof(g)*sizeof(char));
		if((n = read(connfd, buf, MAXLINE)) > 0) {
			int val = atoi(buf);
			if(val > random) write(connfd,"up\n",sizeof(char)*3);
			else if(val < random) write(connfd,"down\n",sizeof(char)*5);	
			else {
				char* Co = "Correct!";
				write(connfd,Co,sizeof(Co));
				close(connfd);
				break;
			}
		}
		fflush(stdout);
	}
//	}
//	wait(NULL);
   }
}
