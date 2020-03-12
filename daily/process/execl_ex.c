#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
int main() {
	if(fork() == 0){
		execl("/bin/ls","ls","/", NULL);
	}
	wait(NULL);
	printf("Completed\n");
	exit(0);
}
