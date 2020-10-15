#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	
	int child1, child2; 
	
	// fork first child 
	child1 = fork();
	if(child1 == -1){ 
		printf("Failed to fork first child\n");
		return 1;
	}
	if(child1 == 0){ // child 1 process
		char *arg[2];
		arg[0] = "Process_P1";
		arg[1] = NULL;
		execv("Process_P1",arg);
	}
	else{
		sleep(1);
		child2 = fork();
		if(child2 == -1){
			printf("Failed to fork second child\n");
			return 1;
		}
		if(child2 == 0){ // child 2 process 
			char * arg2[2];
			arg2[0] = "Process_P2";
			arg2[1] = NULL;
			execv("Process_P2", arg2);
		}
		else{ // parent process
		int status1, status2; 
		waitpid(child1, &status1, 0);
		waitpid(child2, &status2, 0);
		}
	}
	

	return 0; 
}
