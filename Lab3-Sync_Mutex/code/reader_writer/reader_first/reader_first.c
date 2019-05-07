#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int id, begin_time, last_time;
} character;

// state flags
enum {
    f_waiting, f_reading, f_writing
} state = f_waiting;

static pthread_mutex_t mutex;
sem_t read_signal;
sem_t write_signal;
int reader_cnt = 0, writer_cnt = 0;


void * reader(void * args) {
    character chrt = *((character *)args);
    sleep(chrt.begin_time);
    printf("[Reader] thread %d is waiting to read.\n", chrt.id);

    // entering critical section
    pthread_mutex_lock(&mutex);

    // critical section
    ++reader_cnt;
    if (f_waiting == state || f_reading == state) {
        sem_post(&read_signal);
        state = f_reading;
    }

    // exit critical section
    pthread_mutex_unlock(&mutex);
    sem_wait(&read_signal);
    printf("[Reader] thread %d is reading.\n", chrt.id);
    sleep(chrt.last_time);
    printf("[Reader] thread %d finished reading.\n", chrt.id);

    // entering critical section
    pthread_mutex_lock(&mutex);
    
    // critical section
    --reader_cnt;
    if (0 == reader_cnt) {
        if (0 != writer_cnt) {
            sem_post(&write_signal);
            state = f_writing;
        }
        else {
            state = f_waiting;
        }
    }

    // exit critical section
    pthread_mutex_unlock(&mutex);
    return NULL;
}


void * writer(void * args) {
    character chrt = *((character *)args);
    sleep(chrt.begin_time);
    printf("[Writer] thread %d is waiting to write.\n", chrt.id);

    // entering critical section
    pthread_mutex_lock(&mutex);

    // critical section
    ++writer_cnt;
    if (f_waiting == state) {
        sem_post(&write_signal);
        state = f_writing;
    }
    
    // exit critical section
    pthread_mutex_unlock(&mutex);
    sem_wait(&write_signal);
    printf("[Writer] thread %d is writing.\n", chrt.id);
    sleep(chrt.last_time);
    printf("[Writer] thread %d finished writing.\n", chrt.id);

    // entering critical section
    pthread_mutex_lock(&mutex);

    // critical section
    --writer_cnt;
    if (0 != reader_cnt) {
        sem_post(&read_signal);
        state = f_reading;
    }
    else if (0 != writer_cnt) {
        sem_post(&write_signal);
        state = f_writing;
    }
    else {
        state = f_waiting;
    }

    // exiting critical section
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t tid[100];
    character chrt[100];
    int ptcnt = 0;

    FILE * test_file = fopen("./test.txt", "r");
    if (NULL == test_file) printf("[Error ] open file unsuccessfully.\n");

    sem_init(&read_signal, 0, 0);
    sem_init(&write_signal, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    int id, begin, last;
    char ch;
    while (fscanf(test_file, "%d %c %d %d",&id,&ch,&begin,&last) != EOF) {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        character tmp = {id, begin, last};
        chrt[ptcnt] = tmp;
        if ('R' == ch) {
            pthread_create(&tid[ptcnt], &attr, reader, &chrt[ptcnt]);
            printf("[ Main ] create reader thread %d.\n", id);
        }
        else {
            pthread_create(&tid[ptcnt], &attr, writer, &chrt[ptcnt]);
            printf("[ Main ] create writer thread %d.\n", id);
        }
        ++ptcnt;
    }

    for (int i = 0; i < ptcnt; ++i) {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&read_signal);
    sem_destroy(&write_signal);

    exit(0);
}