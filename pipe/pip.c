#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#define MAXLINE 80

int main() {
int n, fd[2];
pid_t pid;
char line[MAXLINE];
if(pipe(fd) < 0) exit(1);
if((pid= fork()) < 0) exit(2);
if (pid> 0) {/* parent */
	close(fd[0]);
	write(fd[1], "hello world\n", 12);
} else {/* child */
	close(fd[1]);
	n = read(fd[0], line, MAXLINE);write(1, line, n);
}
exit(0);
}
