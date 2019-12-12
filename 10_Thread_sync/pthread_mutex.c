#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
// uncompleted
pthread_mutex_t m;// = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t r;// = PTHREAD_MUTEX_INITIALIZER;
int b = 10000;
int num=0;

void *b_read() {
	for(int i=0; i<1000; i++){
		pthread_mutex_lock(&r);
		num++;
		if(num==1) pthread_mutex_lock(&m);
		pthread_mutex_unlock(&r);
		
		printf("%dread_value:%d\n",i,b);
		pthread_mutex_lock(&r);
		num--;
		if(num==0) pthread_mutex_unlock(&m);
		pthread_mutex_unlock(&r);
	}
}
void *b_write(){
	for(int i=0; i<20000; i++){
	        pthread_mutex_lock(&m);	
		b -=1;
		//printf("write:%d\n",b);
        	pthread_mutex_unlock(&m);
	}
}
int main() {
        pthread_t read[5],write;
        pthread_create(&write, NULL, b_write, NULL);
	for(int i=0; i<5; i++){
		pthread_create((read+i), NULL, b_read, NULL);
	}
	pthread_join(write,NULL);
	for(int i=0; i<5; i++){
		pthread_join(read[i],NULL);
	}
	
}


