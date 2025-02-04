/*
 * front-disp.c
 *
 *  Created on: 23 ene 2025
 *
 *
 *      Author: mariano
 */




#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>//para el strlen para saber cuantos caracteres tiene sin el '/0'
#include <stdbool.h>
#include "Libs/disdrv.h"
#include "Libs/joydrv.h"
#include "letras.h"
#include "../backend.h"
#define MAX_HEIGHT 16
#define MAX_WIDTH 16
#define SEC 1000000
#define FALL_TIME 1.5//tiempo que espera para caer en sec
#define SCORE_TIME 0.5
#define END 99
#define MAX_TOP 5
#define HEIGHTNUM 7
#define WIDTHNUM 5

enum {PLAY=1,TOP,STOP,CONT};


static char menu (void);
static void play (void);
static void playini (void);
static int tag (void);
static void print_level (int);
static void board_redraw (void);
static void pausa (char *);
static void gameover (char *);
static char make_top (int);
static void print_top (char);
static void top (void);

extern int level,score;
extern int board[BOARD_HEIGHT][BOARD_WIDTH];
extern bool alive;


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
			top();
			break;
		}
	}

	return 0;
}


static char menu (void)
{
    char state=PLAY,i,j=0;
    char text[]="PLAY TOP STOP";
	disp_clear();
	disp_update();
	dcoord_t coords={1,4};
	joyinfo_t info;
	for(i=0;i<strlen(text);i++)
	{
		if(text[i]!=' ')
		{
			letras_on(coords,text[i]);
			(coords.x)+=4;
		}
		else
		{
			(coords.x)=1;
			if(j++)
				(coords.y)+=5;
			else
				(coords.y)+=6;
		}
	}
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
		usleep((SEC/10)*2);
		disp_write(coords,D_OFF);
		disp_update();
		usleep((SEC/10)*2);
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

static void top (void)
{
	int i=0,j,k,f, users[5]={0};
	char topusers[5][4]={0};
	dcoord_t coord={0,15};
	FILE * ftag = fopen("Top/tag.txt", "r");
	if(!ftag)
	{
		printf("Error al abrir archivo\n");
		fclose(ftag);
	}
	while (i < MAX_TOP && fscanf(ftag, "%d", &users[i]) == 1)
	{
		i++;
	}
	fclose(ftag);
	for(i=0;i<MAX_TOP;i++)
	{
		sprintf(topusers[i],"%d",users[i]);
	}
	for(k=0;k<((HEIGHTNUM+1)*MAX_TOP+MAX_HEIGHT);k++)
	{
		for(i=0;i<MAX_TOP;i++)
		{
			(coord.x)=0;
			for(j=0,f=0;j<strlen(topusers[i]);j++)
			{
				letras_on(coord,topusers[i][j]);
				if(f++)
					(coord.x)+=WIDTHNUM;
				else
					(coord.x)+=WIDTHNUM+1;

			}
			(coord.y)+=(HEIGHTNUM+1);
		}
		(coord.y)-=(HEIGHTNUM+1)*MAX_TOP;
		usleep(SEC/20);
		for(i=0;i<MAX_TOP;i++)
		{
			(coord.x)=0;
			for(j=0,f=0;j<strlen(topusers[i]);j++)
			{
				letras_off(coord,topusers[i][j]);
				if(f++)
					(coord.x)+=WIDTHNUM;
				else
					(coord.x)+=WIDTHNUM+1;
			}
			(coord.y)+=(HEIGHTNUM+1);
		}
		(coord.y)-=(HEIGHTNUM+1)*MAX_TOP+1;
	}
}



static int tag (void)
{
	dcoord_t coords={1,6};
	joyinfo_t info;
	int num=0,i,aux=0;
	int user=0;
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
		usleep((SEC/10)*4);
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
	int aux, i=0;
	char flag=CONT,top;
	int user;
	char nums[20];
	memset(nums,END,19);// inicializa todo el string en un numero cualquierra mayo a 10
	joyinfo_t info;
	clock_t last_fall_time = clock(),last_number_time;
	alive=true;
	user=tag();
	printf("%d",user);
	score=0;
	initBoard();
	nextPiece();
	while(flag!=STOP)
	{
		print_level (level);
		playini();
		aux=level;
		while(level==aux && flag != STOP)
		{
			board_redraw();
			clock_t current_time = clock();
			double elapsed1_time = (double)(current_time - last_fall_time) / CLOCKS_PER_SEC*100,elapsed2_time = (double)(current_time - last_number_time) / CLOCKS_PER_SEC*10;
			if(elapsed1_time >= FALL_TIME*(10.0/(10.0+level)))
			{
				shiftPieceDown(0);
				last_fall_time = clock(); //reinicia el temporizador
			}
			else if(elapsed2_time >= SCORE_TIME)
			{
				dcoord_t coord={11,7};
				letras_off(coord,nums[i-1]);
				if(nums[i]==END)//chequea si ya esta en el ultimo numero
				{
					sprintf(nums,"%d",score);//funcion que guarda cada dijito del int en un arreglo de chars
					i=0;
				}
				letras_on(coord,nums[i++]);
				disp_update();
				last_number_time = clock();
			}
			else if(alive==false)
			{
				gameover(&flag);
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
			usleep(SEC/10);//pausa para no tomar demasiados valores
		}
	}
	top=make_top(user);
	if(top)
	{
		print_top(top);
	}
}

static void gameover (char * flag)
{
	int i,j;
	dcoord_t coord={15,9};
	*flag=STOP;
	disp_clear();
	disp_update();
	for(i=0;i<48;i++)
	{
		letras_on(coord,'G');
		(coord.x)+=5;
		letras_on(coord,'A');
		(coord.x)+=4;
		letras_on(coord,'M');
		(coord.x)+=4;
		letras_on(coord,'E');
		(coord.x)+=7;
		letras_on(coord,'O');
		(coord.x)+=4;
		letras_on(coord,'V');
		(coord.x)+=5;
		letras_on(coord,'E');
		(coord.x)+=4;
		letras_on(coord,'R');
		usleep(SEC/12);

		(coord.x)-=33;
		letras_off(coord,'G');
		(coord.x)+=5;
		letras_off(coord,'A');
		(coord.x)+=4;
		letras_off(coord,'M');
		(coord.x)+=4;
		letras_off(coord,'E');
		(coord.x)+=7;
		letras_off(coord,'O');
		(coord.x)+=4;
		letras_off(coord,'V');
		(coord.x)+=5;
		letras_off(coord,'E');
		(coord.x)+=4;
		letras_off(coord,'R');
		(coord.x)-=34;
	}
	(coord.x)=1;
	(coord.y)=1;
	for(j=0;j<2;j++)
	{
		for(i=0;i<5;i++)
		{
			disp_write(coord,D_ON);
			(coord.x)++;
			(coord.y)++;
		}
		(coord.x)-=5;
		(coord.y)--;
		for(i=0;i<5;i++)
		{
		disp_write(coord,D_ON);
		(coord.x)++;
		(coord.y)--;
		}
		(coord.x)=10;
		(coord.y)=1;
	}
	(coord.x)=1;
	(coord.y)=14;
	for(i=0;i<2;i++)
	{
		disp_write(coord,D_ON);
		(coord.y)--;
	}
	(coord.y)++;
	for(i=0;i<2;i++)
	{
		(coord.x)++;
		(coord.y)--;
		disp_write(coord,D_ON);
	}
	for(i=0;i<9;i++)
	{
		(coord.x)++;
		disp_write(coord,D_ON);
	}
	(coord.x)++;
	(coord.y)++;
	disp_write(coord,D_ON);
	(coord.x)++;
	for(i=0;i<2;i++)
	{
		(coord.y)++;
		disp_write(coord,D_ON);
	}
	disp_update();
	usleep(SEC*3);
	disp_clear();
}

static void print_top(char top)
{
	char i,j,length = 0;
	int num = score;
	disp_update();
	char nums[20];
	memset(nums,END,19);
	sprintf(nums,"%d",score);
	dcoord_t coord={2,5};
	letras_on(coord,'T');
	(coord.x)+=4;
	letras_on(coord,'O');
	(coord.x)+=4;
	letras_on(coord,'P');
	(coord.x)=6;
	(coord.y)=14;
	letras_on(coord,'0'+top);
	usleep(SEC*3); length = 0;
	while (num > 0)
	{
		num /= 10;
		length++;
	}
	disp_clear();
	disp_update();
	(coord.x)=15;
	(coord.y)=10;
	for(i=0;i<(length*6+MAX_WIDTH);i++)
	{
		for(j=0;nums[j]!=END;j++)
		{
			letras_on(coord,nums[j]);
			(coord.x)+=6;
		}
		(coord.x)-=(j*6);
		usleep(SEC/15);
		for(j=0;nums[j]!=END;j++)
		{
			letras_off(coord,nums[j]);
			(coord.x)+=6;
		}
		(coord.x)-=(j*6)+1;
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
	usleep(SEC);//tiempo de espera para que no tome el presionado para poner pausa
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
		usleep((SEC/10)*2);
		disp_write(coords,D_OFF);
		disp_update();
		usleep((SEC/10)*2);
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
			usleep(SEC/100);
			(coord.x)-=21;
		}
		else
		{
			letras_on(coord,'0'+lv/10);
			(coord.x)+=6;
			letras_on(coord,'0'+lv%10);
			usleep(SEC/100);
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


static char make_top(int user)
{
    char top = 0;
    int scr[MAX_TOP] = {0}, users[MAX_TOP] = {0};
    int i = 0, j = MAX_TOP, k = 0, aux1;

    FILE *fscore = fopen("Top/score.txt", "r");
    FILE *ftag = fopen("Top/tag.txt", "r");

    if (!fscore || !ftag)
    {
        printf("Error al abrir archivos\n");
        if (fscore) fclose(fscore);
        if (ftag) fclose(ftag);
        return 0;
    }

    // Leer puntajes del archivo
    while (i < MAX_TOP && fscanf(fscore, "%d", &scr[i]) == 1)
    {
        i++;
    }
    fclose(fscore);

    // Leer usuarios del archivo
    i = 0;
    while (i < MAX_TOP && fscanf(ftag, "%d", &users[i]) == 1)
    {
        i++;
    }
    fclose(ftag);

    // Determinar si el nuevo puntaje entra en el ranking
    for (i = 0; i < MAX_TOP; i++)
    {
        if (scr[i] < score)
        {
            top = i + 1;
            j = i;
            k = i;
            break;
        }
    }

    if (top || i < MAX_TOP)
    {
        // Insertar el nuevo puntaje y usuario en el ranking
        for (j = MAX_TOP - 1; j > k; j--)
        {
            scr[j] = scr[j - 1];
            users[j] = users[j - 1];
        }
        scr[k] = score;
        users[k] = user;
    }

    // Guardar los nuevos valores en los archivos
    fscore = fopen("Top/score.txt", "w");
    ftag = fopen("Top/tag.txt", "w");

    if (!fscore || !ftag)
    {
        printf("Error al abrir archivos para escritura\n");
        if (fscore) fclose(fscore);
        if (ftag) fclose(ftag);
        return 0;
    }

    for (i = 0; i < MAX_TOP; i++)
    {
        fprintf(fscore, "%d\n", scr[i]);
        fprintf(ftag, "%d\n", users[i]);
        fflush(ftag);
        fflush(fscore);
    }

    fclose(fscore);
    fclose(ftag);

    return top;
}

