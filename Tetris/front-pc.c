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

#define TITLE_FILE 		"Assets/title.png"
#define BACKGROUND_FILE "Assets/background.png"
#define BLOCKS_FILE 	"Assets/blocks.png"
#define GAMEOVER_FILE	"Assets/game_over.png"

#define HARD_MODE 15	//Nivel en el que se empieza en el modo difícil.
#define IS_PRINTABLE(x) ( ('a'<=(x)&&(x)<='z') || ('A'<=(x)&&(x)<='Z') || ('0'<=(x)&&(x)<='9') )	//Devuelve 1 si es letra o número.

static struct {	// Facilito usar allegro sin pasar muchos argumentos entre funciones.
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
	bool flagMenu;				// Bandera para menú principal y pausa.
	bool flagPlay;				// Bandera para terminar programa.
	bool redraw;				// Bandera para dibujar.
} argument;


static unsigned int top;
static unsigned int TITLE_WIDTH, TITLE_HEIGHT, BACK_WIDTH, BACK_HEIGHT,
					TILE_WIDTH, TILE_HEIGHT, GAMEOVER_WIDTH, GAMEOVER_HEIGHT;
static unsigned int dx, dy;	//Centrar imagen en pantalla.
static float scale, move_x, move_y;

static void check_init(void *pointer, const char *name);
static void must_init (bool test, const char *description);

static void TetrisMenu();
static void TetrisPlay(const bool hardMode);
static void TetrisPause();
static void TetrisGameOver();

static void menuDraw();
static void playDraw();
static void newTimer(const unsigned int extraVelocity);
static unsigned int switchPieceID(int ID);
static void pauseDraw(const unsigned int index);
static void gameoverDraw(char key, int index, bool flag);


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


	/* Imágenes */
	argument.background = al_load_bitmap(BACKGROUND_FILE);
	check_init(argument.background, "background image");
	argument.title = al_load_bitmap(TITLE_FILE);
	check_init(argument.title, "title image");
	argument.blocks = al_load_bitmap(BLOCKS_FILE);
	check_init(argument.blocks, "blocks image");
	argument.gameover = al_load_bitmap(GAMEOVER_FILE);
	check_init(argument.gameover, "game over image");


	/* Cola de Eventos */
	argument.queue = al_create_event_queue();
	check_init(argument.queue, "queue");
	al_register_event_source(argument.queue, al_get_keyboard_event_source());
	al_register_event_source(argument.queue, al_get_display_event_source(display));
	al_register_event_source(argument.queue, al_get_mouse_event_source());


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
	move_x = TILE_WIDTH*scale;	//Cada bloque (tile) es una unidad de desplazamiento.
	move_y = TILE_HEIGHT*scale;


	/* Fuente de Texto */
	argument.font = al_load_font("Assets/nintendo-nes-font.ttf", move_y, 0);
	check_init(argument.font, "tetris font TTF");


	/* INICIA JUEGO */
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
    if(test) {
    	return;
    }

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
	bool hardMode = false;
	argument.flagMenu = false;
	menuDraw();
	al_flip_display();

	while( !(argument.flagMenu) ) {	//Mientras esté en pausa, no empieza.

		al_wait_for_event(argument.queue, &(argument.event));
		switch ((argument.event).type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				argument.flagMenu = true;	// Al cerrar el display, termina el programa.
				break;

			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				TetrisPlay(hardMode);
				break;

			case ALLEGRO_EVENT_KEY_CHAR:
				al_get_keyboard_state(&(argument.ks));

				if(al_key_down(&(argument.ks), ALLEGRO_KEY_ESCAPE)) {
					argument.flagMenu = true;	//Al presionar Esc, se detiene el programa.
				}
				/* Hard Mode */
				else if(al_key_down(&(argument.ks), ALLEGRO_KEY_H)) {
					argument.redraw = true;
					if(hardMode == false) {
						hardMode = true;
					}
					else {
						hardMode = false;
					}
				}
				else {
					TetrisPlay(hardMode);
				}
				break;

			default:
				break;
		}
		/* Rutina de Impresión */
		if(argument.redraw && !(argument.flagMenu) && al_is_event_queue_empty(argument.queue)) {
			argument.redraw = false;
			menuDraw();
			if(hardMode) {
				al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move_x*9, dy + move_y*20, 0, "HARD MODE!");
			}
			al_flip_display();
		}
	}
}

/* Imprimir el Menú del Juego */
static void menuDraw() {
	al_clear_to_color(al_map_rgb(0, 0, 0));		// Limpio pantalla con negro.
	al_draw_scaled_bitmap(argument.title, 0, 0, TITLE_WIDTH, TITLE_HEIGHT,
						  dx, dy,
						  TITLE_WIDTH * scale, TITLE_HEIGHT * scale, 0);
}



static void TetrisPlay(const bool hardMode) {
	unsigned int extraVelocity = 0;
	if(hardMode) {
		extraVelocity = HARD_MODE;
	}
	newTimer(extraVelocity);	//Crea, o destruye y crea un nuevo timer.

	unsigned int levelStatus = level;
	initBoard();	// Inicializa matriz en 0 y elige primera pieza.

	playDraw(false);	//Dibuja la pantalla, sin cambiar color de las piezas.
	alive = true;	// Flag para pantalla de game over. Se modifica en "backend.c".

	al_play_sample_instance(argument.sample_instance);	//Reproduce música.

	argument.flagPlay = false;
	while( !(argument.flagPlay) ) {

		al_wait_for_event(argument.queue, &(argument.event));
		switch ((argument.event).type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				argument.flagPlay = true;	// Al cerrar el display, termina el programa.
				break;

			case ALLEGRO_EVENT_TIMER:
				argument.redraw = true;

				if(shiftPieceDown(false)) {		// Desplaza la pieza hacia abajo en la matriz "board" sin sumar puntaje. Devuelve el estado de colisión, para saber si realizar el sonido.
					al_play_sample(argument.sfx8, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				if(levelStatus != level) {
					levelStatus = level;
					newTimer(extraVelocity);
				}
				if(alive == false) {
					argument.flagPlay = true;
					TetrisGameOver();
				}
				break;

			case ALLEGRO_EVENT_KEY_CHAR:
				al_get_keyboard_state(&(argument.ks));

				if(al_key_down(&(argument.ks), ALLEGRO_KEY_ESCAPE)) {
					argument.redraw = true;
					TetrisPause();
					
				}

				else {
					if(al_key_down(&(argument.ks), ALLEGRO_KEY_DOWN)) {
						argument.redraw = true;
						if(shiftPieceDown(true)) {		// Desplaza la pieza hacia abajo en la matriz "board", sumando puntaje. Devuelve el estado de colisión, para realizar el sonido.
							al_play_sample(argument.sfx8, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						}
					}
					else if(al_key_down(&(argument.ks), ALLEGRO_KEY_LEFT)) {
						al_play_sample(argument.sfx4, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						argument.redraw = true;
						shiftPieceLeft();	// Desplaza la pieza a la izquierda en la matriz "board".
					}
					else if(al_key_down(&(argument.ks), ALLEGRO_KEY_RIGHT)) {
						al_play_sample(argument.sfx4, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						argument.redraw = true;
						shiftPieceRight();	// Desplaza la pieza a la derecha en la matriz "board".
					}
					else if(al_key_down(&(argument.ks), ALLEGRO_KEY_SPACE)) {
						al_play_sample(argument.sfx6, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						argument.redraw = true;
						rotateClockwise();
					}
				}
				break;

			default:
				break;
		}

		/* Rutina de Impresión */
		if(argument.redraw && !(argument.flagPlay) && al_is_event_queue_empty(argument.queue)) {
			argument.redraw = false;
			playDraw();	//Dibuja la pantalla.
		}
	}
}

/* Iniciar y Actualizar Timer */
static void newTimer(const unsigned int extraVelocity) {

	// Si ya existe un timer, lo destruye.
	if(argument.timer != NULL) {
		al_destroy_timer(argument.timer);
	}

	// Crea un nuevo timer con otra dificultad, y lo agrega a la cola de eventos.
	argument.timer = al_create_timer( (double)3/(3+level+extraVelocity) );
	check_init(argument.timer, "timer");
	al_register_event_source(argument.queue, al_get_timer_event_source(argument.timer));
	al_start_timer(argument.timer);
}

/* Imprimir el Juego con la Matriz */
static void playDraw() {
	unsigned int i, j, tileColor, tileVariant;
	char str[7];

	// Limpio pantalla con negro.
	al_clear_to_color(al_map_rgb(0, 0, 0));
	// Imprimo background
	al_draw_scaled_bitmap(argument.background, 0, 0, BACK_WIDTH, BACK_HEIGHT,
								 dx, dy, BACK_WIDTH * scale, BACK_HEIGHT * scale,
								 0);
	// Paso int a string, usando sprintf,rellenando con determinada cantidad de ceros a la izquierda.
	// Luego, imprimo usando relaciones de escala para desplazamiento.
	sprintf(str, "%03d", lines);
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move_x*19, dy + move_y*2, 0, str);
	sprintf(str, "%06d", top);
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move_x*24, dy + move_y*4, 0, str);
	sprintf(str, "%06d", score);
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move_x*24, dy + move_y*7, 0, str);
	sprintf(str, "%02d", level);
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move_x*26, dy + move_y*20, 0, str);

	for(i=0 ; i<7 ; i++) {
		sprintf(str, "%03d", tetromino[i]);
		al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move_x*6.5, dy + move_y*(11+i*2), 0, str);
	}

	// Selección del color, en base al nivel. Ver archivo "blocks.png"
	tileColor = (level%10)*TILE_HEIGHT;

	// Imprime pieza siguiente
	for(i=0 ; i<4 ; i++) {
		for(j=0 ; j<4 ; j++) {

			if(nextPieceStatus[i][j]) {

				tileVariant = switchPieceID(nextPieceID);

				al_draw_scaled_bitmap(argument.blocks,	tileVariant, tileColor,	TILE_WIDTH, TILE_HEIGHT,
						dx + move_x*(24+j), dy + move_y*(13+i),	TILE_WIDTH * scale, TILE_HEIGHT * scale,	0);
			}
		}
	}
	// Imprime tablero
	for(i=0 ; i<BOARD_HEIGHT ; i++) {
		for(j=0 ; j<BOARD_WIDTH ; j++) {

			if(board[i][j] == 1) {	// Si cumple, es pieza móvil y hay que revisar su ID.

				tileVariant = switchPieceID(PieceID);	// El auxiliar tileVariant determina textura. Ver "blocks.png".

				al_draw_scaled_bitmap(argument.blocks,	tileVariant, tileColor,	TILE_WIDTH, TILE_HEIGHT,
									 dx + move_x*(12+j), dy + move_y*(5+i),	TILE_WIDTH * scale, TILE_HEIGHT * scale,	0);
			}

			else if(board[i][j] > 1) {	// Si cumple, es pieza estática y hay que revisar su valor exacto.

				tileVariant = switchPieceID(board[i][j] - 2);	// La correspondencia es igual al ID de la pieza, pero restada en 2 (por ser estática).

				al_draw_scaled_bitmap(argument.blocks,	tileVariant, tileColor,	TILE_WIDTH, TILE_HEIGHT,
									 dx + move_x*(12+j), dy + move_y*(5+i),	TILE_WIDTH * scale, TILE_HEIGHT * scale,	0);
			}
		}
	}
	// Refleja cambios en la pantalla
	al_flip_display();
}

/* Selección de Texturas para Piezas */
static unsigned int switchPieceID(int ID) {
	unsigned int tileVariant;
	switch(ID) {	//Nota: Ver "background.png" junto a "blocks.png" para ver correspondencias.
		case 0:			// El 0, 3 y 6 corresponden a piezas T, O e I.
		case 3:
		case 6:
			tileVariant = 0*TILE_WIDTH;
			break;
		case 1:			// El 1 y 4 corresponden a piezas J y S.
		case 4:
			tileVariant = 2*TILE_WIDTH;
			break;
		case 2:			// El 2 y 5 corresponden a piezas Z y L.
		case 5:
			tileVariant = 1*TILE_WIDTH;
			break;
		default:
			break;
	}
	return tileVariant;
}


static void TetrisPause() {
	int index = 0;

	// Detengo la reproducción de música, guardando su estado para poder resumirla
	unsigned int paused_position = al_get_sample_instance_position(argument.sample_instance);
	al_stop_sample_instance(argument.sample_instance);

	// Muestro pantalla de pausa con "CONTINUE" seleccionado por defecto (index = 0)
	pauseDraw(0);

	bool resume = false;	//	Bandera para resumir juego.
	while( !resume ) {

		al_wait_for_event(argument.queue, &(argument.event));
		switch ((argument.event).type) {

			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				resume = true;
				argument.flagPlay = true;	// Al cerrar el display, termina el programa.
				argument.flagMenu = true;
				break;

			case ALLEGRO_EVENT_KEY_CHAR:
				al_get_keyboard_state(&(argument.ks));

				if(al_key_down(&(argument.ks), ALLEGRO_KEY_ESCAPE)) {
					//Reanuda música y juego al presionar Esc
					al_set_sample_instance_position(argument.sample_instance, paused_position);
					al_play_sample_instance(argument.sample_instance);
					resume = true;
					argument.redraw = true;
				}
				else if(al_key_down(&(argument.ks), ALLEGRO_KEY_UP)) {
					// Cambia selección en menú pausa (hacia arriba)
					argument.redraw = true;
					index--;
				}
				else if(al_key_down(&(argument.ks), ALLEGRO_KEY_DOWN)) {
					// Cambia selección en menú pausa (hacia abajo)
					argument.redraw = true;
					index++;
				}
				else if(al_key_down(&(argument.ks), ALLEGRO_KEY_ENTER)) {
					// Selecciona la opción marcada, sabiendo el valor de index. Finaliza el menú de pausa.
					resume = true;
					switch(index) {

						case 0:	//CONTINUE
							al_set_sample_instance_position(argument.sample_instance, paused_position);
							al_play_sample_instance(argument.sample_instance);	//Reanuda música.
							argument.redraw = true;
							break;					//Continúa el juego.

						case 1:	//RESTART
							al_play_sample_instance(argument.sample_instance);	//Reinicia música.
							argument.redraw = true;
							initBoard();		// Inicializa matriz en 0 y elige primera pieza.
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

			default:
				break;
		}
		// Resivo casos límites para imprimir pantalla
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

/* Imprimir Menú de Pausa */
static void pauseDraw(const unsigned int index) {
	int pauseSelect[4] = {255, 255, 255, 255};
	pauseSelect[index] = 0;

	// Imprime background
	al_draw_scaled_bitmap(argument.background, 0, 0, BACK_WIDTH, BACK_HEIGHT,
									 dx, dy, BACK_WIDTH * scale, BACK_HEIGHT * scale, 0);

	// El index seleccionado va a imprimir en amarillo, el resto en blanco.
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move_x*17, dy + move_y*10, ALLEGRO_ALIGN_CENTRE, "PAUSE");
	al_draw_text(argument.font, al_map_rgb(255, 255, pauseSelect[0]), dx + move_x*12, dy + move_y*21, 0, "CONTINUE");
	al_draw_text(argument.font, al_map_rgb(255, 255, pauseSelect[1]), dx + move_x*12, dy + move_y*22, 0, "RESTART");
	al_draw_text(argument.font, al_map_rgb(255, 255, pauseSelect[2]), dx + move_x*12, dy + move_y*23, 0, "EXIT");
	al_draw_text(argument.font, al_map_rgb(255, 255, pauseSelect[3]), dx + move_x*12, dy + move_y*24, 0, "CLOSE");

	al_flip_display();
}



static void TetrisGameOver() {
	al_stop_sample_instance(argument.sample_instance); // Detener la reproducción de música
	gameoverDraw('-', 0, true);		// Imprime en pantalla el leaderboard.
	char key;
	unsigned int index = 0;

	bool resume = false;	// Bandera para salir del menú pausa.
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
					resume = true;
					argument.redraw = true;		// Vuelve al menú principal.
				}

				else if(al_key_down(&(argument.ks), ALLEGRO_KEY_BACKSPACE)) {
					if(index > 0)
					{
						index--;
					}
					gameoverDraw('-', index, false);		// Borra el último caracter presionado.
				}

				else {
					key = argument.event.keyboard.unichar;	// Registro el caracter presionado.
					if(IS_PRINTABLE(key)) {
						if(index < 6) {
							gameoverDraw(key, index++, false);		// Imprime el caracter con el puntaje obtenido.
						}
					}
				}

				break;

			default:
				break;
		}
	}
}

/* Imprimo Menú de Game Over */
static void gameoverDraw(char key, int index, bool flag) {
	static unsigned int score2, score3, levels[3];
	static signed aux;		// aux se utiliza para seleccionar el nombre que va a modificarse.
	static char str[3][7] = {"------",  "------", "------"};	// De mayor a menor, el nombre de los 3 mayores puntajes obtenidos.
	unsigned int i;
	char dest[19];	//string auxiliar

	if(flag) {
		if(score > score3) {		// Si cumple, es el tercer mejor puntaje obtenido.
			if(score > score2) {	// Si cumple, es el segundo mejor puntaje obtenido.
				if(score > top) {	// Si cumple, es el mejor puntaje obtenido.
					aux = 0;

					// Desplaza el leaderboard hacia abajo.
					score3 = score2;
					score2 = top;
					top = score;

					levels[2] = levels[1];
					levels[1] = levels[0];
					levels[0] = level;

					for(i=0 ; i<6 ; i++) {
						str[2][i] = str[1][i];
						str[1][i] = str[0][i];
						str[0][i] = '-';
					}
				}
				else {
					aux = 1;

					// Desplaza el leaderboard hacia abajo.
					score3 = score2;
					score2 = score;

					levels[2] = levels[1];
					levels[1] = level;

					for(i=0 ; i<6 ; i++) {
						str[2][i] = str[1][i];
						str[1][i] = '-';
					}
				}
			}
			else {
				aux = 2;

				// Desplaza el leaderboard hacia abajo.
				score3 = score;
				levels[2] = level;
				for(i=0 ; i<6 ; i++) {
					str[2][i] = '-';
				}
			}
		}
		else {	// Si no cumple, no aparece en el leaderboard.
			aux = -1;
		}
	}

	if(aux != -1) {
		str[aux][index] = key;
	}
	al_clear_to_color(al_map_rgb(0, 0, 0));		// Limpio pantalla con negro.
	al_draw_scaled_bitmap(argument.gameover,	0, 0,	GAMEOVER_WIDTH, GAMEOVER_HEIGHT,
						  dx, dy,	BACK_WIDTH * scale, BACK_HEIGHT * scale, 	0);

	// Paso varios datos a una misma string (usando snprintf), para imprimirlos.
	snprintf(dest, sizeof(dest), "1 %.6s %06d %02d", str[0], top, levels[0]);
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move_x*7, dy + move_y*19, 0, dest);
	snprintf(dest, sizeof(dest), "2 %.6s %06d %02d", str[1], score2, levels[1]);
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move_x*7, dy + move_y*21, 0, dest);
	snprintf(dest, sizeof(dest), "3 %.6s %06d %02d", str[2], score3, levels[2]);
	al_draw_text(argument.font, al_map_rgb(255, 255, 255), dx + move_x*7, dy + move_y*23, 0, dest);

	// Reflejo cambios en la pantalla
	al_flip_display();

	// Nota: Si no se supera ningún puntaje, no varía el scoreboard.

}

