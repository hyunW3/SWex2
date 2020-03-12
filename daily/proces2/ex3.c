#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
void al(int sig){
	printf("beep\n");
	fflush(stdout);
}
void handler(int sig){
	signal(SIGALRM,al);
	alarm(1);
	for(int i=0; i<4; i++){
		pause();
		alarm(1);
	}
	sleep(5);
	printf("I'm alive!");
	fflush(stdout);
}

int main() {
	while(1) signal(SIGINT, handler);
}	
