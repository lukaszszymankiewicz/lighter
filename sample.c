#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;

SDL_Window* GFX_init_window(int w, int h) {
    window = SDL_CreateWindow(
        "dddupa",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1366,
        768,
        SDL_WINDOW_FULLSCREEN
    );

    if (window == NULL) {
        printf("window cannot be created");
    }
    return window;
};

SDL_Renderer* GFX_init_renderer(SDL_Window* window) {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_RenderSetLogicalSize(renderer, 320, 640);

    if (renderer == NULL) {
        printf("renderer cannot be initialized!");
    }
    return renderer;
};

#define MAX_LEN 32
#define MAX_PATH_LEN 64
#define MAX_LEVEL_SIZE 5
#define MAX_TILE_PER_LEVEL 256
#define TILE_WIDTH 32
#define TILE_HEIGHT 32

static char LEVEL_FILE_EXTENSION[4] = ".txt";
static char LEVELS_FOLDER[MAX_PATH_LEN] = "levels/";
static char TILESET_FOLDER[MAX_PATH_LEN] = "tilesets/";
static char BASE_LEVEL_NAME[MAX_PATH_LEN] = "level_";
static char CURRENT_FOLDER[MAX_PATH_LEN] = "./";


typedef struct tileset {
  SDL_Texture *surface;
  int          width;
  int          height;
} tileset_t;

typedef struct tile {
  int          x;
  int          y;
  int          width;
  int          height;
  int          tileset_index;
} tile_t;

typedef struct level {
    int        size_x;
    int        size_y;
    tileset_t *tileset_array;
    tile_t    *tile_array;
} level_t;

level_t* LVL_new() {
    level_t *new_level = NULL;

    new_level = (level_t*)malloc(sizeof(level_t));
    new_level->tileset_array = NULL;
    new_level->tile_array = NULL;

    return new_level;
}

tile_t* TILE_new(int tileset_index, int x_offset, int y_offset) {
    tile_t *new_tile = NULL;

    new_tile = (tile_t*)malloc(sizeof(tile_t));

    new_tile->tileset_index = tileset_index;
    new_tile->x = x_offset;
    new_tile->y = y_offset;
    new_tile->width = TILE_WIDTH;
    new_tile->height = TILE_HEIGHT;

    return new_tile;
}

void LVL_set_tile_number(level_t *level, int tile_number) {
    level->tile_array = malloc(tile_number * sizeof(tile_t*));
}

void LVL_set_tileset_number(level_t *level, int tileset_number) {
    level->tileset_array = malloc(tileset_number * sizeof(tileset_t*));
}

void LVL_add_tile(
    level_t *level,
    int      tileset_index,
    int      tile_index,
    int      x_offset,
    int      y_offset
) {
    tile_t* tile = TILE_new(tileset_index, x_offset, y_offset);
    level->tile_array[tileset_index] = *tile;
}

void LVL_free(level_t* level) {
    free(level);
}

int find_integer_in_str(char string[], int *i){
    int   j                       = 0;
    int   k;                      
    char  buffer[MAX_LEVEL_SIZE];
    
    if (i == NULL) {
        k=0;
    }
    else {
        k=(*i);
    }

    while(!isspace(string[k]) && string[k] != '\0'){
        buffer[j++] = string[k++];
    }

    if (i != NULL) {
        (*i) = ++k;
    }

    return strtol(buffer, NULL, 10);
}

int find_str_in_str(char string[], char buffer[]){
    int k = 0;                      

    while(string[k] != '\0'){
        buffer[k] = string[k];
        k++;
    }

    return k;
}

tileset_t* init_texture(char *filepath) {
	SDL_Texture* new_texture = NULL;
    SDL_Surface* loaded_surface = NULL;

    loaded_surface = IMG_Load(filepath);
    SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0x80, 0xFF, 0xFF));

    new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);

    tileset_t* p = malloc(sizeof(tileset_t));

    p->surface = new_texture;
    p->width = loaded_surface->w;
    p->height = loaded_surface->h;

    SDL_FreeSurface(loaded_surface);

    return p;
};

void get_level_name(int level_id, char* level_file_path){
    char  level_number_in_str[MAX_LEN];

    //change level number to string
    snprintf(level_number_in_str, 10, "%d", level_id);

    strcat(level_file_path, LEVELS_FOLDER);
    strcat(level_file_path, BASE_LEVEL_NAME);
    strcat(level_file_path, level_number_in_str);
    strcat(level_file_path, LEVEL_FILE_EXTENSION);
}

void get_tileset_path(char* tileset_filename, char* level_file_path){
    strcat(level_file_path, CURRENT_FOLDER);
    strcat(level_file_path, TILESET_FOLDER);
    strcat(level_file_path, tileset_filename);

}

void read_next_line(char* line_buffer, FILE *file_pointer){
    fgets(line_buffer, MAX_LEN - 1, file_pointer);
    line_buffer[strcspn(line_buffer, "\n")] = 0;
}

int strings_are_equal(char* line_buffer,  char* label){
    return strcmp(label, line_buffer) == 0;
}

void validate_label(char* line_buffer, FILE *file_pointer, char* label){
    read_next_line(line_buffer, file_pointer);

    if (!strings_are_equal(line_buffer, label)) {
        fprintf(stderr, "inproper level structure-lack of label: %s", label);
    }
}

int main() {
    char     level_file_path[MAX_PATH_LEN] = "";
    int      i                               = 0;
    int      z                               = 0;
    int      level_n                         = 1;
    int      tile_number                     = 0;
    int      tilesets_number                 = 0;
    FILE    *file_pointer                    = NULL;
    level_t *level                           = NULL;
    char     line_buffer[MAX_LEN];
    char     buffer[MAX_PATH_LEN];
    char     secondary_buffer[MAX_PATH_LEN]  = "";

    level = LVL_new();
    get_level_name(level_n, level_file_path);
    file_pointer = fopen(level_file_path, "r");

    if (file_pointer == NULL) {
        printf("Failed: ");
    }

    window = GFX_init_window(320, 640);
    renderer = GFX_init_renderer(window);

    while (1) {
        // LEVEL
        validate_label(line_buffer, file_pointer, "LEVEL");

        // SIZE
        validate_label(line_buffer, file_pointer, "SIZE");
        level->size_x = find_integer_in_str(line_buffer, &i);
        level->size_y = find_integer_in_str(line_buffer, &i);
        read_next_line(line_buffer, file_pointer);
        validate_label(line_buffer, file_pointer, "ENDSIZE");

        // TILESETS
        validate_label(line_buffer, file_pointer, "TILESET");
        read_next_line(line_buffer, file_pointer);
        tilesets_number = find_integer_in_str(line_buffer, NULL);
        LVL_set_tileset_number(level, tilesets_number);
        read_next_line(line_buffer, file_pointer);

        while (!strings_are_equal(line_buffer, "ENDTILESET")) {
            find_str_in_str(line_buffer, buffer);
            read_next_line(line_buffer, file_pointer);
            get_tileset_path(buffer, secondary_buffer);
            level->tileset_array[z] = *init_texture(secondary_buffer);
            z++;
        }

        // TILES
        validate_label(line_buffer, file_pointer, "TILE");
        read_next_line(line_buffer, file_pointer);
        tile_number = find_integer_in_str(line_buffer, NULL);
        LVL_set_tile_number(level, tile_number);
        read_next_line(line_buffer, file_pointer);

        z=0;
        while (!strings_are_equal(line_buffer, "ENDTILE")) {
            int i = 0;
            int tileset  = find_integer_in_str(line_buffer, &i);
            int x_offset = find_integer_in_str(line_buffer, &i);
            int y_offset = find_integer_in_str(line_buffer, &i);

            printf("%i \n", tileset);
            printf("%i \n", x_offset);
            printf("%i \n", y_offset);

            LVL_add_tile(level, tileset, z, x_offset, y_offset);

            read_next_line(line_buffer, file_pointer);
            z++;
        }

        // printf("%i ", level->tile_array[0].y);
        // printf("%i ", level->tile_array[1].y);
        // printf("%i ", level->tile_array[2].y);

        break;
    }

    fclose(file_pointer);

    return 0;
    //dummy
}
