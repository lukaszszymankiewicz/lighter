CC = gcc
CFLAGS := --std=c99 -Wall
LINKS = `pkg-config --cflags --libs sdl2 SDL2_image` 
LIBS = -lm
OBJ = lighter
TARGET =            \
	events.c        \
	gfx.c           \
	hero.c          \
	intersection.c  \
	level.c         \
	light.c         \
	lighter.c       \
	lightpt.c       \
	segment.c       \
	sprites.c       \
	tile.c          \

all:
	$(TARGET)

lighter: 
	$(CC) $(CFLAGS) $(TARGET) -o $(OBJ) $(LINKS) $(LIBS)

clean:
	$(RM) $(OBJ)
