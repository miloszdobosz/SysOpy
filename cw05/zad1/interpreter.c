#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define BUFFER_SIZE 256

int execute(char *buffer, int in, int out)
{
  // execvp(buffer, args);
}

int main(int argc, char *argv[])
{
  if (argc < 2)
    printf("Zbyt malo argumentow!\n");

  FILE *file = fopen(argv[1], "r");
  if (file == NULL)
    printf("Blad otwierania pliku!\n");

  char buffer[BUFFER_SIZE];
  int old[2];
  int new[2];

  while (getline(buffer, BUFFER_SIZE, file) != -1)
  {
    if (fork() == 0)
    {
      close(old[1]);
      close(new[0]);
      execute(buffer, old[0], new[1]);
    }
    *old = &new;
  }

  close(file);

  return 0;
}
