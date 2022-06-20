/*	
	This program calculates the result of an operation (base ^ power) mod m
	without calculating base ^ power directly.
*/

#include<stdio.h>

#define MAX 20

// function to convert an interger (positive) into binary (stored in array binary)
int Dec2Bin( const long number, char bin[] ){

	int i, cnt = 0;
	long num;
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
long powermod( const long b, const long p, const long m ){

	char binary[MAX];
	int i,j, len;
	long rem, powof2 ;

	// convert p to binary
	len = Dec2Bin( p, binary );
	rem = powof2 = 1;

	for(i=len-1; i>= 0; i-- ){
		long part = 1;
		if( binary[i] == '1' ){	

			for(j=0;j < powof2; j++)
				part *= b;

			rem *= (part % m);
		}
		powof2 *= 2;
	}
	return (rem % m);
}


int main(){

	long rem, base, power, mod;

	system("clear");
	printf("\n\nEnter the base of operation: ");	scanf("%d", &base );
	printf("\n\nEnter the power: ");	scanf("%d", &power );
	printf("\n\nEnter the mod term: ");	scanf("%d", &mod );

	rem = powermod( base, power, mod );
	printf("\n\n( %d ^ %d ) mod %d 	= 	%d\n", base, power, mod, rem );

	return 0;
}
