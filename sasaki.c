/************************************************************************************
  SIMULATOR	:	DISCRETE EVENT SIMULATOR for Distributed Sorting on a line network
  PROBLEM	: 	SASAKI (n-1) Sort for Distributed Computing
  CODE WRITTEN BY : SHOBHIT MALARYA
  					UG-3 CSE
*************************************************************************************/

/* Including Libraries */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

/* Declaring Defaults */
#define N_DEFAULT 	5
#define SEND		1
#define RECIEVE		0


// Structure for Node 
typedef struct Node {
	int id;
	int leftUnique;
	int rightUnique;
	int leftValue;
	int rightValue;
	int area;
	struct Node *leftNode;
	struct Node *rightNode;
}*NODE;

// Structure for Events
typedef struct Event {
	NODE sourceNode;
	NODE targetNode;
	int eventType;  		// SEND = 1 // RECIEVE = 0
	int eventValue;
	int unique;
	struct Event *eventNext;
} *EVENT;


// Function for creating a list list on (n) nodes
NODE createLinklist(int n){

	int i;

	NODE startNode = (NODE)malloc(sizeof(struct Node));
	NODE node = (NODE)malloc(sizeof(struct Node));

	startNode->id = 1;
	startNode->leftValue = -1;
	startNode->leftUnique = 0;
	startNode->rightUnique = 1;
	startNode->rightValue = rand()%(n*n)+1;
	startNode->area = 0;
	startNode->leftNode = NULL;
	startNode->rightNode = node;

	node->id = 2;
	node->leftValue = rand()%(n*n)+1;
	node->rightValue = node->leftValue ;
	node->area = 0;
	node->leftUnique = 0;
	node->rightUnique = 0;
	node->leftNode = startNode;
	node->rightNode = NULL;

	for( i = 3 ; i <= n-1 ; i++ ){


		NODE newNode = (NODE)malloc(sizeof(struct Node));
		node->rightNode = newNode;
		newNode->id = i;
		newNode->leftUnique = 0;
		newNode->rightUnique = 0;
		newNode->leftValue = rand()%(n*n)+1;
		newNode->rightValue = newNode->leftValue ;
		newNode->area = 0;
		newNode->leftNode = node;
		newNode->rightNode = NULL;

		node = newNode; 		
	}

	NODE endNode = (NODE)malloc(sizeof(struct Node));
	node->rightNode = endNode;
	endNode->id = i;
	endNode->leftUnique = 1;
	endNode->rightUnique = 0;
	endNode->leftValue = rand()%(n*n)+1;
	endNode->rightValue = -1;
	endNode->area = 0;
	endNode->leftNode = node;
	endNode->rightNode = NULL;

	return startNode;
}


// Printing node using area 
void printSasakiNode(NODE node){

	if ( node->leftValue == -1 ){
		printf("-| %d |-", node->rightValue);
	} else if ( node->rightValue == -1 ){
		printf("-| %d |-", node->leftValue);
	} else if ( node->area == -1 ){
		printf("-| %d |-", node->rightValue );
	} else {
		printf("-| %d |-", node->leftValue );
	}
}

// Function to iterate on Linked List for printing nodes using area
void printSasaki(NODE node){
	while (node != NULL){
		printSasakiNode(node);
		node = node->rightNode;
	}
}

// Function for printing a node
void printNode(NODE node){
	printf("-|%d|%d|(%d)|-",  node->leftValue , node->rightValue, node->area);
}

// Function to iterate on Linked List for printing
void printLinkedlist(NODE node){
	while (node != NULL){
		printNode(node);
		node = node->rightNode;
	}
	printf("\n");

}

// Sorting elements in Each node
void sortNode(NODE node){
	while (node->rightNode != NULL){
		if ( ( node->leftValue != -1 ) || ( node->rightValue != -1 ) ){
			if ( node->leftValue > node->rightValue ){
				int temp = node->leftValue;
				node->leftValue = node->rightValue;
				node->rightValue = temp;
				temp=node->leftUnique;
				node->leftUnique=node->rightUnique;
				node->rightUnique=temp;
			}
		}
		node = node->rightNode;
	}
}


// Function for adding events in Event queue
void addEvent(EVENT eventNode, NODE sourceNode, NODE targetNode,int Type,int value,int unique){

	// for adding a new event at the end of the queue
	while ( eventNode->eventNext != NULL ) {
		eventNode=eventNode->eventNext;
	}

	EVENT newEventNode = (EVENT)malloc(sizeof(struct Event));
	newEventNode->sourceNode = sourceNode;
	newEventNode->targetNode = targetNode;
	newEventNode->eventType = Type;
	newEventNode->eventValue = value;
	newEventNode->unique=unique;
	newEventNode->eventNext = NULL;

	// printf("\nadding an event : source - %d , target - %d , Type - %d , value - %d\n", sourceNode->id, targetNode->id, Type, value);
	eventNode->eventNext = newEventNode;

}

// Function for extracting events and respective action
void eventQueue(EVENT eventNode){

	eventNode = eventNode->eventNext;

	while ( eventNode != NULL ){
		if (eventNode->eventType == SEND){										// Adding Recieve event for corresponding send event
			addEvent(eventNode,eventNode->targetNode,eventNode->sourceNode,RECIEVE,eventNode->eventValue,eventNode->unique);
		} else {
			if (eventNode->sourceNode->id > eventNode->targetNode->id){      	// if the recieving node is right and sender is left node
				if (eventNode->eventValue > eventNode->sourceNode->leftValue){ 	// if the recieving value is greater then keep it
					eventNode->sourceNode->leftValue = eventNode->eventValue;
					eventNode->sourceNode->leftUnique = eventNode->unique;
					// printf("\nUpdating %d with %d\n",eventNode->sourceNode->id , eventNode->eventValue );
					if(eventNode->unique==1)									// if unique element is shifted from (i-1) to i -> area updation (area - 1)
 						eventNode->sourceNode->area-=1;
				}
			} else {															// if the recieving node is left and sender is right node
				if (eventNode->sourceNode->rightValue > eventNode->eventValue){	// if the recieving value is smaller then keep it
					eventNode->sourceNode->rightValue = eventNode->eventValue;
					eventNode->sourceNode->rightUnique = eventNode->unique;
					// printf("\nUpdating %d with %d\n",eventNode->sourceNode->id , eventNode->eventValue );
					if(eventNode->unique==1)									// if unique element is shifted from (i+1) to i -> area updation (area + 1)
 						eventNode->sourceNode->area+=1;
				}
			}
		}

		eventNode = eventNode->eventNext;
	}
}


// Function for sasaki algo
void sasaki(NODE node, int n){
	int i = 1;	

	NODE startNode = node;
	while ( i <= n-1 ){

		EVENT headEventNode = (EVENT)malloc(sizeof(struct Event));
		headEventNode->eventNext = NULL;
		while (node != NULL){
			if (node->rightNode)
				addEvent(headEventNode,node,node->rightNode,SEND,node->rightValue,node->rightUnique);
			if (node->leftNode)
				addEvent(headEventNode,node,node->leftNode,SEND,node->leftValue,node->leftUnique);
			node = node->rightNode;
		}
		eventQueue(headEventNode);
		node = startNode;
		sortNode(startNode);
		// printLinkedlist(startNode);
		i++;
	}
}

// Main Function
int main(int argc, char const *argv[]) {
	
	srand(time(0));
	int n;

	n = (argc < 2) ? N_DEFAULT : atoi(argv[1]);

	NODE startNode = createLinklist(n);

	printf("\n\t\t\t----- No. of nodes = %d-----\n", n);

	printf("\n\n\n PRINTING CONVENTION \n\t -| Left Value | Right Value |( area )|-\n\n\n");
	printf("\n\n----INITIAL LINKED LIST----\n\n");
	printLinkedlist(startNode);

	sasaki(startNode,n);

	printf("\n\n----AFTER SASAKI----\n");
	printLinkedlist(startNode);


	printf("\n\n----PRINTING BY AREA SELECTION-----\n");
	printSasaki(startNode);

	printf("\n\n");
	return 0;

}