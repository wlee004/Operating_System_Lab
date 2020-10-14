#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
	int destination1; 
	int destination2; 

	// creating destination1.txt with read, write, and execute permission 
	destination1 = open("destination1.txt", O_CREAT, 00700);

	if(destination1 == -1){
		perror("Open() destination1.txt Failed with error");
		close(destination1);
		return 1; 
	}
	else{
		// creating destination2.txt with read, write, and execute permission 
		destination2 = open("destination2.txt", O_CREAT, 00700);
		if(destination1 == -1){
			perror("Open() destination2.txt Failed with error");
			close(destination1);
			close(destination2);
			return 1; 
		}
		close(destination1);
		close(destination2);
	}
	return 0;
}
