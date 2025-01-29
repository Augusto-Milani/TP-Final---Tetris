#include "backend.h"
#include "pieces.h"

#define PIECES_TETRIS 7
#define PIECE_3x3_AUX 5
#define PIECE_SIZE(x) ((x)!=6	?	((x)!=3 ? 3 : 2)	:	4)	// if x is an I, it's 4x4, if it's an O, its 2x2. Otherwise, it's 3x3

int board[BOARD_HEIGHT][BOARD_WIDTH];

int score, top, lines, level;
int tetromino[7];


static int x_coord, y_coord;
void* pieces[PIECES_TETRIS] = {(void*)T, (void*)J, (void*)Z, (void*)O, (void*)S, (void*)L, (void*)I}; //This array holds pointers to all pieces in game, it has to be global for ease of coding
// IMPORTANT: The arrangment is crutial for deciding if it's a 3x3 piece or 4x4 piece.
// 			  We use 5 pieces of 3x3 and the last two are 4x4.


static int status[4][4];
static int nextPieceID;

/*
int main() {
    srand(time(NULL)); //set random seed

    initBoard();

    printBoard();
    nextPiece();
    printBoard();

    shiftPieceDown();
    printBoard();
    shiftPieceRight();
        printBoard();
        shiftPieceLeft();
            printBoard();


}


*/

//initialize the board, 20*10 size
void initBoard() {
	int i, j;
    for (i = 0; i < BOARD_HEIGHT; i++) {
        for (j = 0; j < BOARD_WIDTH; j++) {
            board[i][j] = 0;
        }
    }
}

void nextPiece() {
	x_coord = (int)BOARD_WIDTH/2 - 2;
	y_coord = 0;

    nextPieceID = rand() % PIECES_TETRIS;	 //Tetris has 7 pieces

    if(nextPieceID < PIECE_3x3_AUX) {
        printMatrix_3by3(pieces[nextPieceID]);
    }
    else {
        printMatrix_4by4(pieces[nextPieceID]);
    }
    addPiece(nextPieceID);


}



// Rotate function
void rotateClockwise() {
    int i, j, aux;
    aux = PIECE_SIZE(nextPieceID);
    
    // Prevent rotation if there's an obstacle
    for (i = y_coord; i < y_coord + aux; i++) {
        for (j = x_coord; j < x_coord + aux; j++) {
        	if(board[i][j] == 2 && board[aux-1-j][i] == 1) {	// Note: 2 is static block, 1 is moving block
        		return;
        	}
        }
    }

	//prevent the piece from rotating if any of its solid blocks will end up outside the boundaries
	for (i = y_coord; i < y_coord + aux; i++) {
        for (j = x_coord; j < x_coord + aux; j++) {
			if (j >= BOARD_WIDTH) { //TO:DO rotating against the left margin is still allowed
				return;
			}
		}
	}

    // Rotates
    for (i = 0; i < aux; i++) {
    	for (j = 0; j < aux; j++) {
    		board[i + y_coord][j + x_coord] = status[aux-1-j][i];	//Rotates any 4x4 matrix of ints, clockwise

    	}
    }
    for (i = 0; i < aux; i++) {
		for (j = 0; j < aux; j++) {
			if(board[i + y_coord][j + x_coord] != 2)
			status[i][j] = board[i + y_coord][j + x_coord];		//Saves the status, except the static ones.
		}
    }
}

void printMatrix_3by3(int matrix[][3]) { //all 3's in this code are because we're working with a fixed size of 3.
	int i, j;
	for (i = 0; i < 3; i++) {        //also, this is a stub
        for (j = 0; j < 3; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printMatrix_4by4(int matrix[][4]) { //all 4's in this code are because we're working with a fixed size of 4.
    int i, j;
	for (i = 0; i < 4; i++) {        //also, this is a stub
        for (j = 0; j < 4; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printBoard(){ //this is a stub
	int i, j;
    for(i = 0; i < BOARD_HEIGHT; i++) {
        for(j = 0; j < BOARD_WIDTH; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


/*void addPiece(int board[20][10], int nextPieceID) {
    if (nextPieceID == 0) { //ID 0 means the piece is an L
         for(int i = 0; i <= 3; i++) {
            for(int j = 3; j <= 6; j++) {
                int (*piece)[3] = (int(*)[3])(pieces[nextPieceID]);
                board[i][j] = piece[i-3][j];
            }
         }
    }

    else if (nextPieceID == 1) { //ID 1 means the piece is an I
         for(int i = 0; i <= 4; i++) {
            for(int j = 3; j <= 6; j++) {
                int (*piece)[4] = (int(*)[4])(pieces[nextPieceID]);
                board[i][j] = piece[i-3][j];
            }
         }
    }
}*/

void addPiece(int nextPieceID) {
	int i, j, aux;
	aux = PIECE_SIZE(nextPieceID);

	int (*piece)[aux] = (int(*)[aux])(pieces[nextPieceID]); // Cast to matrix
	for (i = 0; i < aux; i++) {
		for (j = 0; j < aux; j++) {
			status[i][j] = 0;
			if(piece[i][j] == 1) {
				status[i][j] = 1;
				if(board[i][j + x_coord] == 2) {
					//alive = false; 		//Enters game over
				}
				else {
					//status[i][j] = piece[i][j];
					board[i][j + x_coord] = piece[i][j]; // Place piece at the top, centered horizontally
				}
			}
		}
	}
}




void shiftPieceDown() {
	int i, j, aux;
	aux = PIECE_SIZE(nextPieceID);

    // Prevent shifting if the piece cannot move further down
    for(i = y_coord + aux-1; i >= y_coord; i--) {
		for(j = x_coord; j <= x_coord + aux-1; j++) {
			if((i >= BOARD_HEIGHT-1  &&  board[i][j] == 1)	||	(board[i + 1][j] == 2  &&  board[i][j] == 1)) {
				collision();
				return;
			}
		}
    }

    // Shifts
    for (i = y_coord + aux-1; i >= y_coord; i--) { // Starts from the bottom row
        for (j = x_coord; j <= x_coord + aux-1; j++) {
        	if(board[i][j] == 1) {
        		board[i + 1][j] = board[i][j];
        	}
        	if(board[i][j] != 2) {
				board[i][j] = 0;
			}
        }
    }
    for (j = x_coord; j < x_coord + aux; j++) {
        board[y_coord][j] = 0;
    }

    y_coord++;
}




void shiftPieceRight() {
	int i, j, aux;
	aux = PIECE_SIZE(nextPieceID);

    // Prevent shifting if the piece cannot move further right
    /*if (x_coord + aux >= BOARD_WIDTH) {
        return;
    }*/
	
    for(i = y_coord + aux-1; i >= y_coord; i--) {
		for(j = x_coord; j <= x_coord + aux-1; j++) {
			if((j >= BOARD_HEIGHT-1  &&  board[i][j] == 1)	||	(board[i][j + 1] == 2  &&  board[i][j] == 1)) {
				return;
			}
			else if (board[i+1][j] == 2 && board[i][j]) { 	//are we resting against a piece? then don't shift sideways.
				return;										//this could honestly be part of the upper "if", but it'd look
			}												//too crowded for my taste.

			else if (board[i][BOARD_WIDTH - 1]) { 			//only stop the piece from shifting if it hit the board limit
				return;
			}
		}
	}

    // Shifts
    for (i = y_coord + aux-1; i >= y_coord; i--) { // Starts from the bottom row, right column
		for (j = x_coord + aux-1; j >= x_coord; j--) {
			if(board[i][j] == 1) {
				board[i][j + 1] = board[i][j];
			}
			if(board[i][j] != 2) {
				board[i][j] = 0;
			}
		}
	}
    for (i = y_coord; i < y_coord + aux; i++) {
		board[i][x_coord] = 0;
	}

    x_coord++;
}




void shiftPieceLeft() {
	int i, j, aux;
	aux = PIECE_SIZE(nextPieceID);

    // Prevent shifting if the piece cannot move further left
    for(i = y_coord + aux-1; i >= y_coord; i--) {
		for(j = x_coord; j <= x_coord + aux-1; j++) {

			if((board[i][j - 1] == 2  &&  board[i][j] == 1)) {
			//for the given coordinates (i,j), checks if part of the piece is against an established block.
				return;
			}
			else if (board[i+1][j] == 2 && board[i][j]) { //are we resting against a piece? then don't shift sideways.
				return;
			}
			else if (board[i][0]) { 			//only stop the piece from shifting if it hit the board limit
				return;
			}

		}
	}

    // Shifts
    for (i = y_coord + aux-1; i >= y_coord; i--) { // Starts from the bottom row, left column
		for (j = x_coord; j <= x_coord + aux-1; j++) {
			board[i][j - 1] = board[i][j];
		}
	}
	for (i = y_coord; i < y_coord + aux; i++) {
		board[i][x_coord + aux-1] = 0;
	}

	 x_coord--;
}


void collision() {
	int i, j, k, flag;
	for(i = y_coord + 2; i >= y_coord; i--) {
		for(j = x_coord; j <= x_coord + 2; j++) {
			if(board[i][j] == 1) {
				board[i][j]++;
			}
		}
	}

	for(i = y_coord + 2; i >= y_coord; i--) {
		for(flag=0, j=0; j < BOARD_WIDTH; j++) {
			if(board[i][j] != 2) {
				flag++;
			}
		}
		if(!flag) {
			for(k = i; k > 0; k--) {
				for(j=0; j < BOARD_WIDTH; j++) {
					board[k][j] = board[k-1][j];
				}
			}
			for(j=0; j < BOARD_WIDTH; j++) {
				board[0][j] = 0;
			}

			i++;
			lines++;
		}
	}
	tetromino[nextPieceID]++;
	score++;
	nextPiece();
}

void collisionTest (void) {
	int i, j, k, flag;
	for(i = y_coord + 2; i >= y_coord; i--) {
		for(j = x_coord; j <= x_coord + 2; j++) {
			if(board[i][j] == 1) {
				board[i][j]++;
			}
		}
	}

	for(i = y_coord + 2; i >= y_coord; i--) {
		for(flag=0, j=0; j < BOARD_WIDTH; j++) {
			if(board[i][j] != 2) {
				flag++;
			}
		}
		if(!flag) {
			for(k = i; k > 0; k--) {
				for(j=0; j < BOARD_WIDTH; j++) {
					board[k][j] = board[k-1][j];
				}
			}
			for(j=0; j < BOARD_WIDTH; j++) {
				board[0][j] = 0;
			}

			i++;
			lines++;
		}
	}
}