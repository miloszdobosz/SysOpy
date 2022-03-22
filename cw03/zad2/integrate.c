#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  if (argc < 3) {
    printf("Zbyt malo argumentow!\n");
    return 1;
  } 


  int slices = atoi(argv[1]);
  int processes = atoi(argv[2]);

  double dx = 1/slices;

  for (int i = 0; i < processes; i++) {
    if (fork() == 0) {
      double area = 0.0; 
      for (double x = i/processes; x < (i + 1)/processes; x += dx) {
        area += 4/(x*x + 1);
      }

      char* number = itoa(i);
      char filename[5 + sizeof(number)];
      strcat(filename, "w");
      strcat(filename, number);
      strcat(filename, ".txt");

      FILE* file = fopen(filename, "w");
      fprintf(file, "%f", i, area);
      
      return 0;
    }
  }
  
  for (size_t i = 0; i < processes; i++) {
    wait(0);
  }
  return 0;
}
