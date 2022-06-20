/*	
	PROGRAM TO SELECT EVENTS FROM A LIST OF EVENTS (MAY BE INTERSECTING WITH EACH OTHER)
	SO THAT THE LARGEST NUMBER OF NON-INTERSECTING EVENTS CAN BE INCLUDED
*/

#include<stdio.h>

typedef struct{
	int id;
	float st, end;
}	event;

// sort the list of events in increasing order of finishing times
void SortEvents( event list[], int num ){
	int i,j;
	event t;

	for( i=0; i< num-1; i++){
		for( j=i+1; j < num; j++){
			
			if( list[i].end > list[j].end )
			{	t = list[i]; list[i] = list[j]; list[j] = t; 	}
		}
	}
	return;
}

// select the events so that the max number of events can be included by greedy algorithms
int SelectEvents( event slist[], event selected[], int num ){
	
	int i, index = 0;
	selected[index] = slist[0];

	for( i=1; i<num; i++){
		if( slist[i].st >= selected[index].end ){
			index++ ;
			selected[index] = slist[i];
		}
	}

	return (index+1);
}

int main(){
	int num, i, selnum;
	event *list, *select;

	system("clear");
	printf("\nEnter the number of events to be considered: ");
	scanf("%d", &num);

	list = (event*)calloc( num, sizeof(event) );
	if( list == NULL ){	printf("\nOut of memory\n"); exit(1); 	}

	select = (event*)calloc( num, sizeof(event) );
	if( select == NULL ){	printf("\nOut of memory\n"); exit(1); 	}

	for( i=0; i<num;i++ ){
		printf("\nFOR EVENT #%d\n", i+1 );
		list[i].id = i+1;
		printf("Enter start time: ");	scanf("%f", &(list[i].st) );
		printf("Enter end time: ");	scanf("%f", &(list[i].end) );
	}

	SortEvents( list, num );
	selnum = SelectEvents( list, select, num );

	printf("\nTHE LIST OF SELECTED EVENTS IS:\n\n");

	for( i=0; i< selnum; i++){
		printf("\nEvent %d, starts at %.2f, ends at %.2f.", select[i].id, select[i].st,
						select[i].end );
	}
	printf("\n");
	return 0;
}
