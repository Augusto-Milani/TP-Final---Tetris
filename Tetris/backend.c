#include "pieces.h"
#include <stdio.h>
#include <stdlib.h> //TEMP, JUST A STUB
#include <time.h> //TEMP, JUST A STUB

#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10

void* pieces[] = {(void*)L, (void*)I}; //This array holds pointers to all pieces in game, it has to be global for ease of coding

void nextPiece();

void rotateCW_3by3(int[][3]);
void rotateCW_4by4(int[][4]);

void printMatrix_3by3(int[][3]);
void printMatrix_4by4(int[][4]);
void printBoard();

void addPiece(int);
void shiftPieceDown_3x3(int, int);
void shiftPieceLeft_3x3(int, int);
void shiftPieceRight_3x3(int, int);

void initBoard();
static int board[BOARD_HEIGHT][BOARD_WIDTH];

int main() {
    srand(time(NULL)); //set random seed

    initBoard();

    printBoard();
    nextPiece();
    printBoard();

    int i;
/*
    for(i=0 ; i<BOARD_HEIGHT ; i++) {
		shiftPieceDown_3x3(3, i);
		printBoard();
    }
*/


    for(i=3 ; i<BOARD_WIDTH ; i++) {
		shiftPieceRight_3x3(i, 0);
		printBoard();
	}


/*
    for(i=3 ; i>=0 ; i--) {
		shiftPieceLeft_3x3(i, 0);
		printBoard();
	}
*/


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
    int nextPieceID = 0; //(rand() % 2);
    

    if(nextPieceID == 0) {
        printMatrix_3by3(pieces[nextPieceID]);
    }
    else if(nextPieceID == 1) {
        printMatrix_4by4(pieces[nextPieceID]);
    }
    addPiece(nextPieceID);
}


void rotateCW_3by3(int matrix[][3]) { //all 3's in this code are because we're working with a fixed size of 3.
    int temporaryMatrix[3][3];
    int i, j;
    
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            temporaryMatrix[i][j] = matrix[i][j]; 
        }
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            matrix[i][j] = temporaryMatrix[(2 - j)][i]; //(2-j) remaps the j-coord from (0 -> 2) to (2 -> 0)
        }
    }
}


void rotateCW_4by4(int matrix[][4]) { //all 4's in this code are because we're working with a fixed size of 4.
    int temporaryMatrix[4][4];
    int i, j;
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            temporaryMatrix[i][j] = matrix[i][j]; 
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrix[i][j] = temporaryMatrix[(3 - j)][i]; //(3-j) remaps the j-coord from (0 -> 3) to (3 -> 0)
        }
    }
}

void printMatrix_3by3(int matrix[][3]) { //all 3's in this code are because we're working with a fixed size of 4.
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
    if (nextPieceID == 0) { // ID 0 means the piece is an L (3x3)
        int (*piece)[3] = (int(*)[3])(pieces[nextPieceID]); // Cast to 3x3 matrix
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                board[i][j + 3] = piece[i][j]; // Place piece at the top, centered horizontally
            }
        }
    } else if (nextPieceID == 1) { // ID 1 means the piece is an I (4x4)
        int (*piece)[4] = (int(*)[4])(pieces[nextPieceID]); // Cast to 4x4 matrix
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                board[i][j + 3] = piece[i][j]; // Place piece at the top, centered horizontally
            }
        }
    }
}




void shiftPieceDown_3x3(int x_coord, int y_coord) {
    // Prevent shifting if the piece cannot move further down
    if (y_coord + 3 > BOARD_HEIGHT) {
        return;
    }

    int i, j;
    for (i = y_coord + 2; i >= y_coord; i--) { // Starts from the bottom row
        for (j = x_coord; j < x_coord + 3; j++) {
            board[i + 1][j] = board[i][j];
        }
    }

    for (j = x_coord; j < x_coord + 3; j++) {
        board[y_coord][j] = 0;
    }
}




void shiftPieceRight_3x3(int x_coord, int y_coord) {
    // Prevent shifting if the piece cannot move further right
	printf("%d\n",x_coord);
    if (x_coord + 3 >= BOARD_WIDTH) {
        return;
    }

    int i, j;
    for (i = y_coord + 2; i >= y_coord; i--) { // Starts from the bottom row, right column
		for (j = x_coord + 3; j > x_coord; j--) {
			board[i][j] = board[i][j - 1];
		}
	}

    for (i = y_coord; i < y_coord + 3; i++) {
		board[i][x_coord] = 0;
	}
}




void shiftPieceLeft_3x3(int x_coord, int y_coord) {
    // Prevent shifting if the piece cannot move further left
	printf("%d\n",x_coord);
    if (x_coord == 0) {
        return;
    }

    int i, j;

    for (i = y_coord + 2; i >= y_coord; i--) { // Starts from the bottom row, left column
		for (j = x_coord; j < x_coord + 3; j++) {
			board[i][j - 1] = board[i][j];
		}
	}

	for (i = y_coord; i < y_coord + 3; i++) {
		board[i][x_coord + 2] = 0;
	}
}			//TODO NOTA IMPORTANTE: REVISAR CASO LÍMITE EXTREMO DERECHO, IDEM shiftPieceRight (que no copie afuera de matriz)


void checkCollision(void) {

}
