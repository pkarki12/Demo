#ifndef _CS120_IMAGEMANIP_H
#define _CS120_IMAGEMANIP_H

#include <stdio.h>

Image *cropImage(int x1, int y1, int x2, int y2, Image *img);
Image *invert(Image *img);
Image *swap(Image *img);
Image *brightness(Image *img, int amt);
Image *contrast(Image *img, double amt);
Image *blur(Image *img, double amt);
Image *grayscale(Image *img);
Image *sharpen(Image *img, double amt, double intensity);
Image *bwswap(Image *img);
Image *colorscale(Image *img, char color[]);

#endif
