#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include <poll.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <time.h>

#define SOCKET_PROTOCOL SOCK_STREAM

#define SERVER_NAME "cluster_server"
#define CLIENTS_LIMIT 16
#define PING_INTERVAL 5
#define PING_TIMEOUT 5

#define CLIENT_MAX_NAME_LENGTH 32

#define MODE_INET 1
#define MODE_UNIX 2

struct client
{
    char name[CLIENT_MAX_NAME_LENGTH];
    int socket_fd;
    int client_id;
    int registered;
    int active;
};

#define MESSAGE_SIZE 64
#define CONTENT_SIZE 1024

#define MSG_LOGIN 1
#define MSG_PING 2

struct login_message
{
    char string[CLIENT_MAX_NAME_LENGTH];
};

struct ping_message
{
    int ping_id;
};

int send_message(int socket_fd, char message[MESSAGE_SIZE]);
int receive_message(int socket_fd, char message[MESSAGE_SIZE]);
int check_message_type(char message[MESSAGE_SIZE]);

int send_login_message(int socket_fd, char *string);
int send_ping_message(int socket_fd, int ping_id);

struct login_message get_login_message(char message[MESSAGE_SIZE]);
struct ping_message get_ping_message(char message[MESSAGE_SIZE]);