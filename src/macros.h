#ifndef MACROS_H
#define MACROS_H

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define TRIM(low, high, number) MIN(high, MAX(low, number))


#endif
