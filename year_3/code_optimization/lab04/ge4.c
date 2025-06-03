#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define BLKSIZE 8
#define MAX(a,b) (((a)>(b))?(a):(b))

static double gtod_ref_time_sec = 0.0;

double dclock() {
  double the_time, norm_sec;
  struct timeval tv;
  gettimeofday( &tv, NULL );
  if ( gtod_ref_time_sec == 0.0 ) {
    gtod_ref_time_sec = ( double ) tv.tv_sec;
  }
  norm_sec = ( double ) tv.tv_sec - gtod_ref_time_sec;
  return norm_sec + tv.tv_usec * 1.0e-6;
}

int gauss(double **A, int SIZE) {
  register int i,j,k;
  register double multiplier;
  for (k = 0; k < SIZE; k++) { 
    for (i = k+1; i < SIZE; i++) { 
      multiplier = A[i][k]/A[k][k];
      for (j = k+1; j < SIZE;) { 
        if (j < MAX(SIZE - BLKSIZE, 0)) {
          A[i][j] -= A[k][j] * multiplier;
          A[i][j+1] -= A[k][j+1] * multiplier;
          A[i][j+2] -= A[k][j+2] * multiplier;
          A[i][j+3] -= A[k][j+3] * multiplier;
          A[i][j+4] -= A[k][j+4] * multiplier;
          A[i][j+5] -= A[k][j+5] * multiplier;
          A[i][j+6] -= A[k][j+6] * multiplier;
          A[i][j+7] -= A[k][j+7] * multiplier;
          j += BLKSIZE;
        } else {
          A[i][j] -= A[k][j] * multiplier;
          j++;
        }
      } 
    }
  }
  return 0;
}

int main(void) {
    const int SIZE = 1500;

    double *flat = malloc((size_t)SIZE * SIZE * sizeof(double));
    double **matrix = malloc((size_t)SIZE * sizeof(double*));
    for (int i = 0; i < SIZE; i++) {
        matrix[i] = flat + (size_t)i * SIZE;
    }

    srand(42);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix[i][j] = rand() / (double)RAND_MAX;
        }
    }

    printf("calling gauss...\n");
    double t0 = dclock();
    gauss(matrix, SIZE);
    double elapsed = dclock() - t0;

    printf("Time: %1.6f sec\n", elapsed);

    double check = 0.0;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            check += matrix[i][j];
    printf("Check: %1e\n", check);

    free(matrix);
    free(flat);
    return 0;
}
