#ଅନ୍ତର୍ଭାଵ "matrix.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <math.h>

ଶୂନ୍ୟ free_matrix(matrix m)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < m.rows; ++i) ମୁକ୍ତ(m.vals[i]);
    ମୁକ୍ତ(m.vals);
}

ଦଶମିକ matrix_topk_accuracy(matrix truth, matrix guess, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *indexes = calloc(k, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = truth.cols;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା correct = 0;
    ପାଇଁ(i = 0; i < truth.rows; ++i){
        ଶୀର୍ଷ_k(guess.vals[i], n, k, indexes);
        ପାଇଁ(j = 0; j < k; ++j){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା class = indexes[j];
            ଯଦି(truth.vals[i][class]){
                ++correct;
                ଭାଙ୍ଗ;
            }
        }
    }
    ମୁକ୍ତ(indexes);
    ଫେରନ୍ତୁ (ଦଶମିକ)correct/truth.rows;
}

ଶୂନ୍ୟ scale_matrix(matrix m, ଦଶମିକ scale)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    ପାଇଁ(i = 0; i < m.rows; ++i){
        ପାଇଁ(j = 0; j < m.cols; ++j){
            m.vals[i][j] *= scale;
        }
    }
}

matrix resize_matrix(matrix m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଯଦି (m.rows == size) ଫେରନ୍ତୁ m;
    ଯଦି (m.rows < size) {
        m.vals = realloc(m.vals, size*ଆକାର(ଦଶମିକ*));
        ପାଇଁ (i = m.rows; i < size; ++i) {
            m.vals[i] = calloc(m.cols, ଆକାର(ଦଶମିକ));
        }
    } ଅନ୍ୟଥା ଯଦି (m.rows > size) {
        ପାଇଁ (i = size; i < m.rows; ++i) {
            ମୁକ୍ତ(m.vals[i]);
        }
        m.vals = realloc(m.vals, size*ଆକାର(ଦଶମିକ*));
    }
    m.rows = size;
    ଫେରନ୍ତୁ m;
}

ଶୂନ୍ୟ matrix_add_matrix(matrix from, matrix to)
{
    ପ୍ରତିଜ୍ଞା(from.rows == to.rows && from.cols == to.cols);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    ପାଇଁ(i = 0; i < from.rows; ++i){
        ପାଇଁ(j = 0; j < from.cols; ++j){
            to.vals[i][j] += from.vals[i][j];
        }
    }
}

matrix copy_matrix(matrix m)
{
    matrix c = {0};
    c.rows = m.rows;
    c.cols = m.cols;
    c.vals = calloc(c.rows, ଆକାର(ଦଶମିକ *));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < c.rows; ++i){
        c.vals[i] = calloc(c.cols, ଆକାର(ଦଶମିକ));
        copy_cpu(c.cols, m.vals[i], 1, c.vals[i], 1);
    }
    ଫେରନ୍ତୁ c;
}

matrix make_matrix(ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    matrix m;
    m.rows = rows;
    m.cols = cols;
    m.vals = calloc(m.rows, ଆକାର(ଦଶମିକ *));
    ପାଇଁ(i = 0; i < m.rows; ++i){
        m.vals[i] = calloc(m.cols, ଆକାର(ଦଶମିକ));
    }
    ଫେରନ୍ତୁ m;
}

matrix hold_out_matrix(matrix *m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    matrix h;
    h.rows = n;
    h.cols = m->cols;
    h.vals = calloc(h.rows, ଆକାର(ଦଶମିକ *));
    ପାଇଁ(i = 0; i < n; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = rand()%m->rows;
        h.vals[i] = m->vals[index];
        m->vals[index] = m->vals[--(m->rows)];
    }
    ଫେରନ୍ତୁ h;
}

ଦଶମିକ *କାଢ଼_column(matrix *m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c)
{
    ଦଶମିକ *col = calloc(m->rows, ଆକାର(ଦଶମିକ));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପାଇଁ(i = 0; i < m->rows; ++i){
        col[i] = m->vals[i][c];
        ପାଇଁ(j = c; j < m->cols-1; ++j){
            m->vals[i][j] = m->vals[i][j+1];
        }
    }
    --m->cols;
    ଫେରନ୍ତୁ col;
}

matrix csv_to_matrix(ଅକ୍ଷର *filename)
{
    FILE *fp = fopen(filename, "r");
    ଯଦି(!fp) file_error(filename);

    matrix m;
    m.cols = -1;

    ଅକ୍ଷର *ଧାଡ଼ି;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = 1024;
    m.vals = calloc(size, ଆକାର(ଦଶମିକ*));
    ଯେତେବେଳେ((ଧାଡ଼ି = fgetl(fp))){
        ଯଦି(m.cols == -1) m.cols = ଗଣନା_fields(ଧାଡ଼ି);
        ଯଦି(n == size){
            size *= 2;
            m.vals = realloc(m.vals, size*ଆକାର(ଦଶମିକ*));
        }
        m.vals[n] = parse_fields(ଧାଡ଼ି, m.cols);
        ମୁକ୍ତ(ଧାଡ଼ି);
        ++n;
    }
    m.vals = realloc(m.vals, n*ଆକାର(ଦଶମିକ*));
    m.rows = n;
    ଫେରନ୍ତୁ m;
}

ଶୂନ୍ୟ matrix_to_csv(matrix m)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;

    ପାଇଁ(i = 0; i < m.rows; ++i){
        ପାଇଁ(j = 0; j < m.cols; ++j){
            ଯଦି(j > 0) printf(",");
            printf("%.17g", m.vals[i][j]);
        }
        printf("\n");
    }
}

ଶୂନ୍ୟ print_matrix(matrix m)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    printf("%d X %d Matrix:\n",m.rows, m.cols);
    printf(" __");
    ପାଇଁ(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__ \n");

    printf("|  ");
    ପାଇଁ(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("  |\n");

    ପାଇଁ(i = 0; i < m.rows; ++i){
        printf("|  ");
        ପାଇଁ(j = 0; j < m.cols; ++j){
            printf("%15.7f ", m.vals[i][j]);
        }
        printf(" |\n");
    }
    printf("|__");
    ପାଇଁ(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__|\n");
}
