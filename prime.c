#include<stdio.h>

#define MAX 20

// function to convert an interger (positive) into binary (stored in array binary)
int Dec2Bin( int number, char bin[] ){

	int i, cnt = 0, num;
	char local[MAX];
	num = number;

	if( num == 0 ){		bin[0] =  '0';	return 1;	}
	if( num == 1 ){		bin[0] =  '1';	return 1;	}

	while(num >= 1){
		local[ cnt ] = '0' + num % 2;
		cnt++ ;
		num /= 2;
	}
	local[cnt] = 0;
	// reversing the remainders to get binary representation
	for( i= 0; i < cnt; i++)
		bin[ cnt - 1 - i ] = local[i];
	return cnt;		// return length of binary representation
}
		

// function to calculate ( b^p )mod m
int powermod( int b, int p, int m ){

	char binary[MAX];
	int i,j, len, rem, powof2 ;

	// convert p to binary
	len = Dec2Bin( p, binary );
	rem = powof2 = 1;

	for(i=len-1; i>= 0; i-- ){
		int part = 1;
		if( binary[i] == '1' ){	

			for(j=0;j < powof2; j++)
				part *= b;

			rem *= (part % m);
		}
		powof2 *= 2;
	}
	return (rem % m);
}

int IsCarmichel( const int num ){
	int base, rem;
	for( base =2; base < num; base++ ){
		rem = powermod( base, num-1, num );
		if (rem != 1 ){	printf("\nbase = %d, rem = %d\n",base, rem);	return 0; }
	}
	return 1;
}


int main(){

	int num;
	system("clear");
	num = powermod( 2, 340, 341 );
	printf("\n\n%d\n", num );

/*	printf("\nEnter the number (integer) to be tested for Carmichel number: ");
	scanf("%d", &num);
	if( IsCarmichel( num )  )	printf("\n\n%d is a Carmichel number.\n\n", num);
	else				printf("\n\n%d is NOT a Carmichel number.\n\n", num);
*/
	return 0;
}
