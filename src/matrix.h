#ifndef MATRIX_H
#define MATRIX_H
#অন্তর্ভাব "darknet.h"

matrix copy_matrix(matrix m);
শূন্য print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, পূর্ণসংখ্যা n);
matrix resize_matrix(matrix m, পূর্ণসংখ্যা আকার_2);

দশমিক *বের_করো_column(matrix *m, পূর্ণসংখ্যা c);

#endif
