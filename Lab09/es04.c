#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

sem_t semaphore1, semaphore2, semaphore3, semaphore4, semStart, semReturn;

void* T(void *L){
	char *c, d;
	c = (char *) L;
	d = *c;
	int val;
	switch(d){
		case 'A':
			while(1){
				sem_wait(&semReturn);	//A al primo giro passa, poi aspetta
				printf("A\n");
				sem_post(&semStart);
				sem_post(&semStart);
				sem_post(&semStart);	//sveglio B, C, D
			}
			break;
		case 'B':
			while(1){
				sem_wait(&semStart);
				printf("B\n");
				sem_post(&semaphore1);	//sveglio I
			}
			break;
		case 'C':
			while(1){
				sem_wait(&semStart);
				printf("C\n");
				sem_post(&semaphore2);
				sem_post(&semaphore2);	//sveglio E ed F
			}
			break;
		case 'E':
			while(1){
				sem_wait(&semaphore2);
				printf("E\n");
				sem_post(&semaphore4);	//sveglio G
			}
			break;
		case 'F':
			while(1){
				sem_wait(&semaphore2);
				printf("F\n");
				sem_post(&semaphore4);	//sveglio G
			}
			break;
		case 'G':
			while(1){
				sem_wait(&semaphore4);
				sem_wait(&semaphore4);
				printf("G\n");
				sem_post(&semaphore1);	//sveglio I
			}
			break;
		case 'D':
			while(1){
				sem_wait(&semStart);
				printf("D\n");
				sem_post(&semaphore3);	//sveglio H
			}
			break;
		case 'H':
			while(1){
				sem_wait(&semaphore3);
				printf("H\n");
				sem_post(&semaphore1); //sveglio I
			}
			break;
		case 'I':
			while(1){
				sem_wait(&semaphore1);	//aspetto B
				sem_wait(&semaphore1);	//aspetto H
				sem_wait(&semaphore1);	//aspetto G
				printf("I\n");
				sem_post(&semReturn);	//sveglio A
			}
			break;
	}
	pthread_exit(NULL);
}

int main(){
	sem_init(&semaphore1, 0, 0);
	sem_init(&semaphore2, 0, 0);
	sem_init(&semaphore3, 0, 0);
	sem_init(&semaphore4, 0, 0);
	sem_init(&semStart, 0, 0);
	sem_init(&semReturn, 0, 1);
	int i, rc;
	void *status;
	char argomenti[9] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};
	pthread_t vettoreTid[9];
	for(i = 0; i<9; i++){
		rc = pthread_create(&vettoreTid[i], NULL, T, (void *) &argomenti[i]);
		if(rc){
			fprintf(stderr, "Error in pthread_create");
			exit(-1);
		}
	}
	for(i = 0; i<9; i++){
		pthread_join(vettoreTid[i], &status);
		if(rc){
			fprintf(stderr, "Error in pthread_join");
			exit(-1);
		}
	}
	return 0;
}
