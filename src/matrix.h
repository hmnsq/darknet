#ifndef MATRIX_H
#define MATRIX_H
#অন্তর্ভাৱ "darknet.h"

matrix copy_matrix(matrix m);
শূন্য print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, পূর্ণাঙ্ক n);
matrix resize_matrix(matrix m, পূর্ণাঙ্ক size);

ভিন্ন *বেৰ_কৰো_column(matrix *m, পূর্ণাঙ্ক c);

#endif
