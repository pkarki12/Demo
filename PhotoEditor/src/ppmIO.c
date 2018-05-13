#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "menuUtil.h"

Image *readImage(char filename[]) { 
  char format[10];
  Image *img;
  FILE *fp; 
  fp = fopen(filename, "rb");
  if (!fp) {
              fprintf(stderr, "Unable to open file '%s'\n", filename);
              exit(1);
         }
  if (!fgets(format, sizeof(format), fp)) {
    perror(filename);
    exit(1);
  }
  if (format[0] != 'P' || format[1] != '6') {
    fprintf(stderr, "Invalid image format (must be 'P6')\n");
    exit(1);
    }
  img = malloc(sizeof(Image));
  if (!img) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }
  int c = getc(fp);
  while (c == '#') {
  while (getc(fp) != '\n');
    c = getc(fp);
  }
  ungetc(c, fp);
  
  if (fscanf(fp, "%d %d", &img->rows, &img->cols) != 2) {
    printf("Invalid image size (error loading '%s')\n", filename);
    exit(1);
  }
  if (fscanf(fp, "%d", &img->colors) != 1) {
    printf("Invalid rgb component (error loading '%s')\n");
    exit(1);
  }
  img->data = malloc(img->rows * img->cols * sizeof(Pixel));
  if (!img) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }
  getc(fp);
  if (fread(img->data, sizeof(Pixel), img->cols * img->rows, fp) != img->rows * img->cols) {
    printf("Error loading image '%s'\n", filename);
    exit(1);
  }
  fclose(fp);
  return img;
}

Image *writeImage(char filename[], Image *img) {
  FILE *fp;
  fp = fopen(filename, "wb");
  fprintf(fp, "P6\n");
  fprintf(fp, "# Created by Nole Lin and Prasiddha Karki\n");
  fprintf(fp, "%d %d\n", img->rows, img->cols);
  fprintf(fp, "%d\n", img->colors);
  fwrite(img->data, sizeof(Pixel), img->cols * img->rows, fp);
  fclose(fp);
  return img;
}
