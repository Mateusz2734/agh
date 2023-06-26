#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct _matrix {
    int rows;
    int cols;
    double **data;
} Matrix;

// Initializes a matrix with given dimensions.
// Returns 1 on success, 0 on failure.
int create_matrix(Matrix *pmatrix, int rows, int cols);

int set(Matrix *pm, int row, int col, double val) {
    if (row < 0 || row >= pm->rows || col < 0 || col >= pm->cols || pm->data == NULL) {
        return 0;
    }

    pm->data[row][col] = val;

    return 1;
}

int get(Matrix *pm, int row, int col, double *pval) {
    if (row < 0 || row >= pm->rows || col < 0 || col >= pm->cols || pm->data == NULL) {
        return 0;
    }

    *pval = pm->data[row][col];

    return 1;
}

void create_identity_matrix(Matrix *pm, int size) {
    create_matrix(pm, size, size);

    for (int i = 0; i < size; i++) {
        set(pm, i, i, 1);
    }
} 

double dot_prod_vect(const double *v1, const double *v2, int size ) {
    double sum = 0;

    for (int i = 0; i < size; i++) {
        sum += v1[i] * v2[i];
    }

    return sum;
}

void mul_vect(double *result, const Matrix *pm, const double *vect) {
    for (int i = 0; i < pm->rows; i++) {
        result[i] = dot_prod_vect(pm->data[i], vect, pm->cols);
    }
}

void max_element_in_rows(double *maxdata, const Matrix *pm) {
    for (int i = 0; i < pm->rows; i++) {
        double max = pm->data[i][0];

        for (int j = 1; j < pm->cols; j++) {
            if (pm->data[i][j] > max) {
                max = pm->data[i][j];
            }
        }

        maxdata[i] = max;
    }
}

int main() {}