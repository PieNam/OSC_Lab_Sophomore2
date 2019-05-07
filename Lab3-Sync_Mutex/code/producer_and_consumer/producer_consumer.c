#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5

typedef int buffer_item;
typedef struct {
    int id, begin_time, last_time, item;
} args;

static buffer_item buffer[BUFFER_SIZE];
static int cnt, in_p, out_p;
pthread_mutex_t mutex;
sem_t empty, full;

int insert_item(buffer_item item) {
    if (cnt == BUFFER_SIZE) {
        return -1;
    }
    else {
        buffer[in_p] = item;
        in_p = (in_p + 1) % BUFFER_SIZE;
        ++cnt;
        return 0;
    }
}

int remove_item(buffer_item *item) {
    if (!cnt) {
        return -1;
    }
    else {
        *item = buffer[out_p];
        buffer[out_p] = 0;
        out_p = (out_p + 1) % BUFFER_SIZE;
        --cnt;
        return 0;
    }
}

void buffer_printer() {
    int c = 0;
    printf("[ Buffer ] ");
    while (c < BUFFER_SIZE) {
        printf("%4d", buffer[c++]);
    }
    printf("\n");
}

void * producer(void * param) {
    args tmp = *((args *) param);
    buffer_item item;

    sleep(tmp.begin_time);

    int flag = 1;
    while (flag) {
        item = tmp.item;

        // entering critical section
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        // critical section
        if (-1 == insert_item(item)) {
            // -1 returned  error
            printf("[  Error ] when producer insering an item...\n");
            pthread_mutex_unlock(&mutex);
            sem_post(&empty);
            continue;
        }
        else {
            printf("[Producer] item %2d inserted successfully [Thread %d].\n", item, tmp.id);
            buffer_printer();
            flag = 0;
        }
        sleep(tmp.last_time);

        // exit critical section
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        return NULL;
    }
    return NULL;
}

void * consumer(void * param) {
    args tmp = *((args *) param);
    buffer_item item;

    sleep(tmp.begin_time);

    int flag = 1;
    while (flag) {
        // entering critical section
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        // critical section
        if (remove_item(&item)) {
            // -1 returned  error
            printf("[  Error ] when consumer removing an item...");
            pthread_mutex_unlock(&mutex);
            sem_post(&full);
            sleep(0);
            continue;
        }
        else {
            printf("[Consumer] item %2d  removed successfully [Thread %d].\n", item, tmp.id);
            buffer_printer();
            flag = 0;
        }
        sleep(tmp.last_time);

        // exit critical section
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        return NULL;
    }
    return NULL;
}

int main() {
    FILE * test_file = fopen("./test.txt", "r");
    if (NULL == test_file) printf("[  Error ] open file unsuccessfully.\n");

    pthread_mutex_init(&mutex, NULL);
    // the second arg 0 for sharing between threads in the process
    // the third arg for the initial value
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    int id, begin, last, item;
    char pc;
    pthread_t tid[100];
    args t[100];
    int ptcnt = 0;
    while (fscanf(test_file,"%d %c %d %d",&id,&pc,&begin,&last) != EOF) {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        args tmp = {id, begin, last, -1};
        t[ptcnt] = tmp;
        if ('P' == pc) {
            fscanf(test_file,"%d",&item);
            t[ptcnt].item = item;
            pthread_create(&tid[ptcnt], &attr, producer, &t[ptcnt]);
            printf("[  Main  ] create producer thread %d.\n", id);
        }
        else {
            pthread_create(&tid[ptcnt], &attr, consumer, &t[ptcnt]);
            printf("[  Main  ] create consumer thread %d.\n", id);
        }
        ++ptcnt;
    }
    for (int i = 0; i < ptcnt; ++i) {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);

    exit(0);
}