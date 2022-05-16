#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define PROJ_ID 42

#define SEMAPHORES 6

#define TABLE 0
#define TABLE_PIZZA 1
#define TABLE_SPACE 2

#define OVEN 3
#define OVEN_PIZZA 4
#define OVEN_SPACE 5

#define TABLE_SIZE 5
#define OVEN_SIZE 5

#define MEMORY_SIZE sizeof(struct data)

struct data
{
    int table[TABLE_SIZE];
    int table_last;
    int oven[OVEN_SIZE];
    int oven_last;
    int running;
} * pizzeria;
int memory;
int semaphores;

struct
{
    struct sembuf to_oven[3];
    struct sembuf from_oven[3];
    struct sembuf close_oven[1];

    struct sembuf to_table[3];
    struct sembuf from_table[3];
    struct sembuf close_table[1];
} act = {
    {// Add pizza to the oven
     {OVEN, -1, 0},
     {OVEN_PIZZA, 1, 0},
     {OVEN_SPACE, -1, 0}},
    {// Take pizza from the oven
     {OVEN, -1, 0},
     {OVEN_PIZZA, -1, 0},
     {OVEN_SPACE, 1, 0}},
    {// Close the oven
     {OVEN, 1, 0}},
    {// Add pizza to the table
     {TABLE, -1, 0},
     {TABLE_PIZZA, 1, 0},
     {TABLE_SPACE, -1, 0}},
    {// Take pizza from the table
     {TABLE, -1, 0},
     {TABLE_PIZZA, -1, 0},
     {TABLE_SPACE, 1, 0}},
    {// Close the table
     {TABLE, 1, 0}}};

void clear() {
    // printf("\n\n");
    pizzeria->running = 0;
}

int cook(int key)
{
    int pid = getpid();
    int n, m, k;

    struct data *pizzeria = shmat(shmget(key, 0, S_IRUSR | S_IWUSR), NULL, 0);
    int semaphores = semget(key, 0, S_IRUSR | S_IWUSR);

    while (pizzeria->running)
    {
        n = rand() % 9;
        printf("%d\t%ld\tPrzygotowuje pizze: %d.\n", pid, time(NULL), n);
        sleep(1 + (float)rand() / RAND_MAX);

        semop(semaphores, act.to_oven, 3);
        // Dodaj pizze
        pizzeria->oven[pizzeria->oven_last] = n;
        m = ++pizzeria->oven_last;
        printf("%d\t%ld\tDodalem pizze: %d. Liczba pizz w piecu: %d.\n", pid, time(NULL), n, m);

        semop(semaphores, act.close_oven, 1);

        sleep(4 + (float)rand() / RAND_MAX);

        semop(semaphores, act.from_oven, 3);
        // Wyjmij pizze
        m = --pizzeria->oven_last;
        n = pizzeria->oven[pizzeria->oven_last];

        semop(semaphores, act.close_oven, 1);

        semop(semaphores, act.to_table, 3);
        // Daj na stol
        pizzeria->table[pizzeria->table_last] = n;
        k = ++pizzeria->table_last;
        printf("%d\t%ld\tWyjmuje pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole %d.\n", pid, time(NULL), n, m, k);

        semop(semaphores, act.close_table, 1);
    }

    shmdt(pizzeria);
}

int driver(int key)
{
    int pid = getpid();
    int n, k;

    struct data *pizzeria = shmat(shmget(key, 0, S_IRUSR | S_IWUSR), NULL, 0);
    int semaphores = semget(key, 0, S_IRUSR | S_IWUSR);

    while (pizzeria->running)
    {
        semop(semaphores, act.from_table, 3);

        // Pobierz pizze
        k = --pizzeria->table_last;
        n = pizzeria->table[k];
        printf("%d\t%ld\tPobieram pizze: %d. Liczba pizz na stole: %d.\n", pid, time(NULL), n, k);

        semop(semaphores, act.close_table, 1);

        sleep(4 + (float)rand() / RAND_MAX);
        printf("%d\t%ld\tDostarczam pizze: %d.\n", pid, time(NULL), n);
        sleep(4 + (float)rand() / RAND_MAX);
    }

    shmdt(pizzeria);
}

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        printf("Zbyt malo argumentow!\n");
        exit(1);
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    int key = ftok(".", PROJ_ID);
    memory = shmget(key, MEMORY_SIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    perror("Memory");
    pizzeria = shmat(memory, NULL, 0);
    perror("Table/oven");

    pizzeria->oven_last = 0;
    pizzeria->table_last = 0;
    pizzeria->running = 1;

    semaphores = semget(key, SEMAPHORES, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    union semun
    {
        int val;
    } arg;

    arg.val = 1;
    semctl(semaphores, TABLE, SETVAL, arg);
    semctl(semaphores, OVEN, SETVAL, arg);

    arg.val = 0;
    semctl(semaphores, TABLE_PIZZA, SETVAL, arg);
    semctl(semaphores, OVEN_PIZZA, SETVAL, arg);

    arg.val = TABLE_SIZE;
    semctl(semaphores, TABLE_SPACE, SETVAL, arg);

    arg.val = OVEN_SIZE;
    semctl(semaphores, OVEN_SPACE, SETVAL, arg);

    for (int i = 0; i < n; i++)
    {
        if (fork() == 0)
        {
            cook(key);
        }
    }

    for (int i = 0; i < m; i++)
    {
        if (fork() == 0)
        {
            driver(key);
        }
    }

    signal(SIGINT, clear);

    for (int i = 0; i < n + m; i++) {
        wait(NULL);
    }

    perror("\nStop");

    shmdt(pizzeria);
    shmctl(memory, IPC_RMID, NULL);
    semctl(semaphores, 0, IPC_RMID, NULL);
    semctl(semaphores, 1, IPC_RMID, NULL);
    semctl(semaphores, 2, IPC_RMID, NULL);
    semctl(semaphores, 3, IPC_RMID, NULL);
    semctl(semaphores, 4, IPC_RMID, NULL);
    semctl(semaphores, 5, IPC_RMID, NULL);

    return 0;
}
