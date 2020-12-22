#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#define N 100

sem_t BeginRead, BeginTrans, BeginWrite;
int EndT = 0, EndW = 0;
char RigaLetta[N];

void *functionR(void *arg){
	char *fileR;
	fileR = (char *) arg;
	FILE *fp = fopen(fileR, "r");
	char linea[N+1], c;
	int i;
	while(fgets(linea, N, fp) != NULL){
		i = 1; c = linea[0];
		while(c != '\n'){
			c = linea[i];
			i++;
		}
		i--;
		linea[i] = '\0';	//al posto del \n metto il \0
		sem_wait(&BeginRead);
		strcpy(RigaLetta, linea);	//se l'esercizio fosse esteso a n reader, avrei dovuto aggiungere un semaforo per RigaLetta
		sem_post(&BeginTrans);
	}
	EndT = 1;
	sem_post(&BeginTrans);
	fclose(fp);
	pthread_exit(NULL);
}

void *functionT(){
	int i, counter;
	char c;
	while(1){
		i = 0;
		sem_wait(&BeginTrans);
		if(EndT == 1) break;
		while(RigaLetta[i] != '\0'){	//trasformo in maiuscolo
			toupper(RigaLetta[i]);
			i++;
		}
		counter = i;
		for(i = 0; i<counter/2; i++){	//inverto la riga
			c = RigaLetta[i];
			RigaLetta[i] = RigaLetta[counter-i-1];
			RigaLetta[counter-i-1] = c;
		}
		sem_post(&BeginWrite);
	}
	EndW = 1;
	sem_post(&BeginWrite);
	pthread_exit(NULL);
}

void *functionW(void *arg){
	char *fileW;
	fileW = (char *) arg;
	FILE *fp = fopen(fileW, "w");
	while(1){
		sem_wait(&BeginWrite);	//scrivo su file
		if(EndW == 1) break;
		fprintf(fp, "%s\n", RigaLetta);
		sem_post(&BeginRead);
	}
	fclose(fp);
	pthread_exit(NULL);
}
	
int main(int argc, char *argv[]){
	if(argc!=3){
		fprintf(stderr, "Errore nel passaggio dei parametri\n");
		exit(1);
	}
	pthread_t vettoreTid[3];
	int i, rc;
	void *status;
	sem_init(&BeginRead, 0, 1);
	sem_init(&BeginWrite, 0, 0);
	sem_init(&BeginTrans, 0, 0);
	for(i = 0; i<3; i++){
		switch(i){
		case 0:
			rc = pthread_create(&vettoreTid[i], NULL, functionR, (void *) argv[1]);
			if(rc){
				fprintf(stderr, "Errore nella creazione del thread\n");
				exit(1);
			}
			break;
		case 1:
			rc = pthread_create(&vettoreTid[i], NULL, functionT, NULL);
			if(rc){
				fprintf(stderr, "Errore nella creazione del thread\n");
				exit(1);
			}
			break;
		case 2:
			rc = pthread_create(&vettoreTid[i], NULL, functionW, (void *) argv[2]);
			if(rc){
				fprintf(stderr, "Errore nella creazione del thread\n");
				exit(1);
			}
			break;
		}
	}
	for(i = 0; i<3; i++){
		rc = pthread_join(vettoreTid[i], &status);
		if(rc){
			fprintf(stderr, "Errore nella cattura del thread\n");
			exit(1);
		}
	}
	return 0;
}
