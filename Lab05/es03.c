#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>


void manager(int signo){
	if(signo != SIGCHLD)
		fprintf(stderr, "Signal wrong\n");
	return;
}

int main(){
	int pid;
	while(1){
		signal(SIGCHLD, manager);
		pid = fork();
		if(pid == 0){		//child
			sleep(1);
			fprintf(stdout, "Child woke up\n");
			kill(getppid(), SIGCHLD);
			exit(0);
		}
		else{			//father
			pause();
			fprintf(stdout, "Father woke up\n");
			wait(NULL);
		}
	};
	return 0;
}
