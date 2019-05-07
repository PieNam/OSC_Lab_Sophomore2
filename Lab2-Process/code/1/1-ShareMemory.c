#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_SEQUENCE 10
#define PERM S_IRUSR | S_IWUSR // read and write

typedef struct {
	int fib_sequence[MAX_SEQUENCE];
	int sequence_size;
} share_data;

void fib(int n, int *fibo) {
	fibo[0] = 0;
	fibo[1] = 1;
	if (0 == n || 1 == n) return;
	for (int i = 2; i < n; ++i) fibo[i] = fibo[i-1] + fibo[i-2];
}

int main() {
	int num, shmid;
	char * shmptr;
	share_data *shMemory;
	pid_t pid;
	key_t key;
	
	printf("Please enter a positive integer less than 11: ");
	while (1) {
		scanf("%d", &num);
		if (num <= 0) printf("[invalid] Please input a positive integer: ");
		else if (num > 10) printf("[invalid] Please input a positive integer less than 11: ");
		else break;
	}

	if ((shmid = shmget(IPC_PRIVATE, sizeof(share_data), PERM)) < 0) {
		printf("[error] when creating share memory: %s\n", strerror(errno));
		return -1;
	}
	if ((shMemory = (share_data*)shmat(shmid, 0, 0)) == (void*)-1) {
		printf("[error] when linking share memory: %s\n", strerror(errno));
		return -1;
	}
	shMemory->sequence_size = num;

	while (-1 == (pid = fork()));

	if (0 == pid) {
		fib(shMemory->sequence_size, shMemory->fib_sequence);
		exit(0);
	}
	else if (pid > 0) {
		wait(0);
		printf("[result]: ");
		for (int i = 0; i < shMemory->sequence_size; ++i) 
			printf("%d ", shMemory->fib_sequence[i]);
		printf("\n");
		shmdt(shMemory);
		shmctl(shmid, IPC_PRIVATE, NULL);
		exit(0);
	}
	return 0;
}
