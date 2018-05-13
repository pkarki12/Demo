/* Nole Lin and Prasiddha Karki
 * October 17, 2016 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "menuUtil.h"

int main() {
  printMenu();
  char c[100];
  int spacecount; // Declare counter for spaces to find how many arguments given
  Image *img; // Declare Image pointer
  Image *nextimg;
  while (scanf(" %[^\n]s", c) != 0) {
    spacecount = 0;
    int len = strlen(c);
    for (int i = 0; i<len; i++) {
      if (isspace(c[i])) {
        spacecount = spacecount + 1;
      }
    }
    
    char newstr[strlen(c)]; // Use strtok to split string input delimited by spaces into array of arguments
    strcpy(newstr, c);
    char *p = strtok(newstr, " ");
    char *array[spacecount+1];
    int j = 0;
    
    while (p != NULL) {
      array[j++] = p;
      p = strtok(NULL, " ");
    }
    nextimg = img;
    img = menuInput(spacecount + 1, array, nextimg); // Input number of arguments and array of arguments into menuInput to be modified
    printMenu();
  }
  free(nextimg);
  free(img);
  return 0;
}
