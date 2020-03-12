#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <fcntl.h>
#include <string.h>
int main() {
	int fd[2];
	pipe(fd);
	if(fork() == 0){
		//int fd = open("../db/0",O_CREAT | O_RDWR , 0755);
		//close(fd[0]); // close reading
		dup2(fd[1],fileno(stdout));
		execl("/bin/ls","ls","-t","../blank/", NULL);
		//system("/bin/ls -t ../db | head -n +1");
	}
	wait(NULL);
	close(fd[1]);
	char * cat = (char*)malloc(sizeof(char)*100);
	read(fd[0],cat,100);
	if(strlen(cat) == 0) return 0;
	int i=0;
	for(; cat[i] != '-'; i++);
	i++;
	int start = i;
	for(; cat[i] != '\n'; i++){
		//printf("%c",cat[i]);
	}
	char p[10];
	strncpy(p,cat+start,i-start);
	int num = atoi(p);
	printf("%d\n",num);
	return 0;
}
