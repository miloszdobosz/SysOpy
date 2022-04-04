#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

int n = 0;
int alive = 1;

void increment(int sig_no) {
  n++;
}

void send(int sig_no, siginfo_t *info, void *ucontext) {
  for (int i = 0; i < n; i++) {
    if (info->si_code == SI_QUEUE) {
      union sigval value;
      value.sival_int = i;
      sigqueue(info->si_pid, SIGUSR1, value);
    } else {
      kill(info->si_pid, SIGUSR1);
    }
  }
  
  kill(info->si_pid, SIGUSR2);
  printf("Otrzymano sygnalow: %d\n", n);
  alive = 0;
}

int main (int argc, char *argv[])
{
  printf("%d\n", getpid());

  signal(SIGUSR1, increment);

  struct sigaction act;
  act.sa_sigaction = send;
  act.sa_flags = SA_SIGINFO;
  sigemptyset(&act.sa_mask);
  sigaction(SIGUSR2, &act, NULL);

  signal(SIGRTMIN, increment);
  sigaction(SIGRTMIN+1, &act, NULL);

  while(alive);
  return 0;
}
