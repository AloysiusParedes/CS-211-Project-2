#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//GLOBAL CONSTANTS
int const MAX_LENGTH = 300;
int const TRUE = 1;
int const FALSE = 0;
int debugMode = 0;
//STRUCT DECLARATION
typedef struct Node{
	int size;		//current size of the dynamic array
	int top;		//the top of the stack
	char *arrPtr;	//pointer to the dynamic array that holds the stack
}Node;

//FUNCTION PROTOTYPES
void initializeStack(Node *stack);
int checkEmpty(Node *stack);
void push(Node *stack, int value, int debugMode);
void pop(Node *stack, int debugMode);
char accessTop(Node *stack);
void resetStack(Node *stack);
void displayStack(Node *stack); //FOR DEBUGGING ONLY

//MAIN FUNCTION
int main(int argc, char** argv){
	//VARIABLE DECLARATIONS
	char userInput[MAX_LENGTH];			//to store user input
	
	int i;					//for use in loops
	Node stack;
	initializeStack(&stack);
	
	//sets debug mode
	for(i = 0; i < argc; i++){
		if(strcmp(argv[i], "-d") == 0){
			printf("\nDEBUG MODE ON\n");
			debugMode = TRUE;
		}
	}
	
	while(1){
		char symbol, expecting;
		int j, balanced = 1;
		i = 0;
		//accept user input
		fgets(userInput, MAX_LENGTH, stdin);
				
		//check if user input is q or Q
		if(userInput[0] == 'q' || userInput[0] == 'Q'){
			printf("\n\nEXITING PROGRAM...\n\n");
			break;
		}
		//loop to read in line
		for(i = 0; i < strlen(userInput) -1; i++){
			//modify stack if opening symbol is encountered
			if(userInput[i] == '(' || userInput[i] == '{' || userInput[i] == '[' || userInput[i] == '<'){
				push(&stack, userInput[i], debugMode);
			}
		
			//check stack if closing symbol is encountered
			if(userInput[i] == ')' || userInput[i] == '}' || userInput[i] == ']' || userInput[i] == '>'){
				//change symbol based on user input
				if(userInput[i] == ')')
					symbol = '(';
				else if(userInput[i] == '}')
					symbol = '{';
				else if(userInput[i] == ']')
					symbol = '[';
				else
					symbol = '<';
				
				//if symbol is at top of stack, pop the stack and continue
				if(accessTop(&stack) == symbol){
					//printf("POP\n");
					pop(&stack, debugMode);
					//displayStack(&stack);
					continue;
				}
				//if symbol is not at the top of stack, NOT BALANCED
				if(accessTop(&stack) != symbol){
					if(checkEmpty(&stack) == 0){
						if(accessTop(&stack) == '(')
							expecting = ')';
						else if(accessTop(&stack) == '{')
							expecting = '}';
						else if(accessTop(&stack) == '[')
							expecting = ']';
						else
							expecting = '>';
						
						for(j = 0; j < i; j++)
							printf(" ");
						printf("^ expecting %c\n\n", expecting);
						balanced = 0;
						break;
					}
					else if(checkEmpty(&stack) == 1){
						for(j = 0; j < i; j++)
							printf(" ");
						printf("^ missing %c\n\n", symbol);
						balanced = 0;
						break;
					}
				}
			}
		}//end for loop
		
		//end of the expression is encountered
		if(balanced == 1){
			//balanced
			if(checkEmpty(&stack) == 1){
				printf("Expression is balanced\n\n");
				continue;
			}

			//unbalanced but missing closing symbol
			if(checkEmpty(&stack) == 0){
				if(accessTop(&stack) == '(')
					symbol = ')';
				else if(accessTop(&stack) == '{')
					symbol = '}';
				else if(accessTop(&stack) == '[')
					symbol = ']';
				else
					symbol = '>';
				for(j = 0; j < i; j++)
					printf(" ");
				printf("^ missing %c\n\n", symbol);
			}
		}
		resetStack(&stack);
	}
	return 0;
}//end main(...)

//FUNCTION DEFINITIONS
//sets values of the stack based on input
void initializeStack(Node *stack){
	stack->size = 2;	//set size
	stack->top = 0;		//specify top of stack
	char *arr = (char *) malloc(sizeof(char) * (stack->size)); //create list & allocate space for list
	stack->arrPtr = arr;	//set pointer to dynamic array
}//end initializeStack(...)

//checks to see if the stack is empty
int checkEmpty(Node *stack){
	if(stack->top == 0)
		return 1;
	return 0;
}//end checkEmpty(...)

//push an element onto the stack
void push(Node *stack, int value, int debugMode){
	if(debugMode == TRUE)
		printf("\nDEBUG MODE: PUSH\n");
	//check if stack array is full
	if(stack->top == stack->size){
		int i;
		//grow the array
		char *temp = (char *) malloc((sizeof(char) * (stack->size)) + 2);  //allocate the larger array space
            for(i = 0; i < stack->size; i++) //loop to copy values to larger array space
                temp[i] = stack->arrPtr[i];
				
			if(debugMode == TRUE)
				printf("\nNumber of values copied: %d", i);

            free(stack->arrPtr);	//deallocate current stack
            stack->arrPtr = temp; 	//set "permanent" array identifier to point to larger space
			
			if(debugMode == TRUE)
				printf("\nOld Size: %d", stack->size);
            stack->size += 2; //update size to reflect larger array space
			if(debugMode == TRUE)
				printf("\nNew Size: %d", stack->size);
	}
			
	//add value to the stack array
	stack->arrPtr[stack->top] = value;
	if(debugMode == TRUE)
		printf("\nPushed to stack: %c", stack->arrPtr[stack->top]);
	
	//increment top-of-stack value
	(stack->top) += 1;
	if(debugMode == TRUE)
		printf("\nNew top value: %d", stack->top);
}//end push(...)

//pop an element off of the stack
void pop(Node *stack, int debugMode){
	if(debugMode == TRUE){
		printf("\nDEBUG MODE: POP\n");
		printf("\nPopped from stack: %c", stack->arrPtr[stack->top]);
	}
	
	//decrement top-of-stack value
	(stack->top) -= 1;
	if(debugMode == TRUE)
		printf("\nNew top value: %d", stack->top);
	
	//"remove" top stack value
	stack->arrPtr[stack->top] = 0;

}//end pop(...)

//access top of the array
char accessTop(Node *stack){
	return stack->arrPtr[stack->top - 1];
}//end accessTop(...)

//reset stack so that it's empty and ready to be used again
void resetStack(Node *stack){
	stack->size = 2;	//set size to 2
	stack->top = 0;		//set top index to 0
	char *newArr = (char *) malloc(sizeof(char) * (stack->size)); //creates new stack with allocated space for default size
	free(stack->arrPtr); //deallocates memory of old stack
	stack->arrPtr = newArr; //sets new idetinfier
}//end accessTop(...)

//display contents of the stack
void displayStack(Node *stack){
   int i;
   printf("Stack: ");
   for(i = 0; i < stack->size; i++)
	   printf("%c ", stack->arrPtr[i]);
   printf("\n\n");
}//end displayStack(...)