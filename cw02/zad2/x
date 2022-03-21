#include <stdio.h>

#define FALSE 0
#define TRUE 1


int main (int argc, char *argv[])
{
  if (argc < 3) {
    printf("Zbyt malo argumentow!");
    return 1;
  } 

  char character = argv[1][0];
  char* file_path = argv[2];
  
  printf("%c", character);
  FILE* file = fopen(file_path, "r");
  if (!file) {
    printf("Blad otwierania pliku!");
    return 2;
  }


  int line_contains = FALSE;
  int character_count = 0;
  int line_count = 0;

  char buffer[1];
  while(fread(buffer, 1, 1, file) != 0) {
    if (buffer[0] == character) {
      character_count++;
      line_contains = TRUE;
    } else if (buffer[0] == '\n' && line_contains) {
      line_count++;
      line_contains = FALSE;
    }
  }

  if (line_contains) {
    line_count++;
  }
  
  printf("\n%d\n%d\n", character_count, line_count);
  return 0;
}
