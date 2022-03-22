#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
  if (argc < 2) {
    return 1;
  }


  int pid = 0;
  for (int i = 0; i < atoi(argv[1]); i++) {
    if(fork() == 0) {
      printf("Tu proces potomny %d\n", i);
      return 0;
    }
  }

  return 0;
}
