/************************************************************************************
  SIMULATOR	:	DISCRETE EVENT SIMULATOR for Distributed Sorting on a line network
  PROBLEM	: 	MODULUS (n-1) Sort for Distributed Computing
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
	int value;
	int flag;
	struct Node *leftNode;
	struct Node *rightNode;
}*NODE;

// Structure for Events
typedef struct Event {							
	NODE sourceNode;
	NODE targetNode;
	int eventType;  							// SEND = 1 // RECIEVE = 0
	int eventValue;
	struct Event *eventNext;
} *EVENT;

// Function for creating a list list on (n) nodes
NODE createLinklist(int n){						

	int i;

	NODE startNode = (NODE)malloc(sizeof(struct Node));
	NODE node = (NODE)malloc(sizeof(struct Node));

	startNode->id = 0;
	startNode->value = rand()%(n*n)+1;
	startNode->flag = 0;
	startNode->leftNode = NULL;
	startNode->rightNode = node;

	node->id = 1;
	node->value = rand()%(n*n)+1;
	node->flag = 1;
	node->leftNode = startNode;
	node->rightNode = NULL;

	for( i = 2 ; i < n ; i++ ){


		NODE newNode = (NODE)malloc(sizeof(struct Node));
		node->rightNode = newNode;
		newNode->id = i;
		newNode->value = rand()%(n*n)+1;
		newNode->flag = i%3;
		newNode->leftNode = node;
		newNode->rightNode = NULL;

		node = newNode; 		
	}
	node->rightNode = NULL;

	return startNode;
}

// Function for printing a node
void printNode(NODE node){
	printf(" -|id=%d|v=%d|(%d)|- ",node->id ,node->value, node->flag);
}

// Function to iterate on Linked List for printing
void printLinkedlist(NODE node){
	while (node != NULL){
		printNode(node);
		node = node->rightNode;
	}
	printf("\n");

}


// Function for adding events in Event queue
void addEvent(EVENT eventNode, NODE sourceNode, NODE targetNode,int Type,int value){				


	// for adding a new event at the end of the queue
	while ( eventNode->eventNext != NULL ) {
		eventNode=eventNode->eventNext;
	}

	EVENT newEventNode = (EVENT)malloc(sizeof(struct Event));
	newEventNode->sourceNode = sourceNode;
	newEventNode->targetNode = targetNode;
	newEventNode->eventType = Type;
	newEventNode->eventValue = value;
	newEventNode->eventNext = NULL;

	// printf("\nnew event : source = %d , target = %d , eventType = %d , eventValue = %d\n", sourceNode->id , targetNode->id , Type , value);

	eventNode->eventNext = newEventNode;

}

// Function for extracting events and respective action
void eventQueue(EVENT eventNode){

	eventNode = eventNode->eventNext;

	while(eventNode != NULL){

		if (eventNode->eventType == SEND){		// adding recieve event for a send event

			addEvent( eventNode , eventNode->targetNode , eventNode->sourceNode , RECIEVE , eventNode->eventValue );

		} else if ( eventNode->eventType == 0 ) {		// for a recieve event

			if (eventNode->sourceNode->flag == 1){			// if recieving on flag (1)
				if ( eventNode->eventNext->sourceNode->flag == 1 && eventNode->eventNext->eventType == RECIEVE){		// taking values from both (0) and (2)
					int A = eventNode->eventValue;
					int B = eventNode->sourceNode->value;
					int C = eventNode->eventNext->eventValue ;
					int largest = A > B ? (A > C ? A : C) : ( B > C ? B : C);
					int smallest = A < B ? (A < C ? A : C) : ( B < C ? B : C );
					int middle = A+B+C-smallest-largest;
					// printf("\nvalues arrived = %d %d %d\n",A , B ,C );
					eventNode->sourceNode->value = middle;
					// printf("\nsetting : %d as %d\n", eventNode->sourceNode->id , middle );
					addEvent(eventNode,eventNode->sourceNode,eventNode->targetNode,SEND,smallest);
					addEvent(eventNode,eventNode->sourceNode,eventNode->eventNext->targetNode,SEND,largest);
					
					eventNode = eventNode->eventNext;
				} else {			// only two nodes 
					int largest = eventNode->eventValue > eventNode->sourceNode->value ? eventNode->eventValue : eventNode->sourceNode->value;
					int smallest = eventNode->eventValue < eventNode->sourceNode->value ? eventNode->eventValue : eventNode->sourceNode->value;
					if (eventNode->targetNode->id > eventNode->sourceNode->id){
						eventNode->sourceNode->value = smallest;
						// printf("\nvalues arrived = %d %d\n",largest, smallest);
						// printf("\nsetting : %d as %d\n", eventNode->sourceNode->id , smallest );
						addEvent(eventNode,eventNode->sourceNode,eventNode->targetNode,SEND,largest);
					} else {
						eventNode->sourceNode->value = largest;
						// printf("\nvalues arrived = %d %d\n",largest, smallest);
						// printf("\nsetting : %d as %d\n", eventNode->sourceNode->id , largest );
						addEvent(eventNode,eventNode->sourceNode,eventNode->targetNode,SEND,smallest);
					}
				}
			} else {		// recieving on (0) or (2)
				eventNode->sourceNode->value = eventNode->eventValue;
				// printf("\n NOT A FLAG 1 ===  setting : %d as %d\n", eventNode->sourceNode->id , eventNode->eventValue );
			}

		}

		eventNode=eventNode->eventNext;
	}
}

// Function for updationg flags
void flagUpdation(NODE node){

	while (node != NULL){
		node->flag = ( (node->flag)+2 ) % 3;
		node = node->rightNode;
	}

}


// Function for Modules 3 algo
void mod3( NODE node , int n ){

	int i = 1;
	NODE startNode = node;
	while ( i <= n-1 ){
		EVENT headEventNode = (EVENT)malloc(sizeof(struct Event));
		headEventNode->eventNext = NULL;
		
		while (node != NULL){
			if (node->flag == 0){	// Sending from flag (0) to flag (1)
				if (node->rightNode && node->rightNode->flag == 1)
					addEvent(headEventNode,node,node->rightNode,SEND,node->value);
			} else if (node->flag == 2){	// Sending from flag (2) to flag (1)
				if (node->leftNode && node->leftNode->flag == 1)
					addEvent(headEventNode,node,node->leftNode,SEND,node->value);
			}
			node = node->rightNode;
		}
		// printf("-----------------ANALYZING EVENT QUEUE----------------");
		eventQueue(headEventNode);
		node = startNode;
		flagUpdation(startNode);
		// printLinkedlist(startNode);
		i++;
	}
}

// Main Function
int main(int argc, char const *argv[]) {
	
	srand(time(0));
	int n;

	n = (argc < 2) ? N_DEFAULT : atoi(argv[1]);
	printf("\n\t\t\t----- No. of nodes = %d-----\n", n);

	printf("\n\n\n PRINTING CONVENTION \n \t -| id | value | (flag) |-\n\n\n");
	printf("\n\n----INITIAL LINKED LIST----\n\n");

	NODE startNode = createLinklist(n);

	printLinkedlist(startNode);

	mod3(startNode,n);

	printf("\n\n----FINAL SORTED LIST----\n\n");
	printLinkedlist(startNode);
	printf("\n\n");
	return 0;
}