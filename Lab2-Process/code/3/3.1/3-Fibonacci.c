#include <stdio.h>
#include <pthread.h>

#define MAX_SEQUENCE 50

typedef struct {
    int num;
    long long fibo[MAX_SEQUENCE];
} Data;

void fib(void *data) {
    Data * t = (Data *) data;
    int i;
    t->fibo[0] = 0;
    t->fibo[1] = 1;
    if (0 == t->num || 1 == t->num) return;
    for (i = 2; i < t->num; ++i)
        t->fibo[i] = t->fibo[i-1] + t->fibo[i-2];
    pthread_exit(NULL);
}

int main() {
    Data data;
    pthread_t th;
    int ret, num, i;

    printf("Please enter a positive integer in [1, 50]: ");
    while (1) {
		scanf("%d", &num);
		if (num < 1) printf("[invalid] Please input a positive integer: ");
		else if (num > 50) printf("[invalid] Please input a positive integer less than 51: ");
		else break;
	}
    data.num = num;

    if ((ret = pthread_create(&th, 0, (void *)&fib, (void *)&data)) != 0) {
        printf("[error] when creating thread.\n");
        return -1;
    }

    pthread_join(th, 0);

    printf("[result]: ");
    for (i = 0; i < data.num; ++i) 
        printf("%lld ", data.fibo[i]);
    printf("\n");

    return 0;
}