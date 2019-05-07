#include <ctype.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#include "2-Queue.h"

void setup(char *, char **, int *);