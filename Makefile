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
	src/segment.c       \
	src/obstacle.c      \
	src/point.c         \

SAMPLETARGET = \
	src/gfx.c           \
    src/light.c         \
    src/obstacle.c      \
	src/segment.c       \
	src/geometry.c      \
	src/intersection.c  \
    sample2.c           \
	src/vertex.c        \
	src/point.c         \

all:
	$(TARGET)

lighter: 
	$(CC) $(CFLAGS) $(TARGET) -o $(OBJ) $(LINKS) $(LIBS)

temp: 
	$(CC) $(CFLAGS) $(SAMPLETARGET) -o temp $(LINKS) $(LIBS)

clean:
	$(RM) $(OBJ)
