#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

void gaussian_elimination(double **A, double *B, int n, int num_threads);
void gaussian_thread_function(void *args);
void gaussian_reduce(double **A, double *B, int n, int i);
void gauss_seq(double **A, double *B, int n);

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
	
	gaussian_elimination(A, B, n, 10);

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
	/* call gaussian_elmination */
	//gaussian_elimination(A, B, n, 2);
}

void gauss_seq(double **A, double *B, int n)
{
	int i, j, k;
	double ratio;
	for (i = 0; i < n-1; i++)
	{
		for (j = i+1; j < n; j++)
		{
			ratio = A[j][i]/A[i][i];
			printf("ratio of %f/%f is %f\n", A[j][i], A[i][i], ratio);
			for (k = i+1; k < n; k++)
			{
				A[j][k] -= (ratio * A[i][k]);
				printf("new A[j][k] is %f\n", A[j][k]);
				B[j] -= (ratio * B[i]);
				printf("new B[j] is %f\n", B[j]);
			}
		}
	}
}

void gaussian_elimination(double **A, double *B, int n, int num_threads)
{
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
		free(&threads[i]);
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
