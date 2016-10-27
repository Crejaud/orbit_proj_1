#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

void gaussian_elimination(int num_threads);
void gaussian_thread_function(void *args);
void gaussian_reduce(int i);
double* gauss_seq(double **A, double *B, int n);
void gauss_seq2(double **A, double *B, int n);
int n = 4;
double **A;
double *B, *y;

struct gauss_arg {
	int k;
};

int main()
{
	int range = 100;
	int i, j;

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
	y = (double *) malloc(n * sizeof(double));
	//double *y = gauss_seq(A, B, n);
	gaussian_elimination(6);
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

void gauss_seq2(double **A, double *B, int n)
{
	int i, j, k;
	double c;
	for (j = 0; j < n-1; j++)
	{
		for (i = 1; i < n-1; i++)
		{
			if (i > j)
			{
				c = A[i][j]/A[j][j];
				for (k = 0; k < n; k++)
				{
					A[i][k] -= c * A[j][k];
				}
			}
		}
	}
	
}

double* gauss_seq(double **A, double *B, int n)
{
	int i, j, k;
	for (k = 0; k < n; k++)
	{
		for (j = k + 1; j < n; j++)
		{
			A[k][j] = A[k][j]/A[k][k];
			for (i = k + 1; i  < n; i++)
			{
				A[i][j] -= A[i][k]*A[k][j];
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

void gaussian_elimination(int num_threads)
{
	printf("3");
	int i,k;
	int thread_counter = 0;
	printf("3");
	pthread_t *threads;
	printf("4");	
	for (i = 0; i < n; i++) {
		/* make pthread and call function */
		if (thread_counter < num_threads)
		{
			printf("5");
			struct gauss_arg* args = (struct gauss_arg *) malloc(sizeof(struct gauss_arg));
			args->k = i;
			printf("6");
			pthread_create(&threads[i], NULL, (void *) gaussian_thread_function, &args);
			thread_counter++;
		}
		else
		{
			gaussian_reduce(i);
		}
	}

	

	for (i = 0; i < n ; i++)
	{
		pthread_join(threads[i], NULL);
	}

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
	struct gauss_arg *ga = args;
	printf("7");
	gaussian_reduce(ga->k);
	pthread_exit(NULL);
}
