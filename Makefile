CC = gcc
CFLAGS := --std=c99 -Wall 
LINKS = `pkg-config --cflags --libs sdl2 SDL2_image` 
TARGET = lighter

all:
	$(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET) $(LINKS)

clean:
	$(RM) $(TARGET)
