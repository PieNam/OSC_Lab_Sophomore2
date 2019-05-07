#include "2-Setup.h"

void setup(char * buffer, char * args[], int * isbg) {
    int l, i, start = -1, na = 0;
    l = read(STDIN_FILENO, buffer, MAX_LINE);
    if (!l) exit(0);
    if (l < 0) {
        args[0] = NULL;
        return;
    }

    if ('r'==buffer[0] && '0'<=buffer[1] && buffer[1]<='9' && '\n'==buffer[2]) {
        char * str;
        if ((str = getHistory(buffer[1]-'0')) == NULL) {
            perror("[error] when getting command.\n");
            return;
        }
        else strcpy(buffer, str);
        l = strlen(str) + 1;
        buffer[l-1] = '\n';
        buffer[l] = 0;
    }


    for (i = 0; i < l; ++i) {
        switch(buffer[i]) {
            case ' ':
            case '\t':
                if (start != -1) {
                    args[na] = &buffer[start];
                    ++na;
                }
                buffer[i] = 0;
                start = -1;
                break;
            case '\n':
                if (start != -1) {
                    args[na] = &buffer[start];
                    ++na;
                }
                buffer[i] = 0;
                args[na] = NULL;
                break;
            default:
                if (-1 == start) start = i;
                if (buffer[i] == '&') {
                    *isbg = 1;
                    buffer[i] = 0;
                }
        }
    }
    add(buffer);
    args[na] = NULL;
}