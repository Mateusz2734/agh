#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define BLKSIZE 8
#define MAX(a,b) (((a)>(b))?(a):(b))
#define IDX(i, j, n) (((j)+ (i)*(n)))

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

int gauss(double *A, int SIZE) {
  register int i,j,k;
  register double mul;
  for (k = 0; k < SIZE; k++) { 
    for (i = k+1; i < SIZE; i++) { 
      mul = A[IDX(i,k,SIZE)] / A[IDX(k,k,SIZE)];
      for (j = k+1; j < SIZE;) { 
        if (j < MAX(SIZE - BLKSIZE, 0)) {
          A[IDX(i, j, SIZE)]   = A[IDX(i,j,SIZE)]  - A[IDX(k, j, SIZE)]*mul;
          A[IDX(i, j+1, SIZE)] = A[IDX(i,j+1,SIZE)] - A[IDX(k, j+1, SIZE)]*mul;
          A[IDX(i, j+2, SIZE)] = A[IDX(i,j+2,SIZE)] - A[IDX(k, j+2, SIZE)]*mul;
          A[IDX(i, j+3, SIZE)] = A[IDX(i,j+3,SIZE)] - A[IDX(k, j+3, SIZE)]*mul;
          A[IDX(i, j+4, SIZE)] = A[IDX(i,j+4,SIZE)] - A[IDX(k, j+4, SIZE)]*mul;
          A[IDX(i, j+5, SIZE)] = A[IDX(i,j+5,SIZE)] - A[IDX(k, j+5, SIZE)]*mul;
          A[IDX(i, j+6, SIZE)] = A[IDX(i,j+6,SIZE)] - A[IDX(k, j+6, SIZE)]*mul;
          A[IDX(i, j+7, SIZE)] = A[IDX(i,j+7,SIZE)] - A[IDX(k, j+7, SIZE)]*mul;
          j += BLKSIZE;
        } else {
          A[IDX(i, j, SIZE)]= A[IDX(i,j,SIZE)]-A[IDX(k, j, SIZE)]*mul;
          j++;
        }
      } 
    }
  }
  return 0;
}

int main(void) {
    const int SIZE = 1500;

    double* matrix = (double*) malloc(SIZE*SIZE*sizeof(double));

    srand(42);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix[IDX(i,j,SIZE)] = rand() / (double)RAND_MAX;
        }
    }

    printf("Calling gauss...\n");
    double t0 = dclock();
    gauss(matrix, SIZE);
    double elapsed = dclock() - t0;

    printf("Time: %1.6f sec\n", elapsed);

    double check = 0.0;
    for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {
        check += matrix[IDX(i,j,SIZE)];
      }
    }
    printf("Check: %1e\n", check);

    free(matrix);
    return 0;
}
