#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	int a = 10, b = 25, fq = 0, fr = 0;
	fq = fork();
	if(fq == 0){ // child process Q successfully forked
		a = a + b;
		// print values of a, b, and process_id
		printf("fq_a = %d\nfq_b = %d\nfq_pid = %d\n",a ,b , getpid());

		fr = fork(); 
		if(fr != 0){
			b = b + 20; 
			// print values of a, b, and process_id
			printf("a = %d\nb = %d\npid = %d\n",a ,b , getpid());
		}
		else{
			a = (a*b)+30;
			//print values of a, b, and process_id
			printf("fr_a = %d\nfr_b = %d\nfr_pid = %d\n",a ,b , getpid());
		}
	}
	else{
		b = a+b-5;
		// print values of a, b, and process_id
		printf("P_a = %d\nP_b = %d\nP_pid = %d\n",a ,b , getpid());
	}
	return 0;
}
