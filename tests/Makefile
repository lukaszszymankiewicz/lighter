CC = gcc
CsegmentFLAGS := -Wall -g
TARGET =                   \
	check_gfx.c            \
	check_geometry.c       \
	check_lightpt.c        \
	check_segment.c        \
	check_sprites.c        \
	check_tests.c          \
	check_tile.c           \
	../src/gfx.c           \
	../src/geometry.c      \
	../src/intersection.c  \
	../src/level.c         \
	../src/light.c         \
	../src/lightpt.c       \
	../src/segment.c       \
	../src/sprites.c       \
	../src/tile.c          \

OBJ = check_tests_suite
LINKS = -lcheck -pthread -lcheck_pic -pthread -lrt -lm -lsubunit `pkg-config --cflags --libs sdl2 SDL2_image` 

all:
	$(TARGET)

tests: 
	$(CC) $(TARGET) $(CFLAGS) -o $(OBJ) $(LINKS)

clean:
	$(RM) $(OBJ)
