#include "import.h"

const char *LEVEL_READ_MODE        = "rb";
const char *LEVEL_STRUCTURE_SUFFIX = "level.llv";
const char *LEVEL_TILESET_SUFFIX   = "level.png";
const char *SEPARATOR              = "/";

const int WOBBLE_PREAMBULE[PREAMBULE_LEN]      = { 22338, 16972 };
const int LIGHTSOURCE_PREAMBULE[PREAMBULE_LEN] = { 19539, 21059 };
const int LEVEL_PREAMBULE[PREAMBULE_LEN]       = { 19525, 22092 };


char* IMP_concatenate_string(
    const char *a, const char *b, const char *d
) {
    size_t len = strlen(a) + strlen(b) + strlen(d);
    char* str = NULL;
    str = (char*)malloc(len + 1);

    strcpy(str, a);
    strcat(str, b);
    strcat(str, d); 

    return str;
}

unsigned short int IMP_cast_val_to_dec(char vals[DOUBLE_BYTE]) {
    return (unsigned short int) ((unsigned short int) vals[0]<<8 | (unsigned char) vals[1]);
}

