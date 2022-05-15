#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
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
};

int cook(int key)
{
    int pid = getpid();
    int n, m, k;

    struct data *table_oven = shmat(shmget(key, MEMORY_SIZE, S_IRUSR | S_IWUSR), NULL, 0);
    int semaphores = semget(key, SEMAPHORES, S_IRUSR | S_IWUSR);

    struct sembuf table_buffer[3];
    struct sembuf oven_buffer[3];

    table_buffer[0].sem_num = TABLE;
    table_buffer[1].sem_num = TABLE_PIZZA;
    table_buffer[2].sem_num = TABLE_SPACE;

    table_buffer[0].sem_flg = 0;
    table_buffer[1].sem_flg = 0;
    table_buffer[2].sem_flg = 0;

    oven_buffer[0].sem_num = OVEN;
    oven_buffer[1].sem_num = OVEN_PIZZA;
    oven_buffer[2].sem_num = OVEN_SPACE;

    oven_buffer[0].sem_flg = 0;
    oven_buffer[1].sem_flg = 0;
    oven_buffer[2].sem_flg = 0;

    while (1)
    {
        n = rand() % 9;
        printf("%d\t%ld\nPrzygotowuje pizze: %d.\n\n", pid, time(NULL), n);
        sleep(1 + (float)rand() / RAND_MAX);
        printf("dlaksfjasfd");

        oven_buffer[0].sem_op = -1;
        oven_buffer[1].sem_op = 1;
        oven_buffer[2].sem_op = -1;
        semop(semaphores, oven_buffer, 3);
        // Dodaj pizze
        table_oven->oven[table_oven->oven_last] = n;
        m = ++table_oven->oven_last;
        printf("%d\t%ld\nDodalem pizze: %d. Liczba pizz w piecu: %d.\n\n", pid, time(NULL), n, m);

        oven_buffer[0].sem_op = 1;
        semop(semaphores, oven_buffer, 1);

        printf("dlaksfjasfd");

        sleep(4 + (float)rand() / RAND_MAX);

        oven_buffer[0].sem_op = -1;
        oven_buffer[1].sem_op = -1;
        oven_buffer[2].sem_op = 1;
        semop(semaphores, oven_buffer, 3);
        // Wyjmij pizze
        m = --table_oven->oven_last;
        n = table_oven->oven[table_oven->oven_last];

        oven_buffer[0].sem_op = 1;
        semop(semaphores, oven_buffer, 1);

        table_buffer[0].sem_op = -1;
        table_buffer[1].sem_op = 1;
        table_buffer[2].sem_op = -1;
        semop(semaphores, table_buffer, 3);
        // Daj na stol
        table_oven->table[table_oven->table_last] = n;
        k = ++table_oven->table_last;
        printf("%d\t%ld\nWyjmuje pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole %d.\n\n", pid, time(NULL), n, m, k);

        table_buffer[0].sem_op = 1;
        semop(semaphores, table_buffer, 1);
    }
}

int driver(int key)
{
    int pid = getpid();
    int n, k;

    struct data *table_oven = shmat(shmget(key, MEMORY_SIZE, S_IRUSR | S_IWUSR), NULL, 0);
    int semaphores = semget(key, SEMAPHORES, S_IRUSR | S_IWUSR);

    table_oven->oven_last = 0;
    table_oven->table_last = 0;

    struct sembuf table_buffer[3];

    table_buffer[0].sem_num = TABLE;
    table_buffer[1].sem_num = TABLE_PIZZA;
    table_buffer[2].sem_num = TABLE_SPACE;

    table_buffer[0].sem_flg = 0;
    table_buffer[1].sem_flg = 0;
    table_buffer[2].sem_flg = 0;

    while (1)
    {
        table_buffer[0].sem_op = -1;
        table_buffer[1].sem_op = -1;
        table_buffer[2].sem_op = 1;
        semop(semaphores, table_buffer, 3);

        // Pobierz pizze
        k = --table_oven->table_last;
        n = table_oven->table[k];
        printf("%d\t%ld\nPobieram pizze: %d. Liczba pizz na stole: %d.\n\n", pid, time(NULL), n, k);

        table_buffer[0].sem_op = 1;
        semop(semaphores, table_buffer, 1);

        sleep(4 + (float)rand() / RAND_MAX);
        printf("%d\t%ld\nDostarczam pizze: %d.\n", pid, time(NULL), n);
        sleep(4 + (float)rand() / RAND_MAX);
    }
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
    struct data *table_oven =
        shmat(shmget(key, MEMORY_SIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR), NULL, 0);
    int semaphores = semget(key, SEMAPHORES, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

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

    return 0;
}
