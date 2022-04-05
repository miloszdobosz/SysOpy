#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main (int argc, char *argv[])
{
  char *path = "fifo";

  char buffer[11];

  int file = open(path, O_RDONLY);
  read(file, &buffer, 11);
  printf("%s\n", buffer);
  close(file);

  return 0;
}
