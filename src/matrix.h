#ifndef MATRIX_H
#define MATRIX_H
#അന്തര്ഭാവ "darknet.h"

matrix copy_matrix(matrix m);
ശൂന്യം print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, പൂർണ്ണസംഖ്യ n);
matrix resize_matrix(matrix m, പൂർണ്ണസംഖ്യ size);

ദശാംശം *എടുക്കുക_column(matrix *m, പൂർണ്ണസംഖ്യ c);

#endif
