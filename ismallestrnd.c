/*	
	A PROGRAM TO SELECT THE iTH SMALLEST NUMBER FROM A LIST OF NUMBERS
	THIS PROGRAM ALSO GIVES AN ESTIMATE OF THE TIME REQUIRED TO SELECT THE iTH SMALLEST ELEMENT.
*/

#include<stdio.h>
#include<stdlib.h>

// function to swap the i-th and j-th element of the list
void Swap( int arr[], int i, int j ){
	int t;
	t = arr[i]; arr[i] = arr[j]; arr[j] = t;
	return;
}

/* function to place a randomly selected element in its proper (sorted)
   position in the list	
   i.e. a number will be so placed that all the numbers to its left 
   are smaller than it and all numbers to its right are greater than it
*/
int RandomPartition( int arr[], int left, int right ){
	int i, last;
	if( left == right ) return right;

	Swap( arr, left, (left + right)/2 );
		// we shall position the middle element in its sorted position
	last = left;
	for( i = left+1; i <=right; i++ )
		if( arr[i] <= arr[left] )
			Swap( arr, ++last, i );
		// all smaller elements have been taken to the left
	Swap( arr, left, last );
	return last ;
}

// function to select the ith smallest element using the above function.
int RandomSelect( int arr[], int start, int end, int ss ){

	int rn, part ;
	if( start == end )	return arr[end] ;

	rn = RandomPartition( arr, start, end );
	// the element at position rn is at its proper (sorted) position

	part = rn - start + 1;

	if( ss <= part )	// work with the left hand side only
		return RandomSelect(arr, start, rn, ss) ;
	else			// work with the right hand side only
		return RandomSelect(arr, rn+1, end, ss-part );
}

int main(){
	int *arr, i, choice, num;
	double t1, t2;
	struct timeval before, after;
	char fname[20];	FILE* fp;

	// take inputs from the user
	system("clear");
	printf("\nEnter number of elements: ");
	scanf("%d", &num );
	arr = (int*)calloc( num, sizeof(int)  );
	if( arr == (int*)NULL ){ printf("\nOut of memory\n"); exit(1); }

	printf("\nEnter the name of file for input: ");
	scanf("%s", fname);
	fp = fopen( fname, "r" );
	if( fp == NULL){
		printf("\nCould not open file %s\n", fname);
		exit(1);
	}

	printf("\nALL ELEMENTS NEED TO BE DISTINCT\n");
	for(i=0;i<num;i++){
		//printf("Enter element #%d :  ", i+1 );
		//scanf("%d", &arr[i] );
		if( feof(fp) ){
			printf("\nNot enough numbers\n"); 
			free( arr ); fclose(fp); exit(1);
		}
		fscanf( fp, "%d", &arr[i] );
	}
	while(1)
	{
	t1 = t2 = 0;
	printf("\nEnter the position of desired element (from smallest): ");
	scanf("%d", &choice);
	if( choice <= 0 || choice > num ) break;

	gettimeofday( &before, NULL );
	i = RandomSelect( arr, 0, num-1, choice );
	gettimeofday( &after, NULL );

	printf("\nThe %d-th smallest element is %d", choice, i);
	t1 = (double)( before.tv_sec * 1000000 + before.tv_usec );
	t2 = (double)( after.tv_sec * 1000000 + after.tv_usec );
	printf("\nSearch took %g microseconds\n\n", t2 - t1 );
	}
	free(arr); 	close(fp);	return 0;
}
