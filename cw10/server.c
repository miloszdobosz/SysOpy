#include "lib.h"

char *unix_socket_path_name;
int unix_socket_fd = -1;

in_port_t inet_port;
int inet_socket_fd = -1;

struct client registered_clients[CLIENTS_LIMIT];
int clients_count;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void open_server()
{
    struct sockaddr_un unix_address;
    strcpy(unix_address.sun_path, unix_socket_path_name);
    unix_address.sun_family = AF_UNIX;

    if ((unix_socket_fd = socket(AF_UNIX, SOCKET_PROTOCOL, 0)) == -1)
        printf("Blad tworzenia gniazda UNIX.\n");
    if ((bind(unix_socket_fd, (struct sockaddr *)&unix_address, sizeof(unix_address))) == -1)
        printf("Blad funkcji bind() dla gniazda UNIX.\n");
    if ((listen(unix_socket_fd, CLIENTS_LIMIT)) == -1)
        printf("Blad funkcji listen() dla gniazda UNIX.\n");

    printf("Otwarto gniazdo UNIX: %s\n", unix_socket_path_name);

    struct hostent *host_entry = gethostbyname("localhost");
    struct in_addr host_address = *(struct in_addr *)host_entry->h_addr;

    struct sockaddr_in inet_address;
    inet_address.sin_family = AF_INET;
    inet_address.sin_addr.s_addr = host_address.s_addr;
    inet_address.sin_port = htons(inet_port);

    if ((inet_socket_fd = socket(AF_INET, SOCKET_PROTOCOL, 0)) == -1)
        printf("Blad tworzenia gniazda INET.\n");
    if ((bind(inet_socket_fd, (struct sockaddr *)&inet_address, sizeof(inet_address))) == -1)
        printf("Blad funkcji bind() dla gniazda INET.\n");
    if ((listen(inet_socket_fd, CLIENTS_LIMIT)) == -1)
        printf("Blad funkcji listen() dla gniazda INET.\n");

    printf("Otwarto gniazdo INET: %s:%d\n", inet_ntoa(host_address), inet_port);
}

void close_server()
{
    if (unix_socket_fd != -1)
    {
        close(unix_socket_fd);
        unlink(unix_socket_path_name);
    }

    if (inet_socket_fd != -1)
    {
        close(inet_socket_fd);
    }
}

void sig_int(int signum)
{
    exit(EXIT_SUCCESS);
}

// HELPER - iterates through array of pollfd structures and finds first descriptor that is ready to read from.
int get_input_ready_fd(struct pollfd sockets[], int sockets_length)
{
    for (int i = 0; i < sockets_length; i++)
        if (((sockets[i].revents) & POLLIN) != 0)
            return sockets[i].fd;

    return -1; // TODO: losuje
}

int remove_client(int client_fd, int lock)
{
    if (lock == 1)
        pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < CLIENTS_LIMIT; i++)
        if (registered_clients[i].socket_fd == client_fd)
        {
            printf("Usuwam klienta %d\n", registered_clients[i].client_id);
            registered_clients[i].socket_fd = -1;
            registered_clients[i].registered = 0;
            // registered_clients[i].name[0] = '\0';
            close(client_fd);

            if (lock == 1)
                pthread_mutex_unlock(&clients_mutex);

            return 1;
        }

    if (lock == 1)
        pthread_mutex_unlock(&clients_mutex);

    return 0;
}

int register_client(int client_fd, const char *name)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < CLIENTS_LIMIT; i++)
    {
        if (strcmp(name, registered_clients[i].name) == 0)
        {
            printf("client's name %s already in use\n", name);

            send_login_message(client_fd, "rejected");
            remove_client(client_fd, 0); // potrzebne? tak jest w tresci?
            pthread_mutex_unlock(&clients_mutex);
            return 0;
        }
    }
    for (int i = 0; i < CLIENTS_LIMIT; i++)
    {
        if (registered_clients[i].socket_fd == client_fd)
        {
            strcpy(registered_clients[i].name, name);
            registered_clients[i].registered = 1;
            registered_clients[i].active = 1;

            printf("client's name %s successfully registered\n", name);

            send_login_message(client_fd, "accepted");
            pthread_mutex_unlock(&clients_mutex);
            return 1;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    return -1;
}

// Sets active field in proper client struct as a response to ping message.
int set_active(int client_fd)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < CLIENTS_LIMIT; i++)
    {
        if (registered_clients[i].socket_fd == client_fd)
        {
            registered_clients[i].active = 1;
            pthread_mutex_unlock(&clients_mutex);
            return 1;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    return 0;
}

void process_message(int client_fd, char message[MESSAGE_SIZE])
{
    int msg_type = check_message_type(message);
    if (msg_type == MSG_LOGIN)
    {
        struct login_message lm = get_login_message(message);
        if (strcmp(lm.string, "logout") == 0)
            remove_client(client_fd, 1);
        else
            register_client(client_fd, lm.string);
    }
    else if (msg_type == MSG_PING)
    {
        struct ping_message pm = get_ping_message(message);
        printf("ping response, id: %d\n", pm.ping_id);
        set_active(client_fd);
    }
}

int get_next_registered_client(int starting_point)
{
    pthread_mutex_lock(&clients_mutex);
    int iter = (starting_point + 1) % CLIENTS_LIMIT;

    for (int i = 0; i < CLIENTS_LIMIT - 1; i++)
    {
        if (registered_clients[iter].registered == 1)
        {
            pthread_mutex_unlock(&clients_mutex);
            return iter;
        }
        iter = (iter + 1) % CLIENTS_LIMIT;
    }

    pthread_mutex_unlock(&clients_mutex);
    return -1;
}

void *connection_function(void *arg)
{
    struct pollfd sockets[2 + CLIENTS_LIMIT];

    sockets[CLIENTS_LIMIT + 1].fd = inet_socket_fd;
    sockets[CLIENTS_LIMIT + 1].events = POLLIN;

    sockets[CLIENTS_LIMIT].fd = unix_socket_fd;
    sockets[CLIENTS_LIMIT].events = POLLIN;

    while (1)
    {
        for (int i = 0; i < CLIENTS_LIMIT; i++)
        {
            sockets[i].fd = registered_clients[i].socket_fd;
            sockets[i].events = POLLIN;
            sockets[i].revents = 0;
        }

        for (int i = 0; i < CLIENTS_LIMIT + 2; i++)
            sockets[i].revents = 0;

        poll(sockets, CLIENTS_LIMIT + 2, -1);

        for (int i = 0; i < CLIENTS_LIMIT; i++)
        {
            if (sockets[i].revents & POLLHUP)
            {
                remove_client(sockets[i].fd, 1);
                sockets[i].revents = 0;
            }
        }

        int socket = get_input_ready_fd(sockets, CLIENTS_LIMIT + 2);

        if (socket == -1)
            continue;

        if (socket == unix_socket_fd)
            printf("Nowe polaczenie na sockecie UNIX.\n");
        else if (socket == inet_socket_fd)
            printf("Nowe polaczenie na sockecie INET.\n");
        else
        {
            char msg_buffer[MESSAGE_SIZE];
            receive_message(socket, msg_buffer);
            process_message(socket, msg_buffer);
            continue;
        }

        pthread_mutex_lock(&clients_mutex);

        int empty_slot = -1;
        for (int i = 0; i < CLIENTS_LIMIT; i++)
        {
            if (registered_clients[i].socket_fd == -1)
            {
                empty_slot = i;
                break;
            }
        }

        int client_fd = accept(socket, NULL, 0);
        if (empty_slot == -1)
        {
            printf("no empty slots found - rejecting connection\n");
            send_login_message(client_fd, "rejected");
            close(client_fd);
        }
        else
        {
            printf("new connection saved to slot %d\n", empty_slot);
            registered_clients[empty_slot].socket_fd = client_fd;
            send_login_message(client_fd, "pending");
        }

        pthread_mutex_unlock(&clients_mutex);
    }

    return (void *)0;
}

void *ping_function(void *arg)
{
    while (1)
    {
        sleep(PING_INTERVAL);
        pthread_mutex_lock(&clients_mutex);

        for (int i = 0; i < CLIENTS_LIMIT; i++)
        {
            if (registered_clients[i].registered == 1)
            {
                registered_clients[i].active = 0;
                send_ping_message(registered_clients[i].socket_fd, i);
            }
        }

        pthread_mutex_unlock(&clients_mutex);

        printf("waiting for responses\n");
        sleep(PING_TIMEOUT);

        pthread_mutex_lock(&clients_mutex);

        for (int i = 0; i < CLIENTS_LIMIT; i++)
        {
            if (registered_clients[i].registered == 1 && registered_clients[i].active == 0)
            {
                printf("client at slot %d did not respond to ping message (removing client)\n", i);
                registered_clients[i].registered = 0;
                registered_clients[i].name[0] = '\0';
                close(registered_clients[i].socket_fd);
                registered_clients[i].socket_fd = -1;
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }

    return (void *)0;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Zla ilosc argumentow!\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < CLIENTS_LIMIT; i++)
    {
        registered_clients[i].client_id = i;
        registered_clients[i].socket_fd = -1;
        registered_clients[i].registered = 0;
        registered_clients[i].active = 0;
        registered_clients[i].name[0] = '\0';
    }

    atexit(close_server);

    struct sigaction act_int;
    act_int.sa_handler = sig_int;
    sigemptyset(&act_int.sa_mask);
    act_int.sa_flags = 0;
    sigaction(SIGINT, &act_int, NULL);

    inet_port = (in_port_t)atoi(argv[1]);
    unix_socket_path_name = argv[2];

    open_server();

    pthread_t connection_thread;
    pthread_t ping_thread;

    pthread_create(&connection_thread, NULL, connection_function, NULL);
    pthread_create(&ping_thread, NULL, ping_function, NULL);

    pthread_join(connection_thread, NULL);
    pthread_join(ping_thread, NULL);

    return 0;
}
