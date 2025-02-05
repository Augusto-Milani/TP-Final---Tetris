#SWITCH DE COMPILACIÓN:
#	make pc
#	make raspberry


#Compilador
CC := gcc

#Opciones de compilación
OPTIONS := -Wall -Wextra -g

#Archivos fuente de pc y raspberry
SRC_PC := backend.c
SRC_RASP := backend.c letras.c

#Archivos objeto de pc y raspberry
OBJS_PC := $(SRC_PC:.c=.o)
OBJS_RASP := $(SRC_RASP:.c=.o)

#Headers para pc y raspberry
HEAD_PC := backend.h
HEAD_RASP := backend.h letras.h

#Librerías de Allegro y Raspberry
LIB_ALLEGRO := -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec
LIB_RASP := -ldisdrv -ljoydrv

#Regla principal
all: pc raspberry

pc: front-pc.c $(OBJS_PC) $(HEAD_PC)
	$(CC) $(OPTIONS) $(OBJS_PC) front-pc.c -o pc $(LIB_ALLEGRO)
	
raspberry: front-disp.c $(OBJS_RASP) $(HEAD_RASP)
	$(CC) $(OPTIONS) $(OBJS_RASP) front-disp.c -o raspberry $(LIB_RASP)
	
#Compilar archivos .c a .o
backend.o: backend.c backend.h pieces.h
	$(CC) $(OPTIONS) backend.c -c
	
letras.o: letras.c letras.h
	$(CC) $(OPTIONS) letras.c -c

clean:
	rm -f $(OBJ)	#Borra todos los archivos objetos sin pedir confirmación
	
# Recompilar todo
rebuild: clean all
