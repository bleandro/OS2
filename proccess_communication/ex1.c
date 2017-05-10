#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>

void print_hundred(int);

#define NUMBER_OF_PAGES 1

int main ()
{
  pid_t child_pid;
  int segment_id;
  char* shared_memory;
  struct shmid_ds shmbuffer;
  int segment_size;

  /* Allocate a shared memory segment, writes number 100 and deattach  */
  segment_id = shmget (IPC_PRIVATE, NUMBER_OF_PAGES * getpagesize(), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
  shared_memory = (char*) shmat(segment_id, 0, 0);
  sprintf(shared_memory, "%d", 100);
  shmdt (shared_memory);

  child_pid = fork();

  int i = 0;
  for(i = 0; i < 1000000; i++)
     print_hundred(segment_id);

  /* Deallocate the shared memory segment.  */
  shmctl (segment_id, IPC_RMID, 0);

  return 0;
}

void print_hundred(int segment_id)
{
  char* shared_memory;
  int number;

  /* Attach the shared memory segment */
  shared_memory = (char*) shmat(segment_id, 0, 0);

  // Reads the number. Decrement and save it again.
  sscanf(shared_memory, "%d", &number);
  number++;
  sprintf(shared_memory, "%d", number);

  // Reads the number. Increment and save it again.
  sscanf(shared_memory, "%d", &number);
  number--;
  sprintf(shared_memory, "%d", number);

  // Reads the number and print it
  sscanf(shared_memory, "%d", &number);
  printf("Number: %d - PID: %d\n", number, getpid());

  /* Deatch the shared memory segment.  */
  shmdt (shared_memory);
}
