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

typedef struct Structure {
    char id[BUFFERSIZE];
	int size;
    struct Structure *next;
    struct Structure * prev;
    int startMemory;//the beginning of where there memory is (0)
    int endMemory;//The end of where the memory is, set to how many free spaces we have - 1 as we go from 0 to MAX-1
} Structure;

//Function definitions
int readCommand(char *command);
void status(char *string);

int main(int argc, char *argv[]){
	if(argc >= 2){

		remainingSpace = atoi(argv[1]);//argv is a vector of strings from the command line, we take argv[1] because the number of bytes allocated to memory is this location
		printf("Allocated %d bytes of memory\n", remainingSpace);

		Structure temp;
		temp.startMemory = 0;
		temp.endMemory = remainingSpace - 1;
		temp.size = remainingSpace;

		startHole = &temp;
		endHole = &temp;

		//Allocating space for the start and end Allocated structures
		startAllocated = (Structure *)malloc(sizeof(Structure));
		endAllocated = (Structure *)malloc(sizeof(Structure));

		//Assigning the variables which will be used to keep track of; number of total holes, number of total process and the total allocated space
		totHoles = 1, totProcesses = 0, alloSpace = 0;

// 		int cond = 0; //A condition for the while loop
// 		while(cond == 0){
			printf("command>");//Ask the user for a command
			char cmdInput[BUFFERSIZE];
			fgets(cmdInput, BUFFERSIZE, stdin);//stdin is the standard input so we read from the command line what the user has submitted
			readCommand(cmdInput);
// 		}
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

	printf("%d\n",strcmp(cmd,"Status"));
	return 0;
}

void status(char *string){
	printf("Partitions [Allocated memory = %d]:\n", alloSpace);
}
