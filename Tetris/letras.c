/*
 * letras.c
 *
 *  Created on: 26 ene 2025
 *
 *
 *
 *      Author: mariano
 */


#include "letras.h"

#define CHECK if((coords.x)<MAX_WIDTH && (coords.y)<MAX_HEIGHT && (coords.x)>=0 && (coords.y)>=0)
//macro para no intentar prender fuera de la pantalla cuando el deslizante


void letras_on (dcoord_t coords,char let)//fumcion que recibe coordenadas y un caracte y lo imprime en el display
{
	int i;
	switch(let)//dependiendo el caracter que prende
	{
	case 'A':
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_ON);
		}
		for(i=0;i<4;i++)
		{
			(coords.y)++;
			CHECK
			disp_write(coords,D_ON);
		}
		coords.y-=2;
		coords.x--;
		CHECK
		disp_write(coords,D_ON);
		break;
	case 'P':
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_ON);
		}
		for(i=0;i<2;i++)
		{
			(coords.y)++;
			CHECK
			disp_write(coords,D_ON);
		}
		(coords.x)--;
		CHECK
		disp_write(coords,D_ON);
		break;
	case 'L':
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		coords.y+=5;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
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
			CHECK
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.y)++;
		(coords.x)--;
		CHECK
		disp_write(coords,D_ON);
		(coords.x)+=2;
		CHECK
		disp_write(coords,D_ON);
		break;
	case 'O':
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_ON);
		}
		for(i=0;i<4;i++)
		{
			(coords.y)++;
			CHECK
			disp_write(coords,D_ON);
		}
		(coords.x)--;
		CHECK
		disp_write(coords,D_ON);
		break;
	case 'S':
		for(i=0;i<3;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.x)++;
		}
		(coords.x)--;
		for(i=0;i<2;i++)
		{
			(coords.y)--;
			CHECK
			disp_write(coords,D_ON);
		}
		for(i=0;i<2;i++)
		{
			(coords.x)--;
			CHECK
			disp_write(coords,D_ON);
		}
		for(i=0;i<2;i++)
		{
			(coords.y)--;
			CHECK
			disp_write(coords,D_ON);
		}
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
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
			CHECK
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_ON);
		}
		(coords.y)++;
		CHECK
		disp_write(coords,D_ON);
		(coords.y)++;
		(coords.x)--;
		CHECK
		disp_write(coords,D_ON);
		(coords.x)++;
		for(i=0;i<2;i++)
		{
			(coords.y)++;
			CHECK
			disp_write(coords,D_ON);
		}
		break;
	case 'G':
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<3;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_ON);
		}
		(coords.y)+=2;
		(coords.x)--;
		CHECK
		disp_write(coords,D_ON);
		(coords.x)++;
		for(i=0;i<3;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.y)++;
		}
		(coords.y)--;
		(coords.x)--;
		CHECK
		disp_write(coords,D_ON);
		(coords.x)--;
		CHECK
		disp_write(coords,D_ON);
		break;
	case 'E':
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_ON);
		}
		(coords.x)-=2;
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.y)+=3;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_ON);
		}
		(coords.y)-=2;
		for(i=0;i<2;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.x)--;
		}
		break;
	case 'V':
		(coords.y)-=4;
		for(i=0;i<3;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.y)++;
		}
		(coords.x)++;
		for(i=0;i<2;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.y)++;
		}
		(coords.x)++;
		(coords.y)--;
		for(i=0;i<2;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.x)++;
		for(i=0;i<3;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		break;
	case 'M':
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		(coords.y)+=2;
		(coords.x)++;
		CHECK
		disp_write(coords,D_ON);
		(coords.y)--;
		(coords.x)++;
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.y)++;
		}
		break;
	case 'I':
		for(i=0;i<3;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.x)++;
		}
		(coords.x)-=2;
		for(i=0;i<4;i++)
		{
			(coords.y)--;
			CHECK
			disp_write(coords,D_ON);
		}
		(coords.x)--;
		CHECK
		disp_write(coords,D_ON);
		(coords.x)+=2;
		CHECK
		disp_write(coords,D_ON);
		break;
	case '0':
		(coords.y)--;
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_ON);
			(coords.y)--;
		}
		for(i=0;i<3;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_ON);
		}
		(coords.x)++;
		for(i=0;i<5;i++)
		{
			(coords.y)++;
			CHECK
			disp_write(coords,D_ON);
		}
		(coords.y)++;
		for(i=0;i<3;i++)
		{
			(coords.x)--;
			CHECK
			disp_write(coords,D_ON);
		}
		(coords.y)-=4;
		CHECK
		disp_write(coords,D_ON);
		for(i=0;i<2;i++)
		{
			(coords.y)++;
			(coords.x)++;
			CHECK
			disp_write(coords,D_ON);
		}
		break;
	case '1':
		for(i=0;i<3;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_ON);
		}
		(coords.x)--;
		for(i=0;i<6;i++)
		{
			(coords.y)--;
			CHECK
			disp_write(coords,D_ON);
		}
		(coords.y)++;
		(coords.x)--;
		CHECK
		disp_write(coords,D_ON);
		break;
	case '2':
	    for (i = 0; i < 5; i++)
	    {
	    	CHECK
	        disp_write(coords, D_ON);
	        (coords.x)++;
	    }
	    (coords.x)-=5;
	    for (i = 0; i < 4; i++)
	    {
	        (coords.y)--;
	        (coords.x)++;
	        CHECK
	        disp_write(coords, D_ON);
	    }
	    (coords.y)--;
	    CHECK
        disp_write(coords, D_ON);
        (coords.y)--;
	    for (i = 0; i < 3; i++)
	    {
	        (coords.x)--;
	        CHECK
	        disp_write(coords, D_ON);
	    }
	    (coords.x)--;
	    (coords.y)++;
	    CHECK
        disp_write(coords, D_ON);
	    break;
	case '3':
	    for (i = 0; i < 4; i++)
	    {
	    	CHECK
	        disp_write(coords, D_ON);
	        (coords.x)++;
	    }
	    (coords.y)--;
	    for (i = 0; i < 2; i++)
	    {
	    	CHECK
	        disp_write(coords, D_ON);
	        (coords.y)--;
	    }
	    for (i = 0; i < 2; i++)
	    {
	        (coords.x)--;
	        CHECK
	        disp_write(coords, D_ON);
	    }
	    (coords.x)+=2;
	    for (i = 0; i < 2; i++)
	    {
	        (coords.y)--;
	        CHECK
	        disp_write(coords, D_ON);
	    }
	    (coords.y)--;
	    for (i = 0; i < 4; i++)
	    {
	        (coords.x)--;
	        CHECK
	        disp_write(coords, D_ON);
	    }
	    break;
	case '4':
		(coords.x)+=3;
	    for (i = 0; i < 7; i++)
	    {
	    	CHECK
	        disp_write(coords, D_ON);
	        (coords.y)--;
	    }
	    (coords.y)++;
	    for (i = 0; i < 3; i++)
	    {
	        (coords.x)--;
	        (coords.y)++;
	        CHECK
	        disp_write(coords, D_ON);
	    }
	    for (i = 0; i < 4; i++)
	    {
			(coords.x)++;
	    	if(i!=2)
	    	{
	    		CHECK
				disp_write(coords, D_ON);
	    	}
	    }
	    break;
	case '5':
	    for (i = 0; i < 5; i++)
	    {
	    	CHECK
	        disp_write(coords, D_ON);
	        (coords.x)++;
	    }
	    (coords.x)--;
	    for (i = 0; i < 3; i++)
	    {
	        (coords.y)--;
	        CHECK
	        disp_write(coords, D_ON);
	    }
	    for (i = 0; i < 4; i++)
	    {
	        (coords.x)--;
	        CHECK
	        disp_write(coords, D_ON);
	    }
	    for (i = 0; i < 3; i++)
	    {
	        (coords.y)--;
	        CHECK
	        disp_write(coords, D_ON);
	    }
	    for (i = 0; i < 4; i++)
	    {
	        (coords.x)++;
	        CHECK
	        disp_write(coords, D_ON);
	    }
	    break;
	case '6':
		for (i = 0; i < 5; i++)
		{
			CHECK
			disp_write(coords, D_ON);
			(coords.x)++;
		}
		(coords.x)--;
		for (i = 0; i < 3; i++)
		{
			(coords.y)--;
			CHECK
			disp_write(coords, D_ON);
		}
		for (i = 0; i < 4; i++)
		{
			(coords.x)--;
			CHECK
			disp_write(coords, D_ON);
		}
		for (i = 0; i < 2; i++)
		{
			(coords.y)++;
			CHECK
			disp_write(coords, D_ON);
		}
		(coords.y)-=2;
		for (i = 0; i < 3; i++)
		{
			(coords.y)--;
			CHECK
			disp_write(coords, D_ON);
		}
		for (i = 0; i < 4; i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords, D_ON);
		}
		break;
	case '7':
		for (i = 0; i < 5; i++)
		{
			CHECK
			disp_write(coords, D_ON);
			(coords.x)++;
			(coords.y)--;
		}
		(coords.x)--;
		CHECK
		disp_write(coords, D_ON);
		(coords.y)--;
		for (i = 0; i < 4; i++)
		{
			CHECK
			disp_write(coords, D_ON);
			(coords.x)--;
		}
		break;
	case '8':
		for (i = 0; i < 3; i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords, D_ON);
		}
		(coords.x)++;
		for (i = 0; i < 5; i++)
		{
			(coords.y)--;
			CHECK
			disp_write(coords, D_ON);
		}
		(coords.y)--;
		for (i = 0; i < 3; i++)
		{
			(coords.x)--;
			CHECK
			disp_write(coords, D_ON);
		}
		(coords.x)--;
		for (i = 0; i < 5; i++)
		{
			(coords.y)++;
			CHECK
			disp_write(coords, D_ON);
		}
		(coords.y)-=2;
		for (i = 0; i < 3; i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords, D_ON);
		}
		break;
	case '9':
		for (i = 0; i < 4; i++)
		{
			CHECK
			disp_write(coords, D_ON);
			(coords.x)++;
		}
		for (i = 0; i < 5; i++)
		{
			(coords.y)--;
			if(i!=2)
			{
				CHECK
				disp_write(coords, D_ON);
			}
		}
		(coords.y)--;
		for (i = 0; i < 3; i++)
		{
			(coords.x)--;
			CHECK
			disp_write(coords, D_ON);
		}
		(coords.x)--;
		for (i = 0; i < 2; i++)
		{
			(coords.y)++;
			CHECK
			disp_write(coords, D_ON);
		}
		(coords.y)++;
		for (i = 0; i < 3; i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords, D_ON);
		}
		break;
	}
	disp_update();
}



void letras_off (dcoord_t coords,char let)//igual que on pero para apagar los leds encendidos
{
	int i;
	switch(let)
	{
	case 'A':
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_OFF);
		}
		for(i=0;i<4;i++)
		{
			(coords.y)++;
			CHECK
			disp_write(coords,D_OFF);
		}
		coords.y-=2;
		coords.x--;
		CHECK
		disp_write(coords,D_OFF);
		break;
	case 'P':
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_OFF);
		}
		for(i=0;i<2;i++)
		{
			(coords.y)++;
			CHECK
			disp_write(coords,D_OFF);
		}
		(coords.x)--;
		CHECK
		disp_write(coords,D_OFF);
		break;
	case 'L':
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.y)--;
		}
		coords.y+=5;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_OFF);
		}
		break;
	case 'Y':
		coords.x++;
		for(i=0;i<4;i++)
		{
			disp_write(coords,D_OFF);
			(coords.y)--;
		}
		(coords.x)--;
		disp_write(coords,D_OFF);
		(coords.x)+=2;
		disp_write(coords,D_OFF);
		break;
	case 'T':
		coords.x++;
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.y)--;
		}
		(coords.y)++;
		(coords.x)--;
		CHECK
		disp_write(coords,D_OFF);
		(coords.x)+=2;
		CHECK
		disp_write(coords,D_OFF);
		break;
	case 'O':
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_OFF);
		}
		for(i=0;i<4;i++)
		{
			(coords.y)++;
			CHECK
			disp_write(coords,D_OFF);
		}
		(coords.x)--;
		CHECK
		disp_write(coords,D_OFF);
		break;
	case 'S':
		for(i=0;i<3;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.x)++;
		}
		(coords.x)--;
		for(i=0;i<2;i++)
		{
			(coords.y)--;
			CHECK
			disp_write(coords,D_OFF);
		}
		for(i=0;i<2;i++)
		{
			(coords.x)--;
			CHECK
			disp_write(coords,D_OFF);
		}
		for(i=0;i<2;i++)
		{
			(coords.y)--;
			CHECK
			disp_write(coords,D_OFF);
		}
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_OFF);
		}
		break;
	case 'C':
		for(i=0;i<5;i++)
		{
			disp_write(coords,D_OFF);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			disp_write(coords,D_OFF);
		}
		for(i=0;i<4;i++)
		{
			(coords.y)++;
		}
		disp_write(coords,D_OFF);
		(coords.x)--;
		disp_write(coords,D_OFF);
		break;
	case 'R':
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_OFF);
		}
		(coords.y)++;
		CHECK
		disp_write(coords,D_OFF);
		(coords.y)++;
		(coords.x)--;
		CHECK
		disp_write(coords,D_OFF);
		(coords.x)++;
		for(i=0;i<2;i++)
		{
			(coords.y)++;
			CHECK
			disp_write(coords,D_OFF);
		}
		break;
	case 'G':
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.y)--;
		}
		(coords.y)++;
		for(i=0;i<3;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_OFF);
		}
		(coords.y)+=2;
		(coords.x)--;
		CHECK
		disp_write(coords,D_OFF);
		(coords.x)++;
		for(i=0;i<3;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.y)++;
		}
		(coords.y)--;
		(coords.x)--;
		CHECK
		disp_write(coords,D_OFF);
		(coords.x)--;
		CHECK
		disp_write(coords,D_OFF);
		break;
	case 'E':
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_OFF);
		}
		(coords.x)-=2;
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.y)--;
		}
		(coords.y)+=3;
		for(i=0;i<2;i++)
		{
			(coords.x)++;
			CHECK
			disp_write(coords,D_OFF);
		}
		(coords.y)-=2;
		for(i=0;i<2;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.x)--;
		}
		break;
	case 'V':
		(coords.y)-=4;
		for(i=0;i<3;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.y)++;
		}
		(coords.x)++;
		for(i=0;i<2;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.y)++;
		}
		(coords.x)++;
		(coords.y)--;
		for(i=0;i<2;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.y)--;
		}
		(coords.x)++;
		for(i=0;i<3;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.y)--;
		}
		break;
	case 'M':
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.y)--;
		}
		(coords.y)+=2;
		(coords.x)++;
		CHECK
		disp_write(coords,D_OFF);
		(coords.y)--;
		(coords.x)++;
		for(i=0;i<5;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.y)++;
		}
		break;
	case 'I':
		for(i=0;i<3;i++)
		{
			CHECK
			disp_write(coords,D_OFF);
			(coords.x)++;
		}
		(coords.x)-=2;
		for(i=0;i<4;i++)
		{
			(coords.y)--;
			CHECK
			disp_write(coords,D_OFF);
		}
		(coords.x)--;
		CHECK
		disp_write(coords,D_OFF);
		(coords.x)+=2;
		CHECK
		disp_write(coords,D_OFF);
		break;
	case '0':
			(coords.y)--;
			for(i=0;i<5;i++)
			{
				CHECK
				disp_write(coords,D_OFF);
				(coords.y)--;
			}
			for(i=0;i<3;i++)
			{
				(coords.x)++;
				CHECK
				disp_write(coords,D_OFF);
			}
			(coords.x)++;
			for(i=0;i<5;i++)
			{
				(coords.y)++;
				CHECK
				disp_write(coords,D_OFF);
			}
			(coords.y)++;
			for(i=0;i<3;i++)
			{
				(coords.x)--;
				CHECK
				disp_write(coords,D_OFF);
			}
			(coords.y)-=4;
			CHECK
			disp_write(coords,D_OFF);
			for(i=0;i<2;i++)
			{
				(coords.y)++;
				(coords.x)++;
				CHECK
				disp_write(coords,D_OFF);
			}
			break;
		case '1':
			for(i=0;i<3;i++)
			{
				(coords.x)++;
				CHECK
				disp_write(coords,D_OFF);
			}
			(coords.x)--;
			for(i=0;i<6;i++)
			{
				(coords.y)--;
				CHECK
				disp_write(coords,D_OFF);
			}
			(coords.y)++;
			(coords.x)--;
			CHECK
			disp_write(coords,D_OFF);
			break;
		case '2':
		    for (i = 0; i < 5; i++)
		    {
		    	CHECK
		        disp_write(coords, D_OFF);
		        (coords.x)++;
		    }
		    (coords.x)-=5;
		    for (i = 0; i < 4; i++)
		    {
		        (coords.y)--;
		        (coords.x)++;
		        CHECK
		        disp_write(coords, D_OFF);
		    }
		    (coords.y)--;
		    CHECK
	        disp_write(coords, D_OFF);
	        (coords.y)--;
		    for (i = 0; i < 3; i++)
		    {
		        (coords.x)--;
		        CHECK
		        disp_write(coords, D_OFF);
		    }
		    (coords.x)--;
		    (coords.y)++;
		    CHECK
	        disp_write(coords, D_OFF);
		    break;
		case '3':
		    for (i = 0; i < 4; i++)
		    {
		    	CHECK
		        disp_write(coords, D_OFF);
		        (coords.x)++;
		    }
		    (coords.y)--;
		    for (i = 0; i < 2; i++)
		    {
		    	CHECK
		        disp_write(coords, D_OFF);
		        (coords.y)--;
		    }
		    for (i = 0; i < 2; i++)
		    {
		        (coords.x)--;
		        CHECK
		        disp_write(coords, D_OFF);
		    }
		    (coords.x)+=2;
		    for (i = 0; i < 2; i++)
		    {
		        (coords.y)--;
		        CHECK
		        disp_write(coords, D_OFF);
		    }
		    (coords.y)--;
		    for (i = 0; i < 4; i++)
		    {
		        (coords.x)--;
		        CHECK
		        disp_write(coords, D_OFF);
		    }
		    break;
		case '4':
			(coords.x)+=3;
		    for (i = 0; i < 7; i++)
		    {
		    	CHECK
		        disp_write(coords, D_OFF);
		        (coords.y)--;
		    }
		    (coords.y)++;
		    for (i = 0; i < 3; i++)
		    {
		        (coords.x)--;
		        (coords.y)++;
		        CHECK
		        disp_write(coords, D_OFF);
		    }
		    for (i = 0; i < 4; i++)
		    {
				(coords.x)++;
		    	if(i!=2)
		    	{
		    		CHECK
					disp_write(coords, D_OFF);
		    	}
		    }
		    break;
		case '5':
		    for (i = 0; i < 5; i++)
		    {
		    	CHECK
		        disp_write(coords, D_OFF);
		        (coords.x)++;
		    }
		    (coords.x)--;
		    for (i = 0; i < 3; i++)
		    {
		        (coords.y)--;
		        CHECK
		        disp_write(coords, D_OFF);
		    }
		    for (i = 0; i < 4; i++)
		    {
		        (coords.x)--;
		        CHECK
		        disp_write(coords, D_OFF);
		    }
		    for (i = 0; i < 3; i++)
		    {
		        (coords.y)--;
		        CHECK
		        disp_write(coords, D_OFF);
		    }
		    for (i = 0; i < 4; i++)
		    {
		        (coords.x)++;
		        CHECK
		        disp_write(coords, D_OFF);
		    }
		    break;
		case '6':
			for (i = 0; i < 5; i++)
			{
				CHECK
				disp_write(coords, D_OFF);
				(coords.x)++;
			}
			(coords.x)--;
			for (i = 0; i < 3; i++)
			{
				(coords.y)--;
				CHECK
				disp_write(coords, D_OFF);
			}
			for (i = 0; i < 4; i++)
			{
				(coords.x)--;
				CHECK
				disp_write(coords, D_OFF);
			}
			for (i = 0; i < 2; i++)
			{
				(coords.y)++;
				CHECK
				disp_write(coords, D_OFF);
			}
			(coords.y)-=2;
			for (i = 0; i < 3; i++)
			{
				(coords.y)--;
				CHECK
				disp_write(coords, D_OFF);
			}
			for (i = 0; i < 4; i++)
			{
				(coords.x)++;
				CHECK
				disp_write(coords, D_OFF);
			}
			break;
		case '7':
			for (i = 0; i < 5; i++)
			{
				CHECK
				disp_write(coords, D_OFF);
				(coords.x)++;
				(coords.y)--;
			}
			(coords.x)--;
			CHECK
			disp_write(coords, D_OFF);
			(coords.y)--;
			for (i = 0; i < 4; i++)
			{
				CHECK
				disp_write(coords, D_OFF);
				(coords.x)--;
			}
			break;
		case '8':
			for (i = 0; i < 3; i++)
			{
				(coords.x)++;
				CHECK
				disp_write(coords, D_OFF);
			}
			(coords.x)++;
			for (i = 0; i < 5; i++)
			{
				(coords.y)--;
				CHECK
				disp_write(coords, D_OFF);
			}
			(coords.y)--;
			for (i = 0; i < 3; i++)
			{
				(coords.x)--;
				CHECK
				disp_write(coords, D_OFF);
			}
			(coords.x)--;
			for (i = 0; i < 5; i++)
			{
				(coords.y)++;
				CHECK
				disp_write(coords, D_OFF);
			}
			(coords.y)-=2;
			for (i = 0; i < 3; i++)
			{
				(coords.x)++;
				CHECK
				disp_write(coords, D_OFF);
			}
			break;
		case '9':
			for (i = 0; i < 4; i++)
			{
				CHECK
				disp_write(coords, D_OFF);
				(coords.x)++;
			}
			for (i = 0; i < 5; i++)
			{
				(coords.y)--;
				if(i!=2)
				{
					CHECK
					disp_write(coords, D_OFF);
				}
			}
			(coords.y)--;
			for (i = 0; i < 3; i++)
			{
				(coords.x)--;
				CHECK
				disp_write(coords, D_OFF);
			}
			(coords.x)--;
			for (i = 0; i < 2; i++)
			{
				(coords.y)++;
				CHECK
				disp_write(coords, D_OFF);
			}
			(coords.y)++;
			for (i = 0; i < 3; i++)
			{
				(coords.x)++;
				CHECK
				disp_write(coords, D_OFF);
			}
			break;
	}
	disp_update();
}

