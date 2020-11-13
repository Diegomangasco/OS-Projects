#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

void manager(int sig){
	if(sig == SIGCHLD){
		printf("Signal is managed\n");
	}
	return;
}

void bubbleSortC(char* argv){
	int index = open(argv, O_RDWR);
	int n;
	read(index, &n, sizeof(int));
	int i = 1;
	int k, j;
	int val1, val2, tmp;
	int pid;
	int mark = 0;
	for (k=0; k<n-1; k++) {
		for(j=0; j<n-k-1; j++){
			signal (SIGCHLD, manager);
			pid = fork();
			if(pid == 0){
				i = j + 1;	//il primo elemento rappresenta la lunghezza del vettore
				lseek(index, i*sizeof(int), SEEK_SET);
				read(index, &val1, sizeof(int));	//leggo vett[j]
				i++;
				lseek(index, i*sizeof(int), SEEK_SET);
				read(index, &val2, sizeof(int));	//leggo vett[j+1]
				if(val1>val2){
					if(mark == 1) pause();	//aspetto che il processo che sta operando sul vettore finisca
								//se si tratta del primo figlio della scansione a dover operare,
								// opero subito sul vettore
					else mark = 1;
					tmp = val1;		
					val1 = val2;
					val2 = tmp;
					i--;
					lseek(index, i*sizeof(int), SEEK_SET);	//mi riposiziono su vett[j] 
					write(index, &val1, sizeof(int));
					i++;
					lseek(index, i*sizeof(int), SEEK_SET);	//mi riposiziono su vett[j+1] 
					write(index, &val2, sizeof(int));
					kill(pid, SIGCHLD);	//via libera per gli altri processi in pausa
				}
				exit(0);	//il figlio termina			
			}
		}
		for(j = 0; j<n-k-1; j++) wait(NULL);
		mark = 0;
	}
	close(index);
	return;
}

int main(int argc, char* argv[]){
	if(argc!=2){
		fprintf(stderr, "Errore nel passaggio dei paramteri\n");
		exit(0);
	}
	int k = 12;
	int indexF = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(indexF == -1){
		fprintf(stderr, "Errore nell'apertura del file\n");
		exit(1);
	}
	int nr, nw;
	int i;
	nw = write(indexF, &k, sizeof(int));		//scrittura su file
	if(nw == -1){
		fprintf(stderr, "Errore in scrittura\n");
		exit(2);
	}
	int value;
	for(i = 0; i<k; i++){
		value = k-i;
		nw = write(indexF, &value, sizeof(int));
		if(nw == -1){
			fprintf(stderr, "Errore in scrittura\n");
			exit(3);
		}
	}
	close(indexF);
	bubbleSortC(argv[1]);
	return 0;
}
