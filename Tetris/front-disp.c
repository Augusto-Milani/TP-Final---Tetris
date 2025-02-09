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
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "Libs/disdrv.h"
#include "Libs/joydrv.h"
#include "letras.h"
#include "backend.h"
#define MAX_HEIGHT 16
#define MAX_WIDTH 16
#define SEC 1000000
#define FALL_TIME 1.5//tiempo que espera para caer en sec
#define SCORE_TIME 1.5
#define END 99
#define MAX_TOP 5
#define HEIGHTNUM 7
#define WIDTHNUM 5
#define EASTER_EGG 999//si se elige como tag los controles se invierten


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
static void print_tetris (void);
static void nextpiece_draw (void);


int main (void)
{
	char state;
	disp_init();
	joy_init();
	print_tetris();
	while((state=menu())!=STOP)//vuelve al menu constantemente hasat seleccionar STOP
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

static void print_tetris (void)//Imprime TETRIS al iniciar el programa con texto deslizante
{
	char text[]="TETRIS";
	int i,j;
	dcoord_t coord={15,9};
	for(i=0;i<((int)strlen(text)*4+MAX_WIDTH);i++)//Hace hasat que la ultima letra llegue al inicio de la pantalla
	{
		for(j=0;j<(int)strlen(text);j++)
		{
			letras_on(coord, text[j]);
			(coord.x)+=4;
		}
		(coord.x)-=4*strlen(text);//vuelve al inicio para apagar todas las letras
		for(j=0;j<(int)strlen(text);j++)
		{
			letras_off(coord,text[j]);
			(coord.x)+=4;
		}
		(coord.x)-=4*strlen(text)+1;// va 1 antes que el inicio para ir deslizando el texto
	}
}

static char menu (void)
{
    char state=PLAY,j=0;
    int i;
    char text[]="PLAY TOP STOP";
	disp_clear();
	disp_update();
	dcoord_t coords={1,4};
	joyinfo_t info;
	for(i=0;i<(int)strlen(text);i++)//imprime las palabras PLAY TOP STOP
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
				(coords.y)+=6;//en el primer caso deja un lugar de mas
		}
	}
	coords.x=0;
	do
	{
		info = joy_read();
		switch(state)//segun el estado es donde ubica el led parpadeante
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
		if(state!=PLAY && info.y>50)//con la info de joystick cambia el estado
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
	while (i < MAX_TOP && fscanf(ftag, "%d", &users[i]) == 1)//guarda en el arreglo los tags, uno por cada elemento
	{
		i++;
	}
	fclose(ftag);
	for(i=0;i<MAX_TOP;i++)
	{
		sprintf(topusers[i],"%d",users[i]);//pone cada caracter en la matriz topuser
	}
	for(k=0;k<((HEIGHTNUM+1)*MAX_TOP+MAX_HEIGHT);k++)//imprime deslizante hacia arriba hasta que llegue al ultimo tag
	{
		for(i=0;i<MAX_TOP;i++)
		{
			(coord.x)=0;
			for(j=0,f=0;j<(int)strlen(topusers[i]);j++)//imprime en horizontal hasta llegar al final del tag
			{
				letras_on(coord,topusers[i][j]);
				if(f++)
					(coord.x)+=WIDTHNUM;
				else
					(coord.x)+=WIDTHNUM+1;

			}
			(coord.y)+=(HEIGHTNUM+1);
		}
		(coord.y)-=(HEIGHTNUM+1)*MAX_TOP+1;//va un lugar arriba del inicio
		usleep(SEC/20);
		disp_clear();//para limpiar la pantalla
	}
}



static int tag (void)//funcion para elegir el tag y devuelve su valor
{
	dcoord_t coords={1,6};
	joyinfo_t info;
	int num=0,i;
	int user=0;
	letras_on(coords,'T');
	(coords.x)+=4;
	letras_on(coords,'A');
	(coords.x)+=4;
	letras_on(coords,'G');
	(coords.x)=0;
	(coords.y)=14;
	for(i=0;i<3;i++)//imprime los tres numeros para elegir
	{
		do//cambia el numero que aparece hasta elegir uno
		{
			letras_on(coords,'0'+num);
			usleep((SEC/10)*4);
			letras_off(coords,'0'+num);
			info=joy_read();
			if(num!=0 && info.x <-50)
			{
				num--;
			}
			else if(num!=9 && info.x>50)
			{
				num++;
			}
		}while(info.sw==J_NOPRESS);
		letras_on(coords,'0'+num);
		user= user*10 + num;//va guardando el numero en num
		(coords.x)+=5;
		if(i==1)
		{
			(coords.x)++;
		}
	}
	return user;

}

static void play (void)//funcion que se encarga de todo el juego
{
	int aux, i=0;
	char flag=CONT,top;
	int user;
	char nums[20]={0};//arreglo para imprimir el score
	joyinfo_t info;
	clock_t last_fall_time = clock(),last_number_time = clock();// relojes para la caida de la pieza y la impresion del score
	alive=true;
	user=tag();
	printf("%d",user);
	initBoard();//inicia la matriz
	while(flag!=STOP)//sale cuando termina el juego o se desea parar
	{
		print_level (level);
		playini();
		aux=level;//para saber cuando pasa de nivel e imprimirlo en pantalla
		while(level==aux && flag != STOP)
		{
			board_redraw();
			nextpiece_draw();
			clock_t current_time = clock();//va guardando el tiempo actual del procesador
			double elapsed1_time = (double)(current_time - last_fall_time) / CLOCKS_PER_SEC*100,elapsed2_time = (double)(current_time - last_number_time) / CLOCKS_PER_SEC*100;
			//guarda en cada caso el tiempo que paso desde que se relizo la accion
			if(elapsed1_time >= FALL_TIME*(10.0/(10.0+level)))//va cambiando el teimpo de caida de la pieza segun el nivel
			{
				shiftPieceDown(0);
				last_fall_time = clock(); //reinicia el temporizador
			}
			else if(elapsed2_time >= SCORE_TIME)
			{
				dcoord_t coord={11,7};
				if(i!=0)
				letras_off(coord,nums[i-1]);
				if(i==(int)strlen(nums)+1)//chequea si ya esta en uno mas que el ultimo para hacer un corte entre el primer score y el siguiente
				{
					sprintf(nums,"%d",score);//funcion que guarda cada dijito del int en un arreglo de chars
					i=0;
				}
				letras_on(coord,nums[i++]);//va imprimiendo los numeros del score
				disp_update();
				last_number_time = clock();//reinicia el temporizador
			}
			else if(alive==false)
			{
				gameover(&flag);//entra cuando se pierde
			}
			else
			{
				info=joy_read();//si no hace otra accion lee el joystick
				if((info.y)<-50)
				{
					if(user==EASTER_EGG)
					{
						rotateClockwise();
					}
					else
					{
						shiftPieceDown(1);
					}
				}
				else if((info.x)<-50)
				{
					if(user==EASTER_EGG)
					{
						shiftPieceRight();
					}
					else
					{
						shiftPieceLeft();
					}
				}
				else if((info.x)>50)
				{
					if(user==EASTER_EGG)
					{
						shiftPieceLeft();
					}
					else
					{
						shiftPieceRight();
					}
				}
				else if((info.y)>50)
				{
					if(user==EASTER_EGG)
					{
						shiftPieceDown(1);
					}
					else
					{
						rotateClockwise();
					}
				}
				else if((info.sw)==J_PRESS)
				{
					pausa(&flag);
				}
			}
			usleep(SEC/8);//pausa para no tomar demasiados valores
		}
	}
	top=make_top(user);//guarda la posicion del top o 0 si no entra en el 5
	if(top)
	{
		print_top(top);
	}
}

static void gameover (char * flag)
{
	int i,j;
	dcoord_t coord={15,9};
	*flag=STOP;//pone en flag STOP para parar el juego
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
		disp_clear();

		(coord.x)-=34;
	}
	(coord.x)=1;
	(coord.y)=1;
	for(j=0;j<2;j++)//imprime cara con x
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

static void print_top(char top)//si entra al top se dice en que posicion
{
	int i,j,length = 0, num = score;
	disp_update();
	char nums[20]={0};
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
	usleep(SEC*3);
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
		for(j=0;j<(int)strlen(nums);j++)
		{
			letras_on(coord,nums[j]);
			(coord.x)+=6;
		}
		usleep(SEC/15);
		disp_clear();
		(coord.x)-=(j*6)+1;
	}
}

static void pausa (char * flag)//
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
	do//mismo que en el menu
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
	disp_clear();//deoende que se eligio que pone el flag
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


static void board_redraw (void)//para imprimir como este la matriz board
{
	int i, j;
	dcoord_t coord;
	for(i=0;i<BOARD_HEIGHT;i++)
	{
		for(j=0;j<BOARD_WIDTH;j++)//recorre toda la matriz y chequea si esta en 1 o 0
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
			usleep(SEC/50);
		}
		else
		{
			letras_on(coord,'0'+lv/10);
			(coord.x)+=6;
			letras_on(coord,'0'+lv%10);
			usleep(SEC/50);
		}
		disp_clear();
		if(lv<10)//depende los digitos cuantos lugares vuelve hacia atras
		{
			(coord.x)-=22;
		}
		else
		{
			(coord.x)-=28;
		}
	}
}

static void playini(void)//funcion que imprime el fin del tablero
{
	dcoord_t coord;
	(coord.x)=14;
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
    int i = 0, j = MAX_TOP, k = 0;
    FILE *fscore = fopen("Top/score.txt", "r");
    FILE *ftag = fopen("Top/tag.txt", "r");
    if (!fscore || !ftag)
    {
        printf("Error al abrir archivos\n");
        if (fscore) fclose(fscore);
        if (ftag) fclose(ftag);
        return 0;
    }
    while (i < MAX_TOP && fscanf(fscore, "%d", &scr[i]) == 1)    //lee puntajes del archivo
    {
        i++;
    }
    fclose(fscore);
    i = 0;
    while (i < MAX_TOP && fscanf(ftag, "%d", &users[i]) == 1)    // lee usuarios del archivo
    {
        i++;
    }
    fclose(ftag);
    for (i = 0; i < MAX_TOP; i++) // determina si el nuevo puntaje entra en el ranking
    {
        if (scr[i] < score)
        {
            top = i + 1;
            j = i;
            k = i;
            break;
        }
    }
    if (top || i < MAX_TOP)//si entra en el top o si no hay 5 valores todavia
    {
        for (j = MAX_TOP - 1; j > k; j--)//pone el nuevo puntaje y usuario en el ranking
        {
            scr[j] = scr[j - 1];
            users[j] = users[j - 1];
        }
        scr[k] = score;
        users[k] = user;
    }
    // guarda los nuevos valores en los archivos
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
    }

    fclose(fscore);
    fclose(ftag);

    return top;
}


static void nextpiece_draw(void)
{
    int i, j, f = 0;
    static int k = 0; // arranca en 0
    static int aux[4][4]; //es estático y se mantiene entre llamadas
    if (k++ == 0) // inicializa aux solo la primera vez
    {
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                aux[i][j] = nextPieceStatus[i][j];
            }
        }
    }
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (aux[i][j] != nextPieceStatus[i][j])//si cambia la siguiente pieza incrementa f
            {
                f++;
            }
        }
    }
    if (f > 0) // solo limpiar si hay cambios
    {
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                if (aux[i][j]) //apaga los leds prendidos
                {
                    dcoord_t coord = {12 + (uint8_t)j, 10 + (uint8_t)i};
                    disp_write(coord, D_OFF);
                }
                aux[i][j] = nextPieceStatus[i][j]; // actualiza aux con los nuevos valores
            }
        }
    }
    for (i = 0; i < 4; i++)//prende los leds de la proxima pieza
    {
        for (j = 0; j < 4; j++)
        {
            dcoord_t coord = {12 + (uint8_t)j, 10 + (uint8_t)i};
            if (nextPieceStatus[i][j])
            {
                disp_write(coord, D_ON);
            }
        }
    }
    disp_update();
}

