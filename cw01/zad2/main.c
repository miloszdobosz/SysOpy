#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "bibl1.h"

int main(int argc, char* argv[]) {

  char** table;
  int n;
  int j = 0;

  int mode = 0;
  
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "ct") == 0) {
      mode = 1;
    } else if (strcmp(argv[i], "wc") == 0) {      
      mode = 2;
    } else if (strcmp(argv[i], "rb") == 0) {
      mode = 3;
    } else {
      switch (mode) {
        case 0:
          printf("Argument %d incorrect, skipped.", i);
          break;
        case 1:
          n = atoi(argv[i]);
          table = allocate(n);
          mode = 0;
          break;
        case 2:
          count(argv[i]);
          table[j++] = write_temp();
          break;
        case 3:
          int index = atoi(argv[i]);
          if (index < 0 || index >= n) {
            printf("Index %d out of bounds 0-%d, skipped", i, n);
            continue;
          }
          clear(table, index);
          mode = 0;
          break;
      }
    }
  }


  for (int i = 0; i < n; i++) {
    free(table[i]);
  }
}
