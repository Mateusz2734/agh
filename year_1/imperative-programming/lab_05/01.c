#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n);
void print_vector(double x[], int n);
void read_mat(double A[][SIZE], int m, int n);
void print_mat(double A[][SIZE], int m, int n);
int diagonal_zeros(double A[SIZE][SIZE], int n);
int diagonal_almost_zeros(double A[SIZE][SIZE], int n, double eps);
int is_upper_triangular(double A[SIZE][SIZE], int n);
void swap_rows(double A[SIZE][SIZE], int n, int i, int j);
void backward_substitution(double A[][SIZE], double b[SIZE], double x[SIZE], int n);
void swap_indices(int indices[], int i, int j);
void swap_double(double *a, double *b);
int column_max(double A[][SIZE], int n, int k);
void generate_indices(int indices[], int n);
int max_diagonal_lt_eps(double A[][SIZE], int n, double eps);

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			AB[i][j] = 0;
			for (int k = 0; k < p; k++) {
				AB[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
	double det = 1;
	if (diagonal_zeros(A, n)) {
		return NAN;
	}

	while (!is_upper_triangular(A, n)) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < i; j++) {
				double factor = A[i][j] / A[j][j];
				for (int k = 0; k < n; k++) {
					A[i][k] -= factor * A[j][k];
				}
			}
		}
	}
	
	for (int i = 0; i < n; i++) {
		det *= A[i][i];
	}
	
	return det;
}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.
double gauss(double A[][SIZE], double b[], double x[], const int n, const double eps) {
    int indices[SIZE];
	double det = 1;
	int max;

    generate_indices(indices, n);

    for (int i = 0; i < n; i++) {
		if (max_diagonal_lt_eps(A, n, eps)) {
			return 0;
		}

		if (is_upper_triangular(A, n)) {
			break;
		}
		max = column_max(A, n, i);
        if (max != i) {
            swap_rows(A, n, i, max);
            swap_indices(indices, i, max);
            swap_double(&b[i], &b[max]);
            det *= -1;
        }
        for (int j = i + 1; j < n; j++) {
            double factor = A[j][i] / A[i][i];
            for (int k = i; k < n; k++) {
                A[j][k] -= factor * A[i][k];
            }
            b[j] -= factor * b[i];
        }
    }

    for (int i = 0; i < n; i++) {
        det *= A[i][i];
    }

	if (det != 0 && b != NULL && x != NULL) {
		backward_substitution(A, b, x, n);
	}

    return det;
}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
			if (det) print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

int diagonal_zeros(double A[SIZE][SIZE], int n) {
	int zeros = 0;
	for (int i = 0; i < n; i++) {
		if (A[i][i] == 0) {
			zeros++;
		}
	}
	return zeros;
}

int diagonal_almost_zeros(double A[SIZE][SIZE], int n, double eps) {
	int zeros = 0;
	for (int i = 0; i < n; i++) {
		if (fabs(A[i][i]) < eps) {
			zeros++;
		}
	}
	return zeros;
}

int is_upper_triangular(double A[SIZE][SIZE], int n) {
	for (int i = 1; i < n; i++) {
		for (int j = 0; j < i; j++) {
			if (A[i][j] != 0) {
				return 0;
			}
		}
	}
	return 1;
}

int spec_is_upper_triangular(double A[SIZE][SIZE], int n, double eps) {
	for (int i = 1; i < n; i++) {
		for (int j = 0; j < i; j++) {
			if (A[i][j] < eps) {
				return 0;
			}
		}
	}
	return 1;
}

void swap_rows(double A[SIZE][SIZE], int n, int i, int j) {
	double temp;
	for (int k = 0; k < n; k++) {
		temp = A[i][k];
		A[i][k] = A[j][k];
		A[j][k] = temp;
	}
}

void backward_substitution(double A[][SIZE], double b[SIZE], double x[SIZE], int n) {
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0;
        for (int j = i + 1; j < n; j++) {
            sum += A[i][j] * x[j];
        }
        x[i] = (b[i] - sum) / A[i][i];
    }
}

void swap_indices(int indices[], int i, int j) {
    int temp = indices[i];
    indices[i] = indices[j];
    indices[j] = temp;
}

void swap_double(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

int column_max(double A[][SIZE], int n, int k) {
    int max = -1, max_id = -1;
    for (int i = k; i < n; i++) {
        if (A[i][k] > max) {
            max = A[i][k];
            max_id = i;
        }
    }
    return max_id;
}

void generate_indices(int indices[], int n) {
    for (int i = 0; i < n; i++) {
        indices[i] = i;
    }
}

int max_diagonal_lt_eps(double A[][SIZE], int n, double eps) {
	double max = -1;
	for (int i = 0; i < n; i++) {
		if (fabs(A[i][i]) > max) {
			max = fabs(A[i][i]);
		}
	}
	return max < eps;
}