#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int countCommand(char * command){
	int count = 0; 
	int state = 0; 

	for(int i = 0; command[i] != '\0'; i++){
		if(command[i] == ' ' || command[i] == '\n' || command[i] == '\t')
			state = 0; 
		else if (state == 0){
			state = 1; 
			count++;
		}
	}
	return count; 
}

int main(){
/*
	char command[100];
	printf("Enter command: ");
	scanf("%[^\n]s", command);

	int count = countCommand(command);
	char * arg[count + 1];
	int i = 0; 

	char *ptr = strtok(command, " ");
	while(ptr != NULL){
		arg[i] = ptr; 
		i++;
		ptr = strtok(NULL, " ");
	}
	arg[count] = NULL;
	execvp(arg[0], arg);
*/
		char command[100];
		printf("Enter command: ");
		scanf("%[^\n]s", command);
		char quit[] = "quit";

	while(strcmp(command, "quit") != 0){
		int child; 
		child = fork();
		if(child == -1){ // failed to fork child process
			printf("Failed to fork child\n");
			return 1;
		}
		if(child == 0){ // child process
			int count = countCommand(command); 
			char * arg[count + 1];
			int i = 0; 
			char *ptr = strtok(command, " ");
			while(ptr != NULL){
				arg[i] = ptr; 
				i++; 
				ptr = strtok(NULL, " ");
			}
			arg[count] = NULL;
			execvp(arg[0], arg);
		}
		else{ // parent process
			wait(NULL);
			printf("Enter command: ");
			scanf(" %[^\n]s", command);
		}
//		memset(command, 0, 100);
	}
	return 0;
}
