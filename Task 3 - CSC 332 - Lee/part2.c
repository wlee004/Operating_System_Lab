#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	int child; 
	child = fork(); 
	if(child == -1){
		printf("Faield to fork first child\n");
		return 1; 
	}
	if(child == 0){ // child process 
		char *arg[3];
		arg[0] = "ls";
		arg[1] = "-al"; 
		arg[2] = NULL;
		printf("Successful fork of child pid: %d\n", getpid());
		execvp("ls", arg);
	}
	else{ // parent process 
		int status; 
		wait(NULL);
	}
	return 0; 
}
