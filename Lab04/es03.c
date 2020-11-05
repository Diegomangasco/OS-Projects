#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#define S 150

void togliEnd(char* buf){
	int n = strlen(buf);
	n--;		//mi posiziono sull'ultimo carattere
	n = n-4;	//tolgo il pezzo end\n
	buf[n] = '\0';
	printf("%s\n", buf);
	return;
}

void preparaPerExec(char* buf, char*** parameters){
	int i = 0, j, t, counter = 0;
	char string[S];
	while(buf[i] != ' ' && buf[i] != '\0') i++;	//seleziono il comando
	strcpy(string, "/bin/");
	for(j = 0; j<i; j++)	//copio in string la prima parte
		string[5+j] = buf[j];
	string[5+j] = '\0';
	
	i = 0;
	while(buf[i] != '\0'){
		if(buf[i] == ' ')
			counter++;
		i++;
	}
	counter = counter + 2; //conteggio lo spazio per il (char*) 0 e per l'ultimo parametro
	*parameters = malloc(counter*sizeof(char*));
	for(j = 0; j<counter; j++)
		(*parameters)[j] = malloc(80*sizeof(char));

	i = 0;
	t = i;
	counter = 0;
	while(buf[i] != '\0'){
		if(buf[i] == ' '){
			for(j = 0; t<i; t++, j++)
				(*parameters)[counter][j] = buf[t];
			(*parameters)[counter][j] = '\0';
			t = i + 1;
			counter++;
		}
		i++;
	}
	for(j = 0; t<i; t++, j++)
		(*parameters)[counter][j] = buf[t];	//ultimo parametro
	(*parameters)[counter][j] = '\0';
	(*parameters)[++counter] = (char*) 0;

	for(i = 0; i<strlen(string); i++)	//ricopio in buf il contenuto di string
		buf[i] = string[i];
	buf[i] = '\0';
	return;
}		
int main(int argc, char* argv[]){
	if(argc != 3){
		fprintf(stderr, "Error on parameters.\n");
		exit(0);
	}
	FILE* fp;
	int i = 0;
	int pid;
	char** parameters;
	char buf[S];
	if((fp = fopen(argv[1], "r")) == NULL){
		fprintf(stderr, "Error on the open.\n");
		exit(1);
	}
	switch(atoi(argv[2])){
		case 0:
			printf("Execution with the function system\n");
			while(fgets(buf, 80, fp) != NULL){
				togliEnd(buf);
				system(buf);
				sleep(2);
			}
		break;
		case 1:
			printf("Execution with the function execv\n");
			while(fgets(buf, 80, fp) != NULL){
				if(buf[0] == '\n') break;
				printf("ciao\n");
				togliEnd(buf);
				preparaPerExec(buf, &parameters);
				pid = fork();
				if(pid == 0)
					execv(buf, parameters);
				else
					waitpid(pid, (int*) 0, 0);
				while(parameters[i]!=NULL) {free(parameters[i]); i++;}
				free(parameters);
				sleep(2);
			}
		break;
	}
	fclose(fp);
	return 0;
}
	
