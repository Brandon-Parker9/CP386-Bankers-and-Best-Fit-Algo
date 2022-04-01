# CP386-Bankers-and-Best-Fit-Algo
Implimentation of Bankers and Best fit algo in C/C++

## Preview 

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
Example: 
```
    ./main 10 5 7 8

    rq 0 1 0 0 1

    rq 1 1 1 1 1

    rq 2 2 2 2 2

    rq 3 1 1 1 1

    rq 4 1 0 0 0

    run
```

## About The Developers 
- [Brandon Parker](https://github.com/Brandon-Parker9): Wilfrid Laurier University BSc in Computer Science<br/>

## License
This project uses the **MIT License** which can be located [here](https://github.com/Brandon-Parker9/CP386-Bankers-and-Best-Fit-Algo/blob/main/LICENSE)
