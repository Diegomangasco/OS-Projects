#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void P2457(){
	pid_t pid;
	printf("Process P2, pid = %d\n", getpid());
		sleep(2);
		pid = fork();
		if(pid == 0){
			printf("Process P5, pid = %d\n", getpid());
			sleep(2);
			exit(0);
		}
		else if(pid != 0){
			printf("Process P4, pid = %d\n", getpid());
			waitpid(pid, (int *) 0, 0);
			printf("Process P7, pid = %d\n", getpid());
			sleep(2);
		}
	return;
}

void P368(){
	pid_t pid;
	printf("Process P3, pid = %d\n", getpid());
		pid = fork();
		if(pid == 0){
			sleep(3);
			printf("Process P6, pid = %d\n", getpid());
			sleep(2);
			exit(0);
		}
		else if(pid != 0){
			waitpid(pid, (int *) 0, 0);
			//il padre in questo caso non stampa
			printf("Process P8, pid = %d\n", getpid());
			sleep(2);
		}
	return;
}
int main(){
	printf("Process P1, pid = %d\n", getpid());
	pid_t pid;
	pid = fork();
	if(pid == 0){	//child
		P368();
		exit(0);
	}
	else if(pid != 0){
		P2457();
		waitpid(pid, (int *) 0, 0);
		printf("Process P9, pid = %d\n", getpid());
		exit(0);
	}
	return 0;
}
