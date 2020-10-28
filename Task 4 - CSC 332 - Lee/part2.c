#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


// global variables for this function 
const int X = 2; // number of chapters from textbook 
const int Y = 2; // number of hw per chapter 

int countRow(FILE * fp){
	int count = 0;
	char c; 
	while(fscanf(fp, "%c", &c) != EOF	){
		if(c == '\n'){
			count++; 
		}
	}
	fseek(fp, 0, SEEK_SET); // reset file pointer 
	return count; 
}

float averageColumn(int ** data, int row, int col){
	float average = 0; 
	for(int i = 0; i < row; i++){
		average += data[i][col];
	}
	return average / row; 
}

int main(){
	FILE * fp; 
	int column = X * Y;

	fp = fopen("hw_grades.txt", "r");
	if(fp == NULL){ // failed to open file 
		fclose(fp);
		exit(EXIT_FAILURE);
	}

	int student = countRow(fp);

	// allocate memory to 2d array to put hw grades in
	int ** data = malloc(student * sizeof(*data));
	for(int i = 0; i < student; i++){
		data[i] = malloc(column * sizeof(data[0]));
	}

	// put hw grades from text file into 2d data array
	for(int i = 0; i < student; i++){
		for(int j = 0; j < column; j++){
			fscanf(fp, "%d", &data[i][j]);
		}
	}
	fclose(fp);

	for(int i = 0; i < X; i++){ // create X manager processes (1 per chapter)
		if(fork() == 0){
			for(int j = 0; j < Y; j++){ // create Y worker processes (1 per hw)
				if(fork() == 0){
					printf("Average grades for Chapter %d Hw %d: %0.2f\n", i+1, j+1, averageColumn(data, student, i+i+j));
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
