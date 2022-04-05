#include <unistd.h>
#include <stdio.h>

int main() {
  int fd[2];

  pipe(fd);
  pid_t pid = fork();

  if (pid == 0) { // dziecko
    close(fd[1]); 
    char buffer[3];
    read(fd[0], &buffer, 3); // odczyt danych z potoku
    printf("%s\n", buffer);
  } else { // rodzic
    close(fd[0]);
    write(fd[1], "acb", 3); // zapis danych do potoku
  }
}
