#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define MEMORY_NAME "/memory"

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

struct
{
    sem_t *table;
    sem_t *table_pizza;
    sem_t *table_space;
    sem_t *oven;
    sem_t *oven_pizza;
    sem_t *oven_space;
} semaphores;

struct
{
    char *table;
    char *table_pizza;
    char *table_space;
    char *oven;
    char *oven_pizza;
    char *oven_space;
} names = {
    "/table",
    "/table_pizza",
    "/table_space",
    "/oven",
    "/oven_pizza",
    "/oven_space"};

void to_oven()
{
    sem_wait(semaphores.oven_space);
    sem_wait(semaphores.oven);
    sem_post(semaphores.oven_pizza);
}
void from_oven()
{
    sem_wait(semaphores.oven_pizza);
    sem_wait(semaphores.oven);
    sem_post(semaphores.oven_space);
}
void close_oven()
{
    sem_post(semaphores.oven);
}

void to_table()
{
    sem_wait(semaphores.table_space);
    sem_wait(semaphores.table);
    sem_post(semaphores.table_pizza);
}
void from_table()
{
    sem_post(semaphores.table_pizza);
    sem_wait(semaphores.table);
    sem_wait(semaphores.table_space);
}
void close_table()
{
    sem_post(semaphores.table);
}

void clear()
{
    pizzeria->running = 0;
}

void cook()
{
    int pid = getpid();
    int n, m, k;

    srand(pid);

    memory = shm_open(MEMORY_NAME, O_RDWR, 0);
    pizzeria = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memory, 0);

    semaphores.table = sem_open(names.table, O_RDWR);
    semaphores.table_pizza = sem_open(names.table_pizza, O_RDWR);
    semaphores.table_space = sem_open(names.table_space, O_RDWR);
    semaphores.oven = sem_open(names.oven, O_RDWR);
    semaphores.oven_pizza = sem_open(names.oven_pizza, O_RDWR);
    semaphores.oven_space = sem_open(names.oven_space, O_RDWR);
    perror("Semaphores cook");

    while (pizzeria->running)
    {
        n = rand() % 9;
        printf("%d\t%ld\tPrzygotowuje pizze: %d.\n", pid, time(NULL), n);
        sleep(1 + (float)rand() / RAND_MAX);

        to_oven();
        // Dodaj pizze
        pizzeria->oven[pizzeria->oven_last] = n;
        m = ++pizzeria->oven_last;
        printf("%d\t%ld\tDodalem pizze: %d. Liczba pizz w piecu: %d.\n", pid, time(NULL), n, m);
        close_oven();

        sleep(4 + (float)rand() / RAND_MAX);

        from_oven();
        // Wyjmij pizze
        m = --pizzeria->oven_last;
        n = pizzeria->oven[pizzeria->oven_last];
        close_oven();

        to_table();
        // Daj na stol
        pizzeria->table[pizzeria->table_last] = n;
        k = ++pizzeria->table_last;
        printf("%d\t%ld\tWyjmuje pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole %d.\n", pid, time(NULL), n, m, k);
        close_table();
    }

    munmap(pizzeria, MEMORY_SIZE);

    sem_close(semaphores.table);
    sem_close(semaphores.table_pizza);
    sem_close(semaphores.table_space);
    sem_close(semaphores.oven);
    sem_close(semaphores.oven_pizza);
    sem_close(semaphores.oven_space);
}

void driver()
{
    int pid = getpid();
    int n, k;

    srand(pid);

    memory = shm_open(MEMORY_NAME, O_RDWR, 0);
    pizzeria = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memory, 0);

    semaphores.table = sem_open(names.table, O_RDWR);
    semaphores.table_pizza = sem_open(names.table_pizza, O_RDWR);
    semaphores.table_space = sem_open(names.table_space, O_RDWR);
    semaphores.oven = sem_open(names.oven, O_RDWR);
    semaphores.oven_pizza = sem_open(names.oven_pizza, O_RDWR);
    semaphores.oven_space = sem_open(names.oven_space, O_RDWR);
    perror("Semaphores driver");

    while (pizzeria->running)
    {
        to_table();
        // Pobierz pizze
        k = --pizzeria->table_last;
        n = pizzeria->table[k];
        printf("%d\t%ld\tPobieram pizze: %d. Liczba pizz na stole: %d.\n", pid, time(NULL), n, k);
        close_table();

        sleep(4 + (float)rand() / RAND_MAX);
        printf("%d\t%ld\tDostarczam pizze: %d.\n", pid, time(NULL), n);
        sleep(4 + (float)rand() / RAND_MAX);
    }

    munmap(pizzeria, MEMORY_SIZE);

    sem_close(semaphores.table);
    sem_close(semaphores.table_pizza);
    sem_close(semaphores.table_space);
    sem_close(semaphores.oven);
    sem_close(semaphores.oven_pizza);
    sem_close(semaphores.oven_space);
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

    memory = shm_open(MEMORY_NAME, O_RDWR | O_CREAT | O_EXCL, 0);
    perror("Memory");
    ftruncate(memory, MEMORY_SIZE);
    pizzeria = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memory, 0);
    perror("Table/oven");

    pizzeria->oven_last = 0;
    pizzeria->table_last = 0;
    pizzeria->running = 1;

    semaphores.table = sem_open(names.table, O_RDWR | O_CREAT | O_EXCL, S_IRWXU, 1);
    semaphores.table_pizza = sem_open(names.table_pizza, O_RDWR | O_CREAT | O_EXCL, S_IRWXU, 0);
    semaphores.table_space = sem_open(names.table_space, O_RDWR | O_CREAT | O_EXCL, S_IRWXU, 5);
    semaphores.oven = sem_open(names.oven, O_RDWR | O_CREAT | O_EXCL, S_IRWXU, 1);
    semaphores.oven_pizza = sem_open(names.oven_pizza, O_RDWR | O_CREAT | O_EXCL, S_IRWXU, 0);
    semaphores.oven_space = sem_open(names.oven_space, O_RDWR | O_CREAT | O_EXCL, S_IRWXU, 5);
    perror("Semaphores");

    for (int i = 0; i < n; i++)
    {
        if (fork() == 0)
        {
            cook();
        }
    }

    for (int i = 0; i < m; i++)
    {
        if (fork() == 0)
        {
            driver();
        }
    }

    signal(SIGINT, clear);

    for (int i = 0; i < n + m; i++)
    {
        wait(NULL);
    }

    perror("\nStop");

    munmap(pizzeria, MEMORY_SIZE);
    shm_unlink(MEMORY_NAME);
    sem_close(semaphores.table);
    sem_close(semaphores.table_pizza);
    sem_close(semaphores.table_space);
    sem_close(semaphores.oven);
    sem_close(semaphores.oven_pizza);
    sem_close(semaphores.oven_space);

    sem_unlink(names.table);
    sem_unlink(names.table_pizza);
    sem_unlink(names.table_space);
    sem_unlink(names.oven);
    sem_unlink(names.oven_pizza);
    sem_unlink(names.oven_space);
   
    return 0;
}
