#include <stdio.h>

int main(int argc, char* argv[]) {
  char* source;
  char* target;
  if (argc < 3) {
    printf("Prosze podac 2 pliki:\n");
    scanf("%s%s", source, target);
  } else {
    source = argv[1];
    target = argv[2];
  }

  printf("%s", source);
  printf("%s", target);
}
