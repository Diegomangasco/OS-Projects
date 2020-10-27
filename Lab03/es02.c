#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc!=3){
		printf("Error0\n");
		exit(0);
	}
	
	int n = atoi(argv[1]);
	int t = atoi(argv[2]);
	int counter = 0;
	int i;
	pid_t pid;
	for(i = 0; i<2; i++){
		pid = fork();	
		if(pid == 0){	//processo figlio che deve generare due figli
			i = -1;
			counter++;
			if(counter == n){
				sleep(t);
				printf("\nProcesso %d terminato\n", getpid());
				break;
			}
		}
	}
	exit(pid);
	return(0);
}
