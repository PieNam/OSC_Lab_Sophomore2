#include "2-Setup.h"

void handle_SIGINT() {
    print();
    signal(SIGINT, SIG_IGN);
}

int main() {
    char buffer[MAX_LINE];
    char * args[MAX_LINE/2+1];
    int isbg;
    pid_t pid;
    struct sigaction handler;

    queue.front = -1;
    queue.rear = 0;

    while(1) {
        isbg = 0;
        printf("COMMAND -> ");
        fflush(stdout);
        setup(buffer, args, &isbg);

        if (-1 == (pid = fork())) printf("[error] when forking.\n");
        if (0 == pid) {
            execvp(args[0], args);
            exit(0);
        }
        if (!isbg) wait(0);

        handler.sa_handler = (void (*)(int))handle_SIGINT;
        sigaction(SIGINT, &handler, NULL);
    }

    return 0;
}