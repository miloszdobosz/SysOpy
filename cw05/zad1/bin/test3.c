#include <unistd.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
  FILE* grep_input = popen("grep Ala", "w");
  fputs("Ala ma kota", grep_input); // przesłanie danych do grep-a
  pclose(grep_input);

  return 0;
}
