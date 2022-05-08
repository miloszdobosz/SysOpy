#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include "header.h"

int main(int argc, char *argv[])
{
  int queue = msgget(ftok("$HOME", PROJ_ID), IPC_CREAT);
  if (queue == -1) {
    perror("Blad tworzenia kolejki");
    exit(1);
  }

  printf("%d\n", queue);
  
  msgctl(queue, IPC_RMID, NULL);  
  return 0;
}
