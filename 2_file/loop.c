#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(int argc, char** argv)
{
	while(1)
	{
		if(argc ==2) {
	//		printf("argv[1]:%s ",argv[1]);
		}
		printf("%d %d\n",getpgid(getpid()),getpid());
		sleep(3);
	}
	
}
