#include "matrix.h"
#include "utils.h"
#include "blas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

void free_matrix(matrix m)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < m.rows; ++i) free(m.vals[i]);
    free(m.vals);
}

float matrix_topk_accuracy(matrix truth, matrix guess, පූර්ණ_සංඛ්‍යා k)
{
    පූර්ණ_සංඛ්‍යා *indexes = calloc(k, parimaa_na(පූර්ණ_සංඛ්‍යා));
    පූර්ණ_සංඛ්‍යා n = truth.cols;
    පූර්ණ_සංඛ්‍යා i,j;
    පූර්ණ_සංඛ්‍යා correct = 0;
    සඳහා(i = 0; i < truth.rows; ++i){
        top_k(guess.vals[i], n, k, indexes);
        සඳහා(j = 0; j < k; ++j){
            පූර්ණ_සංඛ්‍යා class = indexes[j];
            නම්(truth.vals[i][class]){
                ++correct;
                bha_nga;
            }
        }
    }
    free(indexes);
    ආපසු (float)correct/truth.rows;
}

void scale_matrix(matrix m, float scale)
{
    පූර්ණ_සංඛ්‍යා i,j;
    සඳහා(i = 0; i < m.rows; ++i){
        සඳහා(j = 0; j < m.cols; ++j){
            m.vals[i][j] *= scale;
        }
    }
}

matrix resize_matrix(matrix m, පූර්ණ_සංඛ්‍යා size)
{
    පූර්ණ_සංඛ්‍යා i;
    නම් (m.rows == size) ආපසු m;
    නම් (m.rows < size) {
        m.vals = realloc(m.vals, size*parimaa_na(float*));
        සඳහා (i = m.rows; i < size; ++i) {
            m.vals[i] = calloc(m.cols, parimaa_na(float));
        }
    } නැතහොත් නම් (m.rows > size) {
        සඳහා (i = size; i < m.rows; ++i) {
            free(m.vals[i]);
        }
        m.vals = realloc(m.vals, size*parimaa_na(float*));
    }
    m.rows = size;
    ආපසු m;
}

void matrix_add_matrix(matrix from, matrix to)
{
    assert(from.rows == to.rows && from.cols == to.cols);
    පූර්ණ_සංඛ්‍යා i,j;
    සඳහා(i = 0; i < from.rows; ++i){
        සඳහා(j = 0; j < from.cols; ++j){
            to.vals[i][j] += from.vals[i][j];
        }
    }
}

matrix copy_matrix(matrix m)
{
    matrix c = {0};
    c.rows = m.rows;
    c.cols = m.cols;
    c.vals = calloc(c.rows, parimaa_na(float *));
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < c.rows; ++i){
        c.vals[i] = calloc(c.cols, parimaa_na(float));
        copy_cpu(c.cols, m.vals[i], 1, c.vals[i], 1);
    }
    ආපසු c;
}

matrix make_matrix(පූර්ණ_සංඛ්‍යා rows, පූර්ණ_සංඛ්‍යා cols)
{
    පූර්ණ_සංඛ්‍යා i;
    matrix m;
    m.rows = rows;
    m.cols = cols;
    m.vals = calloc(m.rows, parimaa_na(float *));
    සඳහා(i = 0; i < m.rows; ++i){
        m.vals[i] = calloc(m.cols, parimaa_na(float));
    }
    ආපසු m;
}

matrix hold_out_matrix(matrix *m, පූර්ණ_සංඛ්‍යා n)
{
    පූර්ණ_සංඛ්‍යා i;
    matrix h;
    h.rows = n;
    h.cols = m->cols;
    h.vals = calloc(h.rows, parimaa_na(float *));
    සඳහා(i = 0; i < n; ++i){
        පූර්ණ_සංඛ්‍යා index = rand()%m->rows;
        h.vals[i] = m->vals[index];
        m->vals[index] = m->vals[--(m->rows)];
    }
    ආපසු h;
}

float *pop_column(matrix *m, පූර්ණ_සංඛ්‍යා c)
{
    float *col = calloc(m->rows, parimaa_na(float));
    පූර්ණ_සංඛ්‍යා i, j;
    සඳහා(i = 0; i < m->rows; ++i){
        col[i] = m->vals[i][c];
        සඳහා(j = c; j < m->cols-1; ++j){
            m->vals[i][j] = m->vals[i][j+1];
        }
    }
    --m->cols;
    ආපසු col;
}

matrix csv_to_matrix(var_na *filename)
{
    FILE *fp = fopen(filename, "r");
    නම්(!fp) file_error(filename);

    matrix m;
    m.cols = -1;

    var_na *line;

    පූර්ණ_සංඛ්‍යා n = 0;
    පූර්ණ_සංඛ්‍යා size = 1024;
    m.vals = calloc(size, parimaa_na(float*));
    අතරතුර((line = fgetl(fp))){
        නම්(m.cols == -1) m.cols = count_fields(line);
        නම්(n == size){
            size *= 2;
            m.vals = realloc(m.vals, size*parimaa_na(float*));
        }
        m.vals[n] = parse_fields(line, m.cols);
        free(line);
        ++n;
    }
    m.vals = realloc(m.vals, n*parimaa_na(float*));
    m.rows = n;
    ආපසු m;
}

void matrix_to_csv(matrix m)
{
    පූර්ණ_සංඛ්‍යා i, j;

    සඳහා(i = 0; i < m.rows; ++i){
        සඳහා(j = 0; j < m.cols; ++j){
            නම්(j > 0) printf(",");
            printf("%.17g", m.vals[i][j]);
        }
        printf("\n");
    }
}

void print_matrix(matrix m)
{
    පූර්ණ_සංඛ්‍යා i, j;
    printf("%d X %d Matrix:\n",m.rows, m.cols);
    printf(" __");
    සඳහා(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__ \n");

    printf("|  ");
    සඳහා(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("  |\n");

    සඳහා(i = 0; i < m.rows; ++i){
        printf("|  ");
        සඳහා(j = 0; j < m.cols; ++j){
            printf("%15.7f ", m.vals[i][j]);
        }
        printf(" |\n");
    }
    printf("|__");
    සඳහා(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__|\n");
}
