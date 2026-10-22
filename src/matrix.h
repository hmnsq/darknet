#ifndef MATRIX_H
#define MATRIX_H
#include "darknet.h"

matrix copy_matrix(matrix m);
void print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, ကိန်းပြည့် n);
matrix resize_matrix(matrix m, ကိန်းပြည့် size);

float *pop_column(matrix *m, ကိန်းပြည့် c);

#endif
