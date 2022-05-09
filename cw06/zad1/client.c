#include <unistd.h>
#include "header.h"

// int id;

int main(int argc, char *argv[])
{
  int server = msgget(ftok(PATH, PROJ_ID), 0);
  int queue = msgget(ftok(PATH, getpid()), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
  on_exit(clear, &queue);
  signal(SIGINT, exit);

  // printf("%d %d\n", ftok(PATH, getpid()), getpid());
  if (server == -1)
  {
    perror("Blad otwierania kolejki");
    exit(1);
  }
  if (queue == -1)
  {
    perror("Blad tworzenia kolejki");
    exit(1);
  }

  printf("Queue: %d\n", queue);

  struct msgbuf message;
  message.type = INIT;
  message.id = getpid(); // Serwer sam sobie liczy klucz przy pomocy ftok()
  message.text = NULL;

  msgsnd(server, &message, MSG_SIZE, 0);
  perror("snd");
  printf("%ld %d %s\n", message.type, message.id, message.text);

  msgrcv(queue, &message, MSG_SIZE, INIT, 0);
  perror("rcv");
  printf("%ld %d %s\n", message.type, message.id, message.text);

  // id = message.id; // Poprawne id odesłane przez server
  // msgctl(queue, IPC_RMID, NULL);
  // while(1);
  return 0;
}
