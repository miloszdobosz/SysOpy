#include <unistd.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
  int fd[2];
  pipe(fd);

  pid_t pid = fork();
  if (pid == 0) { // dziecko
    close(fd[1]); 
    dup2(fd[0], STDIN_FILENO);
    execlp("grep", "grep", "Ala", NULL);
  } else { // rodzic
    close(fd[0]);
    write(fd[1], "Ala ma kota", 11); // przesłanie danych do grep-a
  }
  return 0;
}
