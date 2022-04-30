#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define LINE_SIZE 256
#define MAX_PROGRAMS 8
#define MAX_ARGS 8

int execute(char *line, int in, int out)
{
  char *args[MAX_PROGRAMS][MAX_ARGS];
  args[0][0] = strtok(line, " ");

  for (int i = 0; i < MAX_PROGRAMS; i++)
  {
    for (int j = 1; j < MAX_ARGS; j++)
    {
      args[i][j] = strtok(NULL, " ");

      if (args[i][j] == "|")
      {
        args[i][j] = NULL;
        execvp(line, args);
      }
    }
  }
}

int main(int argc, char *argv[])
{
  if (argc < 2)
    printf("Zbyt malo argumentow!\n");

  FILE *file = fopen(argv[1], "r");
  if (file == NULL)
    printf("Blad otwierania pliku!\n");

  char line[LINE_SIZE];
  int old[2];
  int new[2];

  while (getline(line, LINE_SIZE, file) != -1)
  {
    if (fork() == 0)
    {
      close(old[1]);
      close(new[0]);
      execute(line, old[0], new[1]);
    }
    *old = &new;
  }

  close(file);

  return 0;
}
