#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){

	int child_1, child_2;
	child_1 = fork();
	if(child_1 == -1){
		printf("Failed to fork first child\n");
		return 1;
	}
	if (child_1 == 0){ // child 1 process print child 1 pid
		printf("I am child one, my pid is: %d\n", getpid());	
	}
	else{
		child_2 = fork(); 
		if(child_2 == -1){
			printf("Failed to fork second child\n");
			return 1;
		}
		if(child_2 == 0){ // child 2 process print child 2 pid
			printf("I am child one, my pid is: %d\n", getpid());	
		}
		else{ // parent process wait for both child process
			int status1;
			int status2; 
			waitpid(child_1, &status1, 0);
			waitpid(child_2, &status2, 0);
		}
	}
	return 0;
}
