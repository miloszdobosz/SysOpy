#include <unistd.h>
#include "header.h"

int server, id;

void stop()
{
    sprintf(message, "%d", id);
    send(server, STOP);
}

int main(int argc, char *argv[])
{
    server = mq_open("/server", O_WRONLY);
    sprintf(name, "/client%ld", time(NULL));
    queue = mq_open(name, O_RDONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    atexit(clear);
    signal(SIGINT, exit);

    if (server == -1 || queue == -1)
    {
        perror("Blad otwierania kolejki");
        exit(1);
    }

    printf("Queue: %d\n\n", queue);
    sprintf(message, "%s", name);
    send(server, INIT);
    receive(queue);

    int id = atoi(message); // Poprawne id odesłane przez server
    printf("ID: %d\n\n", id);
    atexit(stop);

    if (fork() == 0)
    {
        while (1)
        {
            if (!receive(queue))
                exit(0);
        }
    }

    unsigned int command;
    while (1)
    {
        scanf("%d", &command);
        // printf("-------------\n");

        switch (command)
        {
        case STOP:
            exit(0);
            break;

        case ALL:
        case ONE:
            scanf("%s", message);
            break;

        default:
            goto outer_break;
        }

        send(server, command);
        outer_break:
    }

    return 0;
}
