/*
 * Name: Brandon Parker
 * ID: park3730
 * Email: park3730@mylaurier.ca
 *
 */

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

// ------------- Objects -------------

struct Customer {
	int id;
	int *max_resources;
	int *allocated_resources;
	int *needed_resources;
	bool finished;
	bool allocated;
};

// ------------- Global Variables -------------

struct Customer *customer_resources;

bool running = true;

const int BUFF_SIZE = 100;
const int MAX_STRING_ARRAY_SIZE = 10;

const int EXIT = 0;
const int CONTINUE_RUNNING = 1;
const int INVALID_COMMAND = 2;
const int SAFE_STATE = 3;
const int UNSAFE_STATE = 4;

int customer_count;
int resource_count;
int *availble_resources;
int *customer_order;

int customer_max_resources[5][4] = { { 6, 4, 7, 3 }, { 4, 2, 3, 2 }, { 2, 5, 3,
		3 }, { 6, 3, 3, 2 }, { 5, 5, 7, 5 } };

// ------------- Function prototypes -------------

void run(); // While loop that runs allowing user input

void printMaxReasources(int option); // prints just the current max resources

void printstatus(); // prints current status

int checkCommand(char *cmd); // decides what to do when user inputs command

char* cleanString(char *cmd); // function to remove carriage returns and new line characters

void checkSafeState(char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE],
		int array_length); // function to properly call the bankers algo and revert if not a safe state

int runBankersAlgo(); // helper function to run the bankers algo

void printSequence(); // function to print the current safe sequence

void runCustomers(); // function to call the customers in the right order

void* threadRun(int *nums); //the thread function, the code executed by each thread

// ------------- Code -------------

int main(int argc, char *argv[]) {

	customer_count = (sizeof customer_max_resources
			/ sizeof customer_max_resources[0]);

	resource_count = argc - 1;

	customer_resources = malloc(sizeof(struct Customer) * customer_count);
	customer_order = (int*) malloc(sizeof(int) * customer_count);

	// setup the initial customers based the users/file inputs
	for (int i = 0; i < customer_count; i++) {
		struct Customer temp_customer;

		temp_customer.max_resources = (int*) malloc(
				sizeof(int) * resource_count);
		temp_customer.needed_resources = (int*) malloc(
				sizeof(int) * resource_count);
		temp_customer.allocated_resources = malloc(
				sizeof(int) * resource_count);

		for (int col = 0; col < resource_count; col++) {
			temp_customer.max_resources[col] = customer_max_resources[i][col];
			temp_customer.needed_resources[col] =
					customer_max_resources[i][col];
			temp_customer.allocated_resources[col] = 0;
		}

		temp_customer.finished = false;
		temp_customer.allocated = false;
		temp_customer.id = i;

		customer_resources[i] = temp_customer;

		customer_order[i] = i;
	}

	printf("Number of Customer: %d\n", customer_count);
	printf("Currently Available resources: ");

	availble_resources = (int*) malloc(sizeof(int) * resource_count);

	// print the available resources
	for (int i = 1; i < argc; i++) {
		printf("%s ", argv[i]);
		availble_resources[i - 1] = atoi(argv[i]);
	}
	printf("\n");
	printMaxReasources(0);

	// start getting user input
	run();
}

/*
 * Description:
 *
 *		Called after initial setup to run while loop
 *		asking the user for in put
 *
 * Return:
 *
 * 		void
 *
 */
void run() {
	while (running) {
		printf("Enter Command: ");
		char input[BUFF_SIZE];

		fgets(input, BUFF_SIZE, stdin);
		int cmdCheck = checkCommand(input);

		if (cmdCheck == EXIT) {
			running = false;
		} else if (cmdCheck == INVALID_COMMAND) {
			printf(
					":::: Invalid input, use one of RQ, RL, Status, Run, Exit\n");
		}
	}
}

/*
 * Description:
 *
 *		Store the original values and set the new ones temporarily until
 *		after calling bankers algorithm and determining if a safe state
 *		is possible. If not possible, values are then reverted and the
 *		users is informed.
 *
 * Parameters:
 * 		char str_array[][] -> stores the most recent user command split
 * 		into and array
 *
 * Return:
 *
 * 		void
 *
 */
void checkSafeState(char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE],
		int array_length) {

	int customer_number = atoi(str_array[1]);
	int *cus_initial_res_alloc = (int*) malloc(sizeof(int) * resource_count);
	int *cus_initial_res_needed = (int*) malloc(sizeof(int) * resource_count);
	int *initial_avail_resources = (int*) malloc(sizeof(int) * resource_count);

	struct Customer curr_customer = customer_resources[customer_number];

	// hold onto original amounts in case they need to be restated
	for (int i = 0; i < resource_count; i++) {
		cus_initial_res_alloc[i] = curr_customer.allocated_resources[i];
		cus_initial_res_needed[i] = curr_customer.needed_resources[i];
		initial_avail_resources[i] = availble_resources[i];
	}

	// change values based on a request command
	if (strcmp(str_array[0], "RQ") == 0) {

		// cycle through all resource values and update
		for (int i = 0; i < resource_count; i++) {

			availble_resources[i] += curr_customer.allocated_resources[i];

			curr_customer.allocated_resources[i] = atoi(str_array[i + 2]);
			curr_customer.needed_resources[i] = curr_customer.max_resources[i]
					- curr_customer.allocated_resources[i];

			availble_resources[i] -= curr_customer.allocated_resources[i];
		}

	} else if (strcmp(str_array[0], "RL") == 0) {

		// cycle through all resource values and update
		for (int i = 0; i < resource_count; i++) {

			if (atoi(str_array[i + 2]) > curr_customer.allocated_resources[i]) {

				availble_resources[i] += curr_customer.allocated_resources[i];
				curr_customer.allocated_resources[i] = 0;
			} else {

				curr_customer.allocated_resources[i] -= atoi(str_array[i + 2]);
				availble_resources[i] += atoi(str_array[i + 2]);
			}

			curr_customer.allocated_resources[i] -= atoi(str_array[i + 2]);

			curr_customer.needed_resources[i] = curr_customer.max_resources[i]
					- curr_customer.allocated_resources[i];
		}
	}

	// only run bankers algorithm when requesting resources
	if (strcmp(str_array[0], "RQ") == 0) {

		int state = runBankersAlgo();

		// if state is unsafe, return all values back to what they were
		if (state == UNSAFE_STATE) {

			printf("State is unsafe, and request is not satisfied\n");

			for (int i = 0; i < resource_count; i++) {
				curr_customer.allocated_resources[i] = cus_initial_res_alloc[i];
				curr_customer.needed_resources[i] = cus_initial_res_needed[i];
				availble_resources[i] = initial_avail_resources[i];
			}

		} else { // let user know the state is safe

			printf("State is safe, and request is satisfied\n");

		}
	}
}

/*
 * Description:
 *
 *		Runs bankers algorithm to determine if a safe state
 *		is plausible or not
 *
 * Return:
 *
 * 		integer -> whether or not a safe state is possible.
 * 					SAFE_STATE = 3 or UNSAFE_STATE = 4
 *
 */
int runBankersAlgo() {

	int allocated_count = 0;
	bool updated = true;
	bool allocatable = true;

	int *calculate_availble_resources = (int*) malloc(
			sizeof(int) * resource_count);

	int *calculate_customer_order = (int*) malloc(sizeof(int) * customer_count);

	for (int cus_i = 0; cus_i < customer_count; cus_i++) {
		customer_resources[cus_i].allocated = false;
	}

	for (int res_i = 0; res_i < resource_count; res_i++) {
		calculate_availble_resources[res_i] = availble_resources[res_i];
	}

	while (updated && allocated_count < customer_count) {

		updated = false;

		for (int cus_i = 0; cus_i < customer_count; cus_i++) {

			allocatable = true;

			if (customer_resources[cus_i].allocated == false) {

				// checks if it can be allocated
				for (int res_i = 0; res_i < resource_count; res_i++) {
					if (customer_resources[cus_i].needed_resources[res_i]
							> calculate_availble_resources[res_i]) {
						allocatable = false;
						break;
					}
				}

				if (allocatable) {
					calculate_customer_order[allocated_count] =
							customer_resources[cus_i].id;
					customer_resources[cus_i].allocated = true;
					allocated_count++;
					updated = true;
					for (int res_i = 0; res_i < resource_count; res_i++) {
						calculate_availble_resources[res_i] +=
								customer_resources[cus_i].allocated_resources[res_i];
					}
				}
			}
		}
	}

	// after running algorithm determine safe or unsafe
	if (allocated_count == customer_count) {
		for (int i = 0; i < customer_count; i++) {
			customer_order[i] = calculate_customer_order[i];
		}
		return SAFE_STATE;
	} else {
		return UNSAFE_STATE;
	}

}

/*
 * Description:
 *
 *		Called when user enters the run command and starts
 *		the customers in a new thread in the safe state order
 *
 * Return:
 *
 * 		void
 *
 */
void runCustomers() {

	printSequence();

	pthread_t t_id;
	int *curr_customer_num = malloc(sizeof(int));

	for (int i = 0; i < customer_count; i++) {

		*curr_customer_num = customer_order[i];

		struct Customer curr_customer = customer_resources[*curr_customer_num];

		printf("--> Customer/Thread %d\n", *curr_customer_num);

		printf("    Allocated resources:");
		for (int i = 0; i < resource_count; i++) {
			printf(" %d", curr_customer.allocated_resources[i]);
		}
		printf("\n");

		printf("    Needed:");
		for (int i = 0; i < resource_count; i++) {
			printf(" %d", curr_customer.needed_resources[i]);
		}
		printf("\n");

		printf("    Available:");
		for (int i = 0; i < resource_count; i++) {
			printf(" %d", availble_resources[i]);
		}
		printf("\n");

		// start and wait for threads to complete
		pthread_create(&t_id, NULL, (void*) &threadRun, curr_customer_num);
		pthread_join(t_id, NULL);

		printf("    Thread is releasing resources\n");

		// update all resource values
		for (int i = 0; i < resource_count; i++) {
			availble_resources[i] += curr_customer.allocated_resources[i];

			curr_customer.needed_resources[i] +=
					curr_customer.allocated_resources[i];

			curr_customer.allocated_resources[i] = 0;
		}

		printf("    New Available:");
		for (int i = 0; i < resource_count; i++) {
			printf(" %d", availble_resources[i]);
		}
		printf("\n");
	}

}

/*
 * Description:
 *
 *		function called when creating a thread
 *
 * Parameters:
 * 		int* num -> customer id
 *
 * Return:
 *
 * 		void*
 *
 */
void* threadRun(int *num) {

	printf("    Thread has started\n");
	printf("    Thread has finished\n");

	pthread_exit(0);

	return NULL;
}

//  ------------------------------------- HELPER FUNCTIONS -------------------------------------

/*
 * Description:
 *
 *		Prints the current safe sequence
 *
 * Return:
 *
 * 		void
 *
 */
void printSequence() {

	printf("Safe Sequence is: ");

	for (int i = 0; i < customer_count; i++) {
		printf("%d ", customer_order[i]);
	}
	printf("\n");
}

/*
 * Description:
 *
 *		Determines what to do based on the users command
 *
 * Parameters:
 * 		char *cmd -> the users input
 *
 * Return:
 *
 * 		int -> CONTINUE_RUNNING = 1, INVALID_COMMAND = 2, or EXIT = 0
 * 				depending on what the user inputs
 *
 */
int checkCommand(char *cmd) {

	char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE];
	char *cleanCmd;
	cleanCmd = cleanString(cmd);

	if (strcmp(cleanCmd, "Status") == 0) {

		printstatus();
		return CONTINUE_RUNNING;

	} else if (strcmp(cleanCmd, "Exit") == 0) {

		printf(":::: Ending Program...\n");
		return EXIT;

	} else if (strcmp(cleanCmd, "Run") == 0) {

		printf(":::: Running threads...\n");
		runCustomers();
		return CONTINUE_RUNNING;

	} else {

		char *pch;
		pch = strtok(cleanCmd, " ");
		int array_size = 0;
		while (pch != NULL) {

			strcpy(str_array[array_size], pch);
			pch = strtok(NULL, " ,.-");
			array_size++;
		}

		if (array_size != 6) {

			return INVALID_COMMAND;

		} else if (strcmp(str_array[0], "RQ") != 0
				&& strcmp(str_array[0], "RL") != 0) {

			return INVALID_COMMAND;

		} else {

			for (int i = 0; i < customer_count; i++) {

				if (availble_resources[i] < atoi(str_array[i + 2])) {
					printf(":::: Requested to many resources!\n");
					return INVALID_COMMAND;
				}
			}

			if (strcmp(str_array[0], "RQ") == 0) {

				checkSafeState(str_array, array_size);

			} else if (strcmp(str_array[0], "RL") == 0) {

				checkSafeState(str_array, array_size);

			} else {
				return INVALID_COMMAND;
			}
		}

		return CONTINUE_RUNNING;
	}

}

/*
 * Description:
 *
 *		cleans and removes any newline or carriage returns
 *		from the passed sting
 *
 * Parameters:
 * 		char *cmd -> the users input
 *
 * Return:
 *
 * 		char* -> a cleaned version of the passed command
 *
 */
char* cleanString(char *cmd) {

	char *cleanCmd = (char*) malloc(sizeof(char) * BUFF_SIZE);

	int i = 0;

	while (cmd[i] != '\0' && cmd[i] != '\r' && cmd[i] != '\n') {
		cleanCmd[i] = cmd[i];
		i++;
	}

	cleanCmd[i] = '\0';

	return cleanCmd;

}

/*
 * Description:
 *
 *		prints out the current maximum resources
 *
 * Parameters:
 * 		int options -> changes the initial message printed
 *
 * 		options = 0 -> "Maximum resources from file:\n"
 *		options != -> "Maximum resources:\n"
 * Return:
 *
 * 		void
 *
 */
void printMaxReasources(int options) {

	if (options == 0) {
		printf("Maximum resources from file:\n");
	} else {
		printf("Maximum resources:\n");
	}

	for (int row = 0; row < customer_count; row++) {
		for (int col = 0; col < resource_count; col++) {
			printf("%d ", customer_resources[row].max_resources[col]);
		}
		printf("\n");
	}
}

/*
 * Description:
 *
 *		called when the user asks for the status, displays
 *		information about Available, Allocated, Max and
 *		Needed resources
 *
 * Return:
 *
 * 		void
 *
 */
void printstatus() {

	printf("Available resources:\n");

	for (int i = 0; i < resource_count; i++) {
		printf("%d ", availble_resources[i]);
	}

	printf("\n");

	printMaxReasources(1);

	printf("Allocated resources:\n");

	for (int row = 0; row < customer_count; row++) {
		for (int col = 0; col < resource_count; col++) {
			printf("%d ", customer_resources[row].allocated_resources[col]);
		}
		printf("\n");
	}

	printf("Needed resources:\n");

	for (int row = 0; row < customer_count; row++) {
		for (int col = 0; col < resource_count; col++) {
			printf("%d ", customer_resources[row].needed_resources[col]);
		}
		printf("\n");
	}

}
