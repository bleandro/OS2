#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

/* BEGIN SEMAPHORE */

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short int *array;
  struct seminfo *__buf;
};

int binary_semaphore_allocation (key_t key, int sem_flags)
{
  return semget (key, 1, sem_flags);
}

int binary_semaphore_deallocate (int semid)
{
  union semun ignored_argument;
  return semctl (semid, 1, IPC_RMID, ignored_argument);
}

int binary_semaphore_initialize (int semid)
{
  union semun argument;
  unsigned short values[1];
  values[0] = 1;
  argument.array = values;
  return semctl (semid, 0, SETALL, argument);
}

int binary_semaphore_wait (int semid)
{
  struct sembuf operations[1];
  /* Use the first (and only) semaphore.  */
  operations[0].sem_num = 0;
  /* Decrement by 1.  */
  operations[0].sem_op = -1;
  /* Permit undo'ing.  */
  operations[0].sem_flg = SEM_UNDO;

  return semop (semid, operations, 1);
}

int binary_semaphore_post (int semid)
{
  struct sembuf operations[1];
  /* Use the first (and only) semaphore.  */
  operations[0].sem_num = 0;
  /* Increment by 1.  */
  operations[0].sem_op = 1;
  /* Permit undo'ing.  */
  operations[0].sem_flg = SEM_UNDO;

  return semop (semid, operations, 1);
}

/* END SEMAPHORE */

void print_hundred(int, int);

#define NUMBER_OF_PAGES 1

int main ()
{
  pid_t child_pid;
  int segment_id;
  char* shared_memory;
  struct shmid_ds shmbuffer;
  int segment_size;

  int semid = binary_semaphore_allocation(IPC_PRIVATE, IPC_CREAT | S_IRUSR | S_IWUSR);
  binary_semaphore_initialize(semid);

  /* Allocate a shared memory segment, writes number 100 and deattach  */
  segment_id = shmget (IPC_PRIVATE, NUMBER_OF_PAGES * getpagesize(), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
  shared_memory = (char*) shmat(segment_id, 0, 0);
  sprintf(shared_memory, "%d", 100);
  shmdt (shared_memory);

  child_pid = fork();

  int i = 0;
  for(i = 0; i < 1000000; i++)
     print_hundred(semid, segment_id);

  /* Deallocate the shared memory segment.  */
  shmctl (segment_id, IPC_RMID, 0);
  binary_semaphore_deallocate(semid);

  return 0;
}

void print_hundred(int semid, int segment_id)
{
  char* shared_memory;
  int number;

  /* Attach the shared memory segment */
  shared_memory = (char*) shmat(segment_id, 0, 0);

  binary_semaphore_wait(semid);

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

  binary_semaphore_post(semid);

  /* Deatch the shared memory segment.  */
  shmdt (shared_memory);
}
