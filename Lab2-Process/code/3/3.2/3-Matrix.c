#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int ma, ka, mb, kb;
float a[20][20] = {0}, b[20][20] = {0}, c[20][20] = {0};
typedef struct {
    int i, j;
} p;

void calculate(void * data) {
    p *t = (p*) data;
    int i = t->i;
    int j = t->j;
	int l;
    for (l = 0; l < ka; ++l) c[i][j] += a[i][l] * b[l][j];
    pthread_exit(NULL);
}

int main() {
	int i, j, x, y;
	float v;
    FILE * fpa = fopen("./a.txt", "r");
    FILE * fpb = fopen("./b.txt", "r");
    if (NULL == fpa || NULL == fpb) {
        printf("[error] when opening file.\n");
		return -1;
    }

	fscanf(fpa, "%d,%d", &ma, &ka);
	if (ma > 20) {
		printf("[error] too huge for mae!\n");
		return -1;
	} else if (ka > ma * ma) {
		printf("[error] invalid input.\n");
		return -1;
	}
	for (i = 0; i < ka; ++i) {
		fscanf(fpa, "%d,%d,%f", &x, &y, &v);
		a[x][y] = v;
	}

	fscanf(fpb, "%d,%d", &mb, &kb);
	if (ma != mb) {
		printf("[error] invalid matrix mul.\n");
		return -1;
	} else if (mb > 20) {
		printf("[error] too huge for mbe!\n");
		return -1;
	} else if (kb > mb * mb) {
		printf("[error] invalid input.\n");
		return -1;
	}
	for (i = 0; i < kb; ++i) {
		fscanf(fpb, "%d,%d,%f", &x, &y, &v);
		b[x][y] = v;
	}

	// for (i = 0; i < ma; ++i) {
	// 	for (j = 0; j < ma; ++j)
	// 		printf("%.2f ", a[i][j]);
	// 	printf("\n");
	// }
	// printf("\n");
	// for (i = 0; i < ma; ++i) {
	// 	for (j = 0; j < ma; ++j)
	// 		printf("%.2f ", b[i][j]);
	// 	printf("\n");
	// }

	pthread_t tid[ma * mb];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	for (i = 0; i < ma; ++i) {
		for (j = 0; j < mb; ++j) {
			p * a = (p *)malloc(sizeof(p));
			a->i = i;
			a->j = j;
			pthread_create(&tid[i * mb + j], &attr, (void *)&calculate, (void *)a);
		}
	}

	for (i = 0; i < ma * mb; ++i) {
		pthread_join(tid[i], NULL);
	}

	printf("The result of A * B: \n");
	for (i = 0; i < ma; ++i) {
		for (j = 0; j < mb; ++j)
			printf("%10.2f ", c[i][j]);
		printf("\n");
	}
	printf("You can also check the result in c.txt\n");
	
    FILE * fpc = fopen("./c.txt", "w");
	int nonzero = 0;
	for (i = 0; i < ma; ++i) 
		for (j = 0; j < mb; ++j)
			if (c[i][j]) ++nonzero;
	fprintf(fpc, "%d, %d\n", ma, nonzero);
	for (i = 0; i < ma; ++i) {
		for (j = 0; j < mb; ++j)
			fprintf(fpc, "%d, %d, %.2f\n", i, j, c[i][j]);
		fprintf(fpc, "\n");
	}

	fclose(fpa);
	fclose(fpb);
	fclose(fpc);
	return 0;
}