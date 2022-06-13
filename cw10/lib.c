#include "lib.h"

int send_message(int socket_fd, char message[MESSAGE_SIZE])
{
    return write(socket_fd, message, MESSAGE_SIZE);
}

int receive_message(int socket_fd, char message[MESSAGE_SIZE])
{
    int result = read(socket_fd, message, MESSAGE_SIZE);
    return result;
}

int check_message_type(char message[MESSAGE_SIZE])
{
    int type;
    sscanf(message, "%d", &type);
    return type;
}

int send_login_message(int socket_fd, char *string)
{
    char buffer[MESSAGE_SIZE];
    sprintf(buffer, "%d %s", MSG_LOGIN, string);
    return send_message(socket_fd, buffer);
}

int send_ping_message(int socket_fd, int ping_id)
{
    char buffer[MESSAGE_SIZE];
    sprintf(buffer, "%d %d", MSG_PING, ping_id);
    return send_message(socket_fd, buffer);
}

struct login_message get_login_message(char message[MESSAGE_SIZE])
{
    int tmp;
    struct login_message lm;
    sscanf(message, "%d %s", &tmp, lm.string);
    return lm;
}

struct ping_message get_ping_message(char message[MESSAGE_SIZE])
{
    int tmp;
    struct ping_message pm;
    sscanf(message, "%d %d", &tmp, &pm.ping_id);
    return pm;
}