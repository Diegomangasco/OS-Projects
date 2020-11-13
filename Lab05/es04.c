#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#define N 100

void manager(int signo){
	if(signo != SIGUSR1 && signo != SIGUSR2)
		fprintf(stderr, "Errore nel segnale\n");
	return;
}

int main(int argc, char* argv[]){
	if(argc != 2){
		fprintf(stderr, "Errore nel passaggio dei parametri\n");
		exit(1);
	}
	int pid;
	int indexF;
	char input[N];
	char output[N];
	char fullName[N];
	int i, n, flag = 0;
	indexF = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(indexF == -1){
		fprintf(stderr, "Errore nella lettura del file\n");
		exit(2);
	}
	signal(SIGUSR1, manager);
	signal(SIGUSR2, manager);
	pid = fork();
	while(1){
		if(pid == 0){		
			sleep(1);
			fprintf(stdout, "Inserisci i caratteri: ");
			fgets(input, N, stdin);
			if(strcmp(input, "end\n") == 0){ 
				//gestire segnale per dire al padre di terminare
				fprintf(stdout, "Arrivederci\n");
				kill(getppid(), SIGINT);
				exit(0);
			}
			n = write(indexF, input, strlen(input));
			lseek(indexF, -strlen(input), SEEK_CUR);	//mi posiziono per leggere gli ultimi caratteri scritti
			kill(getppid(), SIGUSR1);
			pause();
		}
		else{			
			pause();
			n = read(indexF, output, N);
			i = 0;
			while(i<n){		//il buffer è ancora occupato in parte dalla stringa precedente, per questo ciclo fino ad n
				output[i] = toupper(output[i]);
				fprintf(stdout, "%c", output[i]);
				i++;
			}
			sleep(1);
			kill(pid, SIGUSR2);
		}
	}
	close(indexF);
	return 0;
}
