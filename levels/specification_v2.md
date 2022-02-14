```
This is the target format of level file
```

### File name && type
Level extension will be named: ".llv", prefixed name can be any unicoded string.

### Specification

## General rules
Colon is considered to be end of the line, comma is separator for words and numbers.
Comments with double dash ("//") are allowed

## Alloc Header
First line is named alloc header and is consisted from values which show how many memory should be
allocated.
By now it is consisted from four integer numbers:
x_size - x_size of level
y_size - y_size of level
n_tiles - number of differen square tiles present in level.

## Tile matrix
Matrix showing 1:1 which tile is used in which coord

## Obstacle matrix
Matrix showing 1:1 which tile is an obsacle for light
