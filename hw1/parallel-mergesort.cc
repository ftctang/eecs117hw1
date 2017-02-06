/**
 *  \file parallel-mergesort.cc
 *
 *  \brief Implement your parallel mergesort in this file.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#include "sort.hh"
#include <string.h>

void  mergesort(keytype* A , int p , int r,keytype* Init , int threads);
void  merge(keytype* A,int p, int q, int r ,keytype* Init,int threads );
void  pmerge(keytype* A, keytype* Init , int p1,int r1, int p2,int r2 , int p3, int threads);
void smergesort(keytype* A, int p, int r, keytype* Init);
void smerge( keytype* A, int p, int q, int r, keytype* Init);
int binarySearch(keytype* Init, int vertex , int p, int r);

void parallelSort (int N, keytype* A) //Main Function
{	
	omp_set_num_threads(N-1);
	int tcount = omp_get_num_threads();
	keytype* Init = newKeys(N);
	#pragma omptask parallel
	{
	mergesort(A, 0, N-1,Init, N/tcount);	
	}
	
	free (Init); //Release 
}

void  mergesort(keytype* A , int p , int r, keytype* Init, int threads){ //MergeSort
	if ( (r-p + 1) <= threads) {
		smergesort(A,  p, r, Init); //Correction
		return;
	}	
	
	int mid = p + (p+r)/2;
	#pragma omp task
	mergesort( A, p,mid,Init,threads);
	mergesort( A,mid+1, r,Init,threads);
	#pragma omp taskwait
	merge( A ,p,mid,r,Init,threads);
	memcpy (A + p, Init + p, (r-p+1) * sizeof(keytype)); //Copy Init into A
	
}

void merge(keytype* A,int p, int q, int r ,keytype* Init,int threads){//Merge
	pmerge(A,Init ,p ,q , q+1,r,p,threads);// Call parallel merge
}

void pmerge(keytype* A, keytype* Init , int p1,int r1, int p2,int r2 , int p3, int threads ){// Parallel Merge
	
	int n1 = r1 - p1 + 1;
	int n2 = r2 - p2 + 1;
	
	if(n1 + n2 <= threads){
		int x = p1;
		int y = p2;
		int z = p3;

		while ( x <= r1 && y <= r2){ // Moving values from A into Init based on value 
			if(A[x] <= A[y]){
				Init[z] = A[x];
				x++;
			} else{
				Init[z] = A[y];
				y++;
			}
			z++;
		}
		
		while (x <= r1){ // Values left in A[p1,r1]
			Init[z] = A[x];
			x++;
			z++;
		}
		
		while(y <= r2){  //Values left in A[p2,r2]
			Init[z] = A[z];
			y++;
			z++;
		}
		return;
	}
	
	if(n1 < n2){ //ensure n1 <= n2
		int temp = p2;
		p2 = p1;
		p1 = temp;
		temp = r2;
		r2 = r1;
		r1 = temp;
		temp = n2;
		n2 = n1;
		n1 = temp;
	}
	
	if(n1 == 0){ //Empty
		return ;
	}
	
	
	int q1 = (p1 + r1) /2;
	int vertex = A[q1];
	int q2 = binarySearch(A,vertex , p2,r2);
	int q3 = p3 + (q1 - p1) + (q2 -p2);
	Init[q3] = A[q1];
	#pragma omp task
	pmerge(A,Init,p1,q1-1,p2,q2-1,p3,threads);
	pmerge(A,Init,q1+1,r1,q2,r2,q3+1,threads);
	#pragma omp taskwait	
	
}


int binarySearch(keytype* A, int vertex , int p, int r) //Search for Index
{
	int low = p;
	int max ;
	int mid ;
	int high;
	if(p > r+1){
		max = p;
	}
	else{
		max = r+1;
	}
	high = max;
	while (low < high){
		mid = (low + high) / 2;
		if (vertex <= A[mid] ){
			high = mid ; 	
		}
		else {
			low = mid +1;
		}
		
	}
	return high;
}

void smergesort(keytype* A, int p, int r, keytype* Init){ // Base case sorting
	
	if(p < r){
		int q = (p+r)/2;
		smergesort(A, p, q, Init);
		smergesort(A, q+1, r, Init);
		smerge(A, p, q, r, Init);
	}
}


void smerge( keytype* A, int p, int q, int r, keytype* Init){ //For Base case merging
	int x = p;
	int y = q + 1;
	int temp = p;
	
	while ( x <= q && y <= r){
		if (A[x] <= A[y]){
			Init[temp] = A[x];
			x++;
			temp++;
		}
		else{
			Init[temp] = A[y];		
			y++;
			temp++;
		}
	}
	
	while(x<=q){
		Init[temp] = A[x];
		x++;
		temp++;
	}
	
	while(y<=r){
		Init[temp] = A[y];
		y++;
		temp++; 
	}
	memcpy(A + p, Init + p, (r - p + 1) * sizeof (keytype));
}

/* eof */
