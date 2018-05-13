#ifndef _CS120_SCAFFOLD_PPMIO_H
#define _CS120_SCAFFOLD_PPMIO_H

#include <stdio.h>
#include <stdlib.h>

Image *readImage(char filename[]);
Image *writeImage(char filename[], Image *img);

#endif
