#ifndef MATRIX_H
#define MATRIX_H
#ಅನ್ತರ್ಭಾವ "darknet.h"

matrix copy_matrix(matrix m);
ಶೂನ್ಯ print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, ಪೂರ್ಣಾಙ್ಕ n);
matrix resize_matrix(matrix m, ಪೂರ್ಣಾಙ್ಕ size);

ಭಿನ್ನ *ತೆಗೆ_column(matrix *m, ಪೂರ್ಣಾಙ್ಕ c);

#endif
