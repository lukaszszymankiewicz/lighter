CC = gcc
CFLAGS := --std=c99 -Wall -g
LINKS = `pkg-config --cflags --libs sdl2 SDL2_image` 
LIBS = -lm
OBJ = lighter
TARGET =                \
	src/events.c        \
	src/gfx.c           \
	src/hero.c          \
	src/intersection.c  \
	src/level.c         \
	src/light.c         \
	src/lighter.c       \
	src/lightpt.c       \
	src/segment.c       \
	src/sprites.c       \
	src/tile.c          \

all:
	$(TARGET)

lighter: 
	$(CC) $(CFLAGS) $(TARGET) -o $(OBJ) $(LINKS) $(LIBS)

clean:
	$(RM) $(OBJ)
