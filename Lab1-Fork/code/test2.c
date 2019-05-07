#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int value = 5;
int main() {
	pid_t pid;
	pid = fork();
	if (0 == pid) {
		// child process
		value += 15;
	}
	else if (pid > 0) {
		// parent process
		wait(NULL);
		printf("PARENT: value = %d\n", value); // LINE A
		exit(0);
	}
}
