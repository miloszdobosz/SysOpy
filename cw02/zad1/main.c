#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  char* source_path;
  char* target_path;

  if (argc < 3) {
    source_path = malloc(100);
    target_path = malloc(100);

    printf("Podaj 2 pliki:\n");
    scanf("%s%s", source_path, target_path);
  } else {
    source_path = argv[1];
    target_path = argv[2];
  }

  printf("%s\n%s\n", source_path, target_path);

  FILE* source = malloc(sizeof(FILE*));
  FILE* target = malloc(sizeof(FILE*));

  if ((source = fopen(source_path, "r")) == NULL || (source = fopen(target_path, "w")) == NULL) {
    //error
    printf("ERRRORRRR");
    return 1;
  }


}
