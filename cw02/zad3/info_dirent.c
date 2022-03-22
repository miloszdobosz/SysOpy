#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define CWD_LENGTH 100


int main (int argc, char *argv[])
{
  if (argc < 2) {
    perror("Zbyt malo argumentow");
    return 1;
  } 

  char* folder_path = argv[1];
  
  DIR* folder = opendir(folder_path);
  if (folder == NULL) {
    perror("Blad otwierania pliku");
    return 2;
  }

  
  struct dirent* file;
  struct stat* buffer = malloc(sizeof(struct stat*));
  
  char cwd[CWD_LENGTH];
  getcwd(cwd, CWD_LENGTH);

  while((file = readdir(folder)) != NULL) {
    printf("Sciezka:\n\t%s/%s\n", cwd, file->d_name);

    if(stat(file->d_name, buffer) == -1) {
      perror("Blad odczytu stat");
    } else {
      printf("Liczba dowiazan:\n\t%lu\n", buffer->st_nlink);
      printf("Rodzaj pliku:\n\t");
      switch (buffer->st_mode & S_IFMT) {
        case S_IFREG:
          printf("file\n");
          break;
        case S_IFDIR:
          printf("dir\n");
          break;
        case S_IFCHR:
          printf("char dev\n");
          break;
        case S_IFBLK:
          printf("block dev\n");
          break;
        case S_IFIFO:
          printf("fifo\n");
          break;
        case S_IFLNK:
          printf("slink\n");
          break;
        case S_IFSOCK:
          printf("sock\n");
          break;
      }
      printf("Rozmiar:\n\t%lu\n", buffer->st_size);
      printf("Data ostatniego dostepu:\n\t%lu\n", buffer->st_atime);
      printf("Data ostatniej modyfikacji:\n\t%lu\n", buffer->st_mtime);
      printf("\n");
    }
  }


  free(buffer);
  closedir(folder);

  return 0;
}
