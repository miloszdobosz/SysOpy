#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "bibl1.h"

char** allocate(int n) {
  if (n < 0) return NULL;

	char** table = calloc(n, sizeof(void*));
	printf("zarezerwowano");

	return table;
}

void count(char* file) {
  char* command = calloc(strlen(file) + 16, 1);

  strcat(command, "cat ");
  strcat(command, file);
  strcat(command, " | wc > temp");
  system(command);

  free(command);
  printf("policzono");
}

char* write_temp() {
  int file_descriptor = open("./temp", O_RDONLY);
  
  struct stat file_stats;
  fstat(file_descriptor, &file_stats);
  int size = file_stats.st_size;

  char* buf = malloc(size);
  
  read(file_descriptor, buf, size);
  printf("zapisano");

  return buf;
}

void clear(char** table, int index) {
  free(table[index]);
  printf("usunieto");
}
