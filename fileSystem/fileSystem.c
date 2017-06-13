#include<stdio.h>
#include<string.h>
#include <stdlib.h>

#define MAX_STR 255
#define MAX_FILENAME_SIZE 30
#define SYSTEM_RESERVED_SPACE 10

// Used in metadata
typedef enum dt_type {directory, file} dt_type;

// We will actually use metadata just to distinguish directories and files and fileSize
typedef struct dt_metadata {
  dt_type type;
} dt_metadata;

// Directory Table tell us where is the "Starting Block" of a file or directory
typedef struct directoryTable {
  char fileName[MAX_FILENAME_SIZE];
  int startingBlockIndex;
  dt_metadata metadata;
} directoryTable;

// File Allocation Table used to allocate file blocks
typedef struct fileAllocationTable {
  int isBusy; /* 0 for false or 1 for true */
  int nextBlockIndex;
} fileAllocationTable;

//My fileSystem functions
int fs_mkdir(char*);
int fs_mkfile(char*);
int fs_rmdir(char*);
int fs_rmfile(char*);
int fs_lsdir(char*);
int fs_lssec(char*);
int fs_map();
int fs_tree();
int fs_help();
int fs_clear();
int fs_exit();

//Auxiliar functions

int main() {
  char command[MAX_STR];
  char tokenCommand[MAX_STR];

  fprintf(stderr, "  ____________                   \n");
  fprintf(stderr, " < fileSystem >                  \n");
  fprintf(stderr, "  ------------                   \n");
  fprintf(stderr, "         \\   ^__^               \n");
  fprintf(stderr, "          \\  (oo)\\_______      \n");
  fprintf(stderr, "             (__)\\       )\\/\\ \n");
  fprintf(stderr, "                 ||----w |       \n");
  fprintf(stderr, "                 ||     ||       \n");

  while(1) {
    fprintf(stdout, "->");
    fgets(command, sizeof(command), stdin);
    if (command[strlen(command)-1] == '\n')
      command[strlen(command)-1] = '\0';

    strcpy(tokenCommand, command);
    char *parameter = strtok(tokenCommand, " ");

    fprintf(stderr, "%s\n", parameter);

    if (strcmp(command, "mkdir") == 0)
      fs_mkdir("");
    else if (strcmp(command, "mkfile") == 0)
      fs_mkfile("");
    else if (strcmp(command, "rmdir") == 0)
      fs_rmdir("");
    else if (strcmp(command, "rmfile") == 0)
      fs_rmfile("");
    else if (strcmp(command, "lsdir") == 0)
      fs_lsdir("");
    else if (strcmp(command, "lssec") == 0)
      fs_lssec("");
    else if (strcmp(command, "map") == 0)
      fs_map();
    else if (strcmp(command, "tree") == 0)
      fs_tree();
    else if (strcmp(command, "help") == 0)
      fs_help();
    else if (strcmp(command, "clear") == 0)
      fs_clear();
    else if (strcmp(command, "exit") == 0)
      fs_exit();
  }

  return 0;
}

int fs_mkdir(char* parameter) { fprintf(stderr, "mkdir: UNDER DEVELOPMENT\n"); }
int fs_mkfile(char* parameter) { fprintf(stderr, "mkfile: UNDER DEVELOPMENT\n"); }
int fs_rmdir(char* parameter) { fprintf(stderr, "rmdir: UNDER DEVELOPMENT\n"); }
int fs_rmfile(char* parameter) { fprintf(stderr, "rmfile: UNDER DEVELOPMENT\n"); }
int fs_lsdir(char* parameter) { fprintf(stderr, "lsdir: UNDER DEVELOPMENT\n"); }
int fs_lssec(char* parameter) { fprintf(stderr, "lssec: UNDER DEVELOPMENT\n"); }
int fs_map() { fprintf(stderr, "map: UNDER DEVELOPMENT\n"); }
int fs_tree() { fprintf(stderr, "tree: UNDER DEVELOPMENT\n"); }

int fs_help() {
  fprintf(stderr, "mkdir [path\\]dirName    -> Creates directory\n");
  fprintf(stderr, "mkfile [path\\]fileName  -> Creates file\n");
  fprintf(stderr, "rmdir [path\\]dirName    -> Deletes directory\n");
  fprintf(stderr, "rmfile [path\\]fileName  -> Deletes file\n");
  fprintf(stderr, "lsdir [path]            -> Shows directory\n");
  fprintf(stderr, "lssec [path\\]fileName   -> Shows filled sectors by file\n");
  fprintf(stderr, "map                     -> Shows sector table\n");
  fprintf(stderr, "tree                    -> Directory tree\n");
  fprintf(stderr, "clear                   -> Clear screen\n");
  fprintf(stderr, "exit                    -> Exits the fileSystem\n");
}

int fs_clear() { fprintf(stderr, "clear: UNDER DEVELOPMENT"); }

int fs_exit() {
  exit(0);
}
