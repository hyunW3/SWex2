#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int src = open(argv[1], O_RDONLY);
	if (src < 0){
		perror("cp");
		exit(1);
	}	
	int dest = open(argv[2], O_RDWR | O_CREAT, 0644);
	if (dest < 0){
		perror("cp");
		exit(1);
	}

	int nbread;
	char buff[1000];
	while((nbread = read(src, buff, 1000)) > 0){
		int trash = write(dest, buff, nbread);
		trash++;
	}
	close(src);
	close(dest);
	return 0;
}