#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#define CHILD 0 

int num_supply = 10; // number of time agent supplies two inredients

// mutexes 
pthread_mutex_t agent = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t smoker_paper = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t smoker_match = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t smoker_tobacco = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 

// generate random number within range [lower, upper]
int random_number(int lower, int upper){
	int rand_num = (rand() % (upper - lower + 1) + 1);
	return rand_num;
}

// function for agent thread
void * agentFunction(){
	int num;
	
	// put all smokers to sleep 
	pthread_mutex_lock(&smoker_paper);
	pthread_mutex_lock(&smoker_match);
	pthread_mutex_lock(&smoker_tobacco); 

	while(num_supply > 0){
		pthread_mutex_lock(&agent);
		pthread_mutex_lock(&lock);

		num = random_number(1,3);
		if(num == 1){
			printf("Agent puts down tobacco and paper on table\n");
			// wake up smoker with match
			pthread_mutex_unlock(&smoker_match);
		}
		else if(num == 2){
			printf("Agent puts down tobacco and match on table\n");
			// wake up smoker with paper
			pthread_mutex_unlock(&smoker_paper);
		}
		else{
			printf("Agent puts down match and paper on table\n");
			// wake up smoker with tobacco
			pthread_mutex_unlock(&smoker_tobacco);
		}
		num_supply--; 
		pthread_mutex_unlock(&agent);
		pthread_mutex_unlock(&lock);
		pthread_mutex_lock(&agent); // put agent to sleep
	}
}

// function for smoker with paper thread
void * smokerPaperFunction(){
	while(num_supply > 0){
		pthread_mutex_lock(&smoker_paper); // checks if smoker with paper can go
		pthread_mutex_lock(&lock);
		printf("Smoker picks up tobacco and match\n");
		printf("Smoker with paper is smoking...\n\n");
		pthread_mutex_unlock(&agent); // wake up agent
		pthread_mutex_unlock(&lock);
	}
}

// function for smoker with match thread
void * smokerMatchFunction(){
	while(num_supply > 0){
		pthread_mutex_lock(&smoker_match); // checks if smoker with match can go
		pthread_mutex_lock(&lock);
		printf("Smoker picks up tobacco and paper\n");
		printf("Smoker with match is smoking...\n\n");
		pthread_mutex_unlock(&agent); // wake up agent
		pthread_mutex_unlock(&lock); 
	}
}

// function for smoker with tobacco thread
void * smokerTobaccoFunction(){
	while(num_supply > 0){
		pthread_mutex_lock(&smoker_tobacco); // checks if smoker with tobacco can go
		pthread_mutex_lock(&lock);
		printf("Smoker picks up match and paper\n");
		printf("Smoker with tobacco is smoking...\n\n");
		pthread_mutex_unlock(&agent); // wake up agent
		pthread_mutex_unlock(&lock); 
	}
}

int main(){
	srand(time(0)); 

	void * (* functions[4]) ();
	functions[0] = agentFunction;
	functions[1] = smokerMatchFunction;
	functions[2] = smokerPaperFunction;
	functions[3] = smokerTobaccoFunction;

	// allocate memory for thread
	pthread_t thread[4];

	for(int i = 0; i < 4; i++){
		// create thread in order of agent, match, paper, and tobacco
		pthread_create(&thread[i], NULL, functions[i], NULL);
	}
	
	pthread_join(thread[0], NULL); // wait for agent to finish

	return 0; 
}
