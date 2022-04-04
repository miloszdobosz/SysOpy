#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>

int main (int argc, char *argv[])
{
  raise(SIGUSR1);
  return 0;
}
