CC = gcc
LINKER_FLAGS = `pkg-config --cflags --libs sdl2 SDL2_image` 
COMPILER_FLAGS = -w -lm
TARGET = lighter
SOURCES =               \
	src/game.c          \
	src/gfx.c           \
	src/import.c        \
	src/sprites.c       \
	src/events.c        \
	src/hero.c          \
	src/timer.c         \
	src/tile.c          \
	src/level.c         \
	src/geometry.c      \
	src/sorted_list.c   \
	src/light.c         \
	src/vertex.c        \
	src/segment.c       \
	src/point.c         \

.PHONY: run
run: 
	./$(TARGET)

.PHONY: lighter
$(TARGET) : $(SOURCES)
	$(CC) $(SOURCES) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(TARGET)

.PHONY: clean
clean:
	$(RM) $(TARGET)

.PHONY: all
all:
	make clean lighter run
