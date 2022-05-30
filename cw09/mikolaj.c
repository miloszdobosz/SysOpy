#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define RUNDY 3

#define RENIFERY 1
#define ELFY 2

pthread_t mikolaj_thread;
pthread_t renifery_thread[9];
pthread_t elfy_thread[10];

pthread_t *elfy_kolejka[3];

int czekajace_renifery, czekajace_elfy;
int praca = 0;

int dzialanie = 1;

pthread_mutex_t renifery_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t elfy_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t praca_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t elfy_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t praca_cond = PTHREAD_COND_INITIALIZER;

void *mikolaj(void *_)
{
    pthread_mutex_lock(&praca_mutex);
    for (int i = 0; i < RUNDY; i++)
    {
        while (!praca)
        {
            pthread_cond_wait(&praca_cond, &praca_mutex);
            printf("Mikolaj: budze sie");

            if (praca == RENIFERY)
            {
                pthread_mutex_lock(&renifery_mutex);
                czekajace_renifery = 0;
                printf("Mikolaj: dostarczam zabawki");
                sleep(2 + 2 * rand() / RAND_MAX);
                pthread_mutex_unlock(&renifery_mutex);
            }
            else if (praca == ELFY)
            {
                pthread_mutex_lock(&elfy_mutex);
                czekajace_elfy = 0;
                printf(
                    "Mikolaj: rozwiazuje problemy elfow %ld %ld %ld",
                    *elfy_kolejka[0],
                    *elfy_kolejka[1],
                    *elfy_kolejka[2]);
                pthread_mutex_unlock(&elfy_mutex);
            }
        }
    }
    pthread_mutex_unlock(&praca_mutex);
    dzialanie = 0;
    return NULL;
}

void *renifer(pthread_t *id)
{
    while (dzialanie)
    {
        sleep(5 + 5 * (float)rand() / RAND_MAX);

        pthread_mutex_lock(&renifery_mutex);
        printf("Renifer: czeka %d reniferow na Mikolaja, %ld\n", ++czekajace_renifery, *id);
        if (czekajace_renifery == 10)
        {
            pthread_mutex_lock(&praca_mutex);
            praca = RENIFERY;
            printf("Renifer: wybudzam Mikolaja, %ld\n", *id);
            pthread_cond_broadcast(&praca_cond);
            pthread_mutex_unlock(&praca_mutex);
        }
        pthread_mutex_unlock(&renifery_mutex);
    }
    return NULL;
}

void *elf(pthread_t *id)
{
    while (dzialanie)
    {
        sleep(2 + 3 * (float)rand() / RAND_MAX);
        pthread_mutex_lock(&elfy_mutex);
        while (czekajace_elfy == 3)
        {
            printf("Elf: czeka na powrot elfow, %ld\n", *id);
            pthread_cond_wait(&elfy_cond, &elfy_mutex);
        }
        elfy_kolejka[czekajace_elfy++] = id;
        if (czekajace_elfy == 3)
        {
            pthread_mutex_lock(&praca_mutex);
            praca = ELFY;
            printf("Elf: wybudzam Mikolaja, %ld\n", *id);
            pthread_cond_broadcast(&praca_cond);
            pthread_mutex_unlock(&praca_mutex);
        }
        pthread_mutex_unlock(&elfy_mutex);

        printf("Elf: Mikolaj rozwiazuje problem, %ld\n", *id);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_mutex_init(&elfy_mutex, NULL);
    pthread_mutex_init(&renifery_mutex, NULL);
    pthread_mutex_init(&praca_mutex, NULL);

    pthread_cond_init(&elfy_cond, NULL);
    pthread_cond_init(&praca_cond, NULL);

    pthread_create(&mikolaj_thread, NULL, mikolaj, NULL);

    for (int i = 0; i < 9; i++)
    {
        pthread_create(&renifery_thread[i], NULL, mikolaj, &renifery_thread[i]);
    }

    for (int i = 0; i < 10; i++)
    {
        pthread_create(&elfy_thread[i], NULL, mikolaj, &elfy_thread[i]);
    }

    pthread_join(mikolaj_thread, NULL);

    for (int i = 0; i < 9; i++)
    {
        pthread_join(renifery_thread[i], NULL);
    }

    for (int i = 0; i < 10; i++)
    {
        pthread_join(elfy_thread[i], NULL);
    }

    pthread_mutex_destroy(&elfy_mutex);
    pthread_mutex_destroy(&renifery_mutex);
    pthread_mutex_destroy(&praca_mutex);

    pthread_cond_destroy(&elfy_cond);
    pthread_cond_destroy(&praca_cond);

    return 0;
}
