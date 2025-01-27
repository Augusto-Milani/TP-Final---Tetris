/*
 * front-disp.c
 *
 *  Created on: 23 ene 2025
 *      Author: mariano
 */




#include <stdio.h>
#include <stdint.h>
#include "disdrv.h"
#include "joydrv.h"

#define MAX_HEIGHT 16
#define MAX_WEIGHT 16

enum {PLAY=1,TOP,STOP};


static char menu (void);
static void letras (dcoord_t,char);
static void play (void);
static void playini (void);
static short int user (void);

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
	letras (coords,'P');
	coords.x+=4;
	letras (coords,'L');
	coords.x+=4;
	letras (coords,'A');
	coords.x+=4;
	letras (coords,'Y');
	coords.x=1;
	coords.y+=6;
	letras (coords,'T');
	coords.x+=4;
	letras (coords,'O');
	coords.x+=4;
	letras (coords,'P');
	coords.x=1;
	coords.y+=5;
	letras (coords,'S');
	coords.x+=4;
	letras (coords,'T');
	coords.x+=4;
	letras (coords,'O');
	coords.x+=4;
	letras (coords,'P');
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


static void letras (dcoord_t coords,char let)
{
	int i;
	switch(let)
	{
	case 'A':
		for(i=0;i<5;i++)
		{
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			disp_write(coords,D_ON);
		}
		for(i=0;i<4;i++)
		{
			(coords.y)++;
			disp_write(coords,D_ON);
		}
		coords.y-=2;
		coords.x--;
		disp_write(coords,D_ON);
		break;
	case 'P':
		for(i=0;i<5;i++)
		{
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			disp_write(coords,D_ON);
		}
		for(i=0;i<2;i++)
		{
			(coords.y)++;
			disp_write(coords,D_ON);
		}
		(coords.x)--;
		disp_write(coords,D_ON);
		break;
	case 'L':
		for(i=0;i<5;i++)
		{
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		coords.y+=5;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			disp_write(coords,D_ON);
		}
		break;
	case 'Y':
		coords.x++;
		for(i=0;i<4;i++)
		{
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.x)--;
		disp_write(coords,D_ON);
		(coords.x)+=2;
		disp_write(coords,D_ON);
		break;
	case 'T':
		coords.x++;
		for(i=0;i<5;i++)
		{
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.y)++;
		(coords.x)--;
		disp_write(coords,D_ON);
		(coords.x)+=2;
		disp_write(coords,D_ON);
		break;
	case 'O':
		for(i=0;i<5;i++)
		{
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			disp_write(coords,D_ON);
		}
		for(i=0;i<4;i++)
		{
			(coords.y)++;
			disp_write(coords,D_ON);
		}
		(coords.x)--;
		disp_write(coords,D_ON);
		break;
	case 'S':
		for(i=0;i<3;i++)
		{
			disp_write(coords,D_ON);
			(coords.x)++;
		}
		(coords.x)--;
		for(i=0;i<2;i++)
		{
			(coords.y)--;
			disp_write(coords,D_ON);
		}
		for(i=0;i<2;i++)
		{
			(coords.x)--;
			disp_write(coords,D_ON);
		}
		for(i=0;i<2;i++)
		{
			(coords.y)--;
			disp_write(coords,D_ON);
		}
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			disp_write(coords,D_ON);
		}
		break;
	case 'C':
		for(i=0;i<5;i++)
		{
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			disp_write(coords,D_ON);
		}
		for(i=0;i<4;i++)
		{
			(coords.y)++;
		}
		disp_write(coords,D_ON);
		(coords.x)--;
		disp_write(coords,D_ON);
		break;
	case 'R':
		for(i=0;i<5;i++)
		{
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			disp_write(coords,D_ON);
		}
		(coords.y)++;
		disp_write(coords,D_ON);
		(coords.y)++;
		(coords.x)--;
		disp_write(coords,D_ON);
		(coords.x)++;
		for(i=0;i<2;i++)
		{
			(coords.y)++;
			disp_write(coords,D_ON);
		}
		break;
	case 'G':
		for(i=0;i<5;i++)
		{
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<3;i++)
		{
			(coords.x)++;
			disp_write(coords,D_ON);
		}
		(coords.y)+=2;
		(coords.x)--;
		disp_write(coords,D_ON);
		(coords.x)++;
		for(i=0;i<3;i++)
		{
			disp_write(coords,D_ON);
			(coords.y)++;
		}
		(coords.y)--;
		(coords.x)--;
		disp_write(coords,D_ON);
		(coords.x)--;
		disp_write(coords,D_ON);
		break;
	case '0':
		(coords.y)--;
		for(i=0;i<5;i++)
		{
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		for(i=0;i<3;i++)
		{
			(coords.x)++;
			disp_write(coords,D_ON);
		}
		(coords.x)++;
		for(i=0;i<5;i++)
		{
			(coords.y)++;
			disp_write(coords,D_ON);
		}
		(coords.x)--;
		for(i=0;i<3;i++)
		{
			(coords.x)--;
			disp_write(coords,D_ON);
		}
		(coords.y)-=4;
		disp_write(coords,D_ON);
		for(i=0;i<2;i++)
		{
			(coords.y)++;
			(coords.x)++;
			disp_write(coords,D_ON);
		}
		break;
	case '1':
		for(i=0;i<3;i++)
		{
			(coords.x)++;
			disp_write(coords,D_ON);
		}
		(coords.x)--;
		for
	}
	disp_update();
}

static void play (void)
{
	short int user=user();

	playini();
}

short int user (void)
{
	return user
}

static void playini(void)
{
  dcoord_t coord={10,0};
  while(coord.y<MAX_HEIGHT)
  {
    disp_write(coord,D_ON);
    coord.y++;
  }
  
}

