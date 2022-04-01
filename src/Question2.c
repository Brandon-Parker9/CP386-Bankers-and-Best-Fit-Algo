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

// ------------- Global Variables -------------

struct Frame *hole_starting_frame;
struct Frame *hole_ending_frame;

struct Frame *allocated_starting_frame;
struct Frame *allocated_ending_frame;

bool running = true;

#define BUFF_SIZE 100
#define MAX_STRING_ARRAY_SIZE 10

#define EXIT 0
#define CONTINUE_RUNNING 1
#define INVALID_COMMAND 2
#define INVALID_REQUEST 3
#define VALID_REQUEST 4
#define EMPTY_SPACE "NONE"

int total_free_space;
int total_allocated_space;
int total_procces;
int total_holes;

// ------------- Objects -------------

struct Frame {
	char p_id[BUFF_SIZE];
	int mem_start;
	int mem_end;
	int size;
	struct Frame *next;
	struct Frame *prev;
};

// ------------- Function prototypes -------------

void run(); // While loop that runs allowing user input

int checkCommand(char *cmd); // decides what to do when user inputs command

char* cleanString(char *cmd); // function to remove carriage returns and new line characters

int checkrquestFormat(char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE],
		int array_length); // checks format of RQ command

int checkValidRquest(char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE],
		int array_length); // checks if the request for space by the user is possible

int checkValidRelease(char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE],
		int array_length); // checks if the release is valid

void printstatus(); // prints current status

void removeFromList(struct Frame *node, struct Frame **start_node,
		struct Frame **end_node); // properly remove node from list

void addTooList(struct Frame *node, struct Frame **start_node,
		struct Frame **end_node); // properly add node from list

// ------------- Code -------------

int main(int argc, char *argv[]) {

	if (argc > 2) {
		printf(":::: Too many command line arguments, exiting program...\n");
		return 1;
	} else {
		total_free_space = atoi(argv[1]);

		printf("Allocated %d bytes of memory\n", total_free_space);

		struct Frame frame;
		strcpy(frame.p_id, EMPTY_SPACE);
		frame.mem_start = 0;
		frame.mem_end = total_free_space - 1;
		frame.size = total_free_space;
		frame.prev = NULL;
		frame.next = NULL;

		hole_starting_frame = &frame;
		hole_ending_frame = &frame;
		allocated_starting_frame = (struct Frame*) malloc(sizeof(struct Frame));
		allocated_ending_frame = (struct Frame*) malloc(sizeof(struct Frame));
		allocated_starting_frame = NULL;
		allocated_ending_frame = NULL;

		total_holes = 1;
		total_procces = 0;

		total_allocated_space = 0;

		run();

		return 0;
	}

}

/*
 * Author:
 *
 * 		Brandon Parker
 *
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
 * Author:
 *
 * 		Brandon Parker
 *
 * Description:
 *
 *		Checks whether the RL by the user is possible or not
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
int checkValidRelease(char str_array[MAX_STRING_ARRAY_SIZE][BUFF_SIZE],
		int array_length) {

	struct Frame *curr_frame = allocated_starting_frame;
	struct Frame *found_frame = NULL;

	bool found = false;

	// see if process exist
	while (curr_frame != NULL && !found) {
		if (strcmp(curr_frame->p_id, str_array[1]) == 0) {
			found = true;
			found_frame = curr_frame;
		}
		curr_frame = curr_frame->next;
	}

	if (!found) {
		return INVALID_REQUEST;
	}

	bool both_found = false;
	bool after_found = false;
	bool before_found = false;
	struct Frame *before_hole_frame = NULL;
	struct Frame *after_hole_frame = NULL;
	struct Frame *prev_hole_frame = NULL;

	curr_frame = hole_starting_frame;

	// search for holes that connect to the allocated block-
	while (curr_frame != NULL && !both_found) {
		if ((curr_frame->mem_end + 1) == found_frame->mem_start) {

			before_hole_frame = curr_frame;
			before_found = true;
		} else if ((curr_frame->mem_start - 1) == found_frame->mem_end) {

			after_hole_frame = curr_frame;
			after_found = true;
		}

//		printf(":::: curr_end: %d found_start: %d\n", curr_frame->mem_end,
//				found_frame->mem_start);

		if (curr_frame->mem_end < found_frame->mem_start) {
			prev_hole_frame = curr_frame;
		}

		if ((after_hole_frame != NULL) & (before_hole_frame != NULL)) {
			both_found = true;
		}

		curr_frame = curr_frame->next;
	}

	if (both_found) { // surrounded by two empty holes
		struct Frame *frame;
		frame = (struct Frame*) malloc(sizeof(struct Frame*));
		strcpy(frame->p_id, EMPTY_SPACE);
		frame->mem_start = before_hole_frame->mem_start;
		frame->mem_end = after_hole_frame->mem_end;
		frame->size = before_hole_frame->size + found_frame->size
				+ after_hole_frame->size;
		frame->prev = before_hole_frame->prev;
		frame->next = after_hole_frame->next;

		if (before_hole_frame->prev == NULL) {
			hole_starting_frame = frame;
		} else if (after_hole_frame->next == NULL) {
			hole_ending_frame = frame;
		}

	} else if (after_found) { // one empty hole after

		after_hole_frame->mem_start = found_frame->mem_start;
		after_hole_frame->size += found_frame->size;

	} else if (before_found) { // one empty hole before

		before_hole_frame->mem_end = found_frame->mem_end;
		before_hole_frame->size += found_frame->size;

	} else { //no empty frames directly connected

		struct Frame *frame;
		frame = (struct Frame*) malloc(sizeof(struct Frame*));
		strcpy(frame->p_id, EMPTY_SPACE);
		frame->mem_start = found_frame->mem_start;
		frame->mem_end = found_frame->mem_end;
		frame->size = found_frame->size;
		frame->prev = NULL;
		frame->next = NULL;

		if (prev_hole_frame == NULL) { // insert at the start
//			printf(":::: prev is NULL!! \n");
			frame->next = hole_starting_frame;
			hole_starting_frame->prev = frame;
			hole_starting_frame = frame;
		} else if (prev_hole_frame->next == NULL) { // insert at the end
			frame->prev = hole_ending_frame;
			hole_ending_frame->next = frame;
			hole_ending_frame = frame;
		} else { // insert in the middle
			frame->prev = prev_hole_frame;
			frame->next = prev_hole_frame->next;
			prev_hole_frame->next = frame;
		}
	}

	total_allocated_space -= found_frame->size;
	total_free_space += found_frame->size;

	removeFromList(found_frame, &allocated_starting_frame,
			&allocated_ending_frame);

	return VALID_REQUEST;

}

/*
 * Author:
 *
 * 		Brandon Parker
 *
 * Description:
 *
 *		Checks whether the RQ by the user is possible or not
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

	int space_needed = atoi(str_array[2]);

	bool found = false;

	struct Frame *curr_frame = hole_starting_frame;
	struct Frame *possible_frame = NULL;

	while (curr_frame != NULL) {
		if (space_needed <= curr_frame->size) {
			if (!found) {
				possible_frame = curr_frame;
				found = true;
			} else {
				if (curr_frame->size < possible_frame->size) {
					possible_frame = curr_frame;
				}
			}
		}
		curr_frame = curr_frame->next;
	}

	if (!found) {
		return INVALID_REQUEST;
	}

//create new allocate frame to be added to allocate frame linked list
	struct Frame *frame;
	frame = (struct Frame*) malloc(sizeof(struct Frame*));
	strcpy(frame->p_id, str_array[1]);
	frame->mem_start = possible_frame->mem_start;
	frame->mem_end = possible_frame->mem_start + space_needed - 1;
	frame->size = space_needed;
	frame->prev = NULL;
	frame->next = NULL;

// add new frame allocated from to linked list
	addTooList(frame, &allocated_starting_frame, &allocated_ending_frame);

//update old hole to reflect the space taken
	possible_frame->mem_start += space_needed;
	possible_frame->size -= space_needed;

// if all the memory is used remove from list
	if (possible_frame->size == 0) {
		removeFromList(possible_frame, &hole_starting_frame,
				&hole_ending_frame);
	}

	total_free_space -= space_needed;
	total_allocated_space += space_needed;

	return VALID_REQUEST;
}

//  ------------------------------------- HELPER FUNCTIONS -------------------------------------
/*
 * Author:
 *
 * 		Brandon Parker
 *
 * Description:
 *
 *		Removes node from the linked list correctly
 *
 * Return:
 *
 * 		void
 *
 */
void removeFromList(struct Frame *node, struct Frame **start_node,
		struct Frame **end_node) {

	if (node->next == NULL && node->prev == NULL) {
//		printf(":: 1\n");

		*start_node = NULL;
		*end_node = NULL;
	} else if (node->next == NULL) {
//		printf(":: 2\n");
		*end_node = node->prev;
		(*end_node)->next = NULL;

	} else if (node->prev == NULL) {
//		printf(":: 3\n");
		*start_node = node->next;
		(*start_node)->prev = NULL;
	} else {
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}
	node = NULL;
	free(node);
}

/*
 * Author:
 *
 * 		Brandon Parker
 *
 * Description:
 *
 *		Add node to the linked list in the correct position
 *
 * Return:
 *
 * 		void
 *
 */
void addTooList(struct Frame *node, struct Frame **start_node,
		struct Frame **end_node) {

	if ((*start_node) == NULL && (*end_node) == NULL) {
		(*start_node) = node;
		(*end_node) = node;
	} else {
		node->prev = *end_node;
		(*end_node)->next = node;
		(*end_node) = node;
	}
}

/*
 * Author:
 *
 * 		Brandon Parker
 *
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

				if (checkrquestFormat(str_array, array_size) == INVALID_COMMAND) {
					printf(":::: Invalid RQ format, please try again!\n");
				} else {

					if (strcmp(str_array[3], "B")) {
						printf(
								":::: Invalid RQ format, only accepts B size identifier\n");
					} else {

						int state = checkValidRquest(str_array, array_size);

						if (state == VALID_REQUEST) {
							printf("Successfully allocated %s to process %s\n",
									str_array[2], str_array[1]);
						} else {
							printf("No hole of sufficient size\n");
						}
					}
				}
			} else if (strcmp(str_array[0], "RL") == 0) {

				printf("Release command\n");

				int state = checkValidRelease(str_array, array_size);

				if (state == VALID_REQUEST) {
					printf("Successfully released memory for process %s\n",
							str_array[1]);
				} else {
					printf("Process %s not found, please try again!\n",
							str_array[1]);
				}

			}

		}

		return CONTINUE_RUNNING;
	}

}

/*
 * Author:
 *
 * 		Brandon Parker
 *
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
 * Author:
 *
 * 		Brandon Parker
 *
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

/*
 * Author:
 *
 * 		Brandon Parker
 *
 * Description:
 *
 *		called when the user asks for the status, displays
 *		information about which space is taken up by each
 *		process and what parts are free
 *
 * Return:
 *
 * 		void
 *
 */
void printstatus() {
	printf("Partitions [Allocated memory = %d]:\n", total_allocated_space);

	struct Frame *curr_frame = allocated_starting_frame;

	while (curr_frame != NULL) {
		printf("Address [%d:%d] Process %s\n", curr_frame->mem_start,
				curr_frame->mem_end, curr_frame->p_id);
		curr_frame = curr_frame->next;
	}

	printf("\nHoles [Free memory = %d]:\n", total_free_space);

	curr_frame = hole_starting_frame;

	while (curr_frame != NULL) {
		printf("Address [%d:%d] len = %d\n", curr_frame->mem_start,
				curr_frame->mem_end, curr_frame->size);
		curr_frame = curr_frame->next;
	}

}
