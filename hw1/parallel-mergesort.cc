/**
 *  \file parallel-mergesort.cc
 *
 *  \brief Implement your parallel mergesort in this file.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "sort.hh"
keytype* merge(keytype* A,keytype* B , int SizeA, int SizeB );
int binarySearch(keytype* A, int vertex);

keytype* parallelSort (int N, keytype* A)
{	
	if( N < 2){
	  return A;
	}
	keytype* temp = newKeys(N/2);
	keytype* temp2 = newKeys(N/2);
	keytype* temp3 = newKeys(N/2);
	keytype* temp4 = newKeys(N/2);

	for(int i = 0 ; i < N/2 ; i ++){
		temp[i] = A[i];
	}
	
	int x = (N/2) ;
	int y = 0;
	int x2 = N - x;
	for(int i = x ; i < N ; i ++){
		temp2[y] = A[i];
		y++;	
	}
//	#pragma omp parallel 
//	{
		temp3 = parallelSort( x, temp);
//	}	
//	#pragma omp parallel 
//	{
		temp4 = parallelSort( x, temp2);
//	}
//	#pragma omp barrier
	return merge(temp3,temp4,x,x2);
}

keytype* merge(keytype* A,keytype* B,int SizeA,int SizeB ){
	int V1;
	int size = SizeA + SizeB;
	keytype* Small = newKeys(2);
	int x1 = SizeA /2;
	int temp = 0;
	int temp2 = 0;
	if( size <= 2){ //if A and B are one
		
		if (A[0] > B[0]){			
			Small[0] = B[0];
			Small[1] = A[0];
		}
		else{
			Small[0] = A[0];
			Small[1] = B[0];
		}
		return Small;
	}
	
	keytype* Test = newKeys(size);
	V1 = A[x1];
	int k = binarySearch(B, V1);
	int k2 = SizeB - k;
	keytype* C1 = newKeys(x1);
	keytype* C2 = newKeys(x1);
	keytype* D1 = newKeys(k);
	keytype* D2 = newKeys(k2);
	
	//Partition into C
	for(int i = 0 ; i < x1 ; i++ ){ //C1
		C1[i] = A[i];
		
	}
	
	for(int i = x1 ; i < SizeA ; i++ ){ //C2
		C1[temp] = A[i];
		temp++;
	}
	
	
	for(int i = 0 ; i < k ; i ++){ //D1
		D1[i] = B[i];
	}
	
	for(int i = k ; i < SizeB  ; i ++){ //D2
		D2[temp2] = B[i];
		temp2++;
	}
	int size3 = k + x1; //E1
	int size4 = SizeA + k2; //E2
	keytype* E1 = newKeys(size3);
	keytype* E2 = newKeys(size4);
	E1 = merge(C1,D1,x1,k); //E1
	E2 = merge(C2,D2,x1,k2); //E2
	
	size = SizeA + SizeB;
	keytype* Final = newKeys(size);

	return Final;
}


int binarySearch(keytype* A, int vertex)
{
	int vertexspot = 0; 
	int x = sizeof(A) / sizeof(int);
	for(int i = 0 ; i < x ; i++){
			if(vertex > A[i]){
				vertexspot++;
			}
				
	}
}
/* eof */
