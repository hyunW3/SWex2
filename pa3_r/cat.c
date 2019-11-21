#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	for (int i = 1; i < argc; i++){
		int fd = open(argv[i], O_RDONLY);
		if (fd < 0){
			perror("cat");
			exit(1);
		}
		char buf[2];
		while(read(fd, buf, 1) > 0){
			int trash = write(1, buf, 1);
			trash++;
		}
		close(fd);
	}
	return 0;
}