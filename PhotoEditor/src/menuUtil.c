/* Nole Lin
   Prasiddha Karki
   October 17, 2016
   Include functions for menu */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "menuUtil.h"
#include "ppmIO.h"
#include "imageManip.h"

void printMenu() {
  printf("Main menu:\n");
  printf("\tr <filename> - read image from <filename>\n");
  printf("\tw <filename> - write image to <filename>\n");
  printf("\tc <x1> <y1> <x2> <y2> - crop image to the box with the given corners\n");
  printf("\ti - invert intensities\n");
  printf("\ts - swap color chanels\n");
  printf("\tg - convert to grayscale\n");
  printf("\tbl <sigma> - Gaussian blur with the given radius (sigma)\n");
  printf("\tsh <sigma> <amt> - sharpen by given amount (intensity), with radius (sigma)\n");
  printf("\tbr <amt> - change brightness (up or down) by the given amount\n");
  printf("\tcn <amt> - change contrast (up or down) by the given amount\n");
  printf("\tcs <color> - change color scheme to red (r), green (g), or blue (b)\n");
  printf("\tbw - invert black and white only\n");
  printf("\tq - quit\n");
  printf("Enter choice: ");
}

Image *menuInput(int nElements, char *elements[], Image *img) {
  if (nElements == 2) { // If given 2 arguments, perform read or write operation if mode is r or w
     char mode[strlen(elements[0])];
     strcpy(mode, elements[0]);
     if (mode[0] == 'r' || mode[0] == 'w') {
     	char filename[strlen(elements[1])+1];
     	strcpy(filename, elements[1]);
  	if (mode[0] == 'r') {
   	  printf("Reading from %s...\n", filename);
    	  img = readImage(filename);
          return img;
  	}
  	if (mode[0] == 'w') {
   	  printf("Writing to %s...\n", filename);
    	  img = writeImage(filename, img);
          return img;
  	}
      }
     else {
       if (mode[0] == 'b' && mode[1] == 'l') {
         double amount = atof(elements[1]);
         printf("Applying blur filter, sigma %.2f...\n", amount); 
         img = blur(img, amount);
         return img;
       }
       if (mode[0] == 'b' && mode[1] == 'r') {
         int amount = atoi(elements[1]);
         printf("Adjusting brightness by %d...\n", amount);
         img = brightness(img, amount);
         return img;
		 }
       if (mode[0] == 'c' && mode[1] == 'n') {
         double amount = atof(elements[1]);
         printf("Adjusting contrast by %.2f...\n", amount);
         img = contrast(img, amount);
         return img;
       }
		 if (mode[0] == 'c' && mode[1] == 's') {
			char color[strlen(elements[1]) + 1];
			strcpy(color, elements[1]);
			printf("Changing to %s colorscale...\n", color);
			img = colorscale(img, color);
			return img;
		 }
     }
  }
  if (nElements == 5) { // If given 5 arguments, perform cropping
    char mode[strlen(elements[0])];
    strcpy(mode, elements[0]);
    int x1 = atoi(elements[1]);
    int y1 = atoi(elements[2]);
    int x2 = atoi(elements[3]);
    int y2 = atoi(elements[4]);
    if (x1 > x2 || y1 > y2 || x2 > img->rows || y2 > img->cols) {
      printf("Invalid coordinates\n");
      return img;
    }
    printf("Cropping region from (%d, %d) to (%d, %d)\n", x1, y1, x2, y2);
    img = cropImage(x1, y1, x2, y2, img);
    return img;
  }
  if (nElements == 1) { // If given 1 argument, perform inverting or swapping
    char mode[strlen(elements[0])];
    strcpy(mode, elements[0]);
    if (mode[0] == 'i') {
      printf("Inverting intensity...\n");
      img = invert(img);
      return img;
    }
    if (mode[0] == 's') {
      printf("Swapping color channels...\n");
      img = swap(img);
      return img;
    }
    if (mode[0] == 'g') {
      printf("Converting to grayscale...\n");
      img = grayscale(img);
      return img;
    }
	 if (mode[0] == 'b' && mode[1] == 'w') {
		printf("Inverting black and white...\n");
		img = bwswap(img);
		return img;
	 }
    if (mode[0] == 'q') {
      printf("Goodbye!\n");
      exit(1);
    }
  }
  if (nElements == 3) {
    char mode[strlen(elements[0])];
    strcpy(mode, elements[0]);
    double amount = atof(elements[1]);
    double intensity = atof(elements[2]);
    if (mode[0] == 's' && mode[1] == 'h') {
      printf("Applying sharpen filter, sigma %.2f, intensity %.2f...\n", amount, intensity);
      img = sharpen(img, amount, intensity);
      return img;
    }
  }
  printf("Invalid input\n");
  return img;
}
