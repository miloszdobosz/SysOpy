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

#define MSG_SIZE sizeof(struct msgbuf) - sizeof(long)

struct msgbuf
{
    long type;
    int id;
    char text[MAX_MSG_SIZE];
} message;

int queue;

void clear()
{
    msgctl(queue, IPC_RMID, NULL);
}

struct msqid_ds buffer;

int send(int queue)
{
    int status = msgsnd(queue, &message, MSG_SIZE, 0);
    msgctl(queue, IPC_STAT, &buffer);
    perror("\nSend");
    printf("Time: %ld\n%ld %d %s\n\n", buffer.msg_stime, message.type, message.id, message.text);
    return status > 0;
}

int receive(int queue, long type)
{
    int status = msgrcv(queue, &message, MSG_SIZE, type, 0);
    msgctl(queue, IPC_STAT, &buffer);
    perror("\nReceive");
    printf("Time: %ld\n%ld %d %s\n\n", buffer.msg_rtime, message.type, message.id, message.text);
    return status > 0;
}
