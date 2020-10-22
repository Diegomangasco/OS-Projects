#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define S 4096

void copiaCartella(char* nameCartella){
	if(mkdir(nameCartella, 0777)==-1){
		printf("Error3\n");
		exit(3);
	}
	return;
}
void copiaFile(char* fullName2, char* name1){
	int fd1, fd2, nr, nw;
	char buf [S];
	fd1 = open(fullName2, O_WRONLY | O_CREAT, 0777);
	fd2 = open(name1, O_RDONLY);
	
	while((nr = read(fd2, buf, S)) > 0){
		nw = write(fd1, buf, S);
		if(nw!=nr){
			printf("Error in read %d and write %d\n", nr, nw);
		}
	}
	close(fd1);
	close(fd2);
	return;	
}
void ricorsioneSuDirectory(char* name1, char* name2){
	DIR* dp1;
	DIR* dp2;
	char fullName1 [257];
	char fullName2 [257];
	struct stat buffer1;
	struct dirent* dirp1;
	if((dp1 = opendir(name1)) == NULL){
		printf("Error1\n");
		exit(1);
	}
	if((dp2 = opendir(name2)) == NULL){	//apro il direttorio destinazione per verificare che non ci siano errori in apertura
		printf("Error1\n");
		exit(1);
	}
	while((dirp1 = readdir(dp1)) != NULL){
		if(strcmp(dirp1->d_name, "..")!=0 && strcmp(dirp1->d_name, ".")!=0){
			sprintf(fullName1, "%s/%s", name1, dirp1->d_name);
			sprintf(fullName2, "%s/%s", name2, dirp1->d_name);	//path della cartella/file da creare
			if(stat(fullName1, &buffer1)<0){
				printf("Error2\n");
				exit(3);
			}
			if(S_ISDIR(buffer1.st_mode)!=0){
				copiaCartella(fullName2 /*path della nuova cartella*/);
				ricorsioneSuDirectory(fullName1, fullName2);	//ricorro sulla nuova cartella
			}
			else{	
				copiaFile(fullName2 /*path del nuovo file*/, name1 /*path del file da copiare*/);
			}
		}
	}
	closedir(dp1);
	closedir(dp2);
	return;
}
void cutter(char* name, char* string){
	int i = 0;
	int n = 0;
	int j = 0; 
	while(name[i]!='\0')
	{
		if(name[i]=='/')
			n++;
		i++;
	}
	i = 0;
	while(n!=0){
		if(name[i]=='/')
			n--;
		i++;			//una volta trovate tutte le / nel path, mi posiziono dopo l'ultima per creare il direttorio radice del nuovo albero
	}
	while(name[i]!='\0')
		string[j++] = name[i++];
	
	return;
}
int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Error\n");
		exit(0);
	}
	char firstName [257];
	char string [100];
	cutter(argv[1], string);
	sprintf(firstName, "%s/%s", argv[2], string);
	copiaCartella(firstName);	//copio la radice dell'albero di ricorsione
	ricorsioneSuDirectory(argv[1], firstName);	
	return 0;
}
