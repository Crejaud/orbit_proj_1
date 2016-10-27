#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define MAX(A,B) ((A > B) ? A : B)
#define MIN(A,B) ((A > B) ? B : A)
#define UP 0
#define DOWN 1

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


//-------------BITONIC PARALLEL----------

//Bitonic Sort
//For dataBP sizeBPs of powers of 2

int numThreadsBP;
int maxThreadsBP;

struct threadStructP{
	int startIndexBP;
	int sizeBP;
	int* dataBP;
	int dirBP;
};

void mergeBP(int* dataBP, int startIndexBP, int sizeBP, int dirBP){
	if(sizeBP==1){return;}
	else{
		//compare the first half's [i] to the second halfs [i]
		int y;
		for(y=0; y<sizeBP/2; ++y){
			//if swap the pair into the order indicated by dirBP
			if((dataBP[startIndexBP+y]>dataBP[startIndexBP+(sizeBP/2)+y] && dirBP==1)||(dataBP[startIndexBP+y]<dataBP[startIndexBP+(sizeBP/2)+y] && dirBP==0)){
				int temp=dataBP[startIndexBP+y];
				dataBP[startIndexBP+y]=dataBP[startIndexBP+(sizeBP/2)+y];
				dataBP[startIndexBP+(sizeBP/2)+y]=temp;
			}

		}
	
//	print(dataBP, 0, 16);
	//reorder subsets
	mergeBP(dataBP, startIndexBP, sizeBP/2, dirBP);
	mergeBP(dataBP, startIndexBP+(sizeBP/2), sizeBP/2, dirBP);
	}
	return;
}

void* parallelSortBP(void* derefStruct);

//make dirBP true when calling initially
void bitonicSortBP(int* dataBP, int startIndexBP, int sizeBP, int dirBP){


	//standard
	if(sizeBP==1){return;}
	//parallel
	else if(numThreadsBP<maxThreadsBP){
		pthread_t threadL, threadH;
		struct threadStructP* lower = (struct threadStructP *) malloc(sizeof(struct threadStructP));
		lower->startIndexBP=startIndexBP;
		lower->sizeBP=sizeBP/2;
		lower->dataBP=dataBP;
		lower->dirBP=dirBP;

		struct threadStructP* upper = (struct threadStructP *) malloc(sizeof(struct threadStructP));
		upper->startIndexBP=startIndexBP+(sizeBP/2);
		upper->sizeBP=sizeBP/2;
		upper->dataBP=dataBP;
		if(dirBP==1){
			upper->dirBP=0;
		}
		else{
			upper->dirBP=1;
		}

 		pthread_create(&threadL, NULL, parallelSortBP, lower);
 		pthread_create(&threadH, NULL, parallelSortBP, upper);

		pthread_join(threadL, NULL);
		pthread_join(threadH, NULL);
		free(lower);
		free(upper);
	}
	//non parallel
	else{
		//divide into sub arrays
		bitonicSortBP(dataBP, startIndexBP, sizeBP/2, dirBP);
		//note dirBPection switches to maintain bitonic ordering
		if(dirBP==1){
			bitonicSortBP(dataBP, startIndexBP+(sizeBP/2), sizeBP/2, 0);
		}
		else{
			bitonicSortBP(dataBP, startIndexBP+(sizeBP/2), sizeBP/2, 1);
		}
		//once we have reached 1 element pairs, start merging them
	}
	mergeBP(dataBP, startIndexBP, sizeBP, dirBP);
	return;
}

void* parallelSortBP(void* derefStruct){
		++numThreadsBP;
		struct threadStructP* tStruct = derefStruct;
		bitonicSortBP(tStruct->dataBP, tStruct->startIndexBP, tStruct->sizeBP, tStruct->dirBP);
		pthread_exit(NULL);
	return NULL;
}

void callerBP(int n, int* dataBP, int threads){
	//set sizeBP and initialize

	numThreadsBP=0;
	maxThreadsBP=threads;

	//printing and sortingg

	bitonicSortBP(dataBP, 0, n, 1);
}



//------------------_BITONIC SERIAL-----------------

//Bitonic Sort
//For dataSB sizeSBs of powers of 2

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mergeSB(int* dataSB, int startIndexSB, int sizeSB, int dirSB){
	if(sizeSB==1){return;}
	else{
		//compare the first half's [i] to the second halfs [i]
		int i;
		for(i=0; i<sizeSB/2; ++i){
			//if swap the pair into the order indicated by dirSB
			if((dataSB[startIndexSB+i]>dataSB[startIndexSB+(sizeSB/2)+i] && dirSB==1)||(dataSB[startIndexSB+i]<dataSB[startIndexSB+(sizeSB/2)+i] && dirSB==0)){
				int temp=dataSB[startIndexSB+i];
				dataSB[startIndexSB+i]=dataSB[startIndexSB+(sizeSB/2)+i];
				dataSB[startIndexSB+(sizeSB/2)+i]=temp;
			}

		}
	//reorder subsets
	mergeSB(dataSB, startIndexSB, sizeSB/2, dirSB);
	mergeSB(dataSB, startIndexSB+(sizeSB/2), sizeSB/2, dirSB);
	}
	return;
}

//make dirSB true when calling initially
void bitonicSortSB(int* dataSB, int startIndexSB, int sizeSB, int dirSB){
	if(sizeSB==1){return;}
	else{
		//divide into sub arrays
		bitonicSortSB(dataSB, startIndexSB, sizeSB/2, dirSB);
		//note dirSBection switches to maintain bitonic ordering
		if(dirSB==1){
			bitonicSortSB(dataSB, startIndexSB+(sizeSB/2), sizeSB/2, 0);
		}
		else{
			bitonicSortSB(dataSB, startIndexSB+(sizeSB/2), sizeSB/2, 1);
		}
		//once we have reached 1 element pairs, start merging them
		mergeSB(dataSB, startIndexSB, sizeSB, dirSB);
	}
	return;
}

int callerSB(int sizeSB, int* dataSB){
	bitonicSortSB(dataSB, 0, sizeSB, 1);
}


//--------------------MAIN---------------------


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
