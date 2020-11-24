#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define N 80

struct argomento{
	char str1[N];
	char str2[N];
	int tid;
};

void* ordinatore(void* arg){
	struct argomento* a, b;
	int n, i, j;
	a = (struct argomento*) arg;
	b = *a;
	FILE *fp1, *fp2;
	char linea[N];
	fp1 = fopen(b.str1, "r");
	fp2 = fopen(b.str2, "w");
	if(fp1 == NULL){
		fprintf(stderr, "Errore nell'apertura del file di lettura\n");
		exit(3);
	}
	if(fp2 == NULL){
		fprintf(stderr, "Errore nell'apertura del file di scrittura\n");
		exit(4);
	}
	fgets(linea, N, fp1);
	n = atoi(linea);
	int vett[n];
	for(i = 0; i<n; i++)
		fscanf(fp1, "%d\n", &vett[i]);
	fclose(fp1);
	int max, t;
	for(i = 0; i<n; i++){	//selection sort per ordinare
		max = -1;
		for(j = 0; j<n-i; j++){
			if(vett[j] > max){
				max = vett[j];
				t = j;
			}
		}
		j--;
		vett[t] = vett[j];
		vett[j] = max;
	}
			
	for(i = 0; i<n; i++)
		fprintf(fp2, "%d\n", vett[i]);
	fclose(fp2);
	long int ret = (long int) b.tid;
	pthread_exit((void *) ret);
}
		
int main(int argc, char* argv[]){
	if(argc != 4){
		fprintf(stderr, "Errore nel passaggio dei parametri\n");
		exit(0);
	}
	int n = atoi(argv[1]);
	int i, rc;
	void *status;
	pthread_t vettorePid [n];
	char** fileInput = malloc(n * sizeof(char*));
	for(i = 0; i<n; i++){
		fileInput[i] = malloc(N*sizeof(char));
		sprintf(fileInput[i], "%s%d.txt", argv[2], i+1);
	}
	char** fileOutput = malloc(n*sizeof(char*));
	for(i = 0; i<n; i++){
		fileOutput[i] = malloc(N*sizeof(char));
		sprintf(fileOutput[i], "%s%d.txt", argv[3], i+1);
	}
	struct argomento arg[n];	
	//ho bisogno di una struttura, dato che la pthread_create accetta funzoni che hanno un solo argomento
	for(i = 0; i<n; i++){
		strcpy(arg[i].str1, fileInput[i]);
		strcpy(arg[i].str2, fileOutput[i]);
	}
	//ho creato due vettori di stringhe per avere tutti i nominativi dei file di input e di output 
	for(i = 0; i<n; i++){
		arg[i].tid = i;
		rc = pthread_create(&vettorePid[i], NULL, ordinatore, (void*) &arg[i]);
		if(rc){
			fprintf(stderr, "Errore nella creazione del thread\n");
			exit(1);
		}
	}
	for(int i = 0; i<n; i++){
		rc = pthread_join(vettorePid[i], &status);
		if(rc){
			fprintf(stderr, "Errore nella cancellazione del thread\n");
			exit(2);
		}
	}	//aspetto la terminazione di tutti i thread
	
	for(i = 0; i<n; i++){
		free(fileInput[i]);
		free(fileOutput[i]);
	}
	free(fileInput);
	free(fileOutput);
	return 0;
}
