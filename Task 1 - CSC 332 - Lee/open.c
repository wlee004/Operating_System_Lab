#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
	int fd; 
	if(2 != argc){
		printf("\n Usage: ./open.c <file> \n");
		return 1;
	}
	errno = 0;
	fd = open(argv[1],O_RDONLY|O_CREAT);
	if(-1 == fd){
		perror("Open() failed with error");
		close(fd);
		return 1;
	}
	else{
 		printf("\n Open() %s Successful\n", argv[1]);
		close(fd);
	}
	return 0;
}
