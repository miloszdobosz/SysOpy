#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int sig_no) {
  printf("Otrzymano\n");
}

void action() {

}

int main (int argc, char *argv[])
{
  struct sigaction act;
  act.sa_sigaction = action;
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_SIGINFO;
    
  sigaction(SIGUSR1, &act, NULL);
  
  raise(SIGUSR1);
  return 0;
}
