#ifndef MATRIX_H
#define MATRIX_H
#अन्तर्भाव "darknet.h"

matrix copy_matrix(matrix m);
शून्य print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, पूर्णाङ्क n);
matrix resize_matrix(matrix m, पूर्णाङ्क size);

भिन्न *काढा_column(matrix *m, पूर्णाङ्क c);

#endif
