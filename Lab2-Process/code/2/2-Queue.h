#include <stdio.h>
#include <string.h>

#define MAX_LINE 80
#define HISTORY_NUM 10

struct Queue {
    char history[HISTORY_NUM][MAX_LINE];
    int front;
    int rear;
} queue;

void add(char *);
void print();
char * getHistory(int);
int getNum();