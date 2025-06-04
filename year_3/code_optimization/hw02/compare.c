#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void ref_gauss(double*, int);
void gauss(double*, int);

double compare_matrices(double*, double*, int);
void copy_matrix(double*, double*, int);
double dclock();
void random_matrix(double *, int);
void print_matrix(double *a, int SIZE);

#define PFIRST 40
#define PLAST 2000
#define PINC 40
#define NREPEATS 2

double calculate_gflops(int size) {
    double flops = 0;
    for (int i = 0; i < size; i++) {
        flops += (size - i - 1) + 2 * (size - i - 1) * (size - i - 1);
    }
    return flops * 1.0e-09;
}


int main() {
  int p, rep;
  double dtime, dtime_best, gflops, diff;
  double *a, *aold, *aref;

  srand(42);

  for (p = PFIRST; p <= PLAST; p += PINC) {
    gflops = calculate_gflops(p);

    a = (double *)malloc(PLAST * (PLAST + 1) * sizeof(double));
    aold = (double *)malloc(PLAST * PLAST * sizeof(double));
    aref = (double *)malloc(PLAST * PLAST * sizeof(double));

    random_matrix(a, p);

    copy_matrix(a, aref, p);
    copy_matrix(a, aold, p);

    ref_gauss(aref, p);
    for (rep = 0; rep < NREPEATS; rep++) {
      copy_matrix(a, aold, p);
      dtime = dclock();
      gauss(aold, p);
      dtime = dclock() - dtime;

      if (rep == 0) {
        dtime_best = dtime;
      } else {
        dtime_best = (dtime < dtime_best ? dtime : dtime_best);
      }
    }

    diff = compare_matrices(aold, aref, p);

    printf("{\"size\": %d, \"gflops\": %le, \"diff\": %le, \"time\": %le}\n", p, gflops / dtime_best, diff, dtime_best);
    fflush(stdout);

    free(a);
    free(aold);
    free(aref);
  }

  exit(0);
}