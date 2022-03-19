#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define PATH_SIZE 100

#define TRUE 1
#define FALSE 0


int main(int argc, char* argv[]) {
  char* source_path;
  char* target_path;

  int source;
  int target;

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

  if ((source = open(source_path, O_RDONLY)) == -1 ||
    (target = open(target_path, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR)) == -1) {
    printf("Blad otwierania plikow!");
    return 1;
  }


  char current;
  int line_size = 0;
  int line_empty = TRUE;

  while(read(source, &current, 1) != 0) {
    line_size++;

    if (!isspace(current)) {
      line_empty = FALSE;
    } else if (current == '\n') {

      if (line_empty) {
        lseek(target, -line_size, SEEK_CUR);
      }

      line_size = 0;
      line_empty = TRUE;
    }

    write(target, &current, 1);
  }

  close(source);
  close(target);
}
