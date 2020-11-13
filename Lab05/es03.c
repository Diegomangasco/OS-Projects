#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>


void manager(int signo){
	if(signo != SIGUSR1 && signo != SIGUSR2)
		fprintf(stderr, "Signal wrong\n");
	return;
}

int main(){
	int pid;
	signal(SIGUSR1, manager);
	signal(SIGUSR2, manager);
	pid = fork();
	while(1){
		if(pid == 0){		//child
			sleep(1);
			fprintf(stdout, "Child woke up\n");
			kill(getppid(), SIGUSR1);
			pause();
		}
		else{			//father
			pause();
			fprintf(stdout, "Father woke up\n");
			sleep(1);
			kill(pid, SIGUSR2);
		}
	};
	return 0;
}
