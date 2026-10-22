#ifndef MATRIX_H
#define MATRIX_H
#include "darknet.h"

matrix copy_matrix(matrix m);
void print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, පූර්ණ_සංඛ්‍යා n);
matrix resize_matrix(matrix m, පූර්ණ_සංඛ්‍යා size);

float *pop_column(matrix *m, පූර්ණ_සංඛ්‍යා c);

#endif
