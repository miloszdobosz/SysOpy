#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/times.h>

#include "bibl1.h"

int main(int argc, char* argv[]) {

  char** table;
  int n;
  int j = 0;

  int mode = 0;
  
  struct timespec *real_start = malloc(sizeof(struct timespec));
  struct timespec *real_end = malloc(sizeof(struct timespec));
  struct tms *start = malloc(sizeof(struct tms));
  struct tms *end = malloc(sizeof(struct tms));

  clock_gettime(CLOCK_REALTIME, real_start);
  times(start);
  
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "ct") == 0) {
      mode = 1;
    } else if (strcmp(argv[i], "wc") == 0) {      
      mode = 2;
    } else if (strcmp(argv[i], "rb") == 0) {
      mode = 3;
    } else {
      switch (mode) {
        case 0:
          printf("Argument %d incorrect, skipping.\n", i);
          break;
        case 1:
          n = atoi(argv[i]);
          table = allocate(n);
          mode = 0;
          break;
        case 2:
          count(argv[i]);
          if (++j >= n) {
            printf("Too few blocks allocated, skipping.\n");
            continue;
          }
          table[j] = write_temp();
          break;
        case 3:
          int index = atoi(argv[i]);
          if (index < 0 || index >= n) {
            printf("Index %d out of bounds 0-%d, skipping.\n", index, n);
            continue;
          }
          clear(table, index);
          break;
      }
    }
  }

  times(end);
  clock_gettime(CLOCK_REALTIME, real_end);

  FILE* fp = fopen("raport2.txt", "a");
  fprintf(fp, "%lld.%.9ld ", (long long) real_end->tv_sec - real_start->tv_sec, real_end->tv_nsec - real_start->tv_nsec);
  fprintf(fp, "%ld ", end->tms_utime - start->tms_utime);
  fprintf(fp, "%ld\n", end->tms_stime - start->tms_stime);

//  for (int i = 0; i < n; i++) {
//    free(table[i]);
//  }
}
