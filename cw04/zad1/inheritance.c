#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>

void handler(int signum) {
  printf("Otrzymano sygnal o numerze %d.\n", signum);
}

int main (int argc, char *argv[])
{
  if (argc < 2) {
    printf("Zbyt malo argumentow!");
    return 1;
  }

  if (strcmp(argv[1], "ignore") == 0) {
    signal(SIGUSR1, SIG_IGN);
  } else if (strcmp(argv[1], "handler") == 0) {
    signal(SIGUSR1, handler);
  } else if (strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0) {
    sigset_t newmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &newmask, NULL);
    if (strcmp(argv[1], "pending") == 0) {
      sigset_t pendingmask;
      sigpending(&pendingmask);
      printf("Czy sygnal widoczny: %d\n", sigismember(&pendingmask, SIGUSR1));
    }
  } else {
    printf("Argument niepoprawny!\n");
    return 2;
  }
  
  raise(SIGUSR1);

  if (fork() == 0) {
    if (strcmp(argv[1], "pending")) {
      sigset_t pendingmask;
      sigpending(&pendingmask);
      printf("%d\n", sigismember(&pendingmask, SIGUSR1));
    } else {
      raise(SIGUSR1);
    }
  } else {
    execl("./to_exec.c", "", NULL);
  }

  return 0;
}
