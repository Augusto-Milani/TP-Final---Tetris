#ifndef TETRIS_BACKEND_H_
#define TETRIS_BACKEND_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#if BOARD_HEIGHT<4 || BOARD_WIDTH<4
	#error Error: The board is very small.
#endif

#define PIECES_TETRIS 7		// Number of diferent pieces in Tetris

//Global variables
extern unsigned int board[BOARD_HEIGHT][BOARD_WIDTH], nextPieceStatus[4][4];
extern unsigned int PieceID, nextPieceID, score, lines, level, mod, tetromino[PIECES_TETRIS];
extern bool alive;

void rotateClockwise();

bool shiftPieceDown(const bool keyPressed);
void shiftPieceLeft();
void shiftPieceRight();
void initBoard();

#endif /* TETRIS_BACKEND_H_ */
