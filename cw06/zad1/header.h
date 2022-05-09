#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/stat.h>

#define PATH getenv("HOME")
#define PROJ_ID 100

#define STOP 1
#define LIST 2
#define INIT 3
#define ALL 4
#define ONE 5

#define MAX_MSG_SIZE 256
#define MAX_CLIENTS 256

struct msgbuf
{
  long type;
  int id;
  char *text;
};

#define MSG_SIZE sizeof(struct msgbuf) - sizeof(long)

void clear(int status, void *queue)
{
  msgctl(*(int *)queue, IPC_RMID, NULL);
}