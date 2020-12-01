#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "sem.h"

#define CHILD 0 

int random_number(int lower, int upper){
	int rand_num = (rand() % (upper - lower + 1) + 1);
	return rand_num;
}

int main(){

	int smoker_paper, smoker_tobacco, smoker_match, agent, lock, num; 
	int num_supply = 10; // number of time agent supplies two inredients
	int pid; // Process ID after fork call

	// creating semaphores
	if((smoker_paper = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1){
		printf("Failed to create semaphore");
		exit(1);
	}
	sem_create(smoker_paper, 0);

	if((smoker_tobacco = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1){
		printf("Failed to create semaphore");
		exit(1);
	}
	sem_create(smoker_tobacco, 0);

	if((smoker_match = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1){
		printf("Failed to create semaphore");
		exit(1);
	}
	sem_create(smoker_match, 0);

	if((agent = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1){
		printf("Failed to create semaphore");
		exit(1);
	}
	sem_create(agent, 0);

	if((lock = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1){
		printf("Failed to create semaphore");
		exit(1);
	}
	sem_create(lock, 1);

	srand(time(0)); 

	if ((pid = fork()) == -1) 
	{
		//fork failed!
		perror("fork");
		exit(1);
	}
	if (pid == CHILD){ // smoker_paper process
		for(;;){
			P(smoker_paper); // put smoker_paper to sleep
			P(lock);
			printf("Smoker picks up tobacco and match\n");
			printf("Smoker with paper is smoking...\n\n");
			V(agent); // wake up agent
			V(lock);
		}
	}
	else{
		if ((pid = fork()) == -1) 
		{
			//fork failed!
			perror("fork");
			exit(1);
		}
		if (pid == CHILD){ // smoker_match process
			for(;;){
				P(smoker_match); // put smoker_match to sleep
				P(lock);
				printf("Smoker picks up tobacco and paper\n");
				printf("Smoker with match is smoking...\n\n");
				V(agent); // wake up agent
				V(lock);
			}
		}
		else{
			if ((pid = fork()) == -1) 
			{
				//fork failed!
				perror("fork");
				exit(1);
			}
			if (pid == CHILD){ // smoker_tobacco process
				for(;;){
					P(smoker_tobacco); // put smoker_paper to sleep
					P(lock);
					printf("Smoker picks up match and paper\n");
					printf("Smoker with tobacco is smoking...\n\n");
					V(agent); // wake up agent
					V(lock);
				}
			}
			else{
				//agent process 
				for(int i = 0; i < num_supply; i++){
				P(lock);
					num = random_number(1,3); // generate a random number from range 1 - 3
					if(num == 1){
						printf("Agent puts down tobacco and paper on table\n");
						V(smoker_match); // wakes up smoker with match 
					}
					else if(num = 2){
						printf("Agent puts down tobacco and match on table\n");
						V(smoker_paper); // wakes up smoker with paper
					}
					else{
						printf("Agent puts down match and paper on table\n");
						V(smoker_tobacco); // wakes up smoker with tobacco
					}
				V(lock);
				P(agent); // put agent to sleep
				}
			}
			exit(0);
		}
		exit(0);
	}
	exit(0);
}
