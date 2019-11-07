#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]) {
	int status;
	if(fork() == 0){
		//int fd = open(argv[1],O_RDWR,0755);
		//stdout = open(dest,O_WRONLY,0755);
		int newfd = open(argv[2],O_WRONLY | O_CREAT, 0755);
		dup2(newfd,fileno(stdout));
		execl("/bin/cat","cat",argv[1],NULL);
printf("HI");		
//		close(fd);
		close(newfd);
		exit(32); // 		
	}
	wait(&status);
	printf("%d\n",status);
	printf("pid: %d\n",getpid());
	printf("status: %d\n",WEXITSTATUS(status));
}
