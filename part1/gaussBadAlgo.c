#include <iostream>
#include <stdlib.h>
#include <time.h>

int n;

void gauss(float A[3][3], float* B, float* Y){
	for(int k=0; k<n; k++){
		for(int j=k+1; j<n;++j){
			A[k][j]=A[k][j]/A[k][k];
		}
		Y[k]=B[k]/A[k][k];
		A[k][k]=1;
		for(int i=k+1; i<n; ++i){
			for(int j=k+1; j<n;++j){
				A[i][j]=A[i][j]-(A[i][k]*A[k][j]);
			}
			B[i]=B[i]-(A[i][k]*Y[k]);
			A[i][k]=0;
		}
	}
}

int main(){

	//initialize
	time_t t;
	srand((unsigned) time(&t));

	n=3;
	
	float A[3][3];
	float B[n];
	float Y[n];
	for(int i=0; i<n; ++i){
		for(int j=0; j<n;++j){
			A[i][j]=rand()%10;
			while(A[i][j]==0){
				A[i][j]=rand()%10;
			}
			std::cout<<A[i][j]<<" ";
		}
		std::cout<<"\n";
	}

	for(int j=0; j<n;++j){
		B[j]=rand()%10;
		while(B[j]==0){
			B[j]=rand()%10;
		}
		std::cout<<B[j]<<"\n";
	}

	
	//end init

	gauss(A, B, Y);
	for(int i=0;i<n;++i){
		for(int j=0; j<n; ++j){
			std::cout<<A[i][j]<<" ";
		}
		std::cout<<"\n";
	}

	for(int j=0; j<n; ++j){
		std::cout<<B[j]<<"\n";
	}

	for(int j=0; j<n; ++j){
		std::cout<<Y[j]<<"\n";
	}
}
