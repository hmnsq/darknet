#ifndef MATRIX_H
#define MATRIX_H
#ضمن "darknet.h"

matrix copy_matrix(matrix m);
فراغ print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, صحيح n);
matrix resize_matrix(matrix m, صحيح size);

عائم *نکالو_column(matrix *m, صحيح c);

#endif
