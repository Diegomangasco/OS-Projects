#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define N 80

int **matrice;
char **str1;

void* ordinatore(void *i){
	int *p, t, n;
	p = (int *) i;
	t = *p;
	FILE *fp1;
	char linea[N];
	int k;
	fp1 = fopen(str1[t], "r");
	if(fp1 == NULL){
		fprintf(stderr, "Errore nell'apertura del file di lettura\n");
		exit(3);
	}
	fgets(linea, N, fp1);
	n = atoi(linea);
	matrice[t] = malloc(n*sizeof(int));
	for(k = 0; k<n; k++)
		fscanf(fp1, "%d\n", &matrice[t][k]);
	fclose(fp1);
	int max, q, j;
	for(k = 0; k<n; k++){	//selection sort per ordinare
		max = -1;
		for(j = 0; j<n-k; j++){
			if(matrice[t][j] > max){
				max = matrice[t][j];
				q = j;
			}
		}
		j--;
		matrice[t][q] = matrice[t][j];
		matrice[t][j] = max;
	}
	for(k = 0; k<n; k++) printf("%d ", matrice[t][k]);
	printf("\n");
	pthread_exit(NULL);
}

void merge(int *vettA, int *vettB, int l, int q, int r){
	int i, j, k;
	i = l;
	j = q+1;
	for(k = l; k<r; k++){
		if(i>q)
			vettB[k] = vettA[j++];
		else if (j>r)
			vettB[k] = vettA[i++];
		else if (vettA[i] <= vettA[j])
			vettB[k] = vettA[i++];
		else
			vettB[k] = vettA[j++];
	}
	for(k = l; k<r; k++)
		vettA[k] = vettB[k];
	return;
}		
		
void unione(int *vettoreMerge, int *vettore, int length, int counter){
	int i;
	for(i = 0; i<length; i++)
		vettoreMerge[i+counter] = vettore[i];
	return;
}

int main(int argc, char* argv[]){
	if(argc != 5){
		fprintf(stderr, "Errore nel passaggio dei parametri\n");
		exit(0);
	}
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	int i, rc;
	void *status;
	matrice = malloc(n*sizeof(int *));
	int vettoreMerge[n*N];
	int vettoreSupporto[n*N];
	pthread_t vettoreTid [n];
	str1 = malloc(n*sizeof(char *));
	for(i = 0; i<n; i++){
		str1[i] = malloc(N*sizeof(char));
		sprintf(str1[i], "%s%d.txt", argv[3], i+1);
	}
	char* fileOutput = malloc(N*sizeof(char));
	sprintf(fileOutput, "%s.txt", argv[4]);
	int vettore[n]; 
	for(i = 0; i<n; i++){
		vettore[i] = i;
		rc = pthread_create(&vettoreTid[i], NULL, ordinatore, (void *) &vettore[i]);
		if(rc){
			fprintf(stderr, "Errore nella creazione del thread\n");
			exit(1);
		}
	}
	int j;
	int counter = 0;
	for(int i = 0; i<n; i++){
		rc = pthread_join(vettoreTid[i], &status);
		if(rc){
			fprintf(stderr, "Errore nella cancellazione del thread\n");
			exit(2);
		}
		unione(vettoreMerge, matrice[i], m, counter);
		counter+=m;
		merge(vettoreMerge, vettoreSupporto, 0, (i*m) -1, counter);
	}	//aspetto la terminazione di tutti i thread
	FILE *fp2 = fopen(fileOutput, "w");
	i = 0;
	while(i<counter){
		fprintf(fp2, "%d\n", vettoreMerge[i]);
		i++;
	}
	fclose(fp2);
	for(i = 0; i<n; i++){
		free(matrice[i]);
		free(str1[i]);
	}
	free(str1);
	free(matrice);
	free(fileOutput);
	
	return 0;
}
