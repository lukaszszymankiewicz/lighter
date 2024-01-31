CC = gcc
CFLAGS := --std=c99 -Wall -g
LINKS = `pkg-config --cflags --libs sdl2 SDL2_image` 
LIBS = -lGL -lGLU -lGLEW -lm 

FINAL_OBJ = lighter

SRCDIR = src

MEMORYLOG = memory_check.txt

BUILDDIR = build
DATADIR = data
MODDIR = modules

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
	$(BUILDDIR)/$(DATADIR)/shaders.o       \
	$(BUILDDIR)/$(DATADIR)/tiles.o         \
	$(BUILDDIR)/$(DATADIR)/tilesets.o      \
	$(BUILDDIR)/$(DATADIR)/wobbles.o       \
	$(BUILDDIR)/$(MODDIR)/gfx_modules.o    \
	$(BUILDDIR)/animation.o                \
	$(BUILDDIR)/controller.o               \
	$(BUILDDIR)/components.o               \
	$(BUILDDIR)/entity.o                   \
	$(BUILDDIR)/entity_manager.o           \
	$(BUILDDIR)/game.o                     \
	$(BUILDDIR)/geometry.o                 \
	$(BUILDDIR)/gfx.o                      \
	$(BUILDDIR)/level.o                    \
	$(BUILDDIR)/level_manager.o            \
	$(BUILDDIR)/light.o                    \
	$(BUILDDIR)/mat.o                      \
    $(BUILDDIR)/render.o                   \
	$(BUILDDIR)/point.o                    \
	$(BUILDDIR)/segment.o                  \
	$(BUILDDIR)/scene.o                    \
	$(BUILDDIR)/sorted_list.o              \
	$(BUILDDIR)/source.o                   \
	$(BUILDDIR)/tile.o                     \
	$(BUILDDIR)/timer.o                    \
	$(BUILDDIR)/vertex.o                   \

$(BUILDDIR)/lighter:	$(OBJS) $(BUILDDIR)/main.o
	$(CC) $(CFLAGS) $(OBJS) $(BUILDDIR)/main.o -o $(BUILDDIR)/$(FINAL_OBJ)  $(LINKS) $(LIBS)

clean:
	rm -f $(BUILDDIR)/*.o
	rm -f $(BUILDDIR)/$(DATADIR)/*.o
	rm -f $(BUILDDIR)/$(MODDIR)/*.o
	rm -f $(BUILDDIR)/$(FINAL_OBJ)
	rm -f $(BUILDDIR)/$(FINAL_TEST_OBJ)
	rm $(MEMORYLOG)

# TODO: add creating build/data folder
lighter:  $(BUILDDIR)/lighter

run_lighter:
	./$(BUILDDIR)/$(FINAL_OBJ)

all: 
	make lighter run_lighter

debug: 
	gdb ./$(BUILDDIR)/$(FINAL_OBJ)

.PHONY: memory_check
memory_check:
	valgrind --leak-check=yes --log-file="memory_check.txt" --track-origins=yes ./build/lighter gfx_off one_frame
