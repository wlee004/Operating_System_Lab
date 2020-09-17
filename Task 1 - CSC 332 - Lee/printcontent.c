#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){
	char* filepath = argv[1];
	int returnval = access(filepath, F_OK); 
	int fd;
	struct stat buffer;
	int status;
	size_t size; 

	// invalid arguments
	if(2 != argc){
		printf("\n Usage: ./print_content.c <file> \n");
		return 1;
	}	
	// file exists 
	if (returnval == 0){
		// get filepath size in terms of bytes
		status = stat(filepath, &buffer);
		if(status == 0)
			size = buffer.st_size;
		char buf[size];	

		// checking read and write access 
		returnval = access(filepath,O_RDWR);
		
		// have read and write access 
		if(returnval == 0){
			fd = open(filepath,O_RDWR);

			// error opening file 
			if(-1 == fd){
				perror("Open() Failed with error");
				close(fd);
				return 1;
			}

			// able to open file to read and write 
			else{
				read(fd, buf, size);
				write(1, buf, size);
				close(fd);
			}
		} 
		// don't have read and write access 
		else{
			if(errno == EACCES)
				perror("File does not have read & write access");
		}
	}

	// file error 
	else{
		if(errno == ENOENT)
			perror("File does not exist");
		else if (errno == EACCES)
			perror("File is not accessible");
			return 0;
	}
	return 0;
}
