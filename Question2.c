/*
 * Your program must respond to three
different requests:
• Request for a contiguous block of memory
• Release of a contiguous block of memory
• Report the regions of free and allocated memory
We are using the Best-fit algorithm, this requires the program to keep track of different allocations and holes represneting available memory.
When a request for memory has arrived, it will allocate the memory from one of the available hoeles based on the allocation strategy (the first allocation, all memory is available).
If there is insufficient memory to allocate the request, it will output an error message and reject the request
Also keep track of which region of memory has been allocated to which process. Necessary so that we can support the "Status" command and is also neeeded when memory is released via "RL" command (the process releasing memory is passed to this command)
 */

/*
 * Name: Nahor Yirgaalem
 * ID: 1907755040
 * Email: yirg5540@mylaurier.ca
 */

#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define MAX 10
#define BUFFERSIZE 100
#define sID "struc"
//An object that will be used to hold the memory, identify the memroy,...
struct Customer *startHole, *endHole, *startAllocated, *endAllocated;
int remainingSpace, totHoles, totProcesses, alloSpace;

struct Customer {
    char id[BUFFERSIZE];
	int size;
    struct Customer *next;
    struct Customer * prev;
    int startMemory;//the beginning of where there memory is (0)
    int endMemory;//The end of where the memory is, set to how many free spaces we have - 1 as we go from 0 to MAX-1
};

//Function definitions
int readCommand(char *command);
void status(char *string);
int confirmerRQ(char array[MAX][BUFFERSIZE], int size);
int confirmRelease(char array[MAX][BUFFERSIZE]);
int confirmRequest(char array[MAX][BUFFERSIZE], int size);
void releaseData(struct Customer *x , struct Customer *y);
void requestData(struct Customer *x, struct Customer *y, struct Customer *z, char array[MAX][BUFFERSIZE], int reqSpace);
void listAdd(struct Customer *x, struct Customer **y, struct Customer **z);
void listTake(struct Customer *x, struct Customer **y, struct Customer **z);

int main(int argc, char *argv[]){
	if(argc > 2){
		printf("invalid");
	}
	else{
		remainingSpace = atoi(argv[1]);//argv is a vector of strings from the command line, we take argv[1] because the number of bytes allocated to memory is this location
		printf("Allocated %d bytes of memory\n", remainingSpace);

		struct Customer struc;
		struc.startMemory = 0;
		struc.endMemory = remainingSpace - 1;
		struc.size = remainingSpace;
		strcpy(struc.id, sID);

		startHole = &struc;
		endHole = &struc;

		//Allocating space for the start and end Allocated Customers
		endAllocated = (struct Customer *)malloc(sizeof(struct Customer));
		startAllocated = (struct Customer *)malloc(sizeof(struct Customer));
		endAllocated = NULL;startAllocated = NULL;

		//Assigning the variables which will be used to keep track of; number of total holes, number of total process and the total allocated space
		totHoles = 1, totProcesses = 0, alloSpace = 0;

		//A condition for the while loop
		int cond = 0;

		//Loop runs forever, has no end. Only exits when the Exit command is called
		while(cond == 0){
			printf("command>");//Ask the user for a command
			char cmdInput[BUFFERSIZE];
			fgets(cmdInput, BUFFERSIZE, stdin);//stdin is the standard input so we read from the command line what the user has submitted
			readCommand(cmdInput);
		}

	}

	return 0;//End
}

int readCommand(char *command){
	//checks if the last element in the string is a new line character. If it is replace it with a end of line char.
	char *cmd;
	cmd = (char*) malloc(sizeof(char) * BUFFERSIZE);
	char array[MAX][BUFFERSIZE];

	//Index
	int i = 0;

	//While loop will run until a end of string character is found
	while(command[i] != '\0'){
		if(command[i] !='\r' && command[i] !='\n') //As long as the current character is not a new line or a carriage feed, we will append that character into our new string
			cmd[i] = command[i]; //set cmd equal to the character at the index position of command
		i++;//increment
	}

	//confirming the final character is a end of string/Null character
	cmd[i] = '\0';

	//Checks if the command passed was "Status", if so call the status function
	if(strcmp(cmd,"Status")==0)
		status(cmd);//Calls the status function with passing the "filtered" command

	//Checks if the command passed was "Status", if print a statement saying the system will exit and exit.
	else if(strcmp(cmd,"Exit")==0){
		printf("Thank you, exiting the program...\n");
		exit(0);
	}
	else{
		//We will need to take the string from the file and break it into 4 parts if its RQ; 1 to check if its "RQ", 2 to check the process number, 3 to check size and 4 to see if the submitted 'B'
		//If it is RL then we only need 2, 1 to check it is RL, 2 to check the process number/name.
		char *comm;
		comm = strtok(cmd, " ");
		int index = 0;

		while(comm!= NULL){
			strcpy(array[index], comm);
			comm = strtok(NULL, " ");
			index++;
		}
		//Comparing the first value in array which is the first string in the command before a white space is entered.
		//Checking if the commands passed start with RL or RQ, the only possible commands left, if not, print a statement that the command is invalid
		//As mentioned in the comment above, if it is RL we will have two indicies in the array, one for the string "RL" and the other for the process name/number. For RQ we will have 4
		int RQ = (strcmp("RQ",array[0])), RL = (strcmp("RL",array[0]));

		if(RQ == 0 || RL == 0){
			if(RQ == 0 && index == 4){
				if(confirmerRQ(array, index) && strcmp("B",array[3])==0){
					//The RQ command is good, now just check if what we asked for is accesible, check if the request is valid
					int valid = confirmRequest(array, index);
					if(valid == 0)
						printf("Successfully allocated %s to process %s\n",array[2],array[1]);
					else
						printf("No hole of sufficient size\n");
				}
			}
			else if(RL == 0 && index == 2){
				//do stuff
				printf("releasing memory for process %s\n",array[1]);
				int valid = confirmRelease(array);

				if(valid == 0)
					printf("Successfully release memory for process %s\n", array[1]);
				else
					printf("Process %s not found, please try again!\n",array[1]);
			}

			//Runs if the command starts with RQ or RL but isn't proper
			else
				printf("Invalid command, please try again\n");

		}
		else
			printf("Invalid command, please try again\n");

	}
	return 0;
}

void status(char *string){
	printf("Partitions [Allocated memory = %d]:\n", alloSpace);
	struct Customer *x = startAllocated;
	while(!(x==NULL)){
		printf("Address [%d:%d] Process %s\n",x->startMemory,x->endMemory,x->id);
		x = x->next;
	}

	printf("\nHoles [Free memory = %d]:\n",remainingSpace);
	x = startHole;

	while(!(x == NULL)){
		printf("Address [%d:%d] len = %d\n", x->startMemory, x->endMemory,x->size);
		x = x->next;
	}
}

//Function that checks if parts of the RQ command is correct
int confirmerRQ(char array[MAX][BUFFERSIZE], int size){

	//Loop runs if the 2nd elemnt in the array (the process number)'s first character is a letter and if the 4th element in the array is
	if(isalpha(array[1][0]) != 0 && isalpha(array[3][0] != 0)){
		for(int i = 0; i < sizeof(array[2]);i++){
			if(isdigit(array[2][i]) != 0)
				return 0;
		}

	}
	return 1;

}

//This function is called if the Request command is properly submitted. Checks if we can access what the command is asking for, checks if its valid
int confirmRequest(char array[MAX][BUFFERSIZE], int size){
	int reqSpace = atoi(array[2]), fnd = 0;

	struct Customer *x = startHole,*y = NULL, *z = (struct Customer*)malloc(sizeof(struct Customer*));

	while (x != NULL) {
		if (x->size >= reqSpace) {
			if (fnd==0) {
				y = x;
				fnd = 1;
			} else {
				if (y->size > x->size)
					y = x;
			}
		}
		x = x->next;
	}

	if (fnd == 0)
		return 1;

	requestData(x,y,z,array,reqSpace);

	remainingSpace = remainingSpace - reqSpace;
	alloSpace = alloSpace + reqSpace;
	return 0;
}
void requestData(struct Customer *x, struct Customer *y, struct Customer *z, char array[MAX][BUFFERSIZE],int reqSpace){
	strcpy(z->id, array[1]);
	z->startMemory = y->startMemory;
	z->endMemory = y->endMemory + reqSpace -1;
	z->size = reqSpace;
	z->prev = NULL;
	z->next = NULL;

	listAdd(z, &startAllocated, &endAllocated);

	y->startMemory = y->startMemory + reqSpace;
	y->size = y->size - reqSpace;

	if(y->size == 0)
		listTake(y,&startHole,&endHole);
}

void listAdd(struct Customer *x, struct Customer **y, struct Customer **z){
	if (!(*y == NULL && *z == NULL)) {
		x->prev = *z;
		(*z)->next = x;
		(*z) = x;
	}
	else {
		(*y) = x;
		(*z) = x;
	}
}
void listTake(struct Customer *x, struct Customer **y,struct Customer **z) {

	if (x->next == NULL && x->prev == NULL) {
		*z = NULL;
		*y = NULL;

	}
	else if (x->next == NULL) {
		*z = x->prev;
		(*z)->next = NULL;

	}
	else if (x->prev == NULL) {
		*y = x->next;
		(*y)->prev = NULL;
	}
	else {
		x->prev->next = x->next;
		x->next->prev = x->prev;
	}
	x = NULL;
	free(x);
}


int confirmRelease(char array[MAX][BUFFERSIZE]){
	struct Customer *x = startAllocated, *y = NULL;
	int fnd = 0;

	//A simple loop used to check if that which we are looking for exists, if it is, fnd is set to 1 and we break out the loop
	while(!(x==NULL) && fnd == 0){
		if(strcmp(array[1],x->id)==0){
			fnd = 1; y = x;
		}
		x = x->next;
	}

	if(fnd == 0)
		return 1;
	releaseData(x,y);

	return 0;
}

//Finds the data for releasing
//Called by confirmRelease
void releaseData(struct Customer *x , struct Customer *y){
	int bf = 0, af = 0, ff = 0;

	struct Customer *curr = NULL, *post = NULL, *prev = NULL;
	x = startHole;

	while (bf == 0 && !(x == NULL)) {
		if (y->startMemory == (x->endMemory + 1)) {
			ff = 1;
			curr = x;
		}
		else if (y->endMemory == (x->startMemory - 1)) {
			af = 1;
			post = x;
		}

		if (y->startMemory > x->endMemory)
			prev = x;

		if (!(curr == NULL) && !(post == NULL))
			bf = true;

		x = x->next;
	}

	if(af == 1){
		post->startMemory = y->startMemory;
		post->size += y->size;
	}
	else if (ff == 1){
		curr->endMemory = y->endMemory;
		curr->size += y->size;
	}

	else if (bf == 1) {
		struct Customer *hold;
		hold = (struct Customer*) malloc(sizeof(struct Customer*));
		strcpy(hold->id, sID);

		hold->size = curr->size + y->size + post->size;
		hold->prev = curr->prev; hold->next = post->next;
		hold->startMemory = curr->startMemory;hold->endMemory = post->endMemory;

		 if (post->next == NULL)
			endHole = hold;

		 else if (curr->prev == NULL)
			startHole = hold;

	} else {

		struct Customer *hold;
		hold = (struct Customer*) malloc(sizeof(struct Customer*));
		strcpy(hold->id, sID);

		hold->size = y->size;

		hold->startMemory = y->startMemory;
		hold->endMemory = y->endMemory;

		hold->prev = NULL; hold->next = NULL;

		if (prev == NULL) {
			hold->next = startHole;
			startHole->prev = hold;
			startHole = hold;
		}
		else if (prev->next == NULL) {
			hold->prev = endHole;
			endHole->next = hold;
			endHole = hold;
		}
		else {
			hold->prev = prev;
			hold->next = prev->next;
			prev->next = hold;
		}
	}

	alloSpace = alloSpace - y->size;
	remainingSpace = remainingSpace + y->size;

	listTake(y, &startAllocated, &endAllocated);
}

