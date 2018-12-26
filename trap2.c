/* Trap Two on a 8x8 board */

/*

Changes needed:
	- prevent A7 or G1 for example

*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define ASCII_FULL_STOP 46

#define ASCII_0 48
#define ASCII_A 65
#define ASCII_F 70
#define ASCII_a 97
#define ASCII_f 102
#define ADCII_A_to_a 32

#define BOARD_SQUARES 36
#define BOARD_COLUMNS 6
#define BOARD_ROWS 6

#define MAX_MOVES 36

// data type declaration
struct game  {
	int turn;
	char move;
	char * playerOne;
	char * playerTwo;
	char board[BOARD_SQUARES];
}thisGame;

// prototypes
char * playerName(void);
void drawBoard(void);
void promptMove(void);
int checkMove (void);
int flip(void);

int main(void)
{
	// assign starting values
 	thisGame.turn = 0;

 	// fill board array with full stops
 	for (int i = 0; i < BOARD_SQUARES; i++){
 		thisGame.board[i] = ASCII_FULL_STOP;
 	}

	// get names and announce X and O
	thisGame.playerOne = playerName();
	thisGame.playerTwo = playerName();
	printf("\n%s gets x and %s gets o - let's go!!!\n", 
			thisGame.playerOne, thisGame.playerTwo); 

	// prompts for moves until win
	do {
		drawBoard();
		promptMove();
	}
	while (flip() == 0);

	// end of game winning board display and message
	drawBoard();

	if (flip() == 2) {
		drawBoard();
		printf("Play again soon!\n\n");
	}
	else {
		drawBoard();
		printf("\nWay to go, %s!!!!!\n\n",
		    thisGame.turn % 2 == 0 ? thisGame.playerOne : thisGame.playerTwo);
	}

	free(playerName);

	return 0;
}

/*****************************************************************************/
/*****************************************************************************/

char * playerName(void) {
	char * playerName = malloc(sizeof(char) * 25);
	printf("Player name: "); 
	scanf(" %s", playerName);
	fseek(stdin,0,SEEK_END);

	return playerName;
}

/************************************/

void drawBoard(void) {
	printf("\n\n");
	for (int i = 0; i < BOARD_ROWS; i++) {
		printf("     %c   %c   %c   %c   %c   %c   %c\n", i + ASCII_a,
			thisGame.board[i * BOARD_COLUMNS + 0], 
			thisGame.board[i * BOARD_COLUMNS + 1], 
			thisGame.board[i * BOARD_COLUMNS + 2], 
			thisGame.board[i * BOARD_COLUMNS + 3], 
			thisGame.board[i * BOARD_COLUMNS + 4], 
		    thisGame.board[i * BOARD_COLUMNS + 5]);
		printf("\n");
	}
	printf("         1   2   3   4   5   6\n");
	printf("\n");

	return;
}

/************************************/

void promptMove(void) {

	char yInput;
	char xInput;
	// number of items scanned
	int scanVal;

	do {
		printf("%s - pick a square (ex. a5, c1, f7): ", 
		thisGame.turn % 2 == 0 ? thisGame.playerOne : thisGame.playerTwo);
		scanVal = scanf(" %c", &yInput);
		scanVal = scanf(" %c", &xInput);
			
		yInput = (yInput - 97) * 6;
		xInput = xInput - 49;

		thisGame.board[yInput + xInput] = (thisGame.turn % 2 == 0 ? 'x' : 'o');

		thisGame.turn++;

		flip();

		drawBoard();

		// consumes remaining characters preventing perpetual loop
		if (scanVal == 0) {
			scanf("%*s");	
		}
	}
	while (scanVal == 0 || ((yInput < ASCII_A) || 
		  (yInput > ASCII_f)) || 
		  ((yInput > ASCII_F) && (yInput < ASCII_a)) || (xInput < 1) || (xInput > 6) ||
	      (checkMove() == 1));

	return;
}

/************************************/

int checkMove (void) {

	// converts UPPER to lower case, then to 0 thru 6 for array elements
	if ((thisGame.move >= ASCII_A) && (thisGame.move <= ASCII_F)) {
		thisGame.move = (thisGame.move + ADCII_A_to_a - ASCII_a);
	}
	// converts lower case to 0 thru 6 for array elements
	else if ((thisGame.move >= ASCII_a) && (thisGame.move <= ASCII_f)) {
		thisGame.move = (thisGame.move - ASCII_a);
	}
	// translates Y and X to game board
	// thisGame.move = (thisGame.yInput + thisGame.xInput - ASCII_YX);
	printf("move in checkMove is: %d\n", thisGame.move);
	// adds x or o to game board depending on turn
	thisGame.board[thisGame.move] = (thisGame.turn % 2 == 0 ? 'x' : 'o');

	return 0;
}

/************************************/

int flip(void) {

	printf("flip called\n");

	// check for vertical pair
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 3; y++) {

			if (thisGame.board[(y * BOARD_COLUMNS) + x + 0] != 
				ASCII_FULL_STOP &&	

				thisGame.board[(y * BOARD_COLUMNS) + x + 0] == 
				(thisGame.turn % 2 == 0 ? 'o' : 'x') &&

			 	thisGame.board[(y * BOARD_COLUMNS) + x + 6] == 
			 	(thisGame.turn % 2 == 0 ? 'x' : 'o') && 

			 	thisGame.board[(y * BOARD_COLUMNS) + x + 12] == 
			 	(thisGame.turn % 2 == 0 ? 'x' : 'o') &&

			 	thisGame.board[(y * BOARD_COLUMNS) + x + 18] == 
			 	(thisGame.turn % 2 == 0 ? 'o' : 'x')) {

					printf("We have a flipper!\n");
					printf("turn is %d\n", thisGame.turn);

					thisGame.board[(y * BOARD_COLUMNS) + x + 6] = 
					(thisGame.turn % 2 == 0 ? 'o' : 'x');

					thisGame.board[(y * BOARD_COLUMNS) + x + 12] = 
					(thisGame.turn % 2 == 0 ? 'o' : 'x');

					return 0;
			}
		}
	}

	// check for horizontal pair
	for (int y = 0; y < 6; y++) {
		for (int x = 0; x < 4; x++) {
			if ((thisGame.board[(y * BOARD_COLUMNS) + x + 0] != 
				ASCII_FULL_STOP &&

				thisGame.board[(y * BOARD_COLUMNS) + x + 0] == 
				(thisGame.turn % 2 == 0 ? 'x' : 'o') &&

			 	thisGame.board[(y * BOARD_COLUMNS) + x + 1] == 
			 	(thisGame.turn % 2 == 0 ? 'o' : 'x') && 

			 	thisGame.board[(y * BOARD_COLUMNS) + x + 2] == 
			 	(thisGame.turn % 2 == 0 ? 'o' : 'x')) &&

			 	thisGame.board[(y * BOARD_COLUMNS) + x + 3] == 
			 	(thisGame.turn % 2 == 0 ? 'x' : 'o')) {

					thisGame.board[(y * BOARD_COLUMNS) + x + 1] =
			 		(thisGame.turn % 2 == 0 ? 'x' : 'o'); 

			 		thisGame.board[(y * BOARD_COLUMNS) + x + 2] = 
			 		(thisGame.turn % 2 == 0 ? 'x' : 'o');

			 		return 0;
	        }
		}
	}

	// check for back-slant diaginal pair 
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 4; x++) {
			if ((thisGame.board[(y * BOARD_COLUMNS) + x + 0] != 
				ASCII_FULL_STOP &&	

				thisGame.board[(y * BOARD_COLUMNS) + x + 0] == 
				(thisGame.turn % 2 == 0 ? 'x' : 'o') &&

			 	thisGame.board[(y * BOARD_COLUMNS) + x + 8] == 
			 	(thisGame.turn % 2 == 0 ? 'o' : 'x') && 

			 	thisGame.board[(y * BOARD_COLUMNS) + x + 16] == 
			 	(thisGame.turn % 2 == 0 ? 'o' : 'x')) &&

				thisGame.board[(y * BOARD_COLUMNS) + x + 24] == 
			 	(thisGame.turn % 2 == 0 ? 'x' : 'o')) {

					thisGame.board[(y * BOARD_COLUMNS) + x + 8] = 
			 		(thisGame.turn % 2 == 0 ? 'x' : 'o'); 

			 		thisGame.board[(y * BOARD_COLUMNS) + x + 16] = 
			 		(thisGame.turn % 2 == 0 ? 'x' : 'o');

					return 0;
			}
		}
	}

	// check for forward-slant diaginal win 
	for (int y = 0; y < 3; y++) {
		for (int x = 3; x < 7; x++) {
			if ((thisGame.board[(y * BOARD_COLUMNS) + x + 0] != 
				ASCII_FULL_STOP) &&	

				thisGame.board[(y * BOARD_COLUMNS) + x + 0] == 
				(thisGame.turn % 2 == 0 ? 'x' : 'o') &&

			 	thisGame.board[(y * BOARD_COLUMNS) + x + 6] == 
			 	(thisGame.turn % 2 == 0 ? 'o' : 'x') && 

			 	thisGame.board[(y * BOARD_COLUMNS) + x + 12] == 
			 	(thisGame.turn % 2 == 0 ? 'o' : 'x') &&

			 	thisGame.board[(y * BOARD_COLUMNS) + x + 18] == 
			 	(thisGame.turn % 2 == 0 ? 'x' : 'o')) {

					thisGame.board[(y * BOARD_COLUMNS) + x + 6] =
				 	(thisGame.turn % 2 == 0 ? 'x' : 'o');

				 	thisGame.board[(y * BOARD_COLUMNS) + x + 12] =
				 	(thisGame.turn % 2 == 0 ? 'x' : 'o');

					return 0;
			}
		}
	}

	// check for no winner
	if (thisGame.turn > MAX_MOVES) {
		printf("\n\n\nChecking for win... \n\n\n - no winner.\n");
		return 2;
	}
	else {
		// prevents this message befor first move is made
		if (thisGame.turn > 1) {
			printf("\n\n\nChecking for win... \n\n\n     - no win\n");
		}
		return 0;
	}	
}
