#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        printf("Zbyt malo argumentow!\n");
        return 1;
    }

    char *fifo_path = argv[1];
    int row = atoi(argv[2]);
    char *text_path = argv[3];
    int n = atoi(argv[4]);

    FILE *text = fopen(text_path, "r");
    FILE *fifo = fopen(fifo_path, "w");
    if (text == NULL || fifo == NULL)
    {
        printf("Blad otwierania pliku!\n");
        return 2;
    }

    char buffer[n];

    while (fread(buffer, sizeof(char), n, text) == n)
    {
        sleep(rand() / (float) RAND_MAX + 1);

        fwrite(&row, sizeof(int), 1, fifo);
        fwrite(buffer, sizeof(char), n, fifo);
    }

    fclose(text);
    fclose(fifo);
    return 0;
}
