#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h> // for usleep()


#define SIZE 4


unsigned int score = 0;


void PrintBoard(unsigned char board[SIZE][SIZE]);
bool slide(unsigned char array[SIZE]);
unsigned char findTarget(unsigned char array[SIZE], unsigned char x, unsigned char stop);
void rotateBoard(unsigned char board[SIZE][SIZE]);
bool moveUp(unsigned char board[SIZE][SIZE]);
bool moveLeft(unsigned char board[SIZE][SIZE]);
bool moveDown(unsigned char board[SIZE][SIZE]);
bool moveRight(unsigned char board[SIZE][SIZE]);
bool CheckGameEnd(unsigned char board[SIZE][SIZE]);
void addRandom(unsigned char board[SIZE][SIZE]);
void BoardInit(unsigned char board[SIZE][SIZE]);


int main(int argc, char *argv[]) {
	unsigned char board[SIZE][SIZE]; /* SIZE = 4 */
	char ch;
	bool success;

	printf("\n\nClick ENTER after every move\n\n"); /* Have make it buffered input */
	
	printf("\033[?25l\033[2J"); /* "\033[?25l" hides terminal's cursor, "\033[2J" clears terminal's display screen. These are ANSI 						command sequences */

	 
	BoardInit(board);
	
	while(true) {
		ch = getchar();
		switch(ch) {
			case 97:	// 'a' key
			case 104:	// 'h' key
			case 68:	// left arrow
				success = moveLeft(board);  
				break;
			case 100:	// 'd' key
			case 108:	// 'l' key
			case 67:	// right arrow
				success = moveRight(board); 
				break;
			case 119:	// 'w' key
			case 107:	// 'k' key
			case 65:	// up arrow
				success = moveUp(board);    
				break;
			case 115:	// 's' key
			case 106:	// 'j' key
			case 66:	// down arrow
				success = moveDown(board);  
				break;
			default: 
				success = false;
		}
		if(success) {
			PrintBoard(board);
			usleep(150000); // slight delay before the next random number is added
			addRandom(board);
			PrintBoard(board);
			if(CheckGameEnd(board)) {
				printf("         GAME OVER          \n");
				break;
			}
		}
		if(ch == 'q') {
			printf("        QUIT? (y/n)         \n");
			ch = getchar();
			if(ch == 'y') {
				exit(0);
			}
			PrintBoard(board);
		}
		if(ch == 'r') {
			printf("       RESTART? (y/n)       \n");
			ch = getchar();
			if(ch == 'y') {
				BoardInit(board);
			}
			PrintBoard(board);
		}
	}

	printf("\033[?25h"); /* ANSI command sequence to show the cursor */

	return 0; 
}


void BoardInit(unsigned char board[SIZE][SIZE]) {
	unsigned char x, y;
	for(x = 0; x < SIZE; x++) {
		for(y = 0; y < SIZE; y++) {
			board[x][y] = 0;
		}
	}
	addRandom(board);
	addRandom(board);
	PrintBoard(board);
	score = 0;
}


void addRandom(unsigned char board[SIZE][SIZE]) {
	static bool initialized = false;
	unsigned char x, y;
	unsigned char r, len = 0;
	unsigned char n, list[SIZE*SIZE][2];

	if (!initialized) { /* Only enters once in the whole program, i.e, in the starting */
		srand(time(NULL)); 
		
		/* The time() function is the seed because we want a random number that changes every second. 
		If you rerun this program without srand, you will get same set of numbers. By default seed = 1 if you do not use srand 			function. To get different numbers every time we have to use srand(unsigned int seed) function. So you will need a 			different value of seed every time you run the program. Or you can use current time which will always be different so you 			will get a different set of numbers. */
		 
		initialized = true;
	}

	for(x = 0; x < SIZE; x++) {
		for(y = 0; y < SIZE; y++) {
			if(board[x][y] == 0) {
				list[len][0] = x;
				list[len][1] = y;
				len++;
			}
		}
	}

	if (len > 0) {
		r = rand() % len; /* rand() % 16 generates a number between 0 to 15 */
		x = list[r][0]; /* 'x' is 0th column's random row */
		y = list[r][1]; /* 'y' is 1st column's same random row */
		n = (rand() % 10) / 9 + 1; // n will be 1 or 2. Most of the time 1
		board[x][y] = n; 
	}
}


void PrintBoard(unsigned char board[SIZE][SIZE]) {
	unsigned char x, y;

	printf("\033[H"); // 033 is the octal of ESC. [H moves cursor to row 1 column 1
	printf("2048.c %17d pts\n\n",score);

	for(y = 0; y < SIZE; y++) {
		printf("\n");
		for(x = 0; x < SIZE; x++) {
			if(board[x][y] != 0) {
				char s[15];
				snprintf(s, 15, "%u", (unsigned int)1<<board[x][y]); // means 1*2^(board[x][y] . In general x<<y is x*(2^y)
				unsigned char t = 7 - strlen(s);  /* strlen(s) will be 1 for '2', '4', '8'. Will be 2 for '16', '32', etc*/
				printf("%*s%s%*s", t-t/2, " ", s, t/2, " "); /* The * means that the width must be read from the next 									              argument to printf. Here it means t-t/2 times print "", then 									 	      print normal 's', then print t/2 times "" */
			} 
			else { 
				printf("   ·   ");
			}	
		}
		printf("\n");
		for(x = 0; x < SIZE; x++) {
			printf("       ");
		}
		printf("\n");
	}
	printf("\n"); 
	printf("        ←,↑,→,↓ or q        \n"); /* Ctrl+Shift+U and then: 2190 for left arrow, 2192 for right arrow, 2191 for up 							     arrow, 2193 for down arrow */
}

bool slide(unsigned char array[SIZE]) {
	bool success = false;
	unsigned char x, t, stop = 0;

	for(x = 0; x < SIZE; x++) {
		if(array[x] != 0) {
			t = findTarget(array, x, stop);
			
			if(t != x) { /* if target is not original position, then move or merge */
				if(array[t] == 0) { /* if target is zero, this is a move */
					array[t] = array[x];
				} 
				else if(array[t] == array[x]) {
					array[t]++; /* merge (increase power of two) */
					score = score + (unsigned int)1<<array[t]; /* increase score */
					stop = t + 1; /* set stop to avoid double merge */
				}
				array[x] = 0;
				success = true;
			}
		}
	}
	return success;
}

unsigned char findTarget(unsigned char array[SIZE], unsigned char x, unsigned char stop) {
	unsigned char t;	
	if(x == 0) { /* if the position is already on the first, don't evaluate */
		return x;
	}
	
	for(t = x-1; t >= 0; t--) {
		if(array[t] != 0) {
			if(array[t] != array[x]) { /* merge is not possible, take next position */
				return t + 1;
			}
			return t;
		} 
		else {	
			if (t==stop) { /* we should not slide further, return this one */
				return t;
			}
		}
	}
	return x;
}


bool moveUp(unsigned char board[SIZE][SIZE]) {
}


void rotateBoard(unsigned char board[SIZE][SIZE]) { /* Rotates clockwise */
}

bool moveLeft(unsigned char board[SIZE][SIZE]) {
	bool success;
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	return success;
}

bool moveDown(unsigned char board[SIZE][SIZE]) {
	bool success;
	rotateBoard(board);
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	rotateBoard(board);
	return success;
}

bool moveRight(unsigned char board[SIZE][SIZE]) {
	bool success;
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	return success;
}


bool CheckGameEnd(unsigned char board[SIZE][SIZE]) {
}


