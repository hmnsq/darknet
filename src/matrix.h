#ifndef MATRIX_H
#define MATRIX_H
#include "darknet.h"

matrix copy_matrix(matrix m);
void print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, puur_naa_nka n);
matrix resize_matrix(matrix m, puur_naa_nka size);

float *pop_column(matrix *m, puur_naa_nka c);

#endif
