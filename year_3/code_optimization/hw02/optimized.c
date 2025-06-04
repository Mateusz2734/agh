#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <arm_neon.h>

#define BLKSIZE 8
#define IDX(i, j, n) (((j)+ (i)*(n)))

void gauss(double *A, int SIZE) {
  register int i, j, k;
  register double mul;
  register float64x2_t mul_vec;
  for (k = 0; k < SIZE; k++) {
    for (i = k + 1; i < SIZE; i++) {
      mul = A[IDX(i, k, SIZE)] / A[IDX(k, k, SIZE)];
      mul_vec = vdupq_n_f64(mul);
      for (j = k + 1; j < SIZE;) {
        if (j <= SIZE - BLKSIZE) {
          float64x2_t aik0 = vld1q_f64(&A[IDX(i, j + 0, SIZE)]);
          float64x2_t akk0 = vld1q_f64(&A[IDX(k, j + 0, SIZE)]);
          float64x2_t aik1 = vld1q_f64(&A[IDX(i, j + 2, SIZE)]);
          float64x2_t akk1 = vld1q_f64(&A[IDX(k, j + 2, SIZE)]);
          float64x2_t aik2 = vld1q_f64(&A[IDX(i, j + 4, SIZE)]);
          float64x2_t akk2 = vld1q_f64(&A[IDX(k, j + 4, SIZE)]);
          float64x2_t aik3 = vld1q_f64(&A[IDX(i, j + 6, SIZE)]);
          float64x2_t akk3 = vld1q_f64(&A[IDX(k, j + 6, SIZE)]);

          aik0 = vmlsq_f64(aik0, akk0, mul_vec);
          aik1 = vmlsq_f64(aik1, akk1, mul_vec);
          aik2 = vmlsq_f64(aik2, akk2, mul_vec);
          aik3 = vmlsq_f64(aik3, akk3, mul_vec);

          vst1q_f64(&A[IDX(i, j + 0, SIZE)], aik0);
          vst1q_f64(&A[IDX(i, j + 2, SIZE)], aik1);
          vst1q_f64(&A[IDX(i, j + 4, SIZE)], aik2);
          vst1q_f64(&A[IDX(i, j + 6, SIZE)], aik3);

          j += BLKSIZE;
        } else {
          A[IDX(i, j, SIZE)] -= A[IDX(k, j, SIZE)] * mul;
          j++;
        }
      }
    }
  }
}

int main(void) {
    const int SIZE = 5000;

    double* matrix = (double*) malloc(SIZE*SIZE*sizeof(double));

    srand(42);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix[IDX(i,j,SIZE)] = rand() / (double)RAND_MAX;
        }
    }

    gauss(matrix, SIZE);

    free(matrix);
    return 0;
}
