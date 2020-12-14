#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void semaphoreInit(int *S){
	if(pipe(S)==-1){
		printf("Error!");
		exit(-1);
	}
	return;
}

void semaphoreSignal(int *S){
	char c = 'X';
	if(write(S[1], &c, sizeof(char)) == -1){
		printf("Error!");
		exit(-1);
	}
	return;
}

void semaphoreWait(int *S){
	char c;
	if(read(S[0], &c, sizeof(char)) == -1){
		printf("Error");
		exit(-1);
	}
	return;
}

void P(char L){
	printf("%c\n", L);
	return;
}
int main(){
	int fd1[2], fd2[2], fd3[2];
	semaphoreInit(fd1);
	semaphoreInit(fd2);
	semaphoreInit(fd3);
	int i, pid;
	char c;
	while(1){
		P('A');
		if(!fork()){
			P('C');
			pid = fork();
			if(pid == 0){
				P('F');
				semaphoreSignal(fd1);	//sbloco il processo E
				exit(0);	//solo il processo E va avanti e stampa G
			}
			else{
				P('E');
				semaphoreWait(fd1);
			}
			P('G');
			semaphoreWait(fd3);	//aspetto la pipe dall'altro ramo
			P('I');
			exit(0);
		}
		else{
			P('B');
			pid = fork();
			if(pid == 0){
				P('D');
				P('H');
				semaphoreSignal(fd2);	//sblocco il processo B
				exit(0);
			}
			else{
				semaphoreWait(fd2);
				semaphoreSignal(fd3);	//sblocco il processo nell'altro ramo, per la stampa di I
				for(i = 0; i<3; i++) wait(NULL);	//il padre aspetta che tutti i processi terminino
			}
		}
	}
	return 0;
}
