#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void typePrompt();
void replaceHomeDir(char*);

int main() {
  char command[255];

  while(1) {                            // Repeats forever
    typePrompt();                       // Shows prompt on screen

    // Reads and parser user input
    fgets(command, sizeof(command), stdin);
    command[strlen(command)-1] = '\0';

    // If the user types "exit" the shell is exited
    if (strcmp(command, "exit") == 0) exit(0);

    // Counts tokens
    char *token;
    char tokenCommand[255];
    strcpy(tokenCommand, command);
    int argCount = 0;
    for(token = strtok(tokenCommand, " "); token != NULL; token = strtok(NULL, " "))
      argCount++;

    // Allocate the arg_list
    char** arg_list = (char**) malloc((argCount+1) * sizeof(char*));

    // Copies the tokens values to arg_list
    int i = 0;
    strcpy(tokenCommand, command);
    for(token = strtok(tokenCommand, " "); token != NULL; token = strtok(NULL, " ")) {
      arg_list[i] = (char*) malloc(255 * sizeof(char));
      strcpy(arg_list[i], token);
      i++;
    }
    arg_list[i] = NULL;

    // Check for "Change Dir (CD)"
    if (strcmp(arg_list[0], "cd") == 0) {
       if (argCount == 1) {
          char homeDir[50] = "";
          strcpy(homeDir, getenv("HOME"));

          arg_list[1] = (char*) malloc(255 * sizeof(char));
          strcpy(arg_list[1], homeDir);
       }

       if (chdir(arg_list[1]) != 0)
          fprintf(stderr, "cd: '%s' file or directory not found\n", arg_list[1]);

       continue;
    }

    pid_t child_pid = fork();           // Creates child proccess

    // If there was any problem creating child proccess
    if (child_pid < 0) {
      fprintf(stderr, "Unexpected error\n");
      continue;
    }

    int status;
    if (child_pid != 0) {
      waitpid(-1, &status, 0);        // Waiting child proccess
    } else {
      execvp(arg_list[0], arg_list);     // Child does its work
      fprintf(stderr, "Command '%s' not found\n", arg_list[0]);
    }
  }

}

void typePrompt(){
  //Initiate prompt
  fprintf(stdout, "[MySh] ");

  //Prompt user and hostname
  char username[50] = "";
  strcpy(username, getenv("USER"));
  char hostname[50] = "";
  gethostname(hostname, sizeof(hostname));
  fprintf(stdout, "%s@%s:", username, hostname);

  //Prompt current directory
  char currDir[255] = "";
  getcwd(currDir, sizeof(currDir));
  replaceHomeDir(currDir);
  fprintf(stdout, "%s$ ", currDir);
}

void replaceHomeDir(char* currDir) {
  char homeDir[50] = "";
  strcpy(homeDir, getenv("HOME"));

  char tmpCurrDir[255] = "";
  strcpy(tmpCurrDir, currDir);
  tmpCurrDir[strlen(homeDir)] = '\0';

  if (strcmp(tmpCurrDir, homeDir) == 0) {
    strncpy(tmpCurrDir, currDir + strlen(homeDir), strlen(currDir) - strlen(homeDir) + 1);
    strcpy(currDir, "~");
    strcat(currDir, tmpCurrDir);
  }
}
