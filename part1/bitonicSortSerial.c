//Bitonic Sort
//For data sizes of powers of 2

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void merge(int* data, int startIndex, int size, int dir){
	if(size==1){return;}
	else{
		//compare the first half's [i] to the second halfs [i]
		int i;
		for(i=0; i<size/2; ++i){
			//if swap the pair into the order indicated by dir
			if((data[startIndex+i]>data[startIndex+(size/2)+i] && dir==1)||(data[startIndex+i]<data[startIndex+(size/2)+i] && dir==0)){
				int temp=data[startIndex+i];
				data[startIndex+i]=data[startIndex+(size/2)+i];
				data[startIndex+(size/2)+i]=temp;
			}

		}
	//reorder subsets
	merge(data, startIndex, size/2, dir);
	merge(data, startIndex+(size/2), size/2, dir);
	}
	return;
}

//make dir true when calling initially
void bitonicSort(int* data, int startIndex, int size, int dir){
	if(size==1){return;}
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
		merge(data, startIndex, size, dir);
	}
	return;
}

//print array
void print(int* data, int startIndex, int size){
	int r;
	for(r=0;r<size;++r){
		printf("%d ", data[startIndex+r]);
	}
	printf("\n");
	return;
}

int main(){
	//set size and initialize
	int N=8;
	int data[N];

	//seed and set random values
	time_t t;
	srand((unsigned) time(&t));
	int j;
	for(j=0; j<N; ++j){
		data[j]=(rand()%30);
	}
	
	//printing and sortingg
	print(data, 0, N);
	bitonicSort(data, 0, N, 1);
	print(data, 0, N);
}
