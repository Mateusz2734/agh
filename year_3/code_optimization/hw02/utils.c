#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#define A(i, j) a[(j) * SIZE + (i)]
#define B(i, j) b[(j) * SIZE + (i)]
#define abs(x) ((x) < 0.0 ? - (x) : (x))

double compare_matrices(double *a, double *b, int SIZE) {
  int i, j;
  double max_diff = 0.0, diff;

  for (j = 0; j < SIZE; j++) {
    for (i = 0; i < SIZE; i++){
      diff = abs(A(i, j) - B(i, j));
      max_diff = (diff > max_diff ? diff : max_diff);
    }
  }

  return max_diff;
}

void copy_matrix(double *a, double *b, int SIZE) {
  int i, j;

  for (j = 0; j < SIZE; j++) {
    for (i = 0; i < SIZE; i++) {
      B(i, j) = A(i, j);
    }
  }
}

void random_matrix(double *a, int SIZE) {
  double drand48();
  int i, j;

  for (j = 0; j < SIZE; j++) {
    for (i = 0; i < SIZE; i++) {
      A(i, j) = 2.0 * drand48() - 1.0;
    }
  }
}

void print_matrix(double *a, int SIZE )
{
  int i, j;

  for ( j=0; j<SIZE; j++ ){
    for ( i=0; i<SIZE; i++ )
      printf("%le ", A( i,j ) );
    printf("\n");
  }
  printf("\n");
}

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