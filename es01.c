#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void binary(int *, int);

int main (int argc, char *argv[]){
  	int n;
  	int *vet;

  	n = atoi (argv[1]);

  	vet = (int *)malloc(n * sizeof(int));
  	if (vet == NULL) {
    		printf("Allocatin Error.\n");
		exit(1);
  	}

  	printf("Binary Numbers:\n");
  	binary(vet, n);
	wait(NULL);
  	free(vet);
	sleep(1);
  	return 0;
}

void binary (int *vet, int n){
	int j = 0, i, pid;
	int counter = 1;
	for(i = 0; i<n; i++)
		counter = counter*2;
	counter--;
	int pidPunta = getpid();
	fprintf(stdout, "\n");
	while(1){
		pid = fork();
		if(pid == 0)
			vet[j] = 0;
		else 
			vet[j] = 1;
		j++;
		if(j==n){
			for(i = 0; i<n; i++)
				fprintf(stdout, "%d ", vet[i]);
			fprintf(stdout, "\n");
			break;
		}
	}
	if(pidPunta == getpid()){
		for(j = 0; j<counter; j++)
			wait(NULL);
		return;
	}
	exit(0);
}



