#include<stdio.h>
#include<stdlib.h>

#define MAX(A,B) ((A > B) ? A : B)
#define MIN(A,B) ((A > B) ? B : A)
#define UP 0
#define DOWN 1

void bitonicSortSeq(int start, int length, int *arr, int flag);
void bitonicSortPar(int start, int length, int *arr, int flag);
void swap(int a[], int left, int right);
void quickSortSeq(int *arr, int left, int right);
void quickSortPar(int *arr, int left, int right);

int main()
{
	int small, mid, large;
	int range;
	double startTime, endTime;
	double initTime;

	/* pwrs of 2 */
	small = 10;
	mid = 16384;
	large = 131072;

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

	/* start recording */

	printf("Before sorting: ");
	for (i = 0; i < small; i++)
		printf("%d ", arrSmall[i]);
	quickSortSeq(arrSmall, 0, small-1);
	printf("\nAfter sorting: ");
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

void quickSortPar(int *arr, int left, int right)
{
	int mid;
	if (left < right)
	{
		mid = partition(arr, left, right);
		quickSortPar(arr, left, mid - 1);
		quickSortPar(arr, mid + 1, right);
	}
}
