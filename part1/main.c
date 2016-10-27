#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include "bitonicSortSerial.h"
#include "bitonicSortParallel.h"

#define MAX(A,B) ((A > B) ? A : B)
#define MIN(A,B) ((A > B) ? B : A)
#define UP 0
#define DOWN 1

void bitonicSortSeq(int start, int length, int *arr, int flag);
void bitonicSortPar(int start, int length, int *arr, int flag);
void swap(int a[], int left, int right);
void quickSortSeq(int *arr, int left, int right);
void quickSortPar(int *arr, int left, int right, int num_threads);
void qsort_thread_function(void *args);
int thread_counter = 0;

struct partition_arg {
	int left;
	int right;
	int* arr;
	int num_threads;
};


int main()
{
	int small, mid, large;
	int range;
	int i;
	double startTime, endTime;
	double initTime;
	clock_t begin, end;
	int *arrSmall, *arrMid, *arrLarge;
	int *copySmall, *copyMid, *copyLarge;
	double timeSpent;

	/* pwrs of 2 */
	small = 100;
	mid = 10000;
	large = 100000;

	range = 1000000;

	initTime = 0.0;

	arrSmall = (int *) malloc(small * sizeof(int));
	arrMid = (int *) malloc(mid * sizeof(int));
	arrLarge = (int *) malloc(large * sizeof(int));

	/* Initialize arrays with random integers from 0 to (range-1)  */
	for (i = 0; i < small; i++)
		arrSmall[i] = rand() % range;

	for (i = 0; i < mid; i++)
		arrMid[i] = rand() % range;

	for (i = 0; i < large; i++)
		arrLarge[i] = rand() % range;

	/* start recording */
	for (i = 4; i < 10000; i *= 4) {
		copySmall = (int *) malloc(small * sizeof(int));
		memcpy(copySmall, arrSmall, small * sizeof(int));
		printf("Sorting Small...\n");
		begin = clock();
		quickSortPar(copySmall, 0, small-1, i);
		end = clock();
		timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
		free(copySmall);
		printf("time spent on small: %f, threads: %d\n", timeSpent, i);
		thread_counter = 0;

		int j;
		for (j = 0; j < small; j++)
			printf("%d ", copySmall[j]);

		printf("\n");

		copyMid = (int *) malloc(mid * sizeof(int));
		memcpy(copyMid, arrMid, mid * sizeof(int));
		printf("Sorting Mid...\n");
		begin = clock();
		quickSortPar(copyMid, 0, mid-1, i);
		end = clock();
		timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
		free(copyMid);
		printf("time spent on mid: %f, threads: %d\n", timeSpent, i);
		thread_counter = 0;

		copyLarge = (int *) malloc(large * sizeof(int));
		memcpy(copyLarge, arrLarge, large * sizeof(int));
		printf("Sorting Large...\n");
		begin = clock();
		quickSortPar(copyLarge, 0, large-1, i);
		end = clock();
		timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
		free(copyLarge);
		printf("time spent on large: %f, threads: %d\n", timeSpent, i);
		thread_counter = 0;
	}

	begin = clock();
	quickSortSeq(arrSmall, 0, small-1);
	end = clock();
	timeSpent = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("time spent on small: %f\n", timeSpent);

	begin = clock();
	quickSortSeq(arrMid, 0, mid-1);
	end = clock();
	timeSpent = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("time spent on mid: %f\n", timeSpent);
	
	begin = clock();
	quickSortSeq(arrLarge, 0, large-1);
	end = clock();
	timeSpent = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("time spent on large: %f\n", timeSpent);

	for (i = 0; i < small; i++)
		printf("%d ", arrSmall[i]);

}


void bitonicSortSeq(int start, int length, int *arr, int flag)
{
	int i, mid;

	if (length == 1) return;

	if (length % 2 != 0)
	{
		printf("error\n");
		exit(0);
	}

	mid = length / 2;

	for (i = start; i < start + mid; i++)
	{
		if (flag == UP)
			if (arr[i] > arr[i + mid])
				swap(arr, i, i + mid);
		else
			if (arr[i] < arr[i + mid])
				swap(arr, i, i + mid);
	}

	bitonicSortSeq(start, mid, arr, flag);
	bitonicSortSeq(start + mid, mid, arr, flag);
}


void bitonicSortPar(int start, int length, int *arr, int flag)
{
	int i, mid;

	if (length == 1) return;

	if (length % 2 != 0)
	{
		printf("error\n");
		exit(0);
	}

	mid = length / 2;

	/* parallelize this using pthreads*/
	for (i = start; i < start + mid; i++)
	{
		if (flag == UP)
			if (arr[i] > arr[i + mid])
				swap(arr, i, i + mid);
		else
			if (arr[i] < arr[i + mid])
				swap(arr, i, i + mid);
	}

	bitonicSortPar(start, mid, arr, flag);
	bitonicSortPar(start + mid, mid, arr, flag);
}


void swap (int a[], int left, int right)
{
 int temp;
 temp=a[left];
 a[left]=a[right];
 a[right]=temp;
}//end swap

void quickSortSeq(int *arr, int left, int right)
{
	int mid;
	if (left < right)
	{
		mid = partition(arr, left, right);
		quickSortSeq(arr, left, mid - 1);
		quickSortSeq(arr, mid + 1, right);
	}
}

int partition(int arr[], int left, int right)
{
	int leftPointer = left - 1;
	int rightPointer = right;
	int pivot = arr[right];

	while(1)
	{
		while(arr[++leftPointer] < pivot) {}
		while(rightPointer > 0 && arr[--rightPointer] > pivot) {}

		if (leftPointer >= rightPointer)
		{
			break;
		} else {
			swap(arr, leftPointer, rightPointer);
		}
	}

	swap(arr, leftPointer, right);
	return leftPointer;
}


void quickSortPar(int *arr, int left, int right, int num_threads)
{
	int mid;
	pthread_t ptl, ptr;
	if (left < right)
	{
		mid = partition(arr, left, right);
		if (thread_counter < num_threads) {	
			struct partition_arg* l = (struct partition_arg *) malloc(sizeof(struct partition_arg));
			l->left = left;
			l->right = mid - 1;
			l->arr = arr;
			l->num_threads = num_threads;
			struct partition_arg* r = (struct partition_arg *) malloc(sizeof(struct partition_arg));
			r->left = mid + 1;
			r->right = right;
			r->arr = arr;
			r->num_threads = num_threads;
			//printf("start creating\n");
			pthread_create(&ptl, NULL, (void *)qsort_thread_function, l);
			//printf("stop creating 1\n");
			pthread_create(&ptr, NULL, (void *)qsort_thread_function, r);
			//printf("stop creating 2\n");

			pthread_join(ptl, NULL);
			pthread_join(ptr, NULL);

			free(l);
			free(r);
		} else {
			quickSortPar(arr, left, mid - 1, num_threads);
			quickSortPar(arr, mid + 1, right, num_threads);
		}
	}

}

void qsort_thread_function(void *args) {
	thread_counter++;
	struct partition_arg *pa = args;
	quickSortPar(pa->arr, pa->left, pa->right, pa->num_threads);
	pthread_exit(NULL);
}
