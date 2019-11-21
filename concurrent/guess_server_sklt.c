 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netdb.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <strings.h>
 #include <arpa/inet.h>
 #include "unistd.h"
 #include <signal.h>
 #include <sys/wait.h>
 #include <time.h>
 
 #define MAXLINE 80 

int connfd;	

void al(int sig){
	int status; 
	waitpid(-1, &status, 0);
}

int main ( int argc , char * argv []) 
{
	int n, listenfd, caddrlen ;
	struct hostent *h;
	struct sockaddr_in saddr, caddr ;
	char buf [MAXLINE];
	int port =atoi ( argv [1]);
	int num;
	time_t t;

	int ans, i;
	signal(SIGCHLD,al);
	srand(time(NULL));

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


	if (listen( listenfd , 5) < 0) {
		printf ("listen() failed.\n");
		exit(3);
	}
 
	 while (1) { 
	pid_t pid;
	
	
		caddrlen = sizeof ( caddr );
		if (( connfd = accept( listenfd , ( struct sockaddr *)& caddr , & caddrlen )) < 0) {
			printf ("accept() failed.\n");
			continue;
		}
		num = rand()%99 + 1;
	if((pid=fork() == 0) ){		
		
		close(listenfd);

		while ((n = read( connfd, buf, MAXLINE)) > 0) 
		{
			for(ans = 0, i=0; i<n ;i++){
				if(buf[i] >= '0' && buf[i] <='9'){
					 ans *= 10;
					 ans += (int)(buf[i])-48;
				}
			}
			printf("ANSWER[%2d]:CLIENT[%d]\n", num, ans);
			if( num > ans )
				write( connfd, "Up\n", 4);
			else if( num < ans )
				write( connfd, "Down\n", 6);
			else if( num == ans ){
				write( connfd, "Correct\n", 9);
				close( connfd );
				exit(0);
			} else
				write(connfd, "Error\n", 7);
		}
	
		printf ("connection terminated.\n");
		close( connfd );
		}
	}

}



 
