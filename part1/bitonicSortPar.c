//Bitonic Sort
//For dataBP sizeBPs of powers of 2

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

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

int main(){
//set sizeBP and initialize
	int N=16;
	int d[N];
	int threads=4;

	//seed and set random values
	time_t t;
	srand((unsigned) time(&t));
	int j;
	for(j=0; j<N; ++j){
		d[j]=(rand()%100);
	}
	
	//printing and sortingg
	callerBP(N, d, threads);

	int i;
	for(i=0;i<N;++i){
		printf("%d ", d[i]);
	}
	printf("\n");
}
