/*
	PROGRAM TO ORDER A CHAIN OF MATRICES COMPATIBLE FOR CHAIN MULTIPLICATION 
	SUCH THAT THE MULTIPLICATION PROCESS IS OPTIMISED.
*/

#include<stdio.h>
#include<stdlib.h>
#define MAXSZ 40	// max size of a matrix
#define MAXNUM 8	// max number of matrices
#define INF 9999999	// a very large number, infinity

typedef struct matrixtype
{
	int mat[MAXSZ][MAXSZ],row,col;
} matrix;

void Display(matrix*);

int MultiplyMat( matrix* a, matrix* b, matrix* c)		// function to multiply two matrices
{	
	int i,j,k;
	if( a->col != b->row ) 			// matrices suitable for multiplication
		return 0;
	c->row = a->row;
	c->col = b->col;
	
	for( i=0; i< c->row; i++)
	{
		for( j=0; j< c->col; j++)
		{
			
			c->mat[i][j] = 0;
			for( k=0; k< a->col; k++ )
				c->mat[i][j] += a->mat[i][j] * b->mat[j][k] ;
		}
	}
	return 1;
}

void Display( matrix* m )		// function to display a matrix
{

	int i,j;
	if( m->row > MAXSZ || m->col > MAXSZ ){ printf("\nWrong dimensions\n");
				return;}

	for( i=0; i< m->row; i++){
		printf("\n");
		for( j=0; j < m->col; j++ )
			printf("\t%d", m->mat[i][j] );
	}
	return;
}
									// function to order the matrix chain for optimal multiplication
int OrderChain( int p[], int nummat, unsigned long m[MAXNUM][MAXNUM], int s[MAXNUM][MAXNUM] )
{
	int i,j,l,k,q;

	for(i=0;i<nummat;i++)	
		m[i][i] = 0;
	for(i=0;i<nummat-1;i++)
		s[i][i+1] = i;
	
	for(l=2;l<=nummat;l++)		// for chains of length 2,3,...nummat
	{	
		for(i=0;i< nummat-l+1;i++)			// chain (of length l) starting from matrix #i
		{
			j = i+l-1 ;		// end of this chain is at matrix #j
			m[i][j] = INF;		// a very large number

			for(k=i;k<j;k++)			// finding a suitable place k for placing bracket
			{
				q = m[i][k] + m[k+1][j] + p[i]*p[k+1]*p[j+1];
				if( q < m[i][j] )
				{
					m[i][j] = q;	
					s[i][j] = k;
				}
			}		// end loop with k
		}			//		 i
	}				//		 l
	return 1;
}

/************************************************/
// function to display the order of multiplication

void Parenthesize( int start, int end, int s[MAXNUM][MAXNUM] ){

	if( start < end ){
		printf(" ( ");
		Parenthesize( start, s[start][end], s);
		printf(" * ");
		Parenthesize( s[start][end]+1, end, s);
		printf(" ) ");
		return;
	}
	if( start == end ){
		printf( "M%d", end+1 );
		return;
	}
	printf("\nstart = %d, end = %d, MISTAKE IN PRINTING PARENTHESIZATION\n",start, end);
	exit(0);
}

/*******************************************/
// function to multiply a chain recursively

void MultiplyChain( matrix* res, matrix arrmat[], int s[MAXNUM][MAXNUM], int start, int end){

	if( end > start )
	{
		matrix X, Y, Z; int c;
		MultiplyChain( &X, arrmat, s, start, s[start][end] );
		MultiplyChain( &Y, arrmat, s, s[start][end]+1, end );
		c =  MultiplyMat( &X, &Y, &Z );
		if(!c)
		{
			printf("\nError in multiplying\n");
			exit(1);
		}
		*res = Z;

	}
	else
		*res =  arrmat[start] ;

}

int main(){

	matrix arrmat[MAXNUM], result;
	int nummat,i,j,k;
	unsigned long m[MAXNUM][MAXNUM];
	int s[MAXNUM][MAXNUM], p[MAXNUM+1];

	for(i=0;i<MAXNUM; i++)
		for(j=0;j<MAXNUM;j++)
			m[i][j] = s[i][j] = 0;

	system("clear"); 

	while(1)
	{
		printf("\nHow many matrices to multiply (less than %d): ", MAXNUM );
		scanf("%d", &nummat);
		if( nummat < MAXNUM )
		 	break;
	}

	system("clear");
	for( i=0;i< nummat;i++)
	{
		printf("\nFOR MATRIX #%d\n", i+1);
		printf("\nEnter number of rows: ");
		scanf("%d", &(arrmat[i].row) );
		printf("\nEnter number of columns: ");
		scanf("%d", &(arrmat[i].col) );

	}

	// initializing the array p
	p[0] = arrmat[0].row;
	for(i=0;i< nummat;i++)
		p[i+1] = arrmat[i].col ;


	OrderChain( p, nummat, m,s);
	printf("\n\nTHE OPTIMAL PARENTHISIZATION IS:\n");
	Parenthesize( 0, nummat-1, s);  
	printf("\n\nNumber of scalar multiplications: %d\n", m[0][nummat-1] );
	return 0;

	MultiplyChain( &result, arrmat, s, 0, nummat-1 );

	// display result
	printf("\n\n\n******************* RESULT ****************\n");
	Display( &result );

	printf("\n");
	return 0;
}
