#include "header.h"

int main(int argc, char *argv[])
{
    queue = msgget(ftok(PATH, PROJ_ID), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    atexit(clear);
    signal(SIGINT, exit);

    if (queue == -1)
    {
        perror("Blad otwierania kolejki");
        exit(1);
    }

    printf("Queue: %d\n\n", queue);

    int clients[MAX_CLIENTS];
    int clients_length = 0;

    while (1)
    {
        receive(queue, -6);

        switch (message.type)
        {
        case STOP:
            clients[message.id] == -1;
            break;

        case LIST:
            printf("Active clients:\n");
            for (int i = 0; i < clients_length; i++)
            {
                if (clients[i] != -1)
                {
                    printf("%d ", i);
                }
            }
            printf("\n");
            break;

        case INIT:
            clients[clients_length] = msgget(ftok(PATH, message.id), 0);
            message.id = clients_length;
            send(clients[clients_length]);
            clients_length++;
            break;

        case ALL:
            for (int i = 0; i < clients_length; i++)
            {
                if (clients[i] != -1 && i != message.id)
                {
                    send(clients[i]);
                }
            }
            break;

        case ONE:
            send(clients[message.id]);
            break;
        }
    }

    return 0;
}
