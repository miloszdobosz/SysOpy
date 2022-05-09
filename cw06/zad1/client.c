#include <unistd.h>
#include "header.h"

int server, id;

void stop()
{
    message.type = STOP;
    message.id = id;
    send(server);
}

int main(int argc, char *argv[])
{
    queue = msgget(ftok(PATH, getpid()), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    server = msgget(ftok(PATH, PROJ_ID), 0);
    atexit(clear);
    signal(SIGINT, exit);

    if (server == -1 || queue == -1)
    {
        perror("Blad otwierania kolejki");
        exit(1);
    }

    printf("Queue: %d\n\n", queue);

    message.type = INIT;
    message.id = getpid(); // Serwer sam sobie liczy klucz przy pomocy ftok()
    message.text[0] = '\0';

    send(server);
    receive(queue, INIT);

    int id = message.id; // Poprawne id odesłane przez server
    printf("ID: %d\n\n", id);
    atexit(stop);

    if (fork() == 0)
    {
        while (1)
        {
            if (!receive(queue, -6))
                exit(0);
        }
    }

    long command;
    while (1)
    {
        scanf("%ld", &command);
        // printf("-------------\n");

        message.type = command;

        switch (command)
        {
        case STOP:
            exit(0);
            break;

        case ALL:
            message.id = id;
            scanf("%s", message.text);
            break;

        case ONE:
            scanf("%d", &message.id);
            scanf("%s", message.text);
            break;

        default:
            goto outer_break;
        }

        send(server);
        outer_break:
    }

    return 0;
}
