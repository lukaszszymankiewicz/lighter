CC = gcc
CFLAGS := --std=c99 -Wall
LINKS = `pkg-config --cflags --libs sdl2 SDL2_image` 
LIBS = -lm
OBJ = lighter
TARGET = lighter.c tex.c

all:
	$(TARGET)

lighter: 
	$(CC) $(CFLAGS) $(TARGET) -o $(OBJ) $(LINKS) $(LIBS)

clean:
	$(RM) $(OBJ)
