#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "bibl1.h"

void** allocate(int n) {
  if (n < 0) return NULL;

	void** table = calloc(n, sizeof(void*));
	printf("zarezerwowano");

	return table;
}

void count(char* file) {
  char* command = malloc(strlen(file) + 16);

  strcat(command, "cat ");
  strcat(command, file);
  strcat(command, " | wc > temp");
  system(command);

  free(command);
  printf("policzono");
}

void write_temp(void* buf) {
  int file_descriptor = open("./temp", O_RDONLY);

  struct stat file_stats;
  fstat(file_descriptor, &file_stats);
  int size = file_stats.st_size;
  
  read(file_descriptor, buf, size);
  printf("zapisano");
}

void clear(void** table, int index) {
  free(table[index]);
  printf("usunieto");
}
