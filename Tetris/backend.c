#include "pieces.h"
#include <stdio.h>
#include <stdlib.h> //TEMP, JUST A STUB
#include <time.h> //TEMP, JUST A STUB

#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10

#if BOARD_HEIGHT<4 || BOARD_WIDTH<4
	#error Error: The board is very small.
#endif

void nextPiece();

void rotateClockWise(int[][4]);

void printMatrix_3by3(int[][3]);
void printMatrix_4by4(int[][4]);
void printBoard();

void addPiece(int);
void shiftPieceDown();
void shiftPieceLeft();
void shiftPieceRight();
void collision();

void initBoard();
static int board[BOARD_HEIGHT][BOARD_WIDTH];
static int x_coord, y_coord;
void* pieces[] = {(void*)L, (void*)J, (void*)S, (void*)Z, (void*)T, (void*)O, (void*)I}; //This array holds pointers to all pieces in game, it has to be global for ease of coding
static int status[4][4];
static int nextPieceID;

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

/*
    printMatrix_4by4(I);
    printMatrix_3by3(L);
    rotateCW_3by3(L);
    rotateCW_4by4(I);
    rotateCW_4by4(I);
    printMatrix_3by3(L);
    printMatrix_4by4(I);
*/

}

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
	x_coord = 0; 	//(int)BOARD_WIDTH/2 - 2;
	y_coord = 0;

    nextPieceID = rand() % 7;	 //rand() % 7 	// para que sea entre 0 y 6;
    

    if(nextPieceID < 5) {
        printMatrix_3by3(pieces[nextPieceID]);
    }
    else {
        printMatrix_4by4(pieces[nextPieceID]);
    }
    addPiece(nextPieceID);


}



// Rotate function
void rotateClockWise(int matrix[][4]) {
    int i, j, aux;
    aux = (nextPieceID < 5) ? 3 : 4;
    
    // Prevent rotation if there's an obstacle
    for (i = y_coord; i < y_coord + aux; i++) {
        for (j = x_coord; j < x_coord + aux; j++) {
        	if(board[i][j] == 2 && board[aux-1-j][i] == 1) {	// Note: 2 is static, 1 isn't
        		return;
        	}
        }
    }

    // Rotates
    for (i = 0; i < aux; i++) {
    	for (j = 0; j < aux; j++) {
    		board[i + y_coord][j + x_coord] = matrix[aux-1-j][i];	//Rotates any 4x4 matrix of ints, clockwise

    	}
    }
    for (i = 0; i < aux; i++) {
		for (j = 0; j < aux; j++) {
			if(board[i + y_coord][j + x_coord] != 2)
			matrix[i][j] = board[i + y_coord][j + x_coord];		//Saves the status, except the static ones.
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
	aux = (nextPieceID < 5) ? 3 : 4;

	int (*piece)[aux] = (int(*)[aux])(pieces[nextPieceID]); // Cast to 3x3 matrix
	for (i = 0; i < aux; i++) {
		for (j = 0; j < aux; j++) {
			if(piece[i][j] == 1) {
				if(board[i][j + x_coord] == 2) {
					//TODO alive = false; (variable de frontend, global) enters game over
				}
				else {
					status[i][j] = piece[i][j];
					board[i][j + x_coord] = piece[i][j]; // Place piece at the top, centered horizontally
				}
			}
		}
	}
}




void shiftPieceDown() {
	int i, j, aux;
	aux = (nextPieceID < 5) ? 3 : 4;

    // Prevent shifting if the piece cannot move further down
	if (y_coord + aux >= BOARD_HEIGHT) {
		collision();
        return;
    }
    for(i = y_coord + aux-1; i >= y_coord; i--) {
		for(j = x_coord; j <= x_coord + aux-1; j++) {
			if(board[i + 1][j] == 2  &&  board[i][j] == 1) {
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
	aux = (nextPieceID < 5) ? 3 : 4;

    // Prevent shifting if the piece cannot move further right
    if (x_coord + aux >= BOARD_WIDTH) {
        return;
    }
    for(i = y_coord + aux-1; i >= y_coord; i--) {
		for(j = x_coord; j <= x_coord + aux-1; j++) {
			if(board[i][j + 1] == 2  &&  board[i][j] == 1) {
				return;
			}
		}
	}

    // Shifts
    for (i = y_coord + aux-1; i >= y_coord; i--) { // Starts from the bottom row, right column
		for (j = x_coord + aux-1; j >= x_coord; j--) {
			board[i][j+1] = board[i][j];
		}
	}
    for (i = y_coord; i < y_coord + aux; i++) {
		board[i][x_coord] = 0;
	}

    x_coord++;
}




void shiftPieceLeft() {
	int i, j, aux;
	aux = (nextPieceID < 5) ? 3 : 4;

    // Prevent shifting if the piece cannot move further left
    if (x_coord == 0) {
        return;
    }
    for(i = y_coord + aux-1; i >= y_coord; i--) {
		for(j = x_coord; j <= x_coord + aux-1; j++) {
			if(board[i][j - 1] == 2  &&  board[i][j] == 1) {
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

	 x_coord++;
}			//TODO NOTA IMPORTANTE: REVISAR CASO LÍMITE EXTREMO DERECHO, IDEM shiftPieceRight (que no copie afuera de matriz)


void collision() {
	printf("hola\n");
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
			//TODO score y lineas aumenta
		}
	}
	nextPiece();
}

