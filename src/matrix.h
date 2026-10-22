#ifndef MATRIX_H
#define MATRIX_H
#अन्तर्भाव "darknet.h"

matrix copy_matrix(matrix m);
शून्य print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, पूर्णांक n);
matrix resize_matrix(matrix m, पूर्णांक size);

दशांश *काढा_column(matrix *m, पूर्णांक c);

#endif
