2-Shell: 2-Shell.o 2-Setup.o 2-Queue.o
	gcc -std=c99 2-Shell.o 2-Setup.o 2-Queue.o -o 2-Shell

2-Shell.o: 2-Shell.c
	gcc -c -std=c99 -D_XOPEN_SOURCE=600 2-Shell.c

2-Setup.o: 2-Setup.c 2-Setup.h
	gcc -c -std=c99 -D_XOPEN_SOURCE=600 2-Setup.c

2-Queue.o: 2-Queue.c 2-Queue.h
	gcc -c 2-Queue.c

clean:
	rm 2-Shell *.o
