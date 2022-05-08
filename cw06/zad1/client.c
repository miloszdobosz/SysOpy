#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include "header.h"

struct msgbuf
{
  long mtype;
  char *mtext[2];
};

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Zbyt malo argumentow!\n");
    exit(1);
  }
  int server = atoi(argv[1]);

  int queue = msgget(ftok("$HOME", PROJ_ID), IPC_CREAT);
  if (queue == -1)
  {
    perror("Blad tworzenia kolejki");
    exit(2);
  }

  struct msgbuf message = {INIT, NULL};
  msgsnd(server, &message, 0, 0);

  msgctl(queue, IPC_RMID, NULL);  
  return 0;
}
