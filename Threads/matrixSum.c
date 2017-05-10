#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define MAX_i 4
#define MAX_j 4

int matrix[MAX_i][MAX_j]
                 = {1, 2, 3, 4,
                    5, 6, 7, 8,
                    9, 10, 11, 12,
                    13, 14, 15, 16};

// Realiza soma nas linhas da matriz
void* sum_matrix_t (void* row_t)
{
  int row = *((int*) row_t);
  int j, result = 0;

  for(j = 0; j < MAX_j; j++)
     result += matrix[row][j];

  return (void*) result;
}

int main ()
{
  pthread_t t1, t2, t3, t4;

  int result = 0;
  int row_t1, row_t2, row_t3, row_t4, tr1, tr2, tr3, tr4;

  //Create thread to sum row 1
  row_t1 = 0;
  pthread_create(&t1, NULL, &sum_matrix_t, &row_t1);

  //Create thread to sum row 2
  row_t2 = 1;
  pthread_create(&t2, NULL, &sum_matrix_t, &row_t2);

  //Create thread to sum row 3
  row_t3 = 2;
  pthread_create(&t3, NULL, &sum_matrix_t, &row_t3);

  //Create thread to sum row 4
  row_t4 = 3;
  pthread_create(&t4, NULL, &sum_matrix_t, &row_t4);

  pthread_join(t1, (void*) &tr1);
  pthread_join(t2, (void*) &tr2);
  pthread_join(t3, (void*) &tr3);
  pthread_join(t4, (void*) &tr4);

  result = tr1 + tr2 + tr3 + tr4;

  fprintf(stdout, "Resultado final: %d\n", result);
  return 0;
}
