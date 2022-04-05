# CP386-Assignment-4
Project done by students Nahor Yirgaalem and Jonah Eichler. The first question required programmers to write bankers algorithm implemented by a multi-threaded program. "Customers" request resources from the banks as well as release them.
The second questions required programmers to write a best fit algorithm for allocating memory, keeping track of idfferent allocations and holes (which represent the available memory)

##Make File
The Make File allows us to run our files without inputting any values. Simply typing "make runq1" into the terminal runs Question1.c with 10 5 7 8 as the input and "make runq2" runs Question2.c with input 1000000

##Contributors
- [Nahor Yirgaalem](github.com/Yirgaalem)
- [Jonah Eichler](github.com/Eichs01)
.
##Screen Shots
Question1.c
![Question1.png](github.com/Yirgaalem/CP386-Assignment-4/blob/main/Question1.png)
![Question1 pt2.png](github.com/Yirgaalem/CP386-Assignment-4/blob/main/Question1%20pt2.png)

Question2.c
![Question2.png](https://github.com/Yirgaalem/CP386-Assignment-4/blob/main/Question2.png)

##Individual Contributions
Question1.c
Jonah Eichler:
```c
int available_resources(int c, char *args[]);
int customer_resources()
void start();
void request_resources(int c_index, int *r_r);
void r_c(); 
void *r_c_f();
void s_c(); 
void run(); 
bool check_safe();
```

Question2.c
Nahor Yirgaalem
```c
void listAdd(struct Customer *x, struct Customer **y, struct Customer **z);
void listTake(struct Customer *x, struct Customer **y, struct Customer **z);
int readCommand(char *command);
void status(char *string);
int confirmerRQ(char array[MAX][BUFFERSIZE], int size);
int confirmRelease(char array[MAX][BUFFERSIZE]);
int confirmRequest(char array[MAX][BUFFERSIZE], int size);
void releaseData(struct Customer *x , struct Customer *y);
void requestData(struct Customer *x, struct Customer *y, struct Customer *z, char array[MAX][BUFFERSIZE], int reqSpace);
```

##About The Developers
- [Nahor Yirgaalem](github.com/Yirgaalem) is a third year computer science student at Wilfrid Laurier University. 
- [Jonah Eichler](github.com/Eichs01) a third year computer science student at Wilfrid Laurier University.

##License
This project uses the **MIT License** which can be located https://github.com/Yirgaalem/CP386-Assignment-4/blob/main/LICENSE
