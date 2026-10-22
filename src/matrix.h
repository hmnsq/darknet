#ifndef MATRIX_H
#define MATRIX_H
#include "darknet.h"

matrix copy_matrix(matrix m);
void print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, ចំនួនគត់ n);
matrix resize_matrix(matrix m, ចំនួនគត់ size);

float *pop_column(matrix *m, ចំនួនគត់ c);

#endif
