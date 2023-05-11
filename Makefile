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

OBJS =                               \
	$(BUILDDIR)/$(DATADIR)/library.o \
	$(BUILDDIR)/game.o               \
	$(BUILDDIR)/gfx.o                \
    $(BUILDDIR)/import.o             \
    $(BUILDDIR)/img.o                \
	$(BUILDDIR)/controller.o         \
	$(BUILDDIR)/entity.o             \
	$(BUILDDIR)/entity_manager.o     \
	$(BUILDDIR)/timer.o              \
	$(BUILDDIR)/texture.o            \
	$(BUILDDIR)/tile.o               \
	$(BUILDDIR)/files.o              \
	$(BUILDDIR)/level.o              \
	$(BUILDDIR)/geometry.o           \
	$(BUILDDIR)/sorted_list.o        \
	$(BUILDDIR)/light.o              \
	$(BUILDDIR)/vertex.o             \
	$(BUILDDIR)/source.o             \
	$(BUILDDIR)/segment.o            \
	$(BUILDDIR)/animation.o          \
	$(BUILDDIR)/point.o              \

TESTOBJS =                            \
	$(BUILDDIR)/$(DATADIR)/library.o  \
    $(BUILDDIR)/check_controller.o    \
	$(BUILDDIR)/check_geometry.o      \
	$(BUILDDIR)/check_animation.o     \
	$(BUILDDIR)/check_entity.o        \
	$(BUILDDIR)/check_light.o         \
	$(BUILDDIR)/check_point.o         \
	$(BUILDDIR)/check_segment.o       \
	$(BUILDDIR)/check_source.o        \
	$(BUILDDIR)/check_sorted_list.o   \
	$(BUILDDIR)/check_vertex.o        \
	$(BUILDDIR)/controller.o          \
	$(BUILDDIR)/geometry.o            \
	$(BUILDDIR)/gfx.o                 \
	$(BUILDDIR)/import.o              \
	$(BUILDDIR)/img.o                 \
	$(BUILDDIR)/entity.o              \
	$(BUILDDIR)/texture.o             \
	$(BUILDDIR)/level.o               \
	$(BUILDDIR)/light.o               \
	$(BUILDDIR)/files.o               \
	$(BUILDDIR)/point.o               \
	$(BUILDDIR)/segment.o             \
	$(BUILDDIR)/entity_manager.o      \
	$(BUILDDIR)/animation.o           \
	$(BUILDDIR)/source.o              \
	$(BUILDDIR)/sorted_list.o         \
	$(BUILDDIR)/timer.o               \
	$(BUILDDIR)/tile.o                \
	$(BUILDDIR)/vertex.o              \

	# $(BUILDDIR)/check_entity_manager.o       \
	# $(BUILDDIR)/check_level.o        \


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

lighter:  $(BUILDDIR)/lighter

tests:	 $(BUILDDIR)/test_suite

run_lighter_tests:
	./$(BUILDDIR)/$(FINAL_TEST_OBJ)

run_lighter:
	./$(BUILDDIR)/$(FINAL_OBJ)

all: 
	make lighter run_lighter

all_tests:
	make clear_screen tests run_lighter_tests

.PHONY: memory_check
memory_check:
	valgrind --leak-check=yes --log-file="memory_check.txt" --track-origins=yes ./lighter
