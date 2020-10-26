#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


// global variables for this function 
const int X = 2; // number of chapters from textbook 
const int Y = 2; // number of quiz per chapter 
const int student = 10; // number of student (rows) in the data 

float averageColumn(int ** data, int row, int col){
	float average = 0; 
	for(int i = 0; i < student; i++){
		average += data[i][col];
	}
	return average / student; 
}

int main(){
	FILE * fp; 
	int column = X * Y;
	int ** data = malloc(student * sizeof(*data));
	for(int i = 0; i < student; i++){
		data[i] = malloc(column * sizeof(data[0]));
	}

	fp = fopen("quiz_grades.txt", "r");
	
	if(fp == NULL){ // failed to open file 
		fclose(fp);
		exit(EXIT_FAILURE);
	}
	
	// put quiz grades from text file into 2d data array
	for(int i = 0; i < student; i++){
		for(int j = 0; j < column; j++){
			int grade;
			fscanf(fp, "%d", &grade);
			data[i][j] = grade; 
		}
	}
	fclose(fp);

	for(int i = 0; i < X; i++){ // create X manager processes (1 per chapter)
		if(fork() == 0){
			for(int j = 0; j < Y; j++){ // create Y worker processes (1 per quiz)
				if(fork() == 0){
					printf("Average grades for Chapter %d Quiz %d: %0.2f\n", i+1, j+1, averageColumn(data, student, i+i+j));
					exit(0);
				}
			}
			for(int j = 0; j < 2; j++){ // manager process wait for worker process 
				wait(NULL);
			}
			exit(0);
		}
	}

	for(int i = 0; i < 2; i++){ // director process wait for manager process
		wait(NULL);
	}

	// freeing up allocated memeory for the array
	for(int i = 0; i < student; i++){
		free(data[i]);
	}
	free(data);

	return 0; 
}
