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
keytype* merge(keytype* A,keytype* B  );
int binarySearch(keytype* A, int vertex);

keytype* parallelSort (int N, keytype* A)
{	
	if( N < 1){
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
	for(int i = x ; i < N ; i ++){
		temp2[y] = A[i];
		y++;
		
		
	}
//	#pragma omp parallel 
//	{
		temp3 = parallelSort( N/2, temp);
//	}	
//	#pragma omp parallel 
//	{
		temp4 = parallelSort( N/2, temp2);
//	}
	
//	#pragma omp barrier
	return merge(temp,temp4);
}

keytype* merge(keytype* A,keytype* B ){
	int V1;
	int x1 = sizeof(A) / sizeof(int);
	int x2 = x1 /2;
	int y1 = sizeof(B) / sizeof(int);
	int y2 = y1 /2;
	int s1,s2,s3,s4;
	int temp = 0;
	int temp2 = 0;
	int size;
	size = x1 + y1;
	keytype* Small = newKeys(2);
	
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
	
	
	V1 = A[x2];
	int k = binarySearch(B, V1);
	printf("%d",k);
	keytype* C1 = newKeys(x2);
	keytype* C2 = newKeys(x2);
	keytype* D1 = newKeys(y2);
	keytype* D2 = newKeys(y2);
	//Partition into C
	for(int i = 0 ; i < x2 ; i++ ){ //C1
		C1[i] = A[i];
		
	}
	
	for(int i = x2 ; i < x1 ; i++ ){ //C2
		C1[temp] = A[i];
		temp++;
	}
	
	
	for(int i = 0 ; i < k ; i ++){ //D1
		D1[i] = B[i];
	}
	
	for(int i = k ; i < y1  ; i ++){ //D2
		D2[temp2] = B[i];
		temp2++;
	}
	
	s1 = sizeof(C1) / sizeof(int);
	s2 = sizeof(C2) / sizeof(int);
	s3 = sizeof(D1) / sizeof(int);
	s4 = sizeof(D2) / sizeof(int);
	keytype* E1 = newKeys(s1+s3);
	keytype* E2 = newKeys(s2+s4);
	printf("Test");
	E1 = merge(C1,D1); //E1
	
	E2 = merge(C2,D2); //E2
	
	size = x1 + y1;
	keytype* Final = newKeys(size);
	x1 = sizeof(A) / sizeof(int);
	y1 = sizeof(B) / sizeof(int);
	for(int i = 0 ; i < size ; i++){
		
		if( i < x1 ){
			Final[i] = E1[i];

		}
		
		else if (i == x1){
			Final[i] = V1;
			i++;			
		}
		
		else{
			Final[i] = E2[i];
		}
	}
	
/* 	for (int i = 0; i < size ; i++){
		printf("%d",Final[i]);
	}  */
	
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
