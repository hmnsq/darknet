#ifndef MATRIX_H
#define MATRIX_H
#include "darknet.h"

matrix copy_matrix(matrix m);
void print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, จำนวนเต็ม n);
matrix resize_matrix(matrix m, จำนวนเต็ม size);

float *pop_column(matrix *m, จำนวนเต็ม c);

#endif
