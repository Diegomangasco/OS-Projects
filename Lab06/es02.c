#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define N 10

int main(){
	int pid, pid2;
	int fd1[2];
	int fd2[2];
	int flag = 1, i = 0;
	pipe(fd1);
	pipe(fd2);
	pid = fork();
	while(1){
		if(pid > 0){	//padre
			printf("I'm the father (PID = %d)\n", getpid());
			write(fd1[1], &pid, sizeof(int));	//passo il pid del figlio, in modo che possa scriverlo
			read(fd2[0], &flag, sizeof(int)); 
			if(flag == 0)		//terminazione
				exit(0);
		}
		else{
			i++;
			read(fd1[0], &pid2, sizeof(int));
			printf("I'm the child (PID = %d)\n", pid2);	//si può anche fare con una getpid()
			if(i == N){	//numero massimo iterazioni
				flag = 0;
				write(fd2[1], &flag, sizeof(int));
				exit(0);
			} 
			write(fd2[1], &flag, sizeof(int));
		}
	}
	return 0;
}
