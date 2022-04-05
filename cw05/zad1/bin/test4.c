#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main (int argc, char *argv[])
{
  char *path = "fifo";

  mkfifo(path, 0777);
  int file = open(path, O_WRONLY);
  write(file, "asdflkjasdf", 11);
  close(file);

  return 0;
}
