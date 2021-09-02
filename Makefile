CC = gcc
CFLAGS := --std=c99 -Wall -g
LINKS = `pkg-config --cflags --libs sdl2 SDL2_image` 
LIBS = -lm
OBJ = lighter
TARGET =                \
	src/game.c          \
	src/gfx.c           \
	src/sprites.c       \
	src/events.c        \
	src/hero.c          \
	src/timer.c         \
	src/tile.c          \
	src/level.c         \
	src/geometry.c      \
	src/intersection.c  \
	src/light.c         \
	src/vertex.c        \
	src/obstacle.c      \

all:
	$(TARGET)

lighter: 
	$(CC) $(CFLAGS) $(TARGET) -o $(OBJ) $(LINKS) $(LIBS)

clean:
	$(RM) $(OBJ)
