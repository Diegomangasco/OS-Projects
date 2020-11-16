#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

struct pipeline{
	int fd[2];
};

void manager(int sig){
	if(sig != SIGUSR1){
		printf("Signal error\n");
	}
	return;
}

void bubbleSortC(char* argv){
	int index = open(argv, O_RDWR);
	int n;
	read(index, &n, sizeof(int));
	int k, j;
	int val1, val2, tmp;
	int pid;
	signal(SIGUSR1, manager);
	for (k=0; k<n-1; k++) {
		int p [n-k-1];
		for(j=0; j<n-k-1; j++){
			pid = fork();
			if(pid > 0) p[j] = pid;	//memorizzo il pid del figlio
			if(pid == 0){
				pause();
				lseek(index, (j+1)*sizeof(int), SEEK_SET);
				read(index, &val1, sizeof(int));	//leggo vett[j]
				read(index, &val2, sizeof(int));	//leggo vett[j+1]
				if(val1>val2){
					tmp = val1;		
					val1 = val2;
					val2 = tmp;
					lseek(index, (j+1)*sizeof(int), SEEK_SET);	//mi riposiziono su vett[j] 
					write(index, &val1, sizeof(int));
					write(index, &val2, sizeof(int));
				}
				exit(0);		//il figlio termina			
			}
		}
		sleep(2);	//permetto a tutti i figli di andare in pausa
		for(j = 0; j<n-k-1; j++){
			kill(p[j], SIGUSR1);
			wait(NULL);	//prima di svegliare il prossimo figlio aspetto che il vecchio abbia terminato
		}
	}
	close(index);
	return;
	/*in questa soluzione il padre genera tutti i figli, per poi svegliarne uno alla volta, il quale eseguirà la sua porzione di codice*/
}

/*SOLUZIONE ALTERNATIVA SFRUTTANDO LE PIPE*/
/*
	int flag = 1;
	for (k=0; k<n-1; k++) {
		struct pipeline p [n-k+1];
		for(j=0; j<n-k-1; j++){
			pipe(p[j].fd);
			pid = fork();
			if(pid == 0){
				close(p[j].fd[1]);	//non scrive
				read(p[j].fd[0], &flag, sizeof(int));	//pausa finchè il padre non scrive sulla pipe
				lseek(index, (j+1)*sizeof(int), SEEK_SET);
				read(index, &val1, sizeof(int));	//leggo vett[j]
				read(index, &val2, sizeof(int));	//leggo vett[j+1]
				if(val1>val2){
					tmp = val1;		
					val1 = val2;
					val2 = tmp;
					lseek(index, (j+1)*sizeof(int), SEEK_SET);	//mi riposiziono su vett[j] 
					write(index, &val1, sizeof(int));
					write(index, &val2, sizeof(int));
				}
				exit(0);		//il figlio termina			
			}
		}
		sleep(2);	//permetto a tutti i figli di andare in read
		for(j = 0; j<n-k-1; j++){
			close(p[j].fd[0]);	//non scrive
			write(p[j].fd[1], &flag, sizeof(int));	//scrivo sulla pipe del figlio j
			wait(NULL);
		}
	}
	close(index);
	return;
}	
*/
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
