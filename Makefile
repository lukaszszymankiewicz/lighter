CC = gcc
CFLAGS := --std=c99 -Wall
LINKS = `pkg-config --cflags --libs sdl2 SDL2_image` 
LIBS = -lm
TARGET = lighter

all:
	$(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET) $(LINKS) $(LIBS)

clean:
	$(RM) $(TARGET)
