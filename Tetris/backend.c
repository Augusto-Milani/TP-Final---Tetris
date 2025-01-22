#include "pieces.h"
#include <stdio.h>
#include <stdlib.h> //TEMP, JUST A STUB
#include <time.h> //TEMP, JUST A STUB

#define BOARD_HEIGHT 4
#define BOARD_WIDTH 4

#if BOARD_HEIGHT<4 || BOARD_WIDTH<4
	#error Error: The board is very small.
#endif

void nextPiece();

void rotateCW_3by3(int[][3]);
void rotateCW_4by4(int[][4]);

void printMatrix_3by3(int[][3]);
void printMatrix_4by4(int[][4]);
void printBoard();

void addPiece(int);
void shiftPieceDown_3x3();
void shiftPieceLeft_3x3();
void shiftPieceRight_3x3();
void collision();

void initBoard();
static int board[BOARD_HEIGHT][BOARD_WIDTH];
static int x_coord, y_coord;
void* pieces[] = {(void*)L, (void*)J, (void*)S, (void*)Z, (void*)T, (void*)O, (void*)I}; //This array holds pointers to all pieces in game, it has to be global for ease of coding
static int nextPieceID;

int main() {
    srand(time(NULL)); //set random seed

    initBoard();

    printBoard();
    nextPiece();
    printBoard();

    rotateCW_4by4(pieces[nextPieceID]);
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

    nextPieceID = 6;//rand() % 7;	 //rand() % 7 	// para que sea entre 0 y 6;
    

    if(nextPieceID < 5) {
        printMatrix_3by3(pieces[nextPieceID]);
    }
    else {
        printMatrix_4by4(pieces[nextPieceID]);
    }
    addPiece(nextPieceID);
}


void rotateCW_3by3(int matrix[][3]) { //all 3's in this code are because we're working with a fixed size of 3.
    int i, j;
    
    // Prevent rotation if there's an obstacle
    for (i = y_coord; i < y_coord + 3; i++) {
        for (j = x_coord; j < x_coord + 3; j++) {
        	if(board[i][j] == 2 && board[2-j][i] == 1) {	// Note: 2 is static, 1 isn't
        		return;
        	}
        }
    }

    // Rotates
    for (i = 0; i < 3; i++) {
    	for (j = 0; j < 3; j++) {
    		board[i + y_coord][j + x_coord] = matrix[2-j][i];	// 	Note: the expression rotates matrix in 90° clockwise
    																//	Also, matrix" starts from 0,0
    	}
    }

}


void rotateCW_4by4(int matrix[][4]) { //all 4's in this code are because we're working with a fixed size of 4.
    int i, j;
    
    // Prevent rotation if there's an obstacle
    for (i = y_coord; i < y_coord + 4; i++) {
        for (j = x_coord; j < x_coord + 4; j++) {
        	if(board[i][j] == 2 && board[3-j][i] == 1) {	// Note: 2 is static, 1 isn't
        		return;
        	}
        }
    }

    // Rotates
    for (i = 0; i < 4; i++) {
    	for (j = 0; j < 4; j++) {
    		board[i + y_coord][j + x_coord] = matrix[3-j][i];	// 	Note: the expression rotates matrix in 90° clockwise
    																//	Also, matrix" starts from 0,0
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
	int i, j;
    if (nextPieceID < 5) { // ID 0 means the piece is an L (3x3)
        int (*piece)[3] = (int(*)[3])(pieces[nextPieceID]); // Cast to 3x3 matrix
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
            	if(piece[i][j] == 1) {
            		if(board[i][j + x_coord] == 2) {
            			//TODO alive = false; (variable de frontend, global) enters game over
            		}
            		else {
            			board[i][j + x_coord] = piece[i][j]; // Place piece at the top, centered horizontally
            		}
            	}
            }
        }
    }
    else {	// ID 5 and 6 means the piece is an I or O(4x4)
        int (*piece)[4] = (int(*)[4])(pieces[nextPieceID]); // Cast to 4x4 matrix
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
            	if(piece[i][j] == 1) {
            		board[i][j + x_coord] = piece[i][j]; // Place piece at the top, centered horizontally
            	}
            }
        }
    }
}




void shiftPieceDown_3x3() {
	int i, j;

    // Prevent shifting if the piece cannot move further down
	if (y_coord + 2 >= BOARD_HEIGHT-1) { //7+2 > 9
		collision();
        return;
    }
    for(i = y_coord + 2; i >= y_coord; i--) {
		for(j = x_coord; j <= x_coord + 2; j++) {
			if(board[i + 1][j] == 2  &&  board[i][j] == 1) {
				collision();
				return;
			}
		}
    }

    // Shifts
    for (i = y_coord + 2; i >= y_coord; i--) { // Starts from the bottom row
        for (j = x_coord; j <= x_coord + 2; j++) {
        	if(board[i][j] == 1) {
        		board[i + 1][j] = board[i][j];
        	}
        	if(board[i][j] != 2) {
        		board[i][j] = 0;
        	}
        }
    }
    for (j = x_coord; j < x_coord + 3; j++) {
        board[y_coord][j] = 0;
    }

    y_coord++;
}




void shiftPieceRight_3x3() {
	int i, j;

    // Prevent shifting if the piece cannot move further right
    if (x_coord + 2 >= BOARD_WIDTH-1) {
        return;
    }
    for(i = y_coord + 2; i >= y_coord; i--) {
		for(j = x_coord; j <= x_coord + 2; j++) {
			if(board[i][j + 1] == 2  &&  board[i][j] == 1) {
				return;
			}
		}
	}


    for (i = y_coord + 2; i >= y_coord; i--) { // Starts from the bottom row, right column
		for (j = x_coord + 3; j > x_coord; j--) {
			board[i][j] = board[i][j - 1];
		}
	}

    for (i = y_coord; i < y_coord + 3; i++) {
		board[i][x_coord] = 0;
	}

    x_coord++;
}




void shiftPieceLeft_3x3() {
	int i, j;

    // Prevent shifting if the piece cannot move further left
    if (x_coord == 0) {
        return;
    }
    for(i = y_coord + 2; i >= y_coord; i--) {
		for(j = x_coord; j <= x_coord + 2; j++) {
			if(board[i][j - 1] == 2  &&  board[i][j] == 1) {
				return;
			}
		}
	}

    for (i = y_coord + 2; i >= y_coord; i--) { // Starts from the bottom row, left column
		for (j = x_coord; j < x_coord + 3; j++) {
			board[i][j - 1] = board[i][j];
		}
	}

	for (i = y_coord; i < y_coord + 3; i++) {
		board[i][x_coord + 2] = 0;
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

