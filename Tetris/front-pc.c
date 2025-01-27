/***************************************************************************//**
  @file     front-pc.c
  @brief    Frontend del TPF Tetris, para PC
  @author   Augusto Milani
 ******************************************************************************/

#include "backend.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

typedef struct {
		ALLEGRO_EVENT event;			// Evento que ocurrió (ver switch).
		ALLEGRO_KEYBOARD_STATE ks;		// Registro del teclado.
		ALLEGRO_EVENT_QUEUE *queue;		// Registro de eventos.
		ALLEGRO_TIMER *timer;
		ALLEGRO_FONT *font;
		ALLEGRO_BITMAP *title;
		ALLEGRO_BITMAP *background;
		ALLEGRO_BITMAP *blocks;
		ALLEGRO_BITMAP *gameover;
		ALLEGRO_SAMPLE* sfx4;
		ALLEGRO_SAMPLE* sfx6;
		ALLEGRO_SAMPLE* sfx8;
		ALLEGRO_SAMPLE* music1;
		ALLEGRO_SAMPLE_INSTANCE *sample_instance;
		bool flagMenu;						// Bandera para menú principal y pausa.
		bool flagPlay;						// Bandera para terminar programa.
		bool redraw;					// Bandera para dibujar.
	} argument_t;

extern int score, top, lines, level, tetromino[7];
char str1[7] = "------";
char str2[7] = "------";
char str3[7] = "------";
bool alive = true;

static int TITLE_WIDTH, TITLE_HEIGHT, BACK_WIDTH, BACK_HEIGHT,
			TILE_WIDTH, TILE_HEIGHT, GAMEOVER_WIDTH, GAMEOVER_HEIGHT;
static int dx, dy, move;	//Centrar imagen en pantalla.
static float scale;
static argument_t argument;

#define FONT_SIZE 27
#define IS_LETTER(x) ( ('a'<=(x)&&(x)<='z') || ('A'<=(x)&&(x)<='Z') )

static void check_init(void *pointer, const char *name);
static void must_init (bool test, const char *description);

static void TetrisMenu();
static void TetrisPlay();
static void TetrisPause();
static void TetrisGameOver();

static void menuDraw();
static void playDraw();
static void pauseDraw(int index);
static void gameoverDraw();

/*
typedef struct {
	char matrix[16][10];	// Posición de las piezas.
	char next;		// Siguiente pieza.
	char level;		// Nivel de avance en el juego, cambia el color de las piezas.
	int score; 	//Puntaje acumulado por nivel y líneas completadas.
} info_t; */



#define TITLE_FILE 		"Assets/title.png"
#define BACKGROUND_FILE "Assets/background.png"
#define BLOCKS_FILE 	"Assets/blocks.png"
#define GAMEOVER_FILE	"Assets/game_over.png"

extern int board[BOARD_HEIGHT][BOARD_WIDTH];

int main() {
	srand(time(NULL));	//Semilla para función rand().

	/* Inicializo Allegro */
	must_init(al_init(), "allegro");
	must_init(al_install_keyboard(), "keyboard");
	must_init(al_install_mouse(), "mouse");
	must_init(al_init_font_addon(), "font");
	must_init(al_init_ttf_addon(), "ttf");
	must_init(al_init_image_addon(), "image addon");
	must_init(al_install_audio(), "audio");
	must_init(al_init_acodec_addon(), "audio codecs");
	must_init(al_reserve_samples(16), "reserve samples");


	/* Inicializo Display */
	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	ALLEGRO_DISPLAY *display = al_create_display(200, 200);	//Tamaño arbitrario
	check_init(display, "display");


	/* Música */
	argument.music1 = al_load_sample("Assets/Music1.wav");
	must_init(argument.music1, "music");

	argument.sample_instance = al_create_sample_instance(argument.music1);
	if ( !(argument.sample_instance) ) {
		printf("Error: No se pudo crear la instancia del sample.\n");
		al_destroy_sample(argument.music1);
		return -1;
	}

	al_attach_sample_instance_to_mixer(argument.sample_instance, al_get_default_mixer());
	al_set_sample_instance_playmode(argument.sample_instance, ALLEGRO_PLAYMODE_LOOP);


	/* Efectos de Sonido */
	argument.sfx4 = al_load_sample("Assets/SFX 4.wav");
	must_init(argument.sfx4, "SFX 4");
	argument.sfx6 = al_load_sample("Assets/SFX 6.wav");
	must_init(argument.sfx6, "SFX 6");
	argument.sfx8 = al_load_sample("Assets/SFX 8.wav");
	must_init(argument.sfx8, "SFX 8");

	/* Fuente de Texto */
	argument.font = al_load_font("Assets/nintendo-nes-font.ttf", FONT_SIZE, 0);
	check_init(argument.font, "tetris font TTF");

	/* Imágenes */
	argument.background = al_load_bitmap(BACKGROUND_FILE);
	check_init(argument.background, "background image");
	argument.title = al_load_bitmap(TITLE_FILE);
	check_init(argument.title, "title image");
	argument.blocks = al_load_bitmap(BLOCKS_FILE);
	check_init(argument.blocks, "blocks image");
	argument.gameover = al_load_bitmap(GAMEOVER_FILE);
	check_init(argument.gameover, "game over image");

	/* Timer */
	argument.timer = al_create_timer(1.0);	//Creo timer de 1 segundo
	check_init(argument.timer, "timer");

	/* Cola de Eventos */
	argument.queue = al_create_event_queue();
	check_init(argument.queue, "queue");
	al_register_event_source(argument.queue, al_get_keyboard_event_source());
	al_register_event_source(argument.queue, al_get_display_event_source(display));
	al_register_event_source(argument.queue, al_get_mouse_event_source());
	al_register_event_source(argument.queue, al_get_timer_event_source(argument.timer));


	/* Cálculo de Escala */
	int SCREEN_WIDTH = al_get_display_width(display);
	int SCREEN_HEIGHT = al_get_display_height(display);

	TITLE_WIDTH = al_get_bitmap_width(argument.title);
	TITLE_HEIGHT = al_get_bitmap_height(argument.title);

	BACK_WIDTH = al_get_bitmap_width(argument.background);
	BACK_HEIGHT = al_get_bitmap_height(argument.background);

	TILE_WIDTH = al_get_bitmap_width(argument.blocks)/4;
	TILE_HEIGHT = al_get_bitmap_height(argument.blocks)/10;

	GAMEOVER_WIDTH = al_get_bitmap_width(argument.gameover);
	GAMEOVER_HEIGHT = al_get_bitmap_height(argument.gameover);

	float scale_x = (float)SCREEN_WIDTH / BACK_HEIGHT;
	float scale_y = (float)SCREEN_HEIGHT / BACK_WIDTH;
	if(scale_x < scale_y) {
		scale = scale_x;
	}
	else {
		scale = scale_y;
	}

	/* Parámetros de Desplazamiento al Imprimir */
	dy = (SCREEN_HEIGHT - BACK_HEIGHT * scale) / 2;		//Centro la imagen verticalmente.
	dx = (SCREEN_WIDTH - BACK_WIDTH * scale) / 2;		//Centro la imagen horizontalmente.
	move = TILE_WIDTH*scale;	//Cada bloque (tile) es una unidad de desplazamiento.


	/* INICIA JUEGO */
	al_start_timer(argument.timer);
	TetrisMenu();

	/* Finalización del Programa */
	al_destroy_sample_instance(argument.sample_instance);
	al_destroy_sample(argument.sfx4);
	al_destroy_sample(argument.sfx6);
	al_destroy_sample(argument.sfx8);
	al_destroy_sample(argument.music1);
	al_destroy_font(argument.font);
	al_destroy_display(display);
	al_destroy_timer(argument.timer);
	al_destroy_event_queue(argument.queue);

	return 0;
}

/* Reviso si Allegro se inicializó correctamente */
static void must_init(bool test, const char *description) {
    if(test) return;

    printf("couldn't initialize %s.\n", description);
    exit(1);
}

/* Chequeo si los punteros son NULL */
static void check_init(void *pointer, const char *name) {
    if (!pointer) {
        fprintf(stderr, "couldn't iniatialize %s.\n", name);
        exit(-1);
    }
}

static void TetrisMenu() {
	menuDraw();
	argument.flagMenu = false;

	while( !(argument.flagMenu) ) {	//Mientras esté en pausa, no empieza.

		al_wait_for_event(argument.queue, &(argument.event));
		switch ((argument.event).type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				argument.flagMenu = true;	// Al cerrar el display, termina el programa.
				break;

			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				TetrisPlay();
				break;

			case ALLEGRO_EVENT_KEY_CHAR:
				al_get_keyboard_state(&(argument.ks));

				if(al_key_down(&(argument.ks), ALLEGRO_KEY_ESCAPE)) {
					argument.flagMenu = true;	//Al presionar Esc, se detiene el programa.
				}
				else {
					TetrisPlay();
				}
				break;

			default:
				break;
		}
		/* Rutina de Impresión */
		if(argument.redraw && !(argument.flagMenu) && al_is_event_queue_empty(argument.queue)) {
			argument.redraw = false;
			menuDraw();
		}
	}
}

/* Imprimir el Menú del Juego */
static void menuDraw() {
	al_clear_to_color(al_map_rgb(0, 0, 0));		// Limpio pantalla con negro.
	al_draw_scaled_bitmap(argument.title, 0, 0, TITLE_WIDTH, TITLE_HEIGHT,
						  dx, dy,
						  TITLE_WIDTH * scale, TITLE_HEIGHT * scale, 0);
	al_flip_display();
}



static void TetrisPlay() {
	initBoard();	// Inicializa matriz en 0.
	nextPiece();	// Selecciona aleatoriamente la siguiente pieza, y la coloca en la matriz "board".

	playDraw();
	alive = true;

	al_play_sample_instance(argument.sample_instance);	//Reproduce música

	argument.flagPlay = false;

	while( !(argument.flagPlay) ) {

		al_wait_for_event(argument.queue, &(argument.event));
		switch ((argument.event).type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				argument.flagPlay = true;	// Al cerrar el display, termina el programa.
				break;

			case ALLEGRO_EVENT_TIMER:
				shiftPieceDown();	// Desplaza la pieza hacia abajo en la matriz "board".
				argument.redraw = true;
				if(false) {
					al_play_sample(argument.sfx8, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				if(alive == false) {
					argument.flagPlay = true;
					TetrisGameOver();
				}
				break;

			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				al_play_sample(argument.sfx6, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				if((argument.event).mouse.button == 1) {	//Click izquierdo.
					//TODO backend (pieza gira izq)***************
				}
				else if((argument.event).mouse.button == 2) {	//Click derecho.
					rotateClockWise();
				}
				break;

			case ALLEGRO_EVENT_KEY_CHAR:
				al_get_keyboard_state(&(argument.ks));

				if(al_key_down(&(argument.ks), ALLEGRO_KEY_ESCAPE)) {
					TetrisPause();
				}

				else {
					if(al_key_down(&(argument.ks), ALLEGRO_KEY_LEFT)) {
						al_play_sample(argument.sfx4, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						//alive = false;
						argument.redraw = true;
						shiftPieceLeft();	// Desplaza la pieza a la izquierda en la matriz "board".
					}
					else if(al_key_down(&(argument.ks), ALLEGRO_KEY_RIGHT)) {
						al_play_sample(argument.sfx4, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						argument.redraw = true;
						shiftPieceRight();	// Desplaza la pieza a la derecha en la matriz "board".
					}
					else if(al_key_down(&(argument.ks), ALLEGRO_KEY_DOWN)) {
						argument.redraw = true;
						shiftPieceDown();	// Desplaza la pieza hacia abajo en la matriz "board".
						if(false) {
							al_play_sample(argument.sfx8, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						}
					}
				}
				break;

			default:
				break;
		}

		/* Rutina de Impresión */
		if(argument.redraw && !(argument.flagPlay) && al_is_event_queue_empty(argument.queue)) {
			argument.redraw = false;
			playDraw();
		}
	}
}

/* Imprimir el Juego con la Matriz */
static void playDraw() {
	al_clear_to_color(al_map_rgb(0, 0, 0));		// Limpio pantalla con negro.

	al_draw_scaled_bitmap(argument.background, 0, 0, BACK_WIDTH, BACK_HEIGHT,
								 dx, dy, BACK_WIDTH * scale, BACK_HEIGHT * scale,
								 0);
	int i, j;
	char str[7];	//TODO ver que no sea mayor al límite (%03, %06, %02)
	sprintf(str, "%03d", lines);
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move*19, dy +move*2, 0, str);
	sprintf(str, "%06d", top);
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move*24, dy +move*4, 0, str);
	sprintf(str, "%06d", score);
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move*24, dy +move*7, 0, str);
	sprintf(str, "%02d", level);
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move*26, dy +move*20, 0, str);

	for(i=0 ; i<7 ; i++) {
		sprintf(str, "%03d", tetromino[i]);
		al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move*6.5, dy +move*(11+i*2), 0, str);
	}

	for(i=0 ; i<4 ; i++) {
		for(j=0 ; j<4 ; j++) {
		al_draw_scaled_bitmap(argument.blocks,	0, 0,	TILE_WIDTH, TILE_HEIGHT,
				dx + move*(24+j), dy + move*(13+i),	TILE_WIDTH * scale, TILE_HEIGHT * scale,	0);
		}
	}
	for(i=0 ; i<BOARD_HEIGHT ; i++) {
		for(j=0 ; j<BOARD_WIDTH ; j++) {
			if(board[i][j]) {		// Imprime si no es nulo.
				al_draw_scaled_bitmap(argument.blocks,	0, 0,	TILE_WIDTH, TILE_HEIGHT,
									 dx + move*(12+j), dy + move*(5+i),	TILE_WIDTH * scale, TILE_HEIGHT * scale,	0);
			}
		}
	}
	al_flip_display();
}




static void TetrisPause() {

	unsigned int paused_position = al_get_sample_instance_position(argument.sample_instance);
	al_stop_sample_instance(argument.sample_instance); // Detener la reproducción de música.

	int index = 0;
	pauseDraw(index);

	al_flip_display();

	bool resume = false;
	while(!resume  ) {

		al_wait_for_event(argument.queue, &(argument.event));
		switch ((argument.event).type) {

			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				argument.flagPlay = true;	// Al cerrar el display, termina el programa.
				argument.flagMenu = true;
				break;

			case ALLEGRO_EVENT_KEY_CHAR:
				al_get_keyboard_state(&(argument.ks));

				if(al_key_down(&(argument.ks), ALLEGRO_KEY_ESCAPE)) {
					al_set_sample_instance_position(argument.sample_instance, paused_position);	//Reanuda música
					al_play_sample_instance(argument.sample_instance);
					resume = true;
					argument.redraw = true;
				}
				else if(al_key_down(&(argument.ks), ALLEGRO_KEY_UP)) {
					argument.redraw = true;
					index--;
				}
				else if(al_key_down(&(argument.ks), ALLEGRO_KEY_DOWN)) {
					argument.redraw = true;
					index++;
				}
				else if(al_key_down(&(argument.ks), ALLEGRO_KEY_ENTER)) {
					resume = true;
					switch(index) {

						case 0:	//CONTINUE
							al_set_sample_instance_position(argument.sample_instance, paused_position);
							al_play_sample_instance(argument.sample_instance);	//Reanuda música
							argument.redraw = true;
							break;					//Continúa el juego.

						case 1:	//RESTART
							al_play_sample_instance(argument.sample_instance);	//Reinicia música
							argument.redraw = true;
							//TODO backend acá?? ni idea	//Reinicia el juego
							break;

						case 2:	//EXIT
							argument.flagPlay = true;	//Vuelve al menú.
							argument.redraw = true;
							break;

						case 3:	//CLOSE
							argument.flagPlay = true;	//Cierra el programa.
							argument.flagMenu = true;
							break;

						default:
							break;
					}
				}
				break;

			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				argument.flagPlay = true;
				break;

			default:
				break;
		}
		if(argument.redraw && !resume && al_is_event_queue_empty(argument.queue)) {
			if(index < 0) {
				index = 3;
			}
			else if(index > 3) {
				index = 0;
			}

			pauseDraw(index);
			argument.redraw = false;
		}
	}
}

static void pauseDraw(int index) {
	int pauseSelect[4] = {255, 255, 255, 255};
	pauseSelect[index] = 0;

	al_draw_scaled_bitmap(argument.background, 0, 0, BACK_WIDTH, BACK_HEIGHT,
									 dx, dy, BACK_WIDTH * scale, BACK_HEIGHT * scale, 0);
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move*17, dy +move*10, ALLEGRO_ALIGN_CENTRE, "PAUSE");
	al_draw_text(argument.font, al_map_rgb(255, 255, pauseSelect[0]), dx + move*12, dy +move*21, 0, "CONTINUE");
	al_draw_text(argument.font, al_map_rgb(255, 255, pauseSelect[1]), dx + move*12, dy +move*22, 0, "RESTART");
	al_draw_text(argument.font, al_map_rgb(255, 255, pauseSelect[2]), dx + move*12, dy +move*23, 0, "EXIT");
	al_draw_text(argument.font, al_map_rgb(255, 255, pauseSelect[3]), dx + move*12, dy +move*24, 0, "CLOSE");
	//El index seleccionado va a imprimir en amarillo, el resto en blanco.

	al_flip_display();
}



static void TetrisGameOver() {
	al_stop_sample_instance(argument.sample_instance); // Detener la reproducción de música
	gameoverDraw();
	char key;
	int index = 0;

	bool resume = false;
	while( !resume ) {

		al_wait_for_event(argument.queue, &(argument.event));
		switch ((argument.event).type) {

			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				argument.flagPlay = true;	// Al cerrar el display, termina el programa.
				argument.flagMenu = true;
				resume = true;
				break;

			case ALLEGRO_EVENT_KEY_CHAR:
				al_get_keyboard_state(&(argument.ks));

				if(al_key_down(&(argument.ks), ALLEGRO_KEY_ENTER)) {
					//TODO score (llamo a backend?)
					int i;
					//falta un if preguntado si el puntaje es más alto
					for(i=0 ; i<6 ; i++) {
						str3[i] = str2[i];
						str2[i] = str1[i];
						str1[i] = '-';
					}

					resume = true;
					argument.redraw = true;
				}

				else if(al_key_down(&(argument.ks), ALLEGRO_KEY_BACKSPACE)) {

					if(index!=0)
					{
						index--;
					}
					str1[index] = '-';

					argument.redraw = true;
				}

				else {
					key = argument.event.keyboard.unichar;	// Registro el caracter presionado.
					if(IS_LETTER(key)) {
						if(index <= 5) {
							str1[index++] = key;	//TODO STACK SMASHING DETECTED AL PRESIONAR MUCHOS CARACTERES
							argument.redraw = true;
						}
					}
				}

				break;

			default:
				break;
		}
		if(argument.redraw && !resume && al_is_event_queue_empty(argument.queue)) {
			gameoverDraw();
			argument.redraw = false;
		}
	}
}

static void gameoverDraw() {
	al_clear_to_color(al_map_rgb(0, 0, 0));		// Limpio pantalla con negro.
	al_draw_scaled_bitmap(argument.gameover,	0, 0,	GAMEOVER_WIDTH, GAMEOVER_HEIGHT,
						  dx, dy,	BACK_WIDTH * scale, BACK_HEIGHT * scale, 	0);
	char dest[19];
	snprintf(dest, sizeof(dest), "1 %s %06d %02d", str1, score, level);
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move*7, dy +move*19, 0, dest);
	snprintf(dest, sizeof(dest), "2 %s %06d %02d", str2, score, level);
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move*7, dy +move*21, 0, dest);
	snprintf(dest, sizeof(dest), "3 %s %06d %02d", str3, score, level);
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move*7, dy +move*23, 0, dest);

	al_flip_display();
}


/* NOTAS
 *
 * dx y dy indican la esquina izquierda del background, por lo que me desplazo
 * por cantidad de píxeles (26.4 aproximadamente) para cada bloque.
 *
 * Divido por 4.5 y 11 para ajustar con exactitud en base a la paleta de
 * bloques en el PNG usado.
 *
 * La escala se ajusta de manera global una vez al principio, TODO ver
 * con distintas resoluciones.
 *
 * */
