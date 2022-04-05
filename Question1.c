#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

bool on = true; // making sure it's running
const char *FILE_NAME = "sample4_in.txt"; // taking the file used
pthread_mutex_t mutex; //mutex lock

/*
 *
 * A customer structure
 *
 */
struct Customer{
	int id;
	int *max;
	int *allocated;
	int *needs;
	bool done;
};

/*
 *
 * A structure with every customer in it
 *
 */
struct Customer *all_customers;

/*
 *
 * An integer with all remaining resources
 *
 */
int *avaliable;

/*
 *
 * An integer with amount of resources
 *
 */
int r_a;

/*
 *
 * Amount of customers in program
 *
 */

int c_a;

/*
 *
 * Loading all functions
 *
 */

int available_resources(int c, char *args[]);

int customer_resources();

void start();

void request_resources(int c_index, int *r_r);

void r_c(); // run the command

void *r_c_f(); // run the command function

void s_c(); //check status

void run(); //run command

bool check_safe();


int max = 100;
char **s_s(char *in, char *d); // splits the string
int *s_i(char *in, char *d); //spliting the integer
int l_s(char *in, char *d); //getting the length of string

char *scan(); // scanning a line
void release(int c_i, int *r_r);


char **s_s(char *in, char *d){
	char *helper_in = strdup(in);
	int l = l_s(in,d);
	char **a = malloc(sizeof(char*)*l);
	int counter = 0;
	char *tester;
	while((tester = strsep(&helper_in,d))){
		a[counter] = tester;
		counter +=1;
	}
	return a;
}

int *s_a(char *in, char *d){
	char *helper_in = strdup(in);
	int l = l_s(in, d);
	int *a = malloc(sizeof(int) *l);
	int counter = 0;
	char *tester;
	while((tester = strsep(&helper_in, d))){
		a[counter] = atoi(tester);
		counter = counter +1;
	}
	return a;
}


int l_s(char *in, char *d){
	char *in2 = strdup(in);
	int l = 0;
	char *t;
	while((t = strsep(&in2,d))){
		l = l+1;
	}
	return l;
}

char *scan(void){
	char *r = malloc(max);
	int l = max;
	char *line = r;
	if(r==NULL){
		return NULL;
	}
	while(true){
		int check = tolower(fgetc(stdin));
		if(check == '\n' || check == '\r' || check== EOF){
			break;
		}
		if(0 == --l){
			l = max;
			char * new_line = realloc(line, max*=2);
			if(new_line == NULL){
				free(line);
				return NULL;
			}
			r = new_line + (r - line);
			line = new_line;

		}
		if((*r++ = check) == '\n'){
			break;
		}

	}
	*r = '\0';
	r[strcspn(r,"\n")] = 0;
	return line;

}



int *safeseq;

int lines = 1000;
/*
 * main: First function run  runs all required functions
 * @param: int arg_count ,char *args
 * @return none
 */
int main(int arg_count, char *args[]){
	if(arg_count == 1){
		printf("There should be more then one integer amount!\n");
		return 1;
	}
	available_resources(arg_count, args);
	customer_resources(r_a);
	start();
}
/*
 * avaliable_resources: Sets up the avaliable resources
 * @param: int c, char args
 * @return resources amountt
 */
int available_resources(int c, char *args[]){
	avaliable = (int *)malloc((c - 1) * sizeof(int));

	for(int i = 1; i<c;i++){
		avaliable[i-1] = atoi(args[i]);
	}
	r_a = c - 1; //declare resource amount
	return r_a;
}
/*
 * customer_resources: Loads all the customer resources from file
 * @param: none
 * @return: int boolean if it's done or not
 */
int customer_resources(){
	FILE *f = fopen(FILE_NAME, "r"); //opens to read
	if (f==NULL){
		printf("File doesn't exist: %s", FILE_NAME);
		return -1;
	}
	char line[lines];
	while(fgets(line, sizeof(line), f)!=NULL){
		c_a = c_a +1;
	}
	all_customers = malloc(sizeof(struct Customer) * c_a); //all the customers declared after finding out the amount
	fseek(f,0, SEEK_SET);
	int counter = 0;
	while(fgets(line, sizeof(line), f) != NULL){
		struct Customer beta;
		beta.id = counter;
		beta.max = s_a(line, ",");
		beta.allocated = malloc(sizeof(int) * r_a);
		beta.needs = s_a(line, ",");
		beta.done = false;

		all_customers[counter] = beta;
		counter = counter +1;
	}
	fclose(f);
	return 0;
}
/*
 * Start: Starts the function up (runs a loop for user inputs)
 * @param: none
 * @return: none
 *
 */
void start(){
	printf("Number of Customers: %d\n", c_a);
	printf("Currently Available resources: ");
	for(int i = 0; i < r_a;i++){
		printf("%d ", avaliable[i]);
	}
	printf("\nMaximum resources from file: \n");

	for(int i = 0; i< c_a;i++){
		struct Customer temp = all_customers[i];
		for(int j = 0;j<r_a;j++){
			printf("%d ", temp.max[j]);
		}
		printf("\n");
	}
	bool tt = check_safe();
	if(tt==1){
		tt = 2;
	}

	while(on){
		printf("Enter Command: ");
		char *in = scan();
		char **args = s_s(in, " ");
		int length = l_s(in, " ");
		if(strcmp(args[0], "rq") == 0){
			if(2 + r_a != length){
				printf("Error! The command takes the wrong amount of arguments!\n");
				printf("Proper Usage: RQ [int customer_number] ");
				for(int i = 0; i < r_a; i++ ){
					printf("[int Resource %d]  ", i + 1); // because m resource types
				}
				printf("\n");
			}else{
				int index = atoi(args[1]);
				int r_r[r_a];
				for(int i = 0; i<r_a;i++){
					r_r[i] = atoi(args[2+i]);
				}
				request_resources(index,r_r);
			}
		}else if(strcmp(args[0], "exit")==0){
			on = false;
			printf("Exiting Program \n");
		}else if(strcmp(args[0], "status")==0){
			s_c();
		}else if(strcmp(args[0], "run")==0){
			Run();
		}else if(strcmp(args[0], "rl") == 0){
			if(length==2 + r_a){
				int c_id = atoi(args[1]);
				int r_r[r_a];
				for(int i = 0; i<r_a;i++) r_r[i] = atoi(args[2+i]);
				release(c_id, r_r);

			}else{
				printf("Error! Usage: rl [customer id] ");
				for(int i =0;i<r_a;i++) printf("[resource] ");
				printf("\n");
			}
		}else printf("Invalid input, use one of RQ, RL, Status, Run, Exit \n");
	}

}
/*
 * check_safe: Runs bankers algorithm
 * @input: none
 * @return: non
 */
bool check_safe(){
	safeseq = (int *)malloc(sizeof(int) * c_a);
	bool updated = true;
	bool able = true;
	int cou = 0;
	bool safety = false;
	int *temp;
	temp = (int *)malloc((c_a - 1) * sizeof(int));
	for(int i = 0; i<c_a;i++) all_customers[i].done = false;
	for(int i = 0; i<sizeof(avaliable);i++){
		temp[i] = avaliable[i];
	}
	while(updated){
		updated = false;
		for(int i = 0; i<c_a;i++){
			able = true;
			struct Customer a = all_customers[i];
			for(int j =0; j<sizeof(avaliable)-4;j++){
				if(a.needs[j]>temp[j] +5){
					able = false;
				}

			}
			if(able){
				safeseq[cou++] = i;
				for(int j = 0; j<sizeof(avaliable);j++){
					temp[j] += a.allocated[j];


				}

				a.done = true;
			}

		}
	}
	if(cou==c_a){
		safety = true;

	}

	return safety;
}
/*
 * Request_resources: Run using RQ
 * @Param: int c_i (customer index), int *r_r: The information that is added
 * @return none
 *
 */
void request_resources(int c_i, int *r_r){
	for(int i = 0; i <r_a; i++){
		if(avaliable[i]<r_r[i]){
			printf("Not enough resources available. \n");
			return;
		}

	}
	int *tempsafe;
	tempsafe = (int *)malloc(sizeof(int) * c_a);
	int *prev_allocated;
	prev_allocated = (int *)malloc(sizeof(int) * r_a);
	for(int i = 0; i<c_a;i++) tempsafe[i] = safeseq[i];
	for(int i = 0; i<r_a;i++){
		prev_allocated[i] = all_customers[c_i].allocated[i];
		all_customers[c_i].allocated[i] = r_r[i];

	}
	bool check = check_safe();
	if(!check){
		printf("The state is not safe. \n");
		for(int i = 0; i <r_a;i++){
			all_customers[c_i].allocated[i] = prev_allocated[i];
		}
	}else{

		printf("State is safe, and request is satisfied. \n");
		for(int i = 0; i<r_a;i++){
			all_customers[c_i].needs[i] -= r_r[i];
			avaliable[i] = avaliable[i] - r_r[i];
		}


	}
}
/*
 * s_c: Runs the Status (Status command)
 * @param: None
 * @Return: none
 *
 */
void s_c(){
	printf("Available Resources: \n");
	for (int i = 0; i<r_a;i++) printf("%d ", avaliable[i]);
	printf("\n");
	printf("Maximum Resources: \n");
	for(int i = 0; i< c_a; i++){
		struct Customer beta = all_customers[i];
		for (int j = 0; j<r_a; j++){
			printf("%d ", beta.max[j]);
		}
		printf("\n");
	}
	printf("Allocated Resources: \n");
	for(int i = 0; i<c_a; i++){
		struct Customer beta = all_customers[i];
		for(int j = 0; j<r_a;j++){
			printf("%d ", beta.allocated[j]);
		}
		printf("\n");
	}
	printf("Need Resources: \n");
	for(int i =0; i<c_a;i++){
		struct Customer beta = all_customers[i];
		for(int j = 0; j <r_a; j++){
			printf("%d ", beta.needs[j]);
		}
		printf("\n");
	}
}
/*
 * Run: Runs the run command
 * @param: None
 * @return: none
 *
 */
void Run(){
	check_safe();
	printf("Safe Sequence is:");
	for(int i = 0; i<sizeof(safeseq)-3;i++) printf(" %d", safeseq[i]);
	printf("\n");
	for(int i = 0; i<sizeof(safeseq)-3;i++){
		printf("--> Customer/Thread %d \n", safeseq[i]);
		printf("Allocated resources:");
		for(int j = 0; j<r_a; j++){
			printf(" %d", all_customers[i].allocated[j]);
		}
		printf("\n");
		printf("Needed:");
		for(int j = 0; j<r_a; j++){
			printf(" %d", all_customers[i].needs[j]);
		}
		printf("\n");
		printf("Available:");
		for(int j = 0; j<r_a; j++){
			printf(" %d", avaliable[j]);
		}
		printf("\n");
		printf("Thread has started");
		for(int j = 0;j<r_a;j++){
			avaliable[j] += all_customers[i].allocated[j];
		}
		printf("\nThread has finished");
		printf("\nThread is releasing resources");
		printf("\nNew Available: ");
		for(int j = 0;j<r_a;j++){
			printf("%d ", avaliable[j]);
		}
		printf("\n");

	}

}
/*
 * Release function: Releases from index at customer index
 * @param: 2 integers takes in customer index and release information
 * @return: none
 */
void release(int c_i, int *r_r){
	struct Customer beta = all_customers[c_i];
	bool able = true;
	for(int i = 0; i<r_a;i++){
		if(r_r[i] > beta.allocated[i]){
			able = false;
			printf("Can't release resources!");
			return;
		}
	}
	if(able){
		for(int i = 0; i <r_a;i++){
			avaliable[i] = avaliable[i] + r_r[i];
			beta.allocated[i] -= r_r[i];
		}
		printf("Released successfully! \n");
	}
}
