#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
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
	while (1) { 
		caddrlen = sizeof(caddr);
		if ((connfd = accept(listenfd, (struct sockaddr *)&caddr,&caddrlen)) < 0) {
			printf ("accept() failed.\n");
			continue;
	}
	h = gethostbyaddr((const char *)&caddr.sin_addr.s_addr, sizeof(caddr.sin_addr.s_addr), AF_INET);
	printf("server connected to %s (%s)\n",
	h->h_name,
	inet_ntoa(*(struct in_addr *)&caddr.sin_addr));
	// echo
	while ((n = read(connfd, buf, MAXLINE)) > 0) {
		printf ("got %d bytes from client.\n", n);
		write(connfd, buf, n);
	}
	printf("connection terminated.\n");
	close(connfd);
	}
}
