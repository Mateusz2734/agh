#include <stdlib.h>

typedef struct _matrix {
    int rows;
    int cols;
    double **val;
    double *storage;
} Matrix;

static int glob_columns;

int create_matrix(Matrix *pmatrix, int rows, int cols) {
    pmatrix->rows = rows;
    pmatrix->cols = cols;
    
    if (rows <= 0 || cols <= 0) {
        return 0;
    }

    pmatrix->storage = (double *)calloc(rows * cols, sizeof(double));

    if (pmatrix->storage == NULL) {
        return 0;
    }

    pmatrix->val = (double **)calloc(rows, sizeof(double *));

    if (pmatrix->val == NULL) {
        free(pmatrix->storage);
        return 0;
    }

    for (int i = 0; i < rows; i++) {
        pmatrix->val[i] = pmatrix->storage + i * cols;
    }

    return 1;
}

double gen_rand() {
    return rand() / ((double) RAND_MAX);
}

Matrix random_matrix(int rows, int cols) {
    Matrix mat;

    create_matrix(&mat, rows, cols);

    for (int i = 0; i < rows * cols; i++) {
        mat.storage[i] = gen_rand();
    }

    return mat;
}

void copy_matrix(Matrix *ptrg, const Matrix *psrc) {
    create_matrix(ptrg, psrc->rows, psrc->cols);

    for (int i = 0; i < psrc->rows * psrc->cols; i++) {
        ptrg->storage[i] = psrc->storage[i];
    }
}

void multiply_by(Matrix *pmatrix, double v) {
    for (int i = 0; i < pmatrix->rows * pmatrix->cols; i++) {
        pmatrix->storage[i] *= v;
    }
}

int add(Matrix *presult, const Matrix *pm1, const Matrix *pm2) {
    if (pm1->rows != pm2->rows || pm1->cols != pm2->cols) {
        return 0;
    }

    create_matrix(presult, pm1->rows, pm1->cols);

    for (int i = 0; i < pm1->rows * pm1->cols; i++) {
        presult->storage[i] = pm1->storage[i] + pm2->storage[i];
    }

    return 1;
}

double find_average(const double *row) {
    double sum = 0;

    for (int i = 0; i < glob_columns; i++) {
        sum += row[i];
    }

    return sum / (glob_columns);
}

int compar(const void *a, const void *b) {
    double *row1 = (double *)a;
    double *row2 = (double *)b;

    double avg1 = find_average(row1);
    double avg2 = find_average(row2);

    if (avg1 < avg2) {
        return -1;
    } else if (avg1 > avg2) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    // qsort()
}