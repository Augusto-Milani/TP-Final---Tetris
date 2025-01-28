/*
 * front-disp.c
 *
 *  Created on: 23 ene 2025
 *      Author: mariano
 */




#include <stdio.h>
#include <stdint.h>
#include "Libs/disdrv.h"
#include "Libs/joydrv.h"
#include "letras.h"

#define MAX_HEIGHT 16
#define MAX_WEIGHT 16

enum {PLAY=1,TOP,STOP};


static char menu (void);
static void play (void);
static void playini (void);
static short tag (void);

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
	short user;
	user=tag();

	playini();
	usleep(8000000);
	disp_clear();
	disp_update();
}



static void playini(void)
{
	disp_clear();
	disp_update();
	dcoord_t coord;
	(coord.y)=0;
	(coord.x)=10;
	while(coord.y<MAX_HEIGHT)
	{
		disp_write(coord,D_ON);
		(coord.y)++;
	}
	disp_update();
}


