#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define SYMMAX 10	// maximum number of symbols
#define CODEMAX 6	// maximum length of code
#define SZ 50

typedef struct symbol_and_frequency
{
	char sym[SYMMAX];
	float freq;
}symfreq;

typedef struct symbol_and_code
{
	char sym;		// each symbol is a character
	char code[CODEMAX];
}symcode;

void Display( symcode[], int );
void SortOnFrequency( symfreq[], int );
void AddChar( symcode[], int, char, char );
int BuildCode( symfreq[], symcode[], const int );
int Encode( const char[], const symcode[], const int, char[] );
int Decode(const char[],const symcode[], const int,const int, char[]);

/******************************************/
// function to sort a set of num symbols in decreasing order of frequency
void SortOnFrequency(symfreq arr[],int num)
{
	int i,j;
	symfreq temp;
	for(i=0;i<num-1;i++)
	{
		for(j=i+1;j<num;j++)
		{
			if(arr[i].freq<arr[j].freq)
			{					//swap
				temp=arr[i]; 
				arr[i]=arr[j]; 
				arr[j]=temp;
			}
		}
	}
	return;
}

/**************************************/
// function to add a character to the code of a symbol
void AddChar( symcode arr[], int size, char symbol, char toadd ){
	int i,length,j;
	char str[2]; 
	str[0] = toadd; 
	str[1] = 0;

	for(i=0;i<size;i++)
	{
		if(arr[i].sym == symbol)
		{
			length=strlen(arr[i].code);
			printf("\nsymbol=%c,code=%s,toadd=%c\n",symbol,arr[i].code,toadd);

			for(j=length;j>0;j--)
				arr[i].code[j]=arr[i].code[j-1];
			arr[i].code[0]=toadd;
			return ;
		}
	}
}

int BuildCode( symfreq in[], symcode code[], const int totsym )			// function to build the Huffman code for num symbols
{
	int i,num, maxlen;	
	float totfreq;
	num = totsym;

	totfreq = 0.0;
	for(i=0; i< totsym; i++)			// checking if the total of all frequencies is 1.0
		totfreq += in[i].freq ;
	if( totfreq != 1.0 )
	{ 
		printf("\nTotal frequency not 1."); 
		return 0; 
	}

	
	SortOnFrequency(in,totsym);
	
	for(i=0;i<totsym;i++)
	{
		code[i].sym=in[i].sym[0]; 
		code[i].code[0]='\0';
	}
	while(1)
	{
		for(i=0;in[num -1].sym[i];i++)				// add '0' to the code of all symbols in the last entry of in
			AddChar(code,totsym,in[num-1].sym[i],'0');
		for(i=0;in[num-2].sym[i];i++)				// add '1' to the code of all symbols in the last-1 entry of in
			AddChar(code,totsym,in[num-2].sym[i],'1');

		strcat(in[num-2].sym,in[num-1].sym);			// join the symbols and add their frequencies
		in[num-2].freq += in[num-1].freq;
		num--;
		if(num == 1) 
			break;
		SortOnFrequency( in, num );
	}
				// returning the maximum length of a code
	maxlen =  0;
	for( i=0; i < totsym; i++)
		if( strlen( code[i].code ) > maxlen )	
			maxlen = strlen( code[i].code );

	return maxlen;
}

int Encode(const char in[],const symcode list[],const int symnum,char coded[])	  // function to encode an input string using the code built 
{
	int i,j, success;
	coded[0] = 0;

	for( i=0; in[i]; i++)
	{	
		success = 0;
		for( j=0; j< symnum; j++)
		{
			if( in[i] == list[j].sym )
			{
				strcat( coded, list[j].code );
				success = 1;
				break;
			}
		}
		if( !success ) 
			return 0;	// undefined symbol
	}
	return 1;
}

								// function to decode an encoded string using the code built
int Decode( const char en[], const symcode list[], const int symnum, const int maxlen, char de[]){

	int i,j, ai, dei, success;
	char arr[SYMMAX+1];
	arr[0] = de[0] = ai = dei = 0;

	for( i=0; en[i]; i++){
		
		success = 0;
		if( ai >= maxlen ) return 0;		// the code is too long to be correct
		arr[ai] = en[i]; ai++ ; arr[ai] = 0;

		for( j=0; j<symnum; j++){
			
			if( !strcmp( arr, list[j].code ) ){
				de[dei] = list[j].sym;
				dei++ ;
				success = 1;		// a symbol successfully found
				break;
			}
		}
		if( success )	ai=0;
	}
	de[dei] = 0;
	return 1;
}

int main(){
	symfreq* input;
	symcode* codelist;
	int i, numsym, maxcodelen;
	char s;	
	char instring[SZ] = "abaccadf", encoded[SZ], decoded[SZ];

	system("clear");	
	while(1)
	{
		printf("Enter the number of symbols ( <= %d ): ", SYMMAX); 
		scanf("%d", &numsym);
		getchar();
		if( numsym <= SYMMAX ) 
			break;
	}
	
	input = (symfreq*)calloc( numsym, sizeof(symfreq) );
	codelist = (symcode*)calloc( numsym, sizeof(symcode) );
	
	for(i=0; i<numsym;i++)
	{
		printf("\nEnter symbol #%d: ", i+1 );
		scanf("%c", &s ); getchar();
		input[i].sym[0] = s;  input[i].sym[1] = '\0';
		printf("Enter its probability ( between 0 and 1 ): ");
		scanf("%f", &input[i].freq ); getchar();
		
	}


	maxcodelen = BuildCode( input, codelist, numsym );
	if(!maxcodelen)
	{ 
		printf("\nInvalid inputs\n");
		exit(1); 
	} 
	
	printf( "\n\nCodes are: \n\n");
	Display( codelist, numsym );

	while(1){
		char c;

		printf("\nEnter the input string:\n");
		scanf("%[^\n]", instring );	getchar();

		printf("\nINPUT string:  %s\n", instring );

		if( Encode( instring, codelist, numsym, encoded ) )
			printf("\nENCODED STRING:\n%s\n", encoded );
		else
		{
			printf("\nUndefined symbol\n");	
			exit(1);
	 	}
		if( Decode( encoded, codelist, numsym, maxcodelen, decoded ) )
			printf("\nDECODED STRING:\n%s\n", decoded );
		else
		{
			printf("\nUndefined code\n");
		 	exit(1);
	 	}
		printf("\nWish to continue( y/n)?");
		c = getchar(); 
		getchar();
		if( c == 'n' ) break;
	}

	free( input ); free( codelist ); return 0;
}

void Display( symcode arr[], int num )					// function to display the list of symbols and codes
{
	int i;
	for( i=0; i< num; i++)
		printf("\n%c\t%s", arr[i].sym, arr[i].code );
	printf("\n");
}

