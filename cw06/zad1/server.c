#include "header.h"

int main(int argc, char *argv[])
{
  int queue = msgget(ftok(PATH, PROJ_ID), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
  on_exit(clear, &queue);
  signal(SIGINT, exit);

  if (queue == -1)
  {
    perror("Blad tworzenia kolejki");
    exit(1);
  }

  printf("Queue: %d\n", queue);

  key_t clients[MAX_CLIENTS];
  int clients_length = 0;

  struct msgbuf message;
  // while (1)
  for (int i = 0; i < 1; i++)
  {
    msgrcv(queue, &message, MSG_SIZE, -6, 0);
    perror("rcv");
    printf("%ld %d %s\n", message.type, message.id, message.text);

    switch (message.type)
    {
    case STOP:
      clients[message.id] == -1;
      break;

    case LIST:
      for (int i = 0; i < clients_length; i++)
      {
        if (clients[i] != -1)
        {
          printf("%d\n", i);
        }
      }
      break;

    case INIT:
      clients[clients_length] = msgget(ftok(PATH, message.id), 0);
      message.id = clients_length;

      msgsnd(clients[clients_length], &message, MSG_SIZE, 0);
      perror("snd");
      printf("%ld %d %s\n", message.type, message.id, message.text);

      clients_length++;
      break;

    case ALL:
      for (int i = 0; i < clients_length; i++)
      {
        if (clients[i] != -1 && i != message.id)
        {
          msgsnd(clients[i], &message, MSG_SIZE, 0);
        }
      }
      break;

    case ONE:
      msgsnd(clients[message.id], &message, MSG_SIZE, 0);
      break;
    }
  }

  return 0;
}
