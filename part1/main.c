#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

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
int isLarge = 0;

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
	double startTime, endTime;
	double initTime;

	/* pwrs of 2 */
	small = 16;
	mid = 25;
	large = 36;

	range = 1000000;

	initTime = 0.0;

	int *arrSmall = (int *) malloc(small * sizeof(int));
	int *arrMid = (int *) malloc(mid * sizeof(int));
	int *arrLarge = (int *) malloc(large * sizeof(int));

	int i;

	/* Initialize arrays with random integers from 0 to (range-1)  */
	for (i = 0; i < small; i++)
		arrSmall[i] = rand() % range;

	for (i = 0; i < mid; i++)
		arrMid[i] = rand() % range;

	for (i = 0; i < large; i++)
		arrLarge[i] = rand() % range;

	printf("beginning tests\n");
	/* start recording */
	for (i = 4; i < 10000; i *= 4) {
		printf("begin\n");
		printf("num of threads: %d\n", i);
		printf("end\n");
		int *copySmall = malloc(small * sizeof(int));
		memcpy(copySmall, arrSmall, small * sizeof(int));


		//printf("Before sorting: ");
		//for (i = 0; i < 0; i++)
		//	printf("%d ", copy[i]);
		printf("Sorting Small...\n");
		clock_t begin = clock();
		quickSortPar(copySmall, 0, small-1, i);
		clock_t end = clock();
		double time_spent_small = (double)(end - begin) / CLOCKS_PER_SEC;
		free(copySmall);
		printf("\ntime spent on small: %f, threads: %d\n", time_spent_small, i);
		thread_counter = 0;
		int *copyMid = malloc(mid * sizeof(int));
		memcpy(copyMid, arrMid, mid * sizeof(int));

		printf("Sorting Mid...\n");
		begin = clock();
		quickSortPar(copyMid, 0, mid-1, i);
		end = clock();
		double time_spent_mid = (double)(end - begin) / CLOCKS_PER_SEC;
		free(copyMid);
		printf("\ntime spent on mid: %f, threads: %d\n", time_spent_mid, i);
		thread_counter = 0;
		int *copyLarge = malloc(large * sizeof(int));
		memcpy(copyLarge, arrLarge, large * sizeof(int));

		isLarge = 1;
		printf("Sorting Large...\n");
		begin = clock();
		quickSortPar(copyLarge, 0, large-1, i);
		end = clock();
		double time_spent_large = (double)(end - begin) / CLOCKS_PER_SEC;
		free(copyLarge);
		printf("\ntime spent on large: %f, threads: %d\n", time_spent_large, i);
		thread_counter = 0;
	}




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

int partition( int a[], int low, int high )
{
 int left, right;
 int pivot_item;
 int pivot = left = low;
 pivot_item = a[low];
 right = high;
 while ( left < right )
 {
  // Move left while item < pivot
  while( a[left] <= pivot_item )
   left++;
  // Move right while item > pivot
  while( a[right] > pivot_item )
   right--;
  if ( left < right )
   swap(a,left,right);
 }
 // right is final position for the pivot
 a[low] = a[right];
 a[right] = pivot_item;
 return right;
}//end partition


void quickSortPar(int *arr, int left, int right, int num_threads)
{
	int mid;
	pthread_t ptl, ptr;
	if (left < right)
	{
		if (isLarge) {
			printf("doing partition\n");
		}
		mid = partition(arr, left, right);
		struct partition_arg l = {left, mid - 1, arr, num_threads};
		struct partition_arg r = {mid + 1, right, arr, num_threads};
		if (thread_counter < num_threads) {
			printf("start creating\n");
			pthread_create(&ptl, NULL, (void *)qsort_thread_function, &l);
			printf("stop creating 1\n");
			pthread_create(&ptr, NULL, (void *)qsort_thread_function, &r);
			printf("stop creating 2\n");
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
}
