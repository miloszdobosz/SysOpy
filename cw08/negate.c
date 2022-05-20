#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define LINE_MAX 70
#define VAL_MAX 255

void numbers(void *arg)
{
    
}

void block(void *arg)
{
}

int *read(FILE *file)
{
    char buffer[3];
    fscanf(file, "%s", buffer);
    if (strcmp(buffer, "P2") != 0)
    {
        printf("Niepoprawny typ pliku wejsciowego!\n");
        exit(5);
    }

    int width, height, max_val;
    fscanf(file, "%d %d %d", &width, &height, &max_val);

    int *picture = malloc((3 + width * height) * sizeof(int));
    picture[0] = width;
    picture[1] = height;
    picture[2] = max_val;

    for (int i = 3; i < width * height + 3; i++)
    {
        fscanf(file, "%d", &picture[i]);
    }

    return picture;
}

void write(FILE *file, int *picture)
{
    fprintf(file, "P2\n%d %d\n%d\n", picture[0], picture[1], picture[2]);

    for (int i = 3; i < picture[0] * picture[1] + 3; i++)
    {
        fprintf(file, "%d ", picture[i]);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        printf("Zbyt malo argumentow!\n[ilosc watkow] [numbers/block] [wejscie] [wyjscie]\n");
        exit(1);
    }

    void *function;

    int n_threads = atoi(argv[1]);
    char *mode = argv[2];
    char *in_path = argv[3];
    char *out_path = argv[4];

    if (strcmp(mode, "numbers") == 0)
    {
        function = numbers;
    }
    else if (strcmp(mode, "block") == 0)
    {
        function = block;
    }
    else
    {
        printf("Niepoprawny tryb!\n");
        exit(2);
    }

    FILE *in = fopen(in_path, "r");
    if (in == NULL)
    {
        perror("Blad otwierania pliku wejsciowego");
        exit(3);
    }

    FILE *out = fopen(out_path, "w");
    if (out == NULL)
    {
        perror("Blad otwierania pliku wyjsciowego");
        exit(4);
    }

    pthread_t threads[n_threads];

    int *picture = read(in);

    // for (int i = 0; i < n_threads; i++)
    // {
    //     pthread_create(&threads[i], NULL, function, "A");
    // }

    write(out, picture);

    free(picture);
    return 0;
}
