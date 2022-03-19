#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define PATH_SIZE 100

int main(int argc, char* argv[]) {
  char* source_path;
  char* target_path;

  int source;
  int target;

  if (argc < 3) {
    source_path = malloc(PATH_SIZE);
    target_path = malloc(PATH_SIZE);

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

  //printf("%s\n%s\n", source_path, target_path);
  //fflush(stdout);

  if ((source = open(source_path, O_RDONLY)) == -1 || (target = open(target_path, O_WRONLY|O_CREAT)) == -1) {
    printf("Blad otwierania plikow!");
    return 1;
  }


  char next;
  int line_size = 0;
  int line_empty = 1;

  while(read(source, &next, 1) != 0) {
    if (!isspace(next)) {
      line_empty = 0;
    }

    if (next == '\n') {
      if (line_empty) {
        lseek(target, -line_size, SEEK_CUR);
      }
      line_size = 0;
    }

    write(target, &next, 1);
    line_size++;
  }

  close(source);
  close(target);

  free(source_path);
  free(target_path);
}
