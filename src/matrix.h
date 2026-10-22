#ifndef MATRIX_H
#define MATRIX_H
#અન્તર્ભાવ "darknet.h"

matrix copy_matrix(matrix m);
શૂન્ય print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, પૂર્ણાંક n);
matrix resize_matrix(matrix m, પૂર્ણાંક size);

દશાંશ *કાઢો_column(matrix *m, પૂર્ણાંક c);

#endif
