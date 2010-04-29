# Makefile
# Yeti 3d SDL for gp2x
# 13-11-05
#

PRG = yeti3dsdl
OBJ = main.o yeti.o data.o draw.o extra.o font.o game.o maps.o sprites.c model.c
SRC = main.c yeti.c data.c draw.c extra.c font.c game.c maps.c sprites.o model.o

CC = gcc
CFLAGS = -Wall $(SDL_CFLAGS)
LD = 
LDFLAGS = 
DEPEND = makedepend $(CFLAGS) $(SDL_CFLAGS)

SDL_CFLAGS := $(shell sdl-config --cflags)
SDL_LDFLAGS := $(shell sdl-config --libs)

.c.o:
	$(CC) $(CFLAGS) -c $*.c

all: $(OBJ)
	$(CC) $(OBJ) -o $(PRG) $(SDL_LDFLAGS)

compileinfo:
	$(CC) -Wall -O2 compileinfo.c -o compileinfo
	@echo
	@echo You can copy compileinfo into a PATH folder
	@echo

depend: $(SRC)
	$(DEPEND) $(SRC)
	
install:
	@echo
	@echo There is nothing to install ...
	@echo

clean:
	rm -f *.o
	rm -f $(PRG)


# DO NOT DELETE THIS LINE --

