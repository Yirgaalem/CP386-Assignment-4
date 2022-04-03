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

//An object that will be used to hold the memory, identify the memroy,...
struct Structure *startHole, *endHole, *startAllocated, *endAllocated;
int remainingSpace, totHoles, totProcesses, alloSpace;

struct Structure {
    char id[BUFFERSIZE];
	int size;
    struct Structure *next;
    struct Structure * prev;
    int startMemory;//the beginning of where there memory is (0)
    int endMemory;//The end of where the memory is, set to how many free spaces we have - 1 as we go from 0 to MAX-1
};

//Function definitions
int readCommand(char *command);
void status(char *string);
int confirmerRQ(char array[MAX][BUFFERSIZE], int size);
int confrimerRL(char array[MAX][BUFFERSIZE],int size);
int confrimRequest(char array[MAX][BUFFERSIZE], int size);

int main(int argc, char *argv[]){
	if(argc >= 2){

		remainingSpace = atoi(argv[1]);//argv is a vector of strings from the command line, we take argv[1] because the number of bytes allocated to memory is this location
		printf("Allocated %d bytes of memory\n", remainingSpace);

		struct Structure struc;
		struc.startMemory = 0;
		struc.endMemory = remainingSpace - 1;
		struc.size = remainingSpace;

		startHole = &struc;
		endHole = &struc;

		//Allocating space for the start and end Allocated structures
		startAllocated = (struct Structure *)malloc(sizeof(struct Structure));
		endAllocated = (struct Structure *)malloc(sizeof(struct Structure));
		startAllocated = NULL; endAllocated = NULL;
		//Assigning the variables which will be used to keep track of; number of total holes, number of total process and the total allocated space
		totHoles = 1, totProcesses = 0, alloSpace = 0;

//		int cond = 0; //A condition for the while loop
//		while(cond == 0){
			printf("command>");//Ask the user for a command
			char cmdInput[BUFFERSIZE];
			fgets(cmdInput, BUFFERSIZE, stdin);//stdin is the standard input so we read from the command line what the user has submitted
			readCommand(cmdInput);
//		}

	}
	return 00;
}

int readCommand(char *command){
	//checks if the last element in the string is a new line character. If it is replace it with a end of line char.
	char *cmd = (char*) malloc(sizeof(char) * BUFFERSIZE);
	int i = 0;

	//While loop will run until a end of string character is found
	while(command[i] != '\0'){
		if(command[i] !='\r' && command[i] !='\n') //As long as the current character is not a new line or a carriage feed, we will append that character into our new string
			cmd[i] = command[i];
		i++;
	}

	//Confriming the final character is a end of string/Null character
	cmd[i] = '\0';

	//Checks if the command passed was "Status", if so call the status function
	if(strcmp(cmd,"Status")==0)
		status(cmd);

	//Checks if the command passed was "Status", if print a statement saying the system will exit and exit.
	else if(strcmp(cmd,"Exit")==0){
		printf("Thank you, exiting the program...\n");
		exit(0);
	}
	else{
		//We will need to take the string from the file and break it into 4 parts if its RQ; 1 to check if its "RQ", 2 to check the process number, 3 to check size and 4 to see if the submitted 'B'
		//If it is RL then we only need 2, 1 to check it is RL, 2 to check the process number/name.
		int index = 0;
		char *comm = (char *)malloc(sizeof(char)*BUFFERSIZE);
		char array[MAX][BUFFERSIZE];
		comm = strtok(cmd, " ");//Takes all the characters in the strnig up until the first space

		while(comm != NULL){
			strcpy(array[index], comm);
			comm = strtok(NULL, " "); //mayve change to strtok(cmd, " ");
			i++;
		}

		//Comparing the first value in array which is the first string in the command before a white space is entered.
		//Checking if the commands passed start with RL or RQ, the only possible commands left, if not, print a statement that the command is invalid
		//As mentioned in the comment above, if it is RL we will have two indicies in the array, one for the string "RL" and the other for the process name/number. For RQ we will have 4
		int RQ = (strcmp("RQ",array[0])), RL = (strcmp("RL",array[0]));

		if(RQ == 0 || RL == 0){
			if(RQ == 0 && index == 4){
				if(confirmerRQ(array, index) && strcmp("B",array[3])==0){
					//The RQ command is good, now just check if what we asked for is accesible, check if the request is valid
				}
			}
			else if(RL == 0 && index == 2){
				//do stuff
			}

		}
		else
			printf("Invalid command, please try again\n");
	}
	return 0;
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
int confrimRequest(char array[MAX][BUFFERSIZE], int size){
//	int fnd = 0, reqSpace = atoi(array[2]);
//
//	struct Strucutre *x = startHole, *y = NULL, *z = (struct Structure*)malloc(sizeof(struct Structure*));
//
//	while(!(x==NULL)){
//		if(x->size >= reqSpace){
//			if(fnd == 0){
//				y = x;
//				fnd = 1;
//			}
//			else{
//				if(y->size > x->size)
//					y = x;
//			}
//		}x = x->next;
//	}
//	if(fnd == 0)
//		return 1;
	int reqSpace = atoi(array[2]);

	int fnd = 0;

	struct Structure *x = startHole,*y = endHole;

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

}


int conrfimerRL(char array[MAX][BUFFERSIZE],int size){
	return 1;
}

void status(char *string){
	printf("Partitions [Allocated memory = %d]:\n", alloSpace);
}
