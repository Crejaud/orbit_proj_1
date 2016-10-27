//Bitonic Sort
//For data sizes of powers of 2

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int numThreads;
int maxThreads;

struct threadStruct{
	int startIndex;
	int size;
	int* data;
	int dir;
};

void print(int* data, int startIndex, int size);

void merge(int* data, int startIndex, int size, int dir){
	if(size==1){return;}
	else{
		//compare the first half's [i] to the second halfs [i]
		int y;
		for(y=0; y<size/2; ++y){
			//if swap the pair into the order indicated by dir
			if((data[startIndex+y]>data[startIndex+(size/2)+y] && dir==1)||(data[startIndex+y]<data[startIndex+(size/2)+y] && dir==0)){
				int temp=data[startIndex+y];
				data[startIndex+y]=data[startIndex+(size/2)+y];
				data[startIndex+(size/2)+y]=temp;
			}

		}
	
//	print(data, 0, 16);
	//reorder subsets
	merge(data, startIndex, size/2, dir);
	merge(data, startIndex+(size/2), size/2, dir);
	}
	return;
}

void* parallelSort(void* derefStruct);

//make dir true when calling initially
void bitonicSort(int* data, int startIndex, int size, int dir){


	//standard
	if(size==1){return;}
	//parallel
	else if(numThreads<maxThreads){
		pthread_t threadL, threadH;
		struct threadStruct* lower = (struct threadStruct *) malloc(sizeof(struct threadStruct));
		lower->startIndex=startIndex;
		lower->size=size/2;
		lower->data=data;
		lower->dir=dir;

		struct threadStruct* upper = (struct threadStruct *) malloc(sizeof(struct threadStruct));
		upper->startIndex=startIndex+(size/2);
		upper->size=size/2;
		upper->data=data;
		if(dir==1){
			upper->dir=0;
		}
		else{
			upper->dir=1;
		}

 		pthread_create(&threadL, NULL, parallelSort, lower);
 		pthread_create(&threadH, NULL, parallelSort, upper);

		pthread_join(threadL, NULL);
		pthread_join(threadH, NULL);
		free(lower);
		free(upper);
	}
	//non parallel
	else{
		//divide into sub arrays
		bitonicSort(data, startIndex, size/2, dir);
		//note direction switches to maintain bitonic ordering
		if(dir==1){
			bitonicSort(data, startIndex+(size/2), size/2, 0);
		}
		else{
			bitonicSort(data, startIndex+(size/2), size/2, 1);
		}
		//once we have reached 1 element pairs, start merging them
	}
	merge(data, startIndex, size, dir);
	return;
}

void* parallelSort(void* derefStruct){
		++numThreads;
		struct threadStruct* tStruct = derefStruct;
		bitonicSort(tStruct->data, tStruct->startIndex, tStruct->size, tStruct->dir);
		pthread_exit(NULL);
	return NULL;
}


//print array
void print(int* data, int startIndex, int size){
	int i;
	for(i=0;i<size;++i){
		printf("%d ", data[startIndex+i]);
	}
	printf("\n");
	return;
}

int main(){
	//set size and initialize
	int N=16;
	int data[N];

	numThreads=0;
	maxThreads=4;

	//seed and set random values
	time_t t;
	srand((unsigned) time(&t));
	int j;
	for(j=0; j<N; ++j){
		data[j]=(rand()%100);
	}
	
	//printing and sortingg
	print(data, 0, N);
	bitonicSort(data, 0, N, 1);
	print(data, 0, N);
}
