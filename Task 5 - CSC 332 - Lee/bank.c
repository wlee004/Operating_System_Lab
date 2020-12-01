#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "sem.h"

#define CHILD      			0  			/* Return value of child proc from fork call */
#define TRUE       			0  
#define FALSE      			1

FILE *fp1, *fp2, *fp3, *fp4, *fpdad, *fpson1, *fpson2;	/* File Pointers */

int main()
{
	int pid;							   // Process ID after fork call
	int i;								   // Loop index
	int N;								   // Number of times dad does update
	int N_Att;							   // Number of time sons allowed to do update
	int status;							   // Exit status of child process
	int bal1, bal2;					       // Balance read by processes
	int flag, flag1;					   // End of loop variables
	int transaction;  		               // semaphore id  
	int dad_time, son1_time, son2_time;    // time counter          

	// creating semaphore 
	if((transaction = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1){
		printf("Failed to create semaphore");
		exit(1);
	}
	sem_create(transaction, 1);
	
	//Initialize the file balance to be $100
	fp1 = fopen("balance","w");
	bal1 = 100;
	fprintf(fp1, "%d\n", bal1);
	fclose(fp1);
	
	//Initialize the number of attempts to be 20
	fp4 = fopen("attempt", "w");
	N_Att = 20;
	fprintf(fp4, "%d\n", N_Att);
	fclose(fp4);

	//Initialize the time for dad 
	fpdad = fopen("dad_time", "w");
	dad_time = 0;
	fprintf(fpdad, "%d\n", dad_time);
	fclose(fpdad);

	//Initialize the time for son 1
	fpson1 = fopen("son1_time", "w");
	son1_time = 0;
	fprintf(fpson1, "%d\n", son1_time);
	fclose(fpson1);

	//Initialize the time for son 2  
	fpson2 = fopen("son2_time", "w");
	son2_time = 0;
	fprintf(fpson2, "%d\n", son2_time);
	fclose(fpson2);

	//Create child processes that will do the updates
	if ((pid = fork()) == -1) 
	{
		//fork failed!
		perror("fork");
		exit(1);
	}
	
	if (pid == CHILD){
	//First Child Process. Dear old dad tries to do some updates.

		N=5;
		for(i=1;i<=N; i++)
		{   
			P(transaction); // start critical section 

			fp3 = fopen("attempt" , "r+");
			fscanf(fp3, "%d", &N_Att);
						
			// increment son 1 wait time 
			if(N_Att > 1){
				fpson1 = fopen("son1_time", "r+");
				fscanf(fpson1,"%d", &son1_time);
				fseek(fpson1,0L, 0);
				son1_time += 1;
				fprintf(fpson1, "%d\n", son1_time);
				fclose(fpson1);
			}

			// increment son 2 wait time 
			if(N_Att > 1){
				fpson2 = fopen("son2_time", "r+");
				fscanf(fpson2,"%d", &son2_time);
				fseek(fpson2,0L, 0);
				son2_time += 1;
				fprintf(fpson2, "%d\n", son2_time);
				fclose(fpson2);
			}
			
			printf("Dear old dad is trying to do update.\n");
			fp1 = fopen("balance", "r+");
			fscanf(fp1, "%d", &bal2);
			printf("Dear old dad reads balance = %d \n", bal2);
			
			//Dad has to think (0-14 sec) if his son is really worth it
			sleep(rand()%15);
			fseek(fp1,0L,0);
			bal2 += 60;
			printf("Dear old dad writes new balance = %d \n", bal2);
			fprintf(fp1, "%d \n", bal2);
			fclose(fp1);

			printf("Dear old dad is done doing update. \n");

			V(transaction); // end critcal section 

			sleep(rand()%5);	/* Go have coffee for 0-4 sec. */	
		}
	}
	
	else
	{
		//Parent Process. Fork off another child process.
		if ((pid = fork()) == -1)
		{
			//Fork failed!
			perror("fork");
			exit(1);
		}
		if (pid == CHILD)
		{
			printf("First Son's Pid: %d\n",getpid());
			//Second child process. First poor son tries to do updates.
			flag = FALSE;
			while(flag == FALSE) 
			{	
				P(transaction); // start critical section 

				fp3 = fopen("attempt" , "r+");
				fscanf(fp3, "%d", &N_Att);
				
				// increment son 2 wait time 
				if(N_Att > 1){
					fpson2 = fopen("son2_time", "r+");
					fscanf(fpson2,"%d", &son2_time);
					fseek(fpson2,0L, 0);
					son2_time += 1;
					fprintf(fpson2, "%d\n", son2_time);
					fclose(fpson2);
				}

				// increment dad wait time 
				fpdad = fopen("dad_time", "r+");
				fscanf(fpdad,"%d", &dad_time);
				fseek(fpdad,0L, 0);
				dad_time += 1;
				fprintf(fpdad, "%d\n", dad_time);
				fclose(fpdad);

				
				if(N_Att == 0)
				{
					fclose(fp3);
					flag = TRUE;
				}
				else
				{
					printf("Poor SON_1 wants to withdraw money.\n");
					fp2 = fopen("balance", "r+");
					fscanf(fp2,"%d", &bal2);
					printf("Poor SON_1 reads balance. Available Balance: %d \n", bal2);
					if (bal2 == 0)
					{
						fclose(fp2);
						fclose(fp3);
					}
					else
					{
						sleep(rand()%5);
						fseek(fp2,0L, 0);
						bal2 -=20;
						printf("Poor SON_1 write new balance: %d \n", bal2);
						fprintf(fp2,"%d\n", bal2);
						fclose(fp2);
						printf("poor SON_1 done doing update.\n");
						fseek(fp3,0L, 0);
						N_Att -=1;
						fprintf(fp3, "%d\n", N_Att);
						fclose(fp3);
					}
				}
				V(transaction); // end critical section 
			}
		}
		else
		{
		//Parent Process. Fork off one more child process.
			if ((pid = fork()) == -1) 
			{
				//fork failed!
				perror("fork");
				exit(1);
			}
			if (pid == CHILD)
			{
				printf("Second Son's Pid: %d\n",getpid());
				//Third child process. Second poor son tries to do updates.
				flag1 = FALSE;
				while(flag1 == FALSE) 
				{
					P(transaction); // start critical section 

					fp3 = fopen("attempt" , "r+");
					fscanf(fp3, "%d", &N_Att);

					// increment son 1 wait time 
					if(N_Att > 1){
						fpson1 = fopen("son1_time", "r+");
						fscanf(fpson1,"%d", &son1_time);
						fseek(fpson1,0L, 0);
						son1_time += 1;
						fprintf(fpson1, "%d\n", son1_time);
						fclose(fpson1);
					}
					

					// increment dad wait time 
					fpdad = fopen("dad_time", "r+");
					fscanf(fpdad,"%d", &dad_time);
					fseek(fpdad,0L, 0);
					dad_time += 1;
					fprintf(fpdad, "%d\n", dad_time);
					fclose(fpdad);

					
					if(N_Att == 0)
					{
						fclose(fp3);
						flag1 = TRUE;
					}
					else
					{
						printf("Poor SON_2 wants to withdraw money.\n");
						fp2 = fopen("balance", "r+");
						fscanf(fp2,"%d", &bal2);
						printf("Poor SON_2 reads balance. Available Balance: %d \n", bal2);
						if (bal2 == 0)
						{
							fclose(fp2);
							fclose(fp3);
						}
						else
						{
							sleep(rand()%5);
							fseek(fp2,0L, 0);
							bal2 -=20;
							printf("Poor SON_2 write new balance: %d \n", bal2);
							fprintf(fp2,"%d\n", bal2);
							fclose(fp2);

							printf("poor SON_2 done doing update.\n");
							fseek(fp3,0L, 0);
							N_Att -=1;
							fprintf(fp3, "%d\n", N_Att);
							fclose(fp3);
						}
					}
					V(transaction); // end critical section
				}
			}
			else
			{
				//Now parent process waits for the child processes to finish
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);

				fpdad = fopen("dad_time", "r+");
				fscanf(fpdad,"%d", &dad_time);
				fclose(fpdad);

				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);

				fpson1 = fopen("son1_time", "r+");
				fscanf(fpson1,"%d", &son1_time);
				fclose(fpson1);
			
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);

				fpson2 = fopen("son2_time", "r+");
				fscanf(fpson2,"%d", &son2_time);
				fclose(fpson2);

				printf("Process Wait Time\n");
				printf("N_Att: %d\n", N_Att);
				printf("DAD: %d times\n", dad_time);
				printf("SON 1: %d times\n", son1_time);
				printf("SON 2: %d times\n", son2_time);

				semkill(transaction);

			}
			exit(0);
		}
		exit(0);
	}
	exit(0);
}
