#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

char* getFilename(int n) {
  char number[10];
  sprintf(number, "%d", n);

  char* filename = malloc(5 + sizeof(number));
  filename[0] = '\0';

  strcat(filename, "w");
  strcat(filename, number);
  strcat(filename, ".txt");

  return filename;
}

int main (int argc, char *argv[]) {
  if (argc < 3) {
    printf("Zbyt malo argumentow!\n");
    return 1;
  } 


  int slices = atoi(argv[1]);
  int processes = atoi(argv[2]);

  double dx = 1.0/slices;

  for (int i = 0; i < processes; i++) {
    if (fork() == 0) {
      double area = 0.0; 

      for (double x = (double) i/processes; x < (double) (i + 1)/processes; x += dx) {
        area += (4*dx)/(x*x + 1);
      }

      char* filename = getFilename(i);
      FILE* file = fopen(filename, "w");
      fprintf(file, "%f", area);
      free(filename);
      fclose(file);

      return 0;
    }
  }
  

  for (int i = 0; i < processes; i++) {
    wait(0);
  }

  double area;
  double total = 0.0;

  for (int i = 0; i < processes; i++) {
    char* filename = getFilename(i);
    FILE* file = fopen(filename, "r");
    fscanf(file, "%lf", &area);
    free(filename);
    fclose(file);

    total += area;
  }

  printf("%f\n", total);

  return 0;
}
