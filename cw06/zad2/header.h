#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <mqueue.h>
#include <time.h>
#include <sys/stat.h>

#define PATH getenv("HOME")
#define PROJ_ID 100

#define INIT 1
#define ALL 2
#define ONE 3
#define LIST 4
#define STOP 5

#define MAX_MSG_SIZE 256
#define MAX_CLIENTS 256

char message[MAX_MSG_SIZE];

int queue;
char name[21];

void clear()
{
    mq_close(queue);
    mq_unlink(name);
}

int send(int queue, unsigned int priority)
{
    int status = mq_send(queue, message, MAX_MSG_SIZE, priority);
    perror("\nSend");
    printf("Time: %ld\n%d %s\n\n", time(NULL), priority, message);
    return status > 0;
}

unsigned int type;

int receive(int queue)
{
    int status = mq_receive(queue, message, MAX_MSG_SIZE, &type);
    perror("\nReceive");
    printf("Time: %ld\n%d %s\n\n", time(NULL), type, message);
    return status > 0;
}
