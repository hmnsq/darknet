#ifndef MATRIX_H
#define MATRIX_H
#ضمن "darknet.h"

matrix copy_matrix(matrix m);
فراغ بغ_matrix(matrix m);

matrix hold_out_matrix(matrix *m, صحيح n);
matrix resize_matrix(matrix m, صحيح size);

عائم *چرو_column(matrix *m, صحيح c);

#endif
