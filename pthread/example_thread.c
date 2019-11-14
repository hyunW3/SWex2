#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

int gv= 2;
void *thread(void *vargp);
int main() {
	pthread_t tid;
	pthread_create(&tid, NULL, thread, NULL);
	pthread_join(tid, NULL);
	printf("[main   ]value:%2d | PID: %5d | TID: %lud\n",--gv, getpid(), (unsigned)pthread_self() );
	exit(0);
}

void *thread(void *vargp) /* Thread routine */{
	printf("Hello, world!\n");
	printf("[created] value:%2d | PID: %5d | TID: %lud\n",--gv, getpid(), (unsigned)pthread_self() );
	return NULL;
}
