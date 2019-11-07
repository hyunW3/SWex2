#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <fcntl.h>
#include <string.h>

int main() {

	int fd[2];
	char line[100];
	if(pipe(fd) <0){
		perror("pipe error");
		exit(-1);
	}
	if(fork()==0){ // child
		close(fd[1]);
		read(fd[0],line,100);
		dup2(fd[0],fileno(stdin));
		execl("/usr/bin/bc","bc",line,NULL);
	} else { // parent
		close(fd[0]);
		dup2(fd[1],fileno(stdout));
		execl("/bin/echo","echo","124 *(42+3) % 17",NULL);
		//echo("124 * (42+3) % 17");
	}
}

