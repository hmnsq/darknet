#ifndef MATRIX_H
#define MATRIX_H
#ଅନ୍ତର୍ଭାଵ "darknet.h"

matrix copy_matrix(matrix m);
ଶୂନ୍ୟ print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);
matrix resize_matrix(matrix m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size);

ଦଶମିକ *କାଢ଼_column(matrix *m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c);

#endif
