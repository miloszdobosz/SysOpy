#include "lib.h"

#define BUFFER_SIZE 1024

char *client_name;
short socket_mode;

int socket_fd = -1;
char *ip_address;
in_port_t inet_port;
char *unix_socket_path_name;

void sig_int(int signum)
{
    if (socket_fd > 0)
        send_login_message(socket_fd, "logout");
    exit(EXIT_SUCCESS);
}

void server_connect()
{
    if (socket_mode == MODE_UNIX)
    {
        struct sockaddr_un sa;
        strcpy(sa.sun_path, unix_socket_path_name);
        sa.sun_family = AF_UNIX;

        if ((socket_fd = socket(AF_UNIX, SOCKET_PROTOCOL, 0)) == -1)
            printf("Blad funkcji socket!\n");

        if (connect(socket_fd, (struct sockaddr *)&sa, sizeof(sa)) == -1)
            printf("Blad funkcji connect!\n");

        printf("Probuje polaczyc z socketem UNIX\n");
    }
    else
    {
        struct sockaddr_in sa_inet;
        sa_inet.sin_family = AF_INET;
        sa_inet.sin_addr.s_addr = inet_addr(ip_address);
        sa_inet.sin_port = htons(inet_port);

        if ((socket_fd = socket(AF_INET, SOCKET_PROTOCOL, 0)) == -1)
            printf("Blad funkcji socket!\n");

        if (connect(socket_fd, (struct sockaddr *)&sa_inet, sizeof(sa_inet)) == -1)
            printf("Blad funkcji connect!\n");

        printf("Probuje polaczyc z socketem INET\n");
    }
}

void process_message()
{
    char buffer[MESSAGE_SIZE];
    receive_message(socket_fd, buffer);

    int msg_type = check_message_type(buffer);
    if (msg_type == MSG_PING)
    {
        struct ping_message pm = get_ping_message(buffer);
        printf("Odbieram ping, id: %d\n", pm.ping_id);
        send_ping_message(socket_fd, pm.ping_id);
    }
}

int main(int argc, char **argv)
{
    struct sigaction act_int;
    act_int.sa_handler = sig_int;
    sigemptyset(&act_int.sa_mask);
    act_int.sa_flags = 0;

    sigaction(SIGINT, &act_int, NULL);

    client_name = argv[1];

    if (strcmp(argv[2], "unix") == 0)
    {
        if (argc != 4)
        {
            printf("Zla ilosc argumentow!\n");
        }
        socket_mode = MODE_UNIX;
        unix_socket_path_name = argv[3];
    }
    else if (strcmp(argv[2], "inet") == 0)
    {
        if (argc != 5)
        {
            printf("Zla ilosc argumentow!\n");
        }
        socket_mode = MODE_INET;
        ip_address = argv[3];
        inet_port = (in_port_t)atoi(argv[4]);
    }
    else
    {
        printf("Zla ilosc argumentow!\n");
    }

    server_connect();

    char message_buffer[MESSAGE_SIZE];
    receive_message(socket_fd, message_buffer);
    struct login_message lm = get_login_message(message_buffer);

    if (strcmp(lm.string, "pending") == 0)
        printf("Ustanowiono polaczenie\n");
    else
    {
        printf("Odrzucono polaczenie!\n");
    }

    send_login_message(socket_fd, client_name);

    receive_message(socket_fd, message_buffer);
    lm = get_login_message(message_buffer);

    if (strcmp(lm.string, "accepted") == 0)
        printf("Zalogowano\n");
    else
        printf("Blad logowania!\n");

    while (1)
    {
        process_message();
    }
}