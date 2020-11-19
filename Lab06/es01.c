#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#define N 50

int main(){
	int fd[2];
	int pid;
	pipe(fd);
	int length, i;
	char string [N];
	pid = fork();
	while(1){
		if(pid == 0){
			read(fd[0], &length, sizeof(int));	//leggo la lunghezza della parola
			read(fd[0], string, length);		//leggo la parola
			if(strcmp(string, "end") == 0){
				sleep(1);
				exit(0);
			}
			i = 0;
			while(i < length){
				string[i] = toupper(string[i]);
				i++;
			}
			string[length] = '\n';
			write(1, string, length+1);	//scrivo il risultato su stdout
		}
		else{
			i = read(0, string, N);
			i--;	//tolgo il \n
			write(fd[1], &i, sizeof(int));	//invio la lunghezza
			write(fd[1], string, i);	//invio la stringa
			if(strcmp(string, "end\n") == 0){
				printf("Arrivederci!\n");
				wait(NULL);
				break;
			}	
		}
	}
	return 0;
}		
			





			
