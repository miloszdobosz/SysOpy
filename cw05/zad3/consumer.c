#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Zbyt malo argumentow!\n");
        return 1;
    }

    char *fifo_path = argv[1];
    char *text_path = argv[2];
    int n = atoi(argv[3]);

    FILE *text = fopen(text_path, "w");
    FILE *fifo = fopen(fifo_path, "r");
    if (text == NULL || fifo == NULL)
    {
        printf("Blad otwierania pliku!\n");
        return 2;
    }

    char buffer[n];
    int number[1];

    while (fread(number, sizeof(int), 1, fifo) == 1)
    {
        fread(buffer, sizeof(char), n, fifo);

        fseek(text, number[0] * n, SEEK_SET);
        fwrite(buffer, sizeof(char), n, text);
    }

    fclose(text);
    fclose(fifo);
    return 0;
}
