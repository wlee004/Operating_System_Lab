#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){
	char *sourceFile = argv[1];
	char *destinationFile = argv[2];
	int returnval = access(sourceFile, F_OK);
	size_t size;
	struct stat buffer;
	int status;
	int fdSource;
	int fdDestination;


	if(argc != 3){
		printf("\n Usage: ./replace <source file> <destination file> \n");
		return 1;
	} 
	// source file exist and accessible 
	if(returnval == 0){
		status = stat(sourceFile, &buffer);
		if(status == 0)
			size = buffer.st_size;
		// checking read and write access of sourceFile
		returnval = access (sourceFile, O_RDONLY);

		// have read only access for sourceFile
		if(returnval == 0){
			
			// opening source file 
			fdSource = open(sourceFile,O_RDWR);
					
			// failed to open source file
			if(fdSource == -1){
				perror("Open() Failed with error");
				close(fdSource);
				return 1;
			}
			else{
				// open destination file or create it if it doesn't exist
				fdDestination = open(destinationFile, O_WRONLY | O_CREAT, 00700);
				
				// failed to open destination file 
				if(fdDestination == -1){
					perror("Open() Failed with error");
					close(fdSource);
					close(fdDestination);	
					return 1;
				}
				else{
					// FIXES HERE 
					char buf[75];
					char abc[3];
					abc[0] = 'A';
					abc[1] = 'B';
					abc[2] = 'C';
					int r;
					int byteRead = 0;
					while(byteRead < size){
						//reading 75 bytes in source file
						r = read(fdSource, buf, 75);
						// replacing '5' in buffer with 'O'
						for(int i = 0; i < r; i++){
							if(buf[i] == '5')
								buf[i] = 'O';
						}
						// write buffer into destination file
						write(fdDestination, buf, r);
						// write ABC into destination file after 75 characters or end of file 
						write(fdDestination, abc, 3);
						byteRead += r;
					}
					// close files
					close(fdSource);
					close(fdDestination);
				}
			}	
		}
		// source file access error
		else{
			if(errno == EACCES)
				perror("File does not have read access");
		}
	}
	// source file error 
	else{
		if(errno == ENOENT){
			perror("File does not exist");
		}
		else if(errno == EACCES)
			perror("File is not accessible");
			return 0;
	}
	return 0;
}
