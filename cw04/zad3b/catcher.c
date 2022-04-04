#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

int n = 0;
int alive = 1;

void increment(int sig_no, siginfo_t *info, void *ucontext) {
  n++;
  if (info->si_code == SI_QUEUE) {
      union sigval value;
      value.sival_int = n;
      sigqueue(info->si_pid, SIGUSR1, value);
    } else {
      kill(info->si_pid, SIGUSR1);
    }
}

void send(int sig_no, siginfo_t *info, void *ucontext) {
  kill(info->si_pid, SIGUSR2);
  printf("Otrzymano sygnalow: %d\n", n);
  alive = 0;
}

int main (int argc, char *argv[])
{
  printf("%d\n", getpid());

  struct sigaction act1;
  act1.sa_sigaction = increment;
  act1.sa_flags = SA_SIGINFO;
  sigemptyset(&act1.sa_mask);
  sigaction(SIGUSR1, &act1, NULL);

  struct sigaction act;
  act.sa_sigaction = send;
  act.sa_flags = SA_SIGINFO;
  sigemptyset(&act.sa_mask);
  sigaction(SIGUSR2, &act, NULL);

  sigaction(SIGRTMIN, &act1, NULL);
  sigaction(SIGRTMIN+1, &act, NULL);

  while(alive);
  return 0;
}
