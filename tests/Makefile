CC = gcc
CsegmentFLAGS := -Wall -g
TARGET =                     \
	fake.c   				 \
    check_tests.c            \
    check_events.c           \
	check_geometry.c       	 \
	check_gfx.c				 \
	check_hero.c       	     \
	check_level.c       	 \
	check_light.c            \
	check_point.c            \
	check_segment.c          \
	check_sorted_list.c      \
	check_sprites.c          \
	check_vertex.c           \
	../src/events.c          \
	../src/geometry.c        \
	../src/gfx.c             \
	../src/hero.c            \
	../src/level.c           \
	../src/light.c           \
	../src/point.c           \
	../src/segment.c         \
	../src/sorted_list.c     \
	../src/sprites.c         \
	../src/timer.c           \
	../src/tile.c            \
	../src/vertex.c          \

OBJ = check_tests_suite
LINKS = -lcheck -pthread -lcheck_pic -pthread -lrt -lm -lsubunit `pkg-config --cflags --libs sdl2 SDL2_image` 

all:
	$(TARGET)

run: 
	./$(OBJ)

tests: 
	$(CC) $(TARGET) $(CFLAGS) -o $(OBJ) $(LINKS)

clean:
	$(RM) $(OBJ)
