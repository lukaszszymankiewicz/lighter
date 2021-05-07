CC = gcc
CFLAGS := --std=c99 -Wall 
LIBS = `pkg-config --cflags --libs sdl2` 
TARGET = lighter

all:
	$(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET) $(LIBS)

clean:
	$(RM) $(TARGET)

# gcc --std=c99 -Wall lighter.c -o lighter `pkg-config --cflags --libs sdl2`
