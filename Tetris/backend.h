#ifndef TETRIS_BACKEND_H_
#define TETRIS_BACKEND_H_

#include <stdio.h>
#include <stdlib.h> //TEMP, JUST A STUB	TODO
//#include <time.h> //TEMP, JUST A STUB
#include <stdbool.h>

#if BOARD_HEIGHT<4 || BOARD_WIDTH<4
	#error Error: The board is very small.
#endif

#define PIECES_TETRIS 7		// Number of diferent pieces in Tetris

//Global variables
extern int board[BOARD_HEIGHT][BOARD_WIDTH], nextPieceStatus[4][4];
extern int PieceID, nextPieceID, score, lines, level, mod, tetromino[PIECES_TETRIS];
extern bool alive;

void rotateClockwise();

int shiftPieceDown(int);
void shiftPieceLeft();
void shiftPieceRight();
void initBoard();

#endif /* TETRIS_BACKEND_H_ */
