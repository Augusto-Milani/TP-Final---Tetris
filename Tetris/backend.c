#include "backend.h"
#include "pieces.h"

#define PIECES_TETRIS 7
#define PIECE_3x3_AUX 5
#define PIECE_SIZE(x) ((x)!=6	?	((x)!=3 ? 3 : 2)	:	4)	// if x is an I, it's 4x4, if it's an O, its 2x2. Otherwise, it's 3x3

int board[BOARD_HEIGHT][BOARD_WIDTH];

int score, top, lines, level = 0, mod = 0;
int tetromino[7];


static int x_coord, y_coord;
void* pieces[PIECES_TETRIS] = {(void*)T, (void*)J, (void*)Z, (void*)O, (void*)S, (void*)L, (void*)I}; 
//This array holds a pointer to each piece, it was made global as most functions use it in some way.
//The order is not trivial, there's a complementary macro to this array that stores the size of each piece, to make the functions
//accept more than a single size for matrix. We decided this was easier than passing around the size of each piece as an arg. 

static int status[4][4];
static int nextPieceID;

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

    if(mod) { //mod is a variable used only for testing purposes, kinda like a cheat code. don't tell anyone! 
        nextPieceID = 6;
    }
    else {
        nextPieceID = rand() % PIECES_TETRIS;	 //generates a random number between 0 and 1 less than the defined pieces
    }
    addPiece(nextPieceID);

    mod = 0;
    printf("Score: %d\n", score);
}



// Rotate function
void rotateClockwise() {
    int i, j, aux;
    aux = PIECE_SIZE(nextPieceID);
    
    // Prevent rotation if there's an obstacle
    for (i = 0; i < aux; i++) {
        for (j = 0; j < aux; j++) {
        	if((board[i + y_coord][j + x_coord] == 1) && (board[i + y_coord][aux - 1 - j + x_coord] == 2)) {
				//DO NOT touch, took me an hour to get those statements right.
				//for every active cell in play, check if the coordinate where it should land is already filled by an old piece.
				//if so, you can't rotate here.

        		//printf("got here!\n");  this is a testament to how desperate i got. 
				return;
        	}
        }
    }

	//prevent the piece from rotating if any of its solid blocks will end up outside the boundaries
	for (i = y_coord; i < y_coord + aux; i++) {
        for (j = x_coord; j < x_coord + aux; j++) {
			if (board[i][j] == 1 &&  ((aux - 1 - (i-y_coord) + x_coord) < 0 || (aux - 1 - (i-y_coord) + x_coord) >= BOARD_WIDTH)) { 
				/* This statement deserves an explanation. for every moving block, check IF said block would fall outside the
				boundaries of the board if it were rotated. (i-y_coord) gets the position of the block relative to the top left
				corner. (aux - 1 - (i-y_coord)) is where the piece should land in the X axis, relative to the top left corner.
				by adding x_coord, we convert to absolute coordinates. With that done, we can directly check if this value falls
				outside of the allowed range of [0; BOARD_WIDTH).*/
				return;
			}
		}
	}

    // Rotates
    for (i = 0; i < aux; i++) {
    	for (j = 0; j < aux; j++) {
    		board[i + y_coord][j + x_coord] = status[aux-1-j][i];	//Rotates any matrix of ints, clockwise

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
					gameOver();
                    return;
				}
				else {
					//status[i][j] = piece[i][j];
					board[i][j + x_coord] = piece[i][j]; // Place piece at the top, centered horizontally
				}
			}
		}
	}
}




void shiftPieceDown(int keyPressed) {
	int i, j, aux;
	aux = PIECE_SIZE(nextPieceID);

    // Prevent shifting if the piece cannot move further down
    for (i = y_coord + aux - 1; i >= y_coord; i--) {
        for (j = x_coord; j <= x_coord + aux - 1; j++) {
            // Only check cells that are part of the piece
            if (board[i][j] == 1) {

                // Check if the cell is at the bottom or above a stationary block
                if (i >= BOARD_HEIGHT - 1 || board[i + 1][j] == 2) {                   
                    collision();
                    return;
                }
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
        if (board[y_coord][j] != 2) {
			board[y_coord][j] = 0;
		}			
    }

    y_coord++;

    if(keyPressed) {
        score++;
    }
}

void shiftPieceRight() {
    int i, j, aux;
    aux = PIECE_SIZE(nextPieceID);

    // Prevent shifting if the piece cannot move further right
    for (i = y_coord + aux - 1; i >= y_coord; i--) {
        for (j = x_coord + aux - 1; j >= x_coord; j--) {
			if (board[i][j] == 1) {
	            if (board[i][j + 1] == 2 || j + 1 >= BOARD_WIDTH) { 
					//check if active cells can move right without hitting the edge or another piece
    	            return; 
				}
            }
        }
    }

    // Shift the piece right
    for (i = y_coord + aux - 1; i >= y_coord; i--) {
        for (j = x_coord + aux - 1; j >= x_coord; j--) {
            if (board[i][j] == 1) { // Only move if it's part of the active piece
                board[i][j + 1] = board[i][j]; //Shift piece to the right
                board[i][j] = 0; // Clear original position
            }
        }
    }

    x_coord++;
}

void shiftPieceLeft() {
    int i, j, aux;
    aux = PIECE_SIZE(nextPieceID);

    // Prevent shifting if the piece cannot move further left
    for (i = y_coord + aux - 1; i >= y_coord; i--) {
        for (j = x_coord; j < x_coord + aux; j++) {
            if (board[i][j] == 1) {
                if (j - 1 < 0 || board[i][j - 1] == 2) { 
					//check if active cells can move left without hitting the edge or another piece
                    return;
                }
            }
		}
    }

    // Shift the piece left
    for (i = y_coord + aux - 1; i >= y_coord; i--) {
        for (j = x_coord; j < x_coord + aux; j++) {
            if (board[i][j] == 1) { // Only move if it's part of the active piece
                board[i][j - 1] = board[i][j]; // Shift piece to the left
                board[i][j] = 0; // Clear old position
            }
        }
    }

    x_coord--;
}


void collision() {
    printf("%d\n", board[19][0]);
	int i, j, k, flag;

    //Cell upgrading loop

    for (i = 0; i < BOARD_HEIGHT; i++) {        //check the whole matrix for 1's and upgrade them to 2's every cycle.
        for (j = 0; j < BOARD_WIDTH; j++) {
            if(board[i][j] == 1) {
				board[i][j]++;
			}
        }
    }

    //Lane clearing logic

    int currentLines = 0;
    for(int i = BOARD_HEIGHT - 1; i >= 0; i--) {
        int blocksPerRow = 0; 
        int j = 0;
        for(j; j < BOARD_WIDTH; j++) { //check all the board for complete rows. Not the most efficient, but the safest.
            
            if(board[i][j] == 2) {
                blocksPerRow++;
            }
        }
        if(blocksPerRow == BOARD_WIDTH) { //check if we have a whole row. if so,
            currentLines++; //increase the line counter for this event (used later for points counting)
            
            for(int k = i; k > 0; k--) {
                for(j = 0; j < BOARD_WIDTH; j++) {  //run upwards through every row of the board, starting from the full row,
                    
                    board[k][j] = board[k-1][j];    //and copy the line above into the current line.
                    
                }
            }
            for(j = 0; j < BOARD_WIDTH; j++) {
                board[0][j] = 0; //we excluded row 0 before so as not to read nonexistent rows, now we clear row 0 separately.
            }
        i++;
        }
    }
    
    //Point math for lane clearing
    printf("Current lines: %d\n", currentLines);
    switch(currentLines) {
        case 1:
            score += (40 * (level+1));
            break;
        case 2:
            score += (100 * (level+1));
            break;
        case 3:
            score += (300 * (level+1));
            break;
        case 4:
            score += (1200 * (level+1));
            break;
        default:
            break;
    }
	
    lines += currentLines;
    level = (lines / 10);
	

    tetromino[nextPieceID]++;
	//score++;
	nextPiece(mod);
}


void gameOver(void) {
    printf("Womp womp\nYour score was %d\n", score);
    printf("level reached: %d\n", lines);
}

