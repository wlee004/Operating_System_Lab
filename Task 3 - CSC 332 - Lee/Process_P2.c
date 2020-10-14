#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(){
	int fdSource, destination1, destination2, status;
	struct stat buffer; 
	size_t size; 

	if(access("source.txt", F_OK) == 0){ // source file exist and accessible
		if(access("destination1.txt", F_OK) == 0){ // destination1 file exist and accessible
			if(access("destination2.txt", F_OK) == 0){ // destination2 file exist and accessible
					
				// opening source file 
				fdSource = open("source.txt", O_RDONLY);
				if(fdSource == -1){ // failed to open source file 
					perror("Open() source.txt Failed with error");
					close(fdSource);
					return 1;
				}
				else{ // successfully opened source file 
					// open destination1 
					destination1 = open("destination1.txt", O_WRONLY);
					if(destination1 == -1){ // failed to open destination1 file 
						perror("Open() destination1.txt Failed with error");
						close(destination1);
						close(fdSource);
						return 1;
					}
					else{ // successfully opened destination1 file 
						// opening destination2 
						destination2 = open("destination2.txt", O_WRONLY);
						if(destination2 == -1){
							perror("Open() destination2.txt Failed with error");
							close(destination2);
							close(destination1);
							close(fdSource);
							return 1;
						}
						else{ // successfully opened destination2 file

							// get size of source file
							status = stat("source.txt", &buffer);
							if(status == 0)
								size = buffer.st_size;
							
							char buf1[50];
							char updated[50];
							char buf2[100];
							int r; 
							int byteRead = 0; 
							while(byteRead < size){
								
								//read 50 byte in source file 
								r = read(fdSource, buf1, 50);
								int j = 0;
								memset(updated, 0, 50);
								for(int i = 0; i < sizeof(buf1); i++){
									if(buf1[i] == '5' || buf1[i] == '8'){
										updated[j] = buf1[i];	
										j++; 
									}
								}
								write(destination1, updated, j);
								byteRead += r;

								//read 100 byte in source file 
								r = read(fdSource, buf2, 100);
								write(destination2, buf2, sizeof(buf2));
								byteRead +=r;
							}
							// close files
							close(fdSource);
							close(destination1);
							close(destination2);
						}
					}
				}	
			}
			else{
				if(errno == ENOENT){
					perror("File does not exist");
				}
				else if (errno == EACCES){
					perror("File is not accessible");
				}
			}
		}
		else{ // destination1 file error
			if(errno == ENOENT){
				perror("File does not exist");
			}
			else if (errno == EACCES){
				perror("File is not accessible");
			}
		}
	}
	else{ // source file error 
		if(errno == ENOENT){
			perror("File does not exist");
		}
		else if (errno == EACCES){
			perror("File is not accessible");
		}
	}
	return 0; 
}
