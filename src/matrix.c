#شامل "matrix.h"
#شامل "utils.h"
#شامل "blas.h"
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>
#شامل <assert.h>
#شامل <math.h>

تهی free_matrix(matrix m)
{
    صحیح i;
    برای(i = 0; i < m.rows; ++i) آزاد(m.vals[i]);
    آزاد(m.vals);
}

اعشاری matrix_topk_accuracy(matrix truth, matrix guess, صحیح k)
{
    صحیح *indexes = calloc(k, اندازه(صحیح));
    صحیح n = truth.cols;
    صحیح i,j;
    صحیح correct = 0;
    برای(i = 0; i < truth.rows; ++i){
        بالاترین_k(guess.vals[i], n, k, indexes);
        برای(j = 0; j < k; ++j){
            صحیح class = indexes[j];
            اگر(truth.vals[i][class]){
                ++correct;
                بشکن;
            }
        }
    }
    آزاد(indexes);
    بازگشت (اعشاری)correct/truth.rows;
}

تهی scale_matrix(matrix m, اعشاری scale)
{
    صحیح i,j;
    برای(i = 0; i < m.rows; ++i){
        برای(j = 0; j < m.cols; ++j){
            m.vals[i][j] *= scale;
        }
    }
}

matrix resize_matrix(matrix m, صحیح size)
{
    صحیح i;
    اگر (m.rows == size) بازگشت m;
    اگر (m.rows < size) {
        m.vals = realloc(m.vals, size*اندازه(اعشاری*));
        برای (i = m.rows; i < size; ++i) {
            m.vals[i] = calloc(m.cols, اندازه(اعشاری));
        }
    } وگرنه اگر (m.rows > size) {
        برای (i = size; i < m.rows; ++i) {
            آزاد(m.vals[i]);
        }
        m.vals = realloc(m.vals, size*اندازه(اعشاری*));
    }
    m.rows = size;
    بازگشت m;
}

تهی matrix_add_matrix(matrix from, matrix to)
{
    أكد(from.rows == to.rows && from.cols == to.cols);
    صحیح i,j;
    برای(i = 0; i < from.rows; ++i){
        برای(j = 0; j < from.cols; ++j){
            to.vals[i][j] += from.vals[i][j];
        }
    }
}

matrix copy_matrix(matrix m)
{
    matrix c = {0};
    c.rows = m.rows;
    c.cols = m.cols;
    c.vals = calloc(c.rows, اندازه(اعشاری *));
    صحیح i;
    برای(i = 0; i < c.rows; ++i){
        c.vals[i] = calloc(c.cols, اندازه(اعشاری));
        copy_cpu(c.cols, m.vals[i], 1, c.vals[i], 1);
    }
    بازگشت c;
}

matrix make_matrix(صحیح rows, صحیح cols)
{
    صحیح i;
    matrix m;
    m.rows = rows;
    m.cols = cols;
    m.vals = calloc(m.rows, اندازه(اعشاری *));
    برای(i = 0; i < m.rows; ++i){
        m.vals[i] = calloc(m.cols, اندازه(اعشاری));
    }
    بازگشت m;
}

matrix hold_out_matrix(matrix *m, صحیح n)
{
    صحیح i;
    matrix h;
    h.rows = n;
    h.cols = m->cols;
    h.vals = calloc(h.rows, اندازه(اعشاری *));
    برای(i = 0; i < n; ++i){
        صحیح index = rand()%m->rows;
        h.vals[i] = m->vals[index];
        m->vals[index] = m->vals[--(m->rows)];
    }
    بازگشت h;
}

اعشاری *برداشت_column(matrix *m, صحیح c)
{
    اعشاری *col = calloc(m->rows, اندازه(اعشاری));
    صحیح i, j;
    برای(i = 0; i < m->rows; ++i){
        col[i] = m->vals[i][c];
        برای(j = c; j < m->cols-1; ++j){
            m->vals[i][j] = m->vals[i][j+1];
        }
    }
    --m->cols;
    بازگشت col;
}

matrix csv_to_matrix(نویسه *filename)
{
    FILE *fp = fopen(filename, "r");
    اگر(!fp) file_error(filename);

    matrix m;
    m.cols = -1;

    نویسه *سطر;

    صحیح n = 0;
    صحیح size = 1024;
    m.vals = calloc(size, اندازه(اعشاری*));
    وقتی((سطر = fgetl(fp))){
        اگر(m.cols == -1) m.cols = شمار_fields(سطر);
        اگر(n == size){
            size *= 2;
            m.vals = realloc(m.vals, size*اندازه(اعشاری*));
        }
        m.vals[n] = parse_fields(سطر, m.cols);
        آزاد(سطر);
        ++n;
    }
    m.vals = realloc(m.vals, n*اندازه(اعشاری*));
    m.rows = n;
    بازگشت m;
}

تهی matrix_to_csv(matrix m)
{
    صحیح i, j;

    برای(i = 0; i < m.rows; ++i){
        برای(j = 0; j < m.cols; ++j){
            اگر(j > 0) printf(",");
            printf("%.17g", m.vals[i][j]);
        }
        printf("\n");
    }
}

تهی print_matrix(matrix m)
{
    صحیح i, j;
    printf("%d X %d Matrix:\n",m.rows, m.cols);
    printf(" __");
    برای(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__ \n");

    printf("|  ");
    برای(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("  |\n");

    برای(i = 0; i < m.rows; ++i){
        printf("|  ");
        برای(j = 0; j < m.cols; ++j){
            printf("%15.7f ", m.vals[i][j]);
        }
        printf(" |\n");
    }
    printf("|__");
    برای(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__|\n");
}
