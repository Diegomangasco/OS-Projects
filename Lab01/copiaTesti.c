#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Errore nel passaggio parametri");
		exit(1);
	}
	
	FILE* fp; FILE* fp2;
	fp = fopen(argv[1], "r"); fp2 = fopen(argv[2], "w");
	char linea [81];
	while(fgets(linea, 80, fp) != NULL){
		fprintf(fp2, "%s", linea);
	}
	fclose(fp); fclose(fp2);
	return 0;
}
