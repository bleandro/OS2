#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DIR_NUMBER 255
#define MAX_STR 255
#define MAX_FILENAME_SIZE 255
#define SYSTEM_RESERVED_SPACE 10

#define SECTORS_NUMBER 256
#define BYTES_BY_SECTOR 512

// Used in metadata
typedef enum dt_type {dt_directory, dt_file} dt_type;

// We will actually use metadata just to distinguish directories and files
typedef struct dt_metadata {
  dt_type type;
} dt_metadata;

// Directory Table tell us where is the "Starting Block" of a file or directory
typedef struct directoryTable {
  char fileName[MAX_FILENAME_SIZE];
  int startingSector;
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
int fs_tree(); int fs_tree_aux(int parentBlock, int level);
int fs_help();
int fs_clear();
int fs_exit();

directoryTable dt_directoryTable[MAX_DIR_NUMBER];
fileAllocationTable fat_fileAllocationTable[SECTORS_NUMBER][BYTES_BY_SECTOR];

//Auxiliar functions
void init_directoryTable();
void init_fileAllocationTable();
int str_startsWith(const char *str, const char *pre);

int main() {
  char fullCommand[MAX_STR];
  char command[MAX_STR];

  init_directoryTable();
  init_fileAllocationTable();

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
    fgets(fullCommand, sizeof(fullCommand), stdin);
    if (fullCommand[strlen(fullCommand)-1] == '\n')
      fullCommand[strlen(fullCommand)-1] = '\0';

    char *command = strtok(fullCommand, " ");
    char *parameter = strtok(NULL, " ");

    if (strcmp(command, "mkdir") == 0)
      fs_mkdir(parameter);
    else if (strcmp(command, "mkfile") == 0)
      fs_mkfile(parameter);
    else if (strcmp(command, "rmdir") == 0)
      fs_rmdir(parameter);
    else if (strcmp(command, "rmfile") == 0)
      fs_rmfile(parameter);
    else if (strcmp(command, "lsdir") == 0)
      fs_lsdir(parameter);
    else if (strcmp(command, "lssec") == 0)
      fs_lssec(parameter);
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

int fs_mkdir(char* parameter) {
  int nextBlockIndex = -1;

  if (strchr(parameter, '/') != NULL) {
    char parentDir[MAX_STR];
    int slashLastIndex = (strrchr(parameter, '/') - parameter);
    strncpy(parentDir, parameter, slashLastIndex);
    parentDir[slashLastIndex] = '\0';

    int dir_index;
    for(dir_index = 0; dir_index < MAX_DIR_NUMBER; dir_index++)
      if (strcmp(dt_directoryTable[dir_index].fileName, parentDir) == 0)
        nextBlockIndex = dt_directoryTable[dir_index].startingBlockIndex;
  }

  int blank_dt_index;
  for(blank_dt_index = 0; blank_dt_index < MAX_DIR_NUMBER; blank_dt_index++)
    if (dt_directoryTable[blank_dt_index].startingBlockIndex == -1)
      break; // Stop "FOR" cause We've found one blank space

  int sec_iteration, blank_fat_index, foundSpace = 0;
  for(sec_iteration = SYSTEM_RESERVED_SPACE+1; (sec_iteration < SECTORS_NUMBER) && (!foundSpace); sec_iteration++)
    for(blank_fat_index = 0; (blank_fat_index < BYTES_BY_SECTOR) && (!foundSpace); blank_fat_index++)
      if (fat_fileAllocationTable[sec_iteration][blank_fat_index].isBusy == 0)
        foundSpace = 1; // Stop "FOR" cause We've found one blank space

  sec_iteration--; blank_fat_index--;

  fat_fileAllocationTable[sec_iteration][blank_fat_index].isBusy = 1;
  fat_fileAllocationTable[sec_iteration][blank_fat_index].nextBlockIndex = nextBlockIndex;

  strcpy(dt_directoryTable[blank_dt_index].fileName, parameter);
  dt_directoryTable[blank_dt_index].startingSector = sec_iteration;
  dt_directoryTable[blank_dt_index].startingBlockIndex = blank_fat_index;
  dt_directoryTable[blank_dt_index].metadata.type = dt_directory;
}

int fs_rmdir(char* parameter) {
  int dir_index;
  for(dir_index = 0; dir_index < MAX_DIR_NUMBER; dir_index++)
    if (strcmp(dt_directoryTable[dir_index].fileName, parameter) == 0)
      break;

  int startingSector = dt_directoryTable[dir_index].startingSector;
  int startingBlockIndex = dt_directoryTable[dir_index].startingBlockIndex;

  fat_fileAllocationTable[startingSector][startingBlockIndex].isBusy = 0;

  dt_directoryTable[dir_index].startingSector = -1;
  dt_directoryTable[dir_index].startingBlockIndex = -1;
}

int fs_mkfile(char* parameter) { fprintf(stderr, "mkfile: UNDER DEVELOPMENT\n"); }
int fs_rmfile(char* parameter) { fprintf(stderr, "rmfile: UNDER DEVELOPMENT\n"); }
int fs_lsdir(char* parameter) { fprintf(stderr, "lsdir: UNDER DEVELOPMENT\n"); }
int fs_lssec(char* parameter) { fprintf(stderr, "lssec: UNDER DEVELOPMENT\n"); }
int fs_map() { fprintf(stderr, "map: UNDER DEVELOPMENT\n"); }

int fs_tree() {
  fprintf(stdout, "ROOT\n");

  fs_tree_aux(-1, 0);
}

int fs_tree_aux(int parentBlock, int level) {
  int i;
  for(i = 0; i < MAX_DIR_NUMBER; i++) {
    if ((dt_directoryTable[i].startingBlockIndex > -1) && (dt_directoryTable[i].metadata.type == dt_directory)) {

        int nextBlockIndex = fat_fileAllocationTable[dt_directoryTable[i].startingSector][dt_directoryTable[i].startingBlockIndex].nextBlockIndex;

        char fileName[MAX_FILENAME_SIZE];
        strcpy(fileName, dt_directoryTable[i].fileName);

        if (nextBlockIndex == parentBlock)  {
          int spaces;
          for(spaces = 0; spaces < level ; spaces++)
            fprintf(stdout, "│   ");

          int slashLastIndex = (strrchr(fileName, '/') != NULL ) ? (strrchr(fileName, '/') - fileName + 1) : 0;
          fprintf(stdout, "├── %s\n", fileName + slashLastIndex);

          fs_tree_aux(dt_directoryTable[i].startingBlockIndex, level+1);
        }

    }
  }
}

int fs_help() {
  fprintf(stderr, "mkdir [path/]dirName    -> Creates directory\n");
  fprintf(stderr, "mkfile [path/]fileName  -> Creates file\n");
  fprintf(stderr, "rmdir [path/]dirName    -> Deletes directory\n");
  fprintf(stderr, "rmfile [path/]fileName  -> Deletes file\n");
  fprintf(stderr, "lsdir [path]            -> Shows directory\n");
  fprintf(stderr, "lssec [path/]fileName   -> Shows filled sectors by file\n");
  fprintf(stderr, "map                     -> Shows sector table\n");
  fprintf(stderr, "tree                    -> Directory tree\n");
  fprintf(stderr, "clear                   -> Clear screen\n");
  fprintf(stderr, "exit                    -> Exits the fileSystem\n");
}

int fs_clear() {
  int i;
  for (i = 0; i < 100; i++) fprintf(stdout, "\n");
}

int fs_exit() {
  exit(0);
}

// I'm lazy and don't want to separate in .h files
//-------------------------------------- Auxiliar functions ---------------------------------------
void init_directoryTable() {
  int i;
  for(i = 0; i < MAX_DIR_NUMBER; i++) {
    dt_directoryTable[i].startingSector = -1;
    dt_directoryTable[i].startingBlockIndex = -1;
  }
}

void init_fileAllocationTable() {
  int i, j;
  for(i = 0; i < SECTORS_NUMBER; i++)
    for (j = 0; j < BYTES_BY_SECTOR; j++) {
      fat_fileAllocationTable[i][j].nextBlockIndex = -1;
      fat_fileAllocationTable[i][j].isBusy = 0;
    }
}

int str_startsWith(const char *str, const char *pre) {
  return strncmp(pre, str, strlen(pre)) == 0;
}
