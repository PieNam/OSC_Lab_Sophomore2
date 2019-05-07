#include "2-Queue.h"

void add(char * str) {
    if (-1 == queue.front) {
        strcpy(queue.history[queue.rear], str);
        queue.front = queue.rear;
        return;
    }
    queue.rear = (queue.rear + 1) % HISTORY_NUM;
    strcpy(queue.history[queue.rear], str);
    if (queue.front == queue.rear) ++(queue.front);
}

void print() {
    if (-1 == queue.front) {
        printf("[empty] no command.\n");
        return;
    }
    int index = queue.front;
    printf("\n");
    fflush(stdout);
    while ((index % HISTORY_NUM) != (queue.rear + 1)) {
        printf("%s\n", queue.history[index]);
        fflush(stdout);
        ++index;
    }
}

char * getHistory(int index) {
    if (index > getNum() || index <= 0) return NULL;
    int cnt = 1, t = queue.front;
    while (cnt < index) {
        ++cnt;
        t = (t + 1) % HISTORY_NUM;
    }
    return queue.history[t];
}

int getNum() {
    return (queue.rear + HISTORY_NUM - queue.front + 1) % HISTORY_NUM;
}