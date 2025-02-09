#SWITCH DE COMPILACIÓN:
#	make pc
#	make raspberry


#Compilador
CC := gcc

#Opciones de compilación
OPTIONS := -Wall -Wextra -g

# Definiciones para PC y Raspberry
DEFINES_PC := -D BOARD_HEIGHT=20 -D BOARD_WIDTH=10
DEFINES_RASP := -D BOARD_HEIGHT=16 -D BOARD_WIDTH=10

#Archivos fuente de pc y raspberry
SRC_PC := backend.c
SRC_RASP := backend.c letras.c

#Archivos objeto de pc y raspberry
OBJS_PC := backend_pc.o
OBJS_RASP := backend_rasp.o letras.o Libs/disdrv.o Libs/joydrv.o

#Headers para pc y raspberry
HEAD_PC := backend.h
HEAD_RASP := backend.h letras.h

#Librerías de Allegro y Raspberry
LIB_ALLEGRO := -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec
LIB_RASP := 

#Regla principal
all: pc raspberry

pc: front-pc.c $(OBJS_PC) $(HEAD_PC)
	$(CC) $(OPTIONS) $(DEFINES_PC) $(OBJS_PC) front-pc.c -o pc $(LIB_ALLEGRO)
	
raspberry: front-disp.c $(OBJS_RASP) $(HEAD_RASP)
	$(CC) $(OPTIONS) $(DEFINES_RASP) $(OBJS_RASP) front-disp.c -o raspberry $(LIB_RASP)
	
# Compilación de backend.c con diferentes defines
backend_pc.o: backend.c backend.h
	$(CC) $(OPTIONS) $(DEFINES_PC) -c backend.c -o backend_pc.o

backend_rasp.o: backend.c backend.h
	$(CC) $(OPTIONS) $(DEFINES_RASP) -c backend.c -o backend_rasp.o
	
letras.o: letras.c letras.h
	$(CC) $(OPTIONS) letras.c -c

clean:
	rm -f $(OBJ)	#Borra todos los archivos objetos sin pedir confirmación
	
# Recompilar todo
rebuild: clean all
