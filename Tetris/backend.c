#include "backend.h"
#include "pieces.h"
#include <stdbool.h>

int board[BOARD_HEIGHT][BOARD_WIDTH], nextPieceStatus[4][4];
int PieceID, nextPieceID, score, top, lines, level, mod = 0, tetromino[PIECES_TETRIS];	//TODO sacar mod (testing)
extern bool alive;

#define PIECE_SIZE(x) ((x)!=6	?	((x)!=3 ? 3 : 2)	:	4)	// if x is an I, it's 4x4, if it's an O, its 2x2. Otherwise, it's 3x3

static int x_coord, y_coord;
static void* pieces[PIECES_TETRIS] = {(void*)T, (void*)J, (void*)Z, (void*)O, (void*)S, (void*)L, (void*)I};
//This array holds a pointer to each piece, it was made global as most functions use it in some way.
//The order is not trivial, there's a complementary macro to this array that stores the size of each piece, to make the functions
//accept more than a single size for matrix. We decided this was easier than passing around the size of each piece as an arg. 

static int status[4][4];    //This matrix stores a copy of the piece that will spawn in board, so we can do the rotate logic in it instead
                            //of using the ones defined in pieces.h


//**********************************************
//*           DEBUGGING FUNCTIONS              *
//**********************************************



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

//**********************************************
//*        GAME MECHANICS FUNCTIONS            *
//**********************************************

//initialize the board, 20*10 size
void initBoard() {
	int i, j, aux;
    for (i = 0; i < BOARD_HEIGHT; i++) {
        for (j = 0; j < BOARD_WIDTH; j++) {
            board[i][j] = 0;
        }
    }
    for (i = 0; i < PIECES_TETRIS; i++) {
    	tetromino[i] = 0;
    }
    score = 0;
    lines = 0;
    level = 0;
    leaderBoard("------", 0, 0, 1);

    // Cleans matrix
    for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			nextPieceStatus[i][j] = 0;
		}
	}

    nextPieceID = rand() % PIECES_TETRIS;	//at the start, there will be two different pieces displayed in the same instance.
    aux = PIECE_SIZE(nextPieceID);
    int (*piece)[aux] = (int(*)[aux])(pieces[nextPieceID]); // Cast to matrix

    for (i = 0; i < aux; i++) {
		for (j = 0; j < aux; j++) {

			if(piece[i][j] == 1) {
				nextPieceStatus[i][j] = 1;	//only copies the active states
			}
		}
    }

    nextPiece();	//selects the first piece to play. it's placed in the matrix "board".
}

void nextPiece() {
	x_coord = (int)BOARD_WIDTH/2 - 2;
	y_coord = 0;

	PieceID = nextPieceID;		//the next piece is placed in board.

    if(mod) { //TODO volar esto. pd lety dejá las drogas || mod is a variable used only for testing purposes, kinda like a cheat code. don't tell anyone!
        nextPieceID = 6;
    }
    else {
        nextPieceID = rand() % PIECES_TETRIS;	 //generates a random number between 0 and 1 less than the defined pieces
    }
    addPiece();

    mod = 0;	//TODO sacar esto
    //printf("Score: %d\n", score);
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

void addPiece() {
	int i, j, aux;
	aux = PIECE_SIZE(PieceID);	//selects size of piece

	for (i = 0; i < aux; i++) {
		for (j = 0; j < aux; j++) {

			status[i][j] = 0;	//cleans matrix
			if(nextPieceStatus[i][j] == 1) {
				status[i][j] = 1;	//only copies the active state of the next piece

				if(board[i][j + x_coord] > 1) { //if the board has a static piece where the game is trying to put a new piece, 
					alive = false;				//it ends the game.
					gameOver();	//TODO sacar esto
					return;
				}
				else {
					//status[i][j] = piece[i][j];
					board[i][j + x_coord] = status[i][j]; // Place piece at the top, centered horizontally
				}
			}
		}
	}

	aux = PIECE_SIZE(nextPieceID);	//selects size of next piece
	int (*piece)[aux] = (int(*)[aux])(pieces[nextPieceID]); // Cast to matrix

	//cleans the matrix status
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			nextPieceStatus[i][j] = 0;
		}
	}
	for (i = 0; i < aux; i++) {
		for (j = 0; j < aux; j++) {

			if(piece[i][j] == 1) {
				nextPieceStatus[i][j] = 1;	//only copies the active states.
			}
		}
	}
}


//this function handles all the logic needed before every cycle starts with a new piece. 
//it's called when, you guessed it, a piece collides with something.
void collision() {

	int i, j, k;

    //Cell upgrading loop

    for (i = 0; i < BOARD_HEIGHT; i++) {        //check the whole matrix for 1's and upgrade them to 2's every cycle.
        for (j = 0; j < BOARD_WIDTH; j++) {
            if(board[i][j] == 1) {
				board[i][j] += PieceID+1;		// Allows to differentiate the pieces, for colouring in front-pc.
			}
        }
    }

    //Lane clearing logic

    int currentLines, blocksPerRow;
    for(i = BOARD_HEIGHT - 1, currentLines = 0; i >= 0; i--) {
        for(j = 0, blocksPerRow = 0; j < BOARD_WIDTH; j++) { //check all the board for complete rows. Not the most efficient, but the safest.
            
            if(board[i][j] > 1) {
                blocksPerRow++;
            }
        }
        if(blocksPerRow == BOARD_WIDTH) { //check if we have a whole row. if so,
            currentLines++; //increase the line counter for this event (used later for points counting)
            
            for(k = i; k > 0; k--) {
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
	

    tetromino[PieceID]++;
	//score++;
	nextPiece(mod);
}


void gameOver() {
    printf("Womp womp\nYour score was %d\n", score);
    printf("level reached: %d\n", lines);
    
}

void leaderBoard(char inputName[6], unsigned long int inputScore, unsigned int inputLevel, int firstGame) {
    if(firstGame) {
        struct player {
        char name[6];
        unsigned long int score;
        unsigned int level;
        };
        struct player leaderBoard[3];
    }
    
}




//**********************************************
//*            MOVEMENT FUNCTIONS              *
//**********************************************


// Rotate function
void rotateClockwise() {
    int i, j, aux;
    aux = PIECE_SIZE(PieceID);
    
    // OLD Prevent rotation if there's an obstacle
    /*for (i = 0; i < aux; i++) {
        for (j = 0; j < aux; j++) {

            //Testing
            printf("Coords being checked are (%d; %d)\n", (j + x_coord), (i + y_coord));
            if((board[i + y_coord][j + x_coord] == 1)) printf("First statement holds true\n");
            if(board[aux - 1 - j + x_coord][i + y_coord] == 2) printf("Second statement holds true\n");
            //End testing

        	if((board[i + y_coord][j + x_coord] == 1) && (board[aux - 1 - (j + x_coord)][i + y_coord] == 2)) {
				//DO NOT touch, took me an hour to get those statements right.
				//for every active cell in play, check if the coordinate where it should land is already filled by an old piece.
				//if so, you can't rotate here.

        		printf("got here!\n");  //this is a testament to how desperate i got. 
				return;
        	}
        }
    }*/


	//Prevent rotation if there's an obstacle (math before was wrong but i'm too attached to that code to delete it)
	for (i = y_coord; i < y_coord + aux; i++) {
		for (j = x_coord; j < x_coord + aux; j++) {
			if(board[i][j] == 1) {
				int X = j - x_coord;
				int Y = i - y_coord;

				//if(board[(aux - (j - x_coord) + y_coord)][(aux - 1 - (i-y_coord) + x_coord)] > 1) { DEPRICATED
				if (board[y_coord + X][x_coord + (aux - 1 - Y)] > 1) {
					//for every active cell in play, check if the coordinate where it should land is already filled by an old piece.
					//if so, you can't rotate here. The double if instead of an && is only here for readability
					return;
				}
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
    //alternativeRotation();
    for (i = 0; i < aux; i++) {
    	for (j = 0; j < aux; j++) {
    		if (board[i + y_coord][j + x_coord] <= 1) {    
                board[i + y_coord][j + x_coord] = status[aux-1-j][i];	//Rotates any matrix of ints, clockwise
            }
    	}
    }
    for (i = 0; i < aux; i++) {
		for (j = 0; j < aux; j++) {
			if(board[i + y_coord][j + x_coord] < 2) {
				status[i][j] = board[i + y_coord][j + x_coord];		//Saves the status, except the static ones.
			}
		}
    }
}

/*alternativeRotation() {
    int aux = PIECE_SIZE(nextPieceID);
    for(int i = y_coord; i < y_coord + aux; i++) {
        for (int j = x_coord; j < x_coord + aux; j++) {
            if(board[i][j] == 1) {
                
                board[(aux - (j - x_coord) + y_coord)][(aux - 1 - (i-y_coord) + x_coord)] = 1;
                board[i][j] = 0;
            }
        }
    }
}*/


int shiftPieceDown(int keyPressed) {	//Returns 1 if there's collision, 0 if not. (to display sound)
	int i, j, aux;
	aux = PIECE_SIZE(PieceID);

    // Prevent shifting if the piece cannot move further down
    for (i = y_coord + aux - 1; i >= y_coord; i--) {
        for (j = x_coord; j <= x_coord + aux - 1; j++) {
            //only check if the i,j coords are inside the board area
            if ((i < BOARD_HEIGHT && i >= 0) && (j < BOARD_WIDTH && j >= 0)) {
            // Only check cells that are part of the piece
            if (board[i][j] == 1) {

                // Check if the cell is at the bottom or above a stationary block
                if (i >= BOARD_HEIGHT - 1 || board[i + 1][j] > 1) {
                    printf("piece collided, pieceID was %d, i,j coords were (%d, %d)\n", nextPieceID, i, j);
                    collision();
                    return 1;
                }
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
        	if(board[i][j] == 1) {
				board[i][j] = 0;
			}
        }
    }
    for (j = x_coord; j < x_coord + aux; j++) {
        if (board[y_coord][j] == 1) {
			board[y_coord][j] = 0;
		}			
    }

    y_coord++;

    if(keyPressed) {
        score++;
    }

    return 0;
}

void shiftPieceRight() {
    int i, j, aux;
    aux = PIECE_SIZE(PieceID);

    // Prevent shifting if the piece cannot move further right
    for (i = y_coord + aux - 1; i >= y_coord; i--) {
        for (j = x_coord + aux - 1; j >= x_coord; j--) {
			if (board[i][j] == 1) {
	            if (board[i][j + 1] > 1 || j + 1 >= BOARD_WIDTH) {
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
    aux = PIECE_SIZE(PieceID);

    // Prevent shifting if the piece cannot move further left
    for (i = y_coord + aux - 1; i >= y_coord; i--) {
        for (j = x_coord; j < x_coord + aux; j++) {
            if (board[i][j] == 1) {
                if (j - 1 < 0 || board[i][j - 1] > 1) {
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
