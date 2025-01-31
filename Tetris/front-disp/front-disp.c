/*
 * front-disp.c
 *
 *  Created on: 23 ene 2025
 *
 *      Author: mariano
 */




#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "Libs/disdrv.h"
#include "Libs/joydrv.h"
#include "letras.h"
#include "../backend.h"
#define MAX_HEIGHT 16
#define MAX_WIDTH 16
#define SEC 200000
#define FALL_TIME 1.0//tiempo que espera para caer en sec

enum {PLAY=1,TOP,STOP,CONT};


static char menu (void);
static void play (void);
static void playini (void);
static short tag (void);
static void print_level (int);
static void board_redraw (void);
static void pausa (char *);

extern int level,score;
extern int board[BOARD_HEIGHT][BOARD_WIDTH];


int main (void)
{
	char state;
	disp_init();
	joy_init();
	while((state=menu())!=STOP)
	{
		switch(state)
		{
		case PLAY:
			play();
			break;
		case TOP:
			/* funcion que muestra top*/
			break;
		}
	}

	return 0;
}


static char menu (void)
{
    char state=PLAY;
	disp_clear();
	disp_update();
	dcoord_t coords={1,4};
	joyinfo_t info;
	letras_on (coords,'P');
	coords.x+=4;
	letras_on (coords,'L');
	coords.x+=4;
	letras_on (coords,'A');
	coords.x+=4;
	letras_on (coords,'Y');
	coords.x=1;
	coords.y+=6;
	letras_on (coords,'T');
	coords.x+=4;
	letras_on (coords,'O');
	coords.x+=4;
	letras_on (coords,'P');
	coords.x=1;
	coords.y+=5;
	letras_on (coords,'S');
	coords.x+=4;
	letras_on (coords,'T');
	coords.x+=4;
	letras_on (coords,'O');
	coords.x+=4;
	letras_on (coords,'P');
	coords.x=0;
	do
	{
		info = joy_read();
		switch(state)
		{
		case PLAY:
			coords.y=2;
			break;
		case TOP:
			coords.y=8;
			break;
		case STOP:
			coords.y=13;
			break;
		}
		disp_write(coords,D_ON);
		disp_update();
		usleep(200000);
		disp_write(coords,D_OFF);
		disp_update();
		usleep(200000);
		if(state!=PLAY && info.y>50)
		{
		  state--;
		}
		else if(state!=STOP && info.y<-50)
		{
		  state++;
		}
	}while(info.sw == J_NOPRESS);
	disp_clear();
	disp_update();
	return state;
}


short tag (void)
{
	dcoord_t coords={1,6};
	joyinfo_t info;
	char num=0,i,aux=0;
	short user;
	letras_on(coords,'T');
	(coords.x)+=4;
	letras_on(coords,'A');
	(coords.x)+=4;
	letras_on(coords,'G');
	(coords.x)=0;
	(coords.y)=14;
	for(i=0;i<3;i++)
	{
	do
	{
		letras_on(coords,'0'+num);
		usleep(400000);
		letras_off(coords,'0'+num);
		info=joy_read();
		if(num!=0 && info.x <-50)
		{
			num--;
		}else if(num!=9 && info.x>50)
		{
			num++;
		}
	}while(info.sw==J_NOPRESS);
	letras_on(coords,'0'+num);
	user= user*10 + num;
	(coords.x)+=5;
	if(i==2)
	{
		(coords.x)++;
	}
	}
	return user;

}


static void play (void)
{
	int aux;
	char flag=CONT;
	short user;
	joyinfo_t info;
	clock_t last_fall_time = clock();
	user=tag();
	initBoard();
	nextPiece();
	while(flag!=STOP)
	{
		print_level (level);
		playini();
		aux=level;
		while(aux==level && flag != STOP)
		{
			board_redraw();
			clock_t current_time = clock();
			double elapsed_time = (double)(current_time - last_fall_time) / CLOCKS_PER_SEC*100;
			if(elapsed_time >= FALL_TIME)
			{
				shiftPieceDown(0);
				last_fall_time = clock(); //reinicia el temporizador
			}
			else
			{
				info=joy_read();
				if((info.y)<-50)
				{
					shiftPieceDown(1);
				}
				else if((info.x)<-50)
				{
					shiftPieceLeft();
				}
				else if((info.x)>50)
				{
					shiftPieceRight();
				}
				else if((info.y)>50)
				{
					rotateClockwise();
				}
				else if((info.sw)==J_PRESS)
				{
					pausa(&flag);
				}
			}
			usleep(200000);//tiempo de espera para no tomar muchos valores

		}
	}
}


static void pausa (char * flag)
{
	char state=PLAY;
	disp_clear();
	disp_update();
	dcoord_t coords={1,4};
	joyinfo_t info;
	letras_on (coords,'P');
	coords.x+=4;
	letras_on (coords,'L');
	coords.x+=4;
	letras_on (coords,'A');
	coords.x+=4;
	letras_on (coords,'Y');
	(coords.x)=1;
	coords.y+=11;
	letras_on (coords,'S');
	coords.x+=4;
	letras_on (coords,'T');
	coords.x+=4;
	letras_on (coords,'O');
	coords.x+=4;
	letras_on (coords,'P');
	coords.x=0;
	usleep(400000);//tiempo de espera para que no tome el presionado para poner pausa
	do
	{
		info = joy_read();
		switch(state)
		{
		case PLAY:
			coords.y=2;
			break;
		case STOP:
			coords.y=13;
			break;
		}
		disp_write(coords,D_ON);
		disp_update();
		usleep(200000);
		disp_write(coords,D_OFF);
		disp_update();
		usleep(200000);
		if(state!=PLAY && info.y>50)
		{
		  state-=2;
		}
		else if(state!=STOP && info.y<-50)
		{
		  state+=2;
		}
	}while(info.sw == J_NOPRESS);
	disp_clear();
	if(state==PLAY)
	{
		*flag = CONT;
		playini();
	}
	else if (state==STOP)
	{
		*flag=STOP;
	}
	disp_update();
}


static void board_redraw (void)
{
	int i, j;
	dcoord_t coord;
	for(i=0;i<BOARD_HEIGHT;i++)
	{
		for(j=0;j<BOARD_WIDTH;j++)
		{
			coord.y=i;
			coord.x=j;
			if(board[i][j])
			{
				disp_write(coord,D_ON);
			}
			else
			{
				disp_write(coord,D_OFF);
			}
		}
	}
	disp_update();
}

static void print_level (int lv)
{
	disp_clear();
	disp_update();
	char i;
	dcoord_t coord;
	(coord.x)=15;
	for(i=0;i<42;i++)
	{
		(coord.y)=9;
		letras_on(coord,'L');
		(coord.x)+=4;
		letras_on(coord,'E');
		(coord.x)+=4;
		letras_on(coord,'V');
		(coord.x)+=5;
		letras_on(coord,'E');
		(coord.x)+=4;
		letras_on(coord,'L');
		(coord.x)+=4;
		(coord.y)=10;
		if(lv<10)
		{
			letras_on(coord,'0'+lv);
			usleep(50000);
			(coord.x)-=21;
		}
		else
		{
			letras_on(coord,'0'+lv/10);
			(coord.x)+=6;
			letras_on(coord,'0'+lv%10);
			usleep(50000);
			(coord.x)-=27;
		}
		(coord.y)=9;
		letras_off(coord,'L');
		(coord.x)+=4;
		letras_off(coord,'E');
		(coord.x)+=4;
		letras_off(coord,'V');
		(coord.x)+=5;
		letras_off(coord,'E');
		(coord.x)+=4;
		letras_off(coord,'L');
		(coord.x)+=4;
		(coord.y)=10;
		if(lv<10)
		{
			letras_off(coord,'0'+lv);
			(coord.x)-=22;
		}
		else
		{
			letras_off(coord,'0'+lv/10);
			(coord.x)+=6;
			letras_off(coord,'0'+lv%10);
			(coord.x)-=28;
		}
	}
}

static void playini(void)
{
	dcoord_t coord;
	(coord.x)=14;
	char i;
	disp_clear();
	disp_update();
	(coord.y)=0;
	(coord.x)=10;
	while(coord.y<MAX_HEIGHT)
	{
		disp_write(coord,D_ON);
		(coord.y)++;
	}
	disp_update();
}


