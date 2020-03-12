#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
void fork10(){
	int N=5;
	pid_t pid[N];
	int i, child_status;
	for(i =0; i< N; i++){
		if((pid[i] = fork()) == 0)
			exit(100+i); // child
	}
	for(i =0; i< N; i++){
		pid_t wpid = wait(&child_status);
		if(WIFEXITED(child_status)){
			printf("child %d terminated with exit status %d\n", wpid, WEXITSTATUS(child_status));
		} else {
			printf("child %d terminated abnormally\n", wpid);
		}
	}

}

int main() {
	fork10();	
}
