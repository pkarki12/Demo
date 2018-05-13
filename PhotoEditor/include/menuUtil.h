#ifndef _CS120_SCAFFOLD_MENUUTIL_H
#define _CS120_SCAFFOLD_MENUUTIL_H

typedef struct _pixel {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} Pixel;

typedef struct _image {
  Pixel *data;
  int rows;
  int cols;
  int colors;
} Image;

#include <stdio.h>

void printMenu();
Image *menuInput(int nElements, char* elements[], Image *img);

#endif
