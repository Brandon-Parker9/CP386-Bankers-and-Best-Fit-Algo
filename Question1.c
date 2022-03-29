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

struct Customer {
	int id;
	int *max_resources;
	int *allocated_resources;
	int *needed_resources;
	bool finished;
};

// Function prototypes
void run();

void printMaxReasources(int option);

void printstatus();

int checkCommand(char *cmd);

char* cleanString(char *cmd);

void checkSafeState(char **str_array, int array_length);

// Global Variables
struct Customer *customer_resources;

bool running = true;

const int BUFF_SIZE = 100;
const int MAX_STRING_ARRAY_SIZE = 10;

const int INVALID_COMMAND = 2;
const int CONTINUE_RUNNING = 1;
const int EXIT = 0;

int customer_count;
int resource_count;
int *availble_resources;

int customer_max_resources[5][4] = { { 6, 4, 7, 3 }, { 4, 2, 3, 2 }, { 2, 5, 3,
		3 }, { 6, 3, 3, 2 }, { 5, 5, 7, 5 } };

int main(int argc, char *argv[]) {

	customer_count = (sizeof customer_max_resources
			/ sizeof customer_max_resources[0]);

	resource_count = argc - 1;

	customer_resources = malloc(sizeof(struct Customer) * customer_count);

	for (int i = 0; i < customer_count; i++) {
		struct Customer temp_customer;

		temp_customer.max_resources = (int*) malloc(
				sizeof(int) * resource_count);
		temp_customer.max_resources = customer_max_resources[i];

		temp_customer.allocated_resources = malloc(
				sizeof(int) * resource_count);

		temp_customer.needed_resources = (int*) malloc(
				sizeof(int) * resource_count);
		temp_customer.needed_resources = customer_max_resources[i];

		temp_customer.finished = false;
		temp_customer.id = i;

		customer_resources[i] = temp_customer;
//		i++;
	}

	printf("Number of Customer: %d\n", customer_count);
	printf("Currently Available resources: ");

	availble_resources = (int*) malloc(sizeof(int) * resource_count);

	for (int i = 1; i < argc; i++) {
		printf("%s ", argv[i]);
		availble_resources[i - 1] = atoi(argv[i]);
	}
	printf("\n");
	printMaxReasources(0);

	run();
}

void run() {
	while (running) {
		printf("Enter Command: ");
		char input[BUFF_SIZE];

		fgets(input, BUFF_SIZE, stdin);
		int cmdCheck = checkCommand(input);

		if (cmdCheck == EXIT) {
			running = false;
		} else if (cmdCheck == INVALID_COMMAND) {
			printf(":::: Invalid command, please try again!\n");
		}
	}
}

void checkSafeState(char **str_array, int array_length) {

}

//  ------------------------------------- HELPER FUNCTIONS -------------------------------------

int checkCommand(char *cmd) {

	char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE];
	char *cleanCmd;
	cleanCmd = cleanString(cmd);

	printf("Command: |%s|\n", cleanCmd);

	if (strcmp(cleanCmd, "Status") == 0) {
		printstatus();
		return CONTINUE_RUNNING;
	} else if (strcmp(cleanCmd, "Exit") == 0) {
		printf(":::: Ending Program...\n");
		return EXIT;
	} else {
		char *pch;
		pch = strtok(cleanCmd, " ");
		int array_size = 0;
		while (pch != NULL) {
			printf("%s\n", pch);
			strcpy(str_array[array_size], pch);
			pch = strtok(NULL, " ,.-");
			array_size++;
		}

		if (strcmp(str_array[0], "RQ") == 0) {
			printf("Request Command\n");
		} else if (strcmp(str_array[0], "RL") == 0) {
			printf("Release Command\n");
		} else {
			printf("Invalid Command\n");
			return INVALID_COMMAND;
		}

		return CONTINUE_RUNNING;
	}

}

char* cleanString(char *cmd) {

	char *cleanCmd = (char*) malloc(sizeof(char) * BUFF_SIZE);

	int i = 0;

	while (cmd[i] != '\0' && cmd[i] != '\r' && cmd[i] != '\n') {
//		printf("char = %c\n", cmd[i]);
		cleanCmd[i] = cmd[i];
		i++;
	}

	cleanCmd[i] = '\0';

	return cleanCmd;

}

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
