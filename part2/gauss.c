#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

void gaussian_elimination(int num_threads);
void gaussian_thread_function(void *args);
void gaussian_reduce(int i);
int n = 100;
double **A;
double *B, *y;
pthread_barrier_t barrier;

int main()
{
	int range = 100;
	int i, j;

	clock_t begin, end;
	double timeSpent;

	time_t t;
	srand((unsigned) time(&t));

	A = (double **) malloc(n * sizeof(double *));
	B = (double *) malloc(n * sizeof(double));
	for (i = 0; i < n; i++)
	{
		B[i] = ((double)rand() / range);
		A[i] = (double *) malloc(n * sizeof(double));
		for (j = 0; j < n; j++)
		{
			A[i][j] = ((double)rand() / range);
		}
	}

	/* print A first */
	/*printf("A\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%f ",A[i][j]);
		}
		printf("\n");
	}

	printf("B\n");
	for (i = 0; i < n; i++)
	{
		printf("%f\n", B[i]);
	}*/
	y = (double *) malloc(n * sizeof(double));
	begin = clock();
	gaussian_elimination(0);
	end = clock();
	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time spent with no parallelism: %f\n", timeSpent);
	/* print A first */
	/*printf("A\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%f ",A[i][j]);
		}
		printf("\n");
	}

	printf("B\n");
	for (i = 0; i < n; i++)
	{
		printf("%f\n", B[i]);
	}

	printf("y\n");
	for (i = 0; i < n; i++)
	{
		printf("%f\n", y[i]);
	}
	*/
	free(A);
	free(B);
	A = (double **) malloc(n * sizeof(double *));
	B = (double *) malloc(n * sizeof(double));
	for (i = 0; i < n; i++)
	{
		B[i] = ((double)rand() / range);
		A[i] = (double *) malloc(n * sizeof(double));
		for (j = 0; j < n; j++)
		{
			A[i][j] = ((double)rand() / range);
		}
	}

	/* print A first */
	/*printf("A\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%f ",A[i][j]);
		}
		printf("\n");
	}

	printf("B\n");
	for (i = 0; i < n; i++)
	{
		printf("%f\n", B[i]);
	}*/
	free(y);
	y = (double *) malloc(n * sizeof(double));	
	pthread_barrier_init(&barrier, NULL, 5);
	printf("Sequential finished. Now running parallel.\n");
	begin = clock();
	printf("Starting Parallel\n");
	gaussian_elimination(4);
	end = clock();
	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time spent with parallelism: %f\n", timeSpent);
	/* print A first */
	//printf("A\n");
	/*for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%f ",A[i][j]);
		}
		printf("\n");
	}

	printf("B\n");
	for (i = 0; i < n; i++)
	{
		printf("%f\n", B[i]);
	}

	printf("y\n");
	for (i = 0; i < n; i++)
	{
		printf("%f\n", y[i]);
	}*/
	/* call gaussian_elmination */
	//gaussian_elimination(A, B, n, 2);
	free(A);
	free(B);
	free(y);
	exit(0);
}

void gaussian_elimination(int num_threads)
{
	//printf("HULLOH?\n");
	int i,k;
	int thread_counter = 0;
	pthread_t *threads;
	for (i = 0; i < n; i++) {
		/* make pthread and call function */
		if (thread_counter < num_threads)
		{
			int* temp_i = (int *) malloc(sizeof(int));
			*temp_i = i;
			pthread_create(&threads[i], NULL, (void *) gaussian_thread_function, (void *) temp_i);
			thread_counter++;
		}
		else
		{
			gaussian_reduce(i);
		}
	}
	//printf("YOYOYO\n");
	if (num_threads > 0)
		pthread_barrier_wait(&barrier);
	//printf("YOOOO\n");
	for (k = 0; k < n; k++)
	{
		y[k] = B[k]/A[k][k];
		A[k][k] = 1;
		for (i = k + 1; i < n; i++)
		{
			B[i] -= A[i][k]*y[k];
			A[i][k] = 0;
		}
	}
}

void gaussian_reduce(int k)
{
	int j, i;
	for (j = k + 1; j < n; j++)
	{
		A[k][j] = A[k][j]/A[k][k];
		for (i = k + 1; i  < n; i++)
		{
			A[i][j] -= A[i][k]*A[k][j];
		}
	}
}

void gaussian_thread_function(void *args)
{
	int *k = (int *) args;
	gaussian_reduce(*k);
	//pthread_exit(NULL);
	pthread_barrier_wait(&barrier);
}

