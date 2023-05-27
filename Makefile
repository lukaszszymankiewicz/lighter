CC = gcc
CFLAGS := --std=c99 -Wall -g
LINKS = `pkg-config --cflags --libs sdl2 SDL2_image` 
LIBS = -lGL -lGLU -lGLEW -lm 

FINAL_OBJ = lighter
FINAL_TEST_OBJ = lighter_tests

TESTOBJ = check_tests_suite
TESTLINKS = `pkg-config --cflags --libs check sdl2 SDL2_image` 

SRCDIR = src
TESTDIR = tests

LEVELLOG = level_read_log.txt
MEMORYLOG = memory_check.txt

BUILDDIR = build
DATADIR = data

clear_screen:
	clear

$(BUILDDIR)/%.o:	$(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/$(DATADIR)/%.o:	$(SRCDIR)/$(DATADIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o:	$(TESTDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

OBJS =                                     \
	$(BUILDDIR)/$(DATADIR)/entities.o      \
	$(BUILDDIR)/$(DATADIR)/levels.o        \
	$(BUILDDIR)/$(DATADIR)/library.o       \
	$(BUILDDIR)/$(DATADIR)/lightsources.o  \
	$(BUILDDIR)/$(DATADIR)/sprites.o       \
	$(BUILDDIR)/$(DATADIR)/tiles.o         \
	$(BUILDDIR)/$(DATADIR)/tilesets.o     \
	$(BUILDDIR)/$(DATADIR)/wobbles.o       \
	$(BUILDDIR)/animation.o                \
	$(BUILDDIR)/controller.o               \
	$(BUILDDIR)/entity.o                   \
	$(BUILDDIR)/entity_manager.o           \
	$(BUILDDIR)/game.o                     \
	$(BUILDDIR)/geometry.o                 \
	$(BUILDDIR)/gfx.o                      \
	$(BUILDDIR)/level.o                    \
	$(BUILDDIR)/light.o                    \
	$(BUILDDIR)/point.o                    \
	$(BUILDDIR)/segment.o                  \
	$(BUILDDIR)/sorted_list.o              \
	$(BUILDDIR)/source.o                   \
	$(BUILDDIR)/texture.o                  \
	$(BUILDDIR)/tile.o                     \
	$(BUILDDIR)/timer.o                    \
	$(BUILDDIR)/vertex.o                   \
    $(BUILDDIR)/img.o                      \

TESTOBJS =                                    \
	$(BUILDDIR)/$(DATADIR)/entities.o         \
	$(BUILDDIR)/$(DATADIR)/levels.o           \
	$(BUILDDIR)/$(DATADIR)/library.o          \
	$(BUILDDIR)/$(DATADIR)/lightsources.o     \
	$(BUILDDIR)/$(DATADIR)/sprites.o          \
	$(BUILDDIR)/$(DATADIR)/tiles.o            \
	$(BUILDDIR)/$(DATADIR)/tilesets.o         \
	$(BUILDDIR)/$(DATADIR)/wobbles.o          \
	$(BUILDDIR)/check_animation.o             \
	$(BUILDDIR)/check_entity.o                \
	$(BUILDDIR)/check_entity_manager.o        \
	$(BUILDDIR)/check_geometry.o              \
	$(BUILDDIR)/check_game.o                  \
	$(BUILDDIR)/check_level.o                 \
	$(BUILDDIR)/check_library.o               \
	$(BUILDDIR)/check_light.o                 \
	$(BUILDDIR)/check_point.o                 \
	$(BUILDDIR)/check_segment.o               \
	$(BUILDDIR)/check_sorted_list.o           \
	$(BUILDDIR)/check_source.o                \
	$(BUILDDIR)/check_vertex.o                \
    $(BUILDDIR)/check_controller.o            \
	$(BUILDDIR)/animation.o                   \
	$(BUILDDIR)/controller.o                  \
	$(BUILDDIR)/entity.o                      \
	$(BUILDDIR)/entity_manager.o              \
	$(BUILDDIR)/game.o                        \
	$(BUILDDIR)/geometry.o                    \
	$(BUILDDIR)/gfx.o                         \
	$(BUILDDIR)/img.o                         \
	$(BUILDDIR)/level.o                       \
	$(BUILDDIR)/light.o                       \
	$(BUILDDIR)/point.o                       \
	$(BUILDDIR)/segment.o                     \
	$(BUILDDIR)/sorted_list.o                 \
	$(BUILDDIR)/source.o                      \
	$(BUILDDIR)/texture.o                     \
	$(BUILDDIR)/tile.o                        \
	$(BUILDDIR)/timer.o                       \
	$(BUILDDIR)/vertex.o                      \


$(BUILDDIR)/lighter:	$(OBJS) $(BUILDDIR)/main.o
	$(CC) $(CFLAGS) $(OBJS) $(BUILDDIR)/main.o -o $(BUILDDIR)/$(FINAL_OBJ)  $(LINKS) $(LIBS)

$(BUILDDIR)/test_suite:	$(TESTOBJS) $(BUILDDIR)/check_tests.o
	$(CC) $(CFLAGS) $(TESTOBJS) $(BUILDDIR)/check_tests.o -o $(BUILDDIR)/$(FINAL_TEST_OBJ) $(TESTLINKS) $(LIBS)

clean:
	rm -f $(BUILDDIR)/*.o
	rm -f $(BUILDDIR)/$(FINAL_OBJ)
	rm -f $(BUILDDIR)/$(FINAL_TEST_OBJ)
	rm $(LEVELLOG)
	rm $(MEMORYLOG)

# TODO: add creating build/data folder

lighter:  $(BUILDDIR)/lighter

tests:	 $(BUILDDIR)/test_suite

run_lighter_tests:
	./$(BUILDDIR)/$(FINAL_TEST_OBJ)

run_lighter:
	./$(BUILDDIR)/$(FINAL_OBJ)

all: 
	make lighter run_lighter

debug: 
	gdb ./$(BUILDDIR)/$(FINAL_OBJ)

all_tests:
	make clear_screen tests run_lighter_tests

.PHONY: memory_check
memory_check:
	valgrind --leak-check=yes --log-file="memory_check.txt" --track-origins=yes ./lighter
