#ifndef TETRIS_BACKEND_H_
#define TETRIS_BACKEND_H_

#include <stdio.h>
#include <stdlib.h> //TEMP, JUST A STUB
#include <time.h> //TEMP, JUST A STUB

#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#if BOARD_HEIGHT<4 || BOARD_WIDTH<4
	#error Error: The board is very small.
#endif

void nextPiece();

void rotateClockWise();

void printMatrix_3by3(int[][3]);
void printMatrix_4by4(int[][4]);
void printBoard();

void addPiece(int);
void shiftPieceDown();
void shiftPieceLeft();
void shiftPieceRight();
void collision();

void initBoard();


#endif /* TETRIS_BACKEND_H_ */
