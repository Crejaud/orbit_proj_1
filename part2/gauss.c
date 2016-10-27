#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

void gaussian_elimination(double **A, double *B, int n, int num_threads);
void gaussian_thread_function(void *args);
void gaussian_reduce(double **A, double *B, int n, int i);
double* gauss_seq(double **A, double *B, int n);

struct gauss_arg {
	double **A;
	double *B;
	int n;
	int i;
};

int main()
{
	int range = 1000;
	int i, j;
	int n = 5;

	double **A = (double **) malloc(n * sizeof(double *));
	double *B = (double *) malloc(n * sizeof(double));
	for (i = 0; i < n; i++)
	{
		B[i] = (rand() % range);
		A[i] = (double *) malloc(n * sizeof(double));
		for (j = 0; j < n; j++)
		{
			A[i][j] = (rand() % range);
		}
	}

	/* print A first */
	printf("A\n");
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
	
	double *y = gauss_seq(A, B, n);

	/* print A first */
	printf("A\n");
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
	/* call gaussian_elmination */
	//gaussian_elimination(A, B, n, 2);
}

double* gauss_seq(double **A, double *B, int n)
{
	int i, j, k;
	for (i = 0; i < n; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			printf("1");
			A[i][j] = A[i][j]/A[i][i];
			for (k = i + 1; k < n; k++)
			{
				printf("2");
				A[k][j] -= A[k][i]*A[i][j];
			}
		}
	}
	double *y = (double *) malloc(n * sizeof(double));
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
	return y;
}

void gaussian_elimination(double **A, double *B, int n, int num_threads)
{
	printf("3");
	int i;
	int thread_counter = 0;
	printf("3");
	pthread_t *threads = (pthread_t *) malloc(n * sizeof(pthread_t));
	printf("4");	
	for (i = 0; i < n; i++) {
		/* make pthread and call function */
		if (thread_counter < num_threads)
		{
			printf("5");
			struct gauss_arg* args = (struct gauss_arg *) malloc(sizeof(struct gauss_arg));
			args->A = A;
			args->B = B;
			args->n = n;
			args->i = i;
			printf("6");
			pthread_create(&threads[i], NULL, (void *) gaussian_thread_function, &args);
			thread_counter++;
		}
		else
		{
			gaussian_reduce(A, B, n, i);
		}
	}

	

	for (i = 0; i < n ; i++)
	{
		pthread_join(threads[i], NULL);
	}
}

void gaussian_reduce(double **A, double *B, int n, int i)
{
	int j, k;
	for (j = i + 1; j < n; j++)
	{
		printf("1");
		A[i][j] = A[i][j]/A[i][i];
		for (k = i + 1; k < n; k++)
		{
			printf("2");
			A[k][j] -= A[k][i]*A[i][j];
		}
	}
}

void gaussian_thread_function(void *args)
{
	struct gauss_arg *ga = args;
	printf("7");
	gaussian_reduce(ga->A, ga->B, ga->n, ga->i);
	free(ga);
	pthread_exit(NULL);
}
