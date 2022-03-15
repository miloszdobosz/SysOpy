#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 100
#define PATH_SIZE 100

int main(int argc, char* argv[]) {
  char* source_path;
  char* target_path;

  FILE* source;
  FILE* target;

  if (argc < 3) {
    source_path[PATH_SIZE];
    target_path[PATH_SIZE];

    printf("Podaj 2 pliki:\n");
    scanf("%s%s", source_path, target_path);

  } else {
    source_path = argv[1];
    target_path = argv[2];
  }

  while(strcmp(source_path, target_path) == 0) {
    printf("Podaj 2 poprawne pliki:\n");
    scanf("%s%s", source_path, target_path);
  } 

  printf("%s\n%s\n", source_path, target_path);

  if ((source = fopen(source_path, "r")) == NULL || (target = fopen(target_path, "w")) == NULL) {
    printf("Blad otwierania plikow!");
    return 1;
  }


  char buffer[BUFFER_SIZE];

  while (fgets(buffer, BUFFER_SIZE, source) != NULL) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
      if (buffer[i] == '\n') {
        break;
      } else if (!isspace(buffer[i])) {
        fprintf(target, "%s", buffer);
        break;
      }
    }
  }

  fclose(source);
  fclose(target);
}
