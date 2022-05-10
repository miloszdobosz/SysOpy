#include "header.h"

int main(int argc, char *argv[])
{
    sprintf(name, "/server");
    queue = mq_open("/server", O_RDONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
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

    int id;
    while (1)
    {
        receive(queue);

        switch (type)
        {
        case STOP:
            clients[atoi(message)] == -1;
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
            clients[clients_length] = mq_open(message, O_WRONLY);
            sprintf(message, "%d", clients_length);
            send(clients[clients_length], INIT);
            clients_length++;
            break;

        case ALL:
            sscanf(message, "%d %s", &id, message);
            for (int i = 0; i < clients_length; i++)
            {
                if (clients[i] != -1 && i != id)
                {
                    send(clients[i], ALL);
                }
            }
            break;

        case ONE:
            sscanf(message, "%d %s", &id, message);
            send(clients[id], ONE);
            break;
        }
    }

    return 0;
}
