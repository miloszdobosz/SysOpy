#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

int total;
int n = 0;
int catcher_got = 0;
int alive = 1;
int sq = 0;

void increment(int sig_no, siginfo_t *info, void *ucontext) {
  if (info->si_code == SI_QUEUE) {
    catcher_got = info->si_value.sival_int;
    printf("%d\n", catcher_got);
  }
  n++;
}

void finish(int sig_no) {
  printf("Otrzymano sygnalow: %d\n", n);
  printf("Powinienem otrzymac: %d\n", total);
  if (sq) {
    printf("Catcher otrzymal: %d\n", catcher_got);
  }
  alive = 0;
}

int main (int argc, char* argv[])
{
  if (argc < 4) {
    printf("Zbyt malo argumentow!\n");
    return 1;
  }

  int pid = atoi(argv[1]);
  total = atoi(argv[2]);

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
     
  if (strcmp(argv[3], "KILL") == 0) {
    for (int i = 0; i < total; i++) {
      kill(pid, SIGUSR1); 
      sigsuspend(&set);
    }
    kill(pid, SIGUSR2);
  } else if (strcmp(argv[3], "SIGQUEUE") == 0) {
    union sigval value;
    sq = 1;

    for (int i = 0; i < total; i++) {
      value.sival_int = i;
      sigqueue(pid, SIGUSR1, value); 
      sigsuspend(&set);
    }
    kill(pid, SIGUSR2);
  } else if (strcmp(argv[3], "SIGRT") == 0) {

    sigemptyset(&set);
    sigaddset(&set, SIGRTMIN);
    
    for (int i = 0; i < total; i++) {
      kill(pid, SIGRTMIN); 
      sigsuspend(&set);
    }
    kill(pid, SIGRTMIN+1);
  } else {
    printf("Tryb niepoprawny!\n");
    return 2;
  }

  signal(SIGRTMIN+1, finish);

  while(alive);
  return 0;
}
