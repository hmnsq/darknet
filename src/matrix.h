#ifndef MATRIX_H
#define MATRIX_H
#شامل "darknet.h"

matrix copy_matrix(matrix m);
تهی print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, صحیح n);
matrix resize_matrix(matrix m, صحیح size);

اعشاری *برداشت_column(matrix *m, صحیح c);

#endif
