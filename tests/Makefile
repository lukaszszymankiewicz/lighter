CC = gcc
CsegmentFLAGS := -Wall -g
TARGET =                     \
    check_tests.c            \
    check_events.c           \
	check_vertex.c           \
	check_light.c            \
	check_point.c            \
	check_segment.c          \
	check_gfx.c				 \
	check_level.c       	 \
	check_geometry.c       	 \
	check_hero.c       	     \
	check_sorted_list.c      \
	check_sprites.c          \
	../src/gfx.c             \
	../src/sprites.c         \
	../src/events.c          \
	../src/hero.c            \
	../src/timer.c           \
	../src/tile.c            \
	../src/level.c           \
	../src/geometry.c        \
	../src/sorted_list.c     \
	../src/light.c           \
	../src/vertex.c          \
	../src/segment.c         \
	../src/point.c           \

OBJ = check_tests_suite
LINKS = -lcheck -pthread -lcheck_pic -pthread -lrt -lm -lsubunit `pkg-config --cflags --libs sdl2 SDL2_image` 

all:
	$(TARGET)

tests: 
	$(CC) $(TARGET) $(CFLAGS) -o $(OBJ) $(LINKS)

clean:
	$(RM) $(OBJ)
