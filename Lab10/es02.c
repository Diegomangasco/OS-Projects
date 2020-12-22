#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

sem_t beginFirst, beginSecond, beginThird, beginSum;
float vettore[3];
FILE *fp;
int finish = 0;

void *FirstM(){
	int i;
	float c, x;
	int n;
	float sum;
	while(1){
		sem_wait(&beginFirst);
		if(fscanf(fp, "%f%f%d\n", &c, &x, &n) == -1){
			finish = 1;
			sem_post(&beginSecond);
			break;
		}
		sem_post(&beginSecond);
		sum = 1.0;
		for(i = 0; i<n; i++){
			if(i == 0) sum = x;
			else sum = sum * x;
		}
		vettore[0] = sum * c;
	}
	pthread_exit(NULL);
}

void *SecondM(){
	int i;
	float c, x, sum;
	int n;
	while(1){
		sem_wait(&beginSecond);
		if(finish == 1){
			sem_post(&beginThird);
			break;
		}
		if(fscanf(fp, "%f%f%d\n", &c, &x, &n) == -1){
			vettore[1] = 0.0;
			sem_post(&beginSecond);
		}
		else{
			sem_post(&beginThird);
			sum = 1.0;
			for(i = 0; i<n; i++){
				if(i == 0) sum = x;
				else sum = sum * x;
			}
			vettore[1] = sum * c;
		}
	}
	pthread_exit(NULL);
}

void *ThirdM(){
	int i;
	float c, x, sum;
	int n;
	while(1){
		sem_wait(&beginThird);
		if(finish == 1){
			sem_post(&beginSum);
			break;
		}
		if(fscanf(fp, "%f%f%d\n", &c, &x, &n) == -1){
			vettore[2] = 0.0;
			sem_post(&beginSum);
		}
		else{
			sem_post(&beginSum);
			sum = 1.0;
			for(i = 0; i<n; i++){
				if(i == 0) sum = x;
				else sum = sum * x;
			}
			vettore[1] = sum * c;
		}
	}
	pthread_exit(NULL);
}

void *SumM(){
	float sum;
	while(1){
		sem_wait(&beginSum);
		if(finish == 1) break;
		sum = vettore[0] + vettore[1] + vettore[2];
		sem_post(&beginFirst);
		fprintf(stdout, "Risultato polinomio: %f\n", sum);
	}
	pthread_exit(NULL);
}
		
int main(int argc, char *argv[]){
	if(argc != 2)
	{
		fprintf(stderr, "Errore nel passaggio di parametri.\n");
		exit(1);
	}
	int i, rc;
	void *status;
	pthread_t vettoreTid[4];
	//convertitore(fp, argv[1]);
	sem_init(&beginFirst, 0, 1);
	sem_init(&beginSecond, 0, 0);
	sem_init(&beginThird, 0, 0);
	sem_init(&beginSum, 0, 0);
	fp = fopen("testo.txt", "r");
	for(i = 0; i<4; i++){
		switch(i){
			case 0:
			rc = pthread_create(&vettoreTid[i], NULL, FirstM, NULL);
			if(rc){
				fprintf(stderr, "Errore nella creazione dei thread.\n");
				exit(1);
			}
			break;
			case 1:
			rc = pthread_create(&vettoreTid[i], NULL, SecondM, NULL);
			if(rc){
				fprintf(stderr, "Errore nella creazione dei thread.\n");
				exit(1);
			}
			break;
			case 2:
			rc = pthread_create(&vettoreTid[i], NULL, ThirdM, NULL);
			if(rc){
				fprintf(stderr, "Errore nella creazione dei thread.\n");
				exit(1);
			}
			break;
			case 3:
			rc = pthread_create(&vettoreTid[i], NULL, SumM, NULL);
			if(rc){
				fprintf(stderr, "Errore nella creazione dei thread.\n");
				exit(1);
			}
			break;
		}
	}
	for(i = 0; i<4; i++){
		rc = pthread_join(vettoreTid[i], &status);
		if(rc){
			fprintf(stderr, "Errore nella cattura dei thread.\n");
			exit(1);
		}
	}
	return 0;
}
			
