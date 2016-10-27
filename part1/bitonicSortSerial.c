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
	callerSB(N, d);

	int i;
	for(i=0;i<N;++i){
		printf("%d ", d[i]);
	}
	printf("\n");
}
