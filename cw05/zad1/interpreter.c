#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main (int argc, char *argv[])
{
  if (argc < 2) {
    printf("Zbyt malo argumentow!\n");
  }

  char* path = argv[1];
  int file = open(path, O_RDONLY);

  FILE* grep_input = popen("grep Ala", "w");
  fputs("Ala ma kota", grep_input); // przesłanie danych do grep-a
  pclose(grep_input);

  close(file);

  return 0;
}
