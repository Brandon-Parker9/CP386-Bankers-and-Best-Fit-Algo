# CP386-Bankers-and-Best-Fit-Algo
Implimentation of Bankers and Best fit algo in C/C++

## Preview 

Question1.c<br/>
![rq&statusCommand](/pictures/q1_request.jpg)<br/>
![runCommand](/pictures/q1_run.jpg)<br/>
![rlCommand](/pictures/q1_release.jpg)<br/>

Question2.c<br/>
![rq&rlCommand](/pictures/q2_request_release.jpg)<br/>
![statusCommand](/pictures/q2_status.jpg)<br/>

## Contributors
- [Brandon Parker](https://github.com/Brandon-Parker9)<br/>

Question1.c
```c
void run(); // Brandon Parker
void printMaxReasources(int option); // Brandon Parker
void printstatus(); // Brandon Parker
int checkCommand(char *cmd); // Brandon Parker
char* cleanString(char *cmd); // Brandon Parker
void checkSafeState(char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE], int array_length); // Brandon Parker
int runBankersAlgo(); // Brandon Parker
void printSequence(); // Brandon Parker
void runCustomers(); // Brandon Parker
void* threadRun(int *nums); // Brandon Parker
```

Question2.c
```c
void run(); // Brandon Parker
int checkCommand(char *cmd); // Brandon Parker
char* cleanString(char *cmd); // Brandon Parker
int checkrquestFormat(char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE],int array_length); // Brandon Parker
int checkValidRquest(char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE], int array_length); // Brandon Parker
int checkValidRelease(char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE], int array_length); // Brandon Parker
void printstatus(); // Brandon Parker
void removeFromList(struct Frame *node, struct Frame **start_node, struct Frame **end_node); // Brandon Parker
void addTooList(struct Frame *node, struct Frame **start_node, struct Frame **end_node); // Brandon Parker
```

## Tests
Example Question 1: 
```
    ./Question1 10 5 7 8
    RQ 0 1 0 0 1
    RQ 1 1 1 1 1
    RQ 2 2 2 2 2
    RQ 3 1 1 1 1
    RQ 4 1 0 0 0

    Run
    
    Exit
```
Example Question 2: 
```
    ./Question2 1000000
    RQ P0 200000 B
    RQ P1 350000 B
    RQ P2 300000 B
    RL P0
    
    Status
    
    Exit
```

## About The Developers 
- [Brandon Parker](https://github.com/Brandon-Parker9): Wilfrid Laurier University BSc in Computer Science<br/>

## License
This project uses the **MIT License** which can be located [here](https://github.com/Brandon-Parker9/CP386-Bankers-and-Best-Fit-Algo/blob/main/LICENSE)
