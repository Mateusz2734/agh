#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define IDX(i, j, n) (((j)+ (i)*(n)))

// static double gtod_ref_time_sec = 0.0;

// double dclock() {
//   double the_time, norm_sec;
//   struct timeval tv;
//   gettimeofday( &tv, NULL );
//   if ( gtod_ref_time_sec == 0.0 ) {
//     gtod_ref_time_sec = ( double ) tv.tv_sec;
//   }
//   norm_sec = ( double ) tv.tv_sec - gtod_ref_time_sec;
//   return norm_sec + tv.tv_usec * 1.0e-6;
// }

void ref_gauss(double *A, int SIZE) {
  int i,j,k;
  double mul;
  for (k = 0; k < SIZE; k++) { 
    for (i = k + 1; i < SIZE; i++) { 
      mul = A[IDX(i, k, SIZE)] / A[IDX(k, k, SIZE)];
      for (j = k + 1; j < SIZE; j++) {
        A[IDX(i, j, SIZE)] -= A[IDX(k, j, SIZE)] * mul;
      } 
    }
  }
}

// int main(void) {
//     const int SIZE = 40;

//     double *flat = malloc((size_t)SIZE * SIZE * sizeof(double));
//     double **matrix = malloc((size_t)SIZE * sizeof(double*));
//     for (int i = 0; i < SIZE; i++) {
//         matrix[i] = flat + (size_t)i * SIZE;
//     }

//     srand(42);
//     for (int i = 0; i < SIZE; i++) {
//         for (int j = 0; j < SIZE; j++) {
//             matrix[i][j] = rand() / (double)RAND_MAX;
//         }
//     }

//     printf("calling gauss...\n");
//     double t0 = dclock();
//     gauss(matrix, SIZE);
//     double elapsed = dclock() - t0;

//     printf("Time: %1.6f sec\n", elapsed);

//     double check = 0.0;
//     for (int i = 0; i < SIZE; i++)
//         for (int j = 0; j < SIZE; j++)
//             check += matrix[i][j];
//     printf("Check: %1e\n", check);

//     free(matrix);
//     free(flat);
//     return 0;
// }
