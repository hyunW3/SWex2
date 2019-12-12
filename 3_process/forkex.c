#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void fork1(){
	int x =1;
	pid_t pid = fork();
	if( pid == 0) {
		printf("Child has x = %d\n",++x);
	} else {
		printf("Parent has x = %d\n",--x);
	}
	printf("Bye from process %d with x = %d\n",getpid(), x);
}

int main(){
	fork1();
}

