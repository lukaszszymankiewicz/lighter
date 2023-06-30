#include "../texture.h"

#ifndef UTIL_H
#define UTIL_H

texture_t* TXTR_read_from_file(const char *filepath);
void TXTR_free(texture_t* texture);

#endif
