/*
 * Name: Brandon Parker
 * ID: park3730
 * Email: park3730@mylaurier.ca
 *
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

// ------------- Objects -------------

struct Frame {
	char *p_id;
	int mem_start;
	int mem_end;
	int size;
	bool free;
};

// ------------- Global Variables -------------

struct Frame *frames;

bool running = true;

const int BUFF_SIZE = 100;
const int MAX_STRING_ARRAY_SIZE = 10;

const int EXIT = 0;
const int CONTINUE_RUNNING = 1;
const int INVALID_COMMAND = 2;
const int INVALID_REQUEST = 3;
const int VALID_REQUEST = 4;
const char *EMPTY_SPACE = "NONE";

int total_space;
int total_procces;

// ------------- Function prototypes -------------

void run(); // While loop that runs allowing user input

int checkCommand(char *cmd); // decides what to do when user inputs command

char* cleanString(char *cmd); // function to remove carriage returns and new line characters

int checkrquestFormat(char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE],
		int array_length); // checks format of RQ command

int checkValidRquest(char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE],
		int array_length); // checks if the request for space by the user is possible

// ------------- Code -------------

int main(int argc, char *argv[]) {

	if (argc > 2) {
		printf(":::: Too many command line arguments, exiting program...\n");
		return 1;
	} else {
		total_space = atoi(argv[1]);

		printf("Allocated %d bytes of memory\n", total_space);

		frames = malloc(sizeof(struct Frame) * total_space);

		struct Frame frame;
		frame.p_id = malloc(sizeof(char) * BUFF_SIZE);
		strcpy(frame.p_id, EMPTY_SPACE);
		frame.mem_start = 0;
		frame.mem_end = total_space;
		frame.size = total_space;
		frame.free = true;

		frames[0] = frame;

		total_procces = 1;

		run();

		return 0;
	}

}

/*
 * Description:
 *
 * Called after initial setup to run while loop
 * asking the user for in put
 *
 * Return:
 *
 * void
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
			printf(":::: Invalid input, use one of RQ, RL, Status, Exit\n");

		}
	}
}

/*
 * Description:
 *
 *		Checks whether the rq by the user is possible or not
 *
 * Parameters:
 *
 * 		char str_array[][] -> stores the most recent user command split
 * 		into and array
 *
 * 		int array_length -> the number of strings in the array
 *
 * Return:
 *
 * 		int -> INVALID_REQUEST = 1 or VALID_REQUEST = 4
 *
 */

int checkValidRquest(char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE],
		int array_length) {

	return VALID_REQUEST;
}

//  ------------------------------------- HELPER FUNCTIONS -------------------------------------

int checkCommand(char *cmd) {

	char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE];
	char *cleanCmd;
	cleanCmd = cleanString(cmd);

	if (strcmp(cleanCmd, "Status") == 0) {

//		printstatus();
		return CONTINUE_RUNNING;

	} else if (strcmp(cleanCmd, "Exit") == 0) {

		printf(":::: Ending Program...\n");
		return EXIT;

	} else {

		char *pch;
		pch = strtok(cleanCmd, " ");
		int array_size = 0;
		while (pch != NULL) {

			strcpy(str_array[array_size], pch);
			pch = strtok(NULL, " ,.-");
			array_size++;
		}

		if (strcmp(str_array[0], "RQ") != 0
				&& strcmp(str_array[0], "RL") != 0) {

			return INVALID_COMMAND;

		} else if (((strcmp(str_array[0], "RQ") == 0 && array_size != 4))
				|| (strcmp(str_array[0], "RL") == 0 && array_size != 2)) {

			return INVALID_COMMAND;

		} else {

			if (strcmp(str_array[0], "RQ") == 0) {
				printf("Request command\n");
				if (checkrquestFormat(str_array, array_size)
						== INVALID_COMMAND) {
					printf(":::: Invalid RQ format, please try again!\n");
				} else {
					printf("Command accepted\n");

					checkValidRquest(str_array, array_size);
				}
			} else if (strcmp(str_array[0], "RL") == 0) {
				printf("Release command\n");
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
 *		Checks whether the format of the RQ command is in the proper
 *		format or not
 *
 * Parameters:
 *
 * 		char str_array[][] -> stores the most recent user command split
 * 		into and array
 *
 * 		int array_length -> the number of strings in the array
 *
 * Return:
 *
 * 		int -> CONTINUE_RUNNING = 1 or INVALID_COMMAND = 2
 *
 */
int checkrquestFormat(char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE],
		int array_length) {

	if (isalpha(str_array[1][0]) == 0 || isalpha(str_array[3][0]) == 0) {
		return INVALID_COMMAND;

	}

	// check if 3 item in request command is a number
	int length = strlen(str_array[2]);
	for (int i = 0; i < length; i++) {
		if (!isdigit(str_array[2][i])) {
			return INVALID_COMMAND;
		}
	}

	return CONTINUE_RUNNING;
}
