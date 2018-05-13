#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "menuUtil.h"
#include "imageManip.h"
#include "ppmIO.h"

Image *cropImage(int x1, int y1, int x2, int y2, Image *img) {
  int newrows = x2 - x1 - 1;
  int newcols = y2 - y1 - 1;
  int counter = 0;
  Pixel *newimg = malloc(sizeof(Pixel) * newrows * newcols);
  Pixel *ip = img->data;
  for (int i = 0; i < img->cols; i++) {
    for (int j = 0; j < img->rows; j++) {
      if (i > y1 && i < y2 && j > x1 && j < x2) {     
        newimg[counter] = ip[(i * img->rows) + j];
        counter = counter + 1;
      }
    }
  }
  img->data = newimg;
  img->rows = newrows;
  img->cols = newcols;
  return img;
}

Image *blur(Image *img, double amt) {
  double redsum = 0;
  double greensum = 0;
  double bluesum = 0;
  double gridsum = 0;
  double pi = 3.14;
  double sig2 = amt * amt;
  int N = (int) 5*amt;
  N = 2*N;
  if (N % 2 == 0) {
    N = N + 1;
  }
  int half = (N-1)/2;
  Pixel *ip = img->data;
  double *grid = malloc(sizeof(double) * N * N);
  Pixel *newpix = malloc(sizeof(Pixel) * img->rows * img->cols);
  int center[2] = {(N - 1)/2, (N - 1)/2};
  for (int a = 0; a < N; a++) {
    for (int b = 0; b < N; b++) {
      int dx2 = (a - center[0]) * (a - center[0]);
      int dy2 = (b - center[1]) * (b - center[1]);
      grid[(a * N) + b] = (1.0/(2.0*pi*sig2)) * exp(-(dx2 + dy2)/(2.0*sig2));
    }
  }
  int counter = 0;
  for (int i = 0; i < img->cols; i++) {
    for (int j = 0; j < img->rows; j++) {
      for (int k = -1 * half; k <= half; k++) {
        for (int l = -1 * half; l <= half; l++) {
          if (i + k < 0 || i + k >= img->rows || j + l < 0 || j + l >= img->cols) {
            continue;
          }
          redsum = redsum + grid[((k + half) * N) + l + half] * (double) ip[((i+k) * img->rows)+j+l].r;
          greensum = greensum + grid[(k + half) * N + l + half] * (double) ip[((i+k) * img->rows)+j+l].g;
          bluesum = bluesum + grid[(k + half) * N + l + half] * (double) ip[((i+k) * img->rows) + j+l].b;
          gridsum = gridsum + grid[(k + half) * N + l + half];
        }
      }
      newpix[counter].r = redsum / gridsum;
      newpix[counter].g = greensum / gridsum;
      newpix[counter].b = bluesum / gridsum;
      counter = counter + 1;
      redsum = 0;
      greensum = 0;
      bluesum = 0;
      gridsum = 0;
    }
  }
  free(grid);
  img->data = newpix;
  free(ip);
  return img;
}

Image *sharpen(Image *img, double amt, double intensity) {
  double redsum = 0;
  double greensum = 0;
  double bluesum = 0;
  double gridsum = 0;
  double pi = 3.14;
  double sig2 = amt * amt;
  int N = (int) 5*amt;
  N = 2*N;
  if (N % 2 == 0) {
    N = N + 1;
  }
  int half = (N-1)/2;
  Pixel *ip = img->data;
  double *grid = malloc(sizeof(double) * N * N);
  Pixel *newpix = malloc(sizeof(Pixel) * img->rows * img->cols);
  int center[2] = {(N - 1)/2, (N - 1)/2};
  for (int a = 0; a < N; a++) {
    for (int b = 0; b < N; b++) {
      int dx2 = (a - center[0]) * (a - center[0]);
      int dy2 = (b - center[1]) * (b - center[1]);
      grid[(a * N) + b] = (1.0/(2.0*pi*sig2)) * exp(-(dx2 + dy2)/(2.0*sig2));
    }
  }
  int counter = 0;
  for (int i = 0; i < img->cols; i++) {
    for (int j = 0; j < img->rows; j++) {
      for (int k = -1 * half; k <= half; k++) {
        for (int l = -1 * half; l <= half; l++) {
          if (i + k < 0 || i + k >= img->rows || j + l < 0 || j + l >= img->cols) {
            continue;
          }
          redsum = redsum + grid[((k + half) * N) + l + half] * (double) ip[((i+k) * img->rows)+j+l].r;
          greensum = greensum + grid[(k + half) * N + l + half] * (double) ip[((i+k) * img->rows)+j+l].g;
          bluesum = bluesum + grid[(k + half) * N + l + half] * (double) ip[((i+k) * img->rows) + j+l].b;
          gridsum = gridsum + grid[(k + half) * N + l + half];
        }
      }
      newpix[counter].r = redsum / gridsum;
      newpix[counter].g = greensum / gridsum;
      newpix[counter].b = bluesum / gridsum;
      counter = counter + 1;
      redsum = 0;
      greensum = 0;
      bluesum = 0;
      gridsum = 0;
    }
  }
  int rdiff = 0;
  int gdiff = 0;
  int bdiff = 0;
  for (int i = 0; i<img->rows*img->cols;i++) {
    rdiff = img->data[i].r - newpix[i].r;
    gdiff = img->data[i].g - newpix[i].g;
    bdiff = img->data[i].b - newpix[i].b;
    if (img->data[i].r + (int) (rdiff * intensity) > 255) {
      img->data[i].r = 255;
    }
    else if (img->data[i].r + (int) (rdiff * intensity) < 0) {
      img->data[i].r = 0;
    }
    else {
      img->data[i].r = img->data[i].r + (int) (rdiff * intensity);
    }
    if (img->data[i].g + (int) (gdiff * intensity) > 255) {
      img->data[i].g = 255;
    }
    else if (img->data[i].g + (int) (gdiff * intensity) < 0) {
      img->data[i].g = 0;
    }
    else {
      img->data[i].g = img->data[i].g + (int) (gdiff * intensity);
    }
    if (img->data[i].b + (int) (bdiff * intensity) > 255) {
      img->data[i].b = 255;
    }
    else if (img->data[i].b + (int) (bdiff * intensity) < 0) {
      img->data[i].b = 0;
    }
    else {
      img->data[i].b = img->data[i].b + (int) (bdiff * intensity);
    }
  }
  free(grid);
  free(newpix);
  return img;
}

Image *invert(Image *img) {
  int i;
  if(img){
         for(i=0;i<img->rows*img->cols;i++){
              img->data[i].r=255-img->data[i].r;
              img->data[i].g=255-img->data[i].g;
              img->data[i].b=255-img->data[i].b;
         }
    }
  return img;
}


Image *swap(Image *img) {
  int i;
  if(img){
     for(i=0;i<img->rows*img->cols;i++){
       unsigned char newg = img->data[i].b;
       unsigned char newr = img->data[i].g;
       unsigned char newb = img->data[i].r;
       img->data[i].r = newr;
       img->data[i].g = newg;
       img->data[i].b = newb;
     }
  }
  return img;
}

Image *brightness(Image *img, int amt) {
  int i;
  for (i=0; i<img->rows*img->cols;i++) {
    unsigned char red = img->data[i].r;
    unsigned char green = img->data[i].g;
    unsigned char blue = img->data[i].b;
    if (red + amt > 255) {
      img->data[i].r = 255;
    }
    else if (red + amt < 0) {
      img->data[i].r = 0;
    }
    else {
      img->data[i].r = red + amt;
    }
    if (green + amt > 255) {
      img->data[i].g = 255;
    }
    else if (green + amt < 0) {
      img->data[i].g = 0;
    }
    else {
      img->data[i].g = green + amt;
    }
    if (blue + amt > 255) {
      img->data[i].b = 255;
    }
    else if (blue + amt < 0) {
      img->data[i].b = 0;
    }
    else {
      img->data[i].b = blue + amt;
    }
  }
  return img;
}

Image *contrast(Image *img, double amt) {
  int i;
  for (i=0; i<img->rows*img->cols;i++) {
    double scaler = (img->data[i].r / 255.0) - 0.5;
    double scaleg = (img->data[i].g / 255.0) - 0.5;
    double scaleb = (img->data[i].b / 255.0) - 0.5;
    double scaler2 = scaler * amt;
    double scaleg2 = scaleg * amt;
    double scaleb2 = scaleb * amt;
    if ((scaler2 + 0.5) * 255.0 > 255) {
      img->data[i].r = 255;
    }
    else if ((scaler2 + 0.5) * 255.0 < 0) {
      img->data[i].r = 0;
    }
    else {
      img->data[i].r = (unsigned char) ((scaler2 + 0.5) * 255.0);
    }
    if ((scaleg2 + 0.5) * 255.0 > 255) {
      img->data[i].g = 255;
    }
    else if ((scaleg2 + 0.5) * 255.0 < 0) {
      img->data[i].g = 0;
    }
    else {
      img->data[i].g = (unsigned char) ((scaleg2 + 0.5) * 255.0);
    }
    if ((scaleb2 + 0.5) * 255.0 > 255) {
      img->data[i].b = 255;
    }
    else if ((scaleb2 + 0.5) * 255.0 < 0) {
      img->data[i].b = 0;
    }
    else {
      img->data[i].b = (unsigned char) ((scaleb2 + 0.5) * 255.0);
    }
  }
  return img;
}


Image *grayscale(Image *img) {
	int i;
	if (img) {
		for (i=0; i<img->rows*img->cols; i++) {
			unsigned char gray = 0.3*img->data[i].r + 0.59*img->data[i].g + 0.11*img->data[i].b;			
			img->data[i].r = gray;
			img->data[i].g = gray;
			img->data[i].b = gray;
		 }
	 }
	 return img;
}


Image *bwswap(Image *img) {
// This function will invert very dark (black) or very light (white) pixels only.
	int i;
	if (img) { 
		for (i=0; i<img->rows*img->cols; i++) {
			//Checks if pixel is very light (white)
	   	if (img->data[i].r > 200 && img->data[i].g > 200 && img->data[i].b > 200) {
			   img->data[i].r=255-img->data[i].r;
            img->data[i].g=255-img->data[i].g;
            img->data[i].b=255-img->data[i].b;
         }
			//Checks if pixel is very dark (black)
	     	if (img->data[i].r < 55 && img->data[i].g < 55 && img->data[i].b < 55) {
			   img->data[i].r=255-img->data[i].r;
            img->data[i].g=255-img->data[i].g;
            img->data[i].b=255-img->data[i].b;
         }
	       
      }
   }
   return img;
}		


Image *colorscale(Image *img, char color[]) {
//This function will change the color scheme of the picture to either red, green, or blue.
	int i;
	if (img) {
		for (i=0; i<img->rows*img->cols; i++) {
			if (color[0] == 'r') {
				img->data[i].g = 0;
				img->data[i].b = 0;
			}
			if(color[0] == 'g') {
				img->data[i].r = 0;
				img->data[i].b = 0;
			}
			if(color[0] == 'b') {
				img->data[i].r = 0;
				img->data[i].g = 0;
			}
		}
	}
	return img;
}







