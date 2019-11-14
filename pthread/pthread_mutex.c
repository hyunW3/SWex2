#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
// uncompleted
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int b = 10000;

int get_balance(){
	return b;
}
void put_balance(int k){
	b = k;
}
int *deposit(int amount) {
	int balance;
	pthread_mutex_lock(&m);
	balance = get_balance();
	balance += amount;
	put_balance(balance);
	printf("%d\n",balance);
	pthread_mutex_unlock(&m);
	return balance;
}
int *withdraw(int amount){
        int balance;
        pthread_mutex_lock(&m);
        balance = get_balance();
        balance -= amount;
        put_balance(balance);
	printf("%d\n",balance);
        pthread_mutex_unlock(&m);
        return balance;

	
}
int main() {
        pthread_t tid;
        pthread_create(&tid, NULL, withdraw(1000), NULL);
       	printf("%d\n",b);
	pthread_create(&tid, NULL, deposit(100), NULL);
        printf("%d\n",b);
	
	int money = withdraw(1000);
}


