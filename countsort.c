#include<stdio.h>
#include<stdlib.h>
int complexity;
int CountSort( int arr[], int size, int max)			// input: arr[] of length 'size' & all elements less than 'max'
{
	int *count, *b; int i;
	complexity=0;
	count = (int*)calloc( max+1, sizeof(int) );
	b = (int*)calloc( size, sizeof(int) );			// array b is used to store intermediate results
	for(i=0;i<size;i++)					// counting the number of occurences of each element
	{
		if( arr[i] < 0 || arr[i] > max )		// unsuitable input values
		{
			free(count);
			free(b);
			return 0;
		}
		count[ arr[i] ]++ ;
		complexity ++;
	}
	for( i=1; i<=max; i++)					// calculating the 'cumulative less than or equal to' values
	{
		count[i] += count[i-1] ;			
		complexity ++;
	}
	for( i=size-1; i>=0; i--)				// placing elements in their suitable positions in array b
	{
		int index;
		index = count[ arr[i] ] ;
		b[index-1] = arr[i];
		count[ arr[i] ]-- ;
		complexity ++;
	}
	for(i=0;i<size;i++)					// putting the sorted list back into the input array
		arr[i] = b[i];

	free(count);
	free(b);
	return 1;
}

int main()
{
	int num, *arr,i, max ;
	system("clear");
	printf("\n			COUNTING SORT \n");

	printf("Enter the number of numbers : ");
	scanf("%d", &num);

	printf("Enter the maximum number : ");
	scanf("%d", &max);

	arr = (int*)calloc( num, sizeof(int) );
	for(i=0;i<num;i++)
		arr[i]=rand()%max;
	printf("\nThe input list is:\n");
	for(i=0;i<num;i++)
		printf("%d ", arr[i] );
	printf("\n");
	
	if( CountSort( arr, num, max ) )
	{
		

		printf("\nThe sorted list is:\n");
		for(i=0;i<num;i++)
			printf("%d ",arr[i] );
		printf("\n\n\noperations to sort %d numbers is %d .\n",num,complexity);
	}
	else
		printf("\nUnsuitable inputs\n");
	
	printf("\n");
	return 0;
}
