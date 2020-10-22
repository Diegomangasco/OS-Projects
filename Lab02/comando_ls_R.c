#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void visualizzaDirettorio(DIR* dp, char* name){
	struct dirent* dirp;
	struct stat buffer;
	char fullName[257];
	while((dirp = readdir(dp)) != NULL){
		if(strcmp(dirp->d_name, "..")!=0 && strcmp(dirp->d_name, ".")!=0){
			sprintf(fullName, "%s/%s", name, dirp->d_name);
			if(stat(fullName, &buffer)<0){
				printf("Error2\n");
				exit(2);
			}
			if(S_ISDIR(buffer.st_mode)==0)	//stampo file e directory
				printf("\tFile: %s\n", fullName);
			else
				printf("\tDirectory: %s\n", fullName);
		}
	}
	printf("\n");
	return;
}
void ricorsioneSuDirectory(char* name, int* i){
	DIR* dp;
	char fullName [257];
	struct stat buffer;
	struct dirent* dirp;
	if((dp = opendir(name)) == NULL){
		printf("Error1\n");
		exit(1);
	}
	if(*i==0){	//i mi serve solo per un fattore grafico di stampa
		printf(".:\n");
		*i = 1;
	}
	else
		printf(".%s\n", name);
	visualizzaDirettorio(dp, name);
	closedir(dp);	//chiudo la directory per rileggerla da capo e trovare il nuovo direttorio in cui voglio andare
	printf("\n");
	dp = opendir(name);
	while((dirp = readdir(dp)) != NULL){
		if(strcmp(dirp->d_name, "..")!=0 && strcmp(dirp->d_name, ".")!=0){
			sprintf(fullName, "%s/%s", name, dirp->d_name);
			if(stat(fullName, &buffer)<0){
				printf("Error3\n");
				exit(3);
			}
			if(S_ISDIR(buffer.st_mode)!=0){
				ricorsioneSuDirectory(fullName, i);	//ricorro sulla nuova cartella
			}
		}
	}
	closedir(dp);
	return;
}
int main(int argc, char* argv[]){
	int i = 0;
	if(argc != 2){
		printf("Error\n");
		exit(0);
	}
	ricorsioneSuDirectory(argv[1], &i);	
	return 0;
}
