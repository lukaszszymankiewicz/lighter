CC = gcc
CFLAGS := --std=c99 -Wall
LINKS = `pkg-config --cflags --libs sdl2 SDL2_image` 
LIBS = -lm
OBJ = lighter

TESTOBJ = check_tests_suite
TESTLINKS = `pkg-config --cflags --libs check sdl2 SDL2_image` 

SRCDIR = src
TESTDIR = tests

TARGET =                      \
	$(SRCDIR)/game.c          \
	$(SRCDIR)/gfx.c           \
	$(SRCDIR)/import.c        \
	$(SRCDIR)/events.c        \
	$(SRCDIR)/hero.c          \
	$(SRCDIR)/timer.c         \
	$(SRCDIR)/tile.c          \
	$(SRCDIR)/files.c         \
	$(SRCDIR)/level.c         \
	$(SRCDIR)/geometry.c      \
	$(SRCDIR)/sorted_list.c   \
	$(SRCDIR)/light.c         \
	$(SRCDIR)/vertex.c        \
	$(SRCDIR)/segment.c       \
	$(SRCDIR)/point.c         \

TESTTARGET =                        \
    $(TESTDIR)/check_tests.c        \
    $(TESTDIR)/check_events.c       \
	$(TESTDIR)/check_geometry.c     \
	$(TESTDIR)/check_gfx.c	        \
	$(TESTDIR)/check_import.c       \
	$(TESTDIR)/check_hero.c         \
	$(TESTDIR)/check_level.c        \
	$(TESTDIR)/check_light.c        \
	$(TESTDIR)/check_point.c        \
	$(TESTDIR)/check_segment.c      \
	$(TESTDIR)/check_sorted_list.c  \
	$(TESTDIR)/check_vertex.c       \
	$(SRCDIR)/events.c              \
	$(SRCDIR)/geometry.c            \
	$(SRCDIR)/gfx.c                 \
	$(SRCDIR)/import.c              \
	$(SRCDIR)/hero.c                \
	$(SRCDIR)/level.c               \
	$(SRCDIR)/light.c               \
	$(SRCDIR)/files.c               \
	$(SRCDIR)/point.c               \
	$(SRCDIR)/segment.c             \
	$(SRCDIR)/sorted_list.c         \
	$(SRCDIR)/timer.c               \
	$(SRCDIR)/tile.c                \
	$(SRCDIR)/vertex.c              \

all:
	$(TARGET)

.PHONY: lighter
lighter: 
	$(CC) $(CFLAGS) $(TARGET) -o $(OBJ) $(LINKS) $(LIBS)

.PHONY: tests
tests: 
	$(CC) $(CFLAGS) $(TESTTARGET) -o $(TESTOBJ) $(TESTLINKS) $(LIBS)

.PHONY: run_check_tests_suite
run_check_tests_suite:
	./$(TESTOBJ)

.PHONY: run_lighter
run_lighter:
	./$(OBJ)

.PHONY: run
run:
	make clean lighter run_lighter

.PHONY: runtests
runtests:
	make clean tests run_check_tests_suite

.PHONY: clean
clean:
	$(RM) $(OBJ) $(TESTOBJ)

