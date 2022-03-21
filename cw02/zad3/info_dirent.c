#include <stdio.h>
#include <dirent.h>

#define FALSE 0
#define TRUE 1


int main (int argc, char *argv[])
{
  if (argc < 2) {
    printf("Zbyt malo argumentow!");
    return 1;
  } 

  char* folder_path = argv[1];
  
  DIR* folder = opendir(folder_path);
  if (folder == NULL) {
    printf("Blad otwierania pliku!");
    return 2;
  }


  struct dirent* file;
  struct stat* buffer;
  while((file = readdir(folder)) != NULL) {

  }


  closedir(folder);
  return 0;
}
