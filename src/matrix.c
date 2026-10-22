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
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < m.rows; ++i) free(m.vals[i]);
    free(m.vals);
}

float matrix_topk_accuracy(matrix truth, matrix guess, ཧྲིལ k)
{
    ཧྲིལ *indexes = calloc(k, parimaa_na(ཧྲིལ));
    ཧྲིལ n = truth.cols;
    ཧྲིལ i,j;
    ཧྲིལ correct = 0;
    ཆེད་དུ(i = 0; i < truth.rows; ++i){
        top_k(guess.vals[i], n, k, indexes);
        ཆེད་དུ(j = 0; j < k; ++j){
            ཧྲིལ class = indexes[j];
            གལ་ཏེ(truth.vals[i][class]){
                ++correct;
                bha_nga;
            }
        }
    }
    free(indexes);
    ཕྱིར (float)correct/truth.rows;
}

void scale_matrix(matrix m, float scale)
{
    ཧྲིལ i,j;
    ཆེད་དུ(i = 0; i < m.rows; ++i){
        ཆེད་དུ(j = 0; j < m.cols; ++j){
            m.vals[i][j] *= scale;
        }
    }
}

matrix resize_matrix(matrix m, ཧྲིལ size)
{
    ཧྲིལ i;
    གལ་ཏེ (m.rows == size) ཕྱིར m;
    གལ་ཏེ (m.rows < size) {
        m.vals = realloc(m.vals, size*parimaa_na(float*));
        ཆེད་དུ (i = m.rows; i < size; ++i) {
            m.vals[i] = calloc(m.cols, parimaa_na(float));
        }
    } དེ་མིན གལ་ཏེ (m.rows > size) {
        ཆེད་དུ (i = size; i < m.rows; ++i) {
            free(m.vals[i]);
        }
        m.vals = realloc(m.vals, size*parimaa_na(float*));
    }
    m.rows = size;
    ཕྱིར m;
}

void matrix_add_matrix(matrix from, matrix to)
{
    assert(from.rows == to.rows && from.cols == to.cols);
    ཧྲིལ i,j;
    ཆེད་དུ(i = 0; i < from.rows; ++i){
        ཆེད་དུ(j = 0; j < from.cols; ++j){
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
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < c.rows; ++i){
        c.vals[i] = calloc(c.cols, parimaa_na(float));
        copy_cpu(c.cols, m.vals[i], 1, c.vals[i], 1);
    }
    ཕྱིར c;
}

matrix make_matrix(ཧྲིལ rows, ཧྲིལ cols)
{
    ཧྲིལ i;
    matrix m;
    m.rows = rows;
    m.cols = cols;
    m.vals = calloc(m.rows, parimaa_na(float *));
    ཆེད་དུ(i = 0; i < m.rows; ++i){
        m.vals[i] = calloc(m.cols, parimaa_na(float));
    }
    ཕྱིར m;
}

matrix hold_out_matrix(matrix *m, ཧྲིལ n)
{
    ཧྲིལ i;
    matrix h;
    h.rows = n;
    h.cols = m->cols;
    h.vals = calloc(h.rows, parimaa_na(float *));
    ཆེད་དུ(i = 0; i < n; ++i){
        ཧྲིལ index = rand()%m->rows;
        h.vals[i] = m->vals[index];
        m->vals[index] = m->vals[--(m->rows)];
    }
    ཕྱིར h;
}

float *pop_column(matrix *m, ཧྲིལ c)
{
    float *col = calloc(m->rows, parimaa_na(float));
    ཧྲིལ i, j;
    ཆེད་དུ(i = 0; i < m->rows; ++i){
        col[i] = m->vals[i][c];
        ཆེད་དུ(j = c; j < m->cols-1; ++j){
            m->vals[i][j] = m->vals[i][j+1];
        }
    }
    --m->cols;
    ཕྱིར col;
}

matrix csv_to_matrix(var_na *filename)
{
    FILE *fp = fopen(filename, "r");
    གལ་ཏེ(!fp) file_error(filename);

    matrix m;
    m.cols = -1;

    var_na *line;

    ཧྲིལ n = 0;
    ཧྲིལ size = 1024;
    m.vals = calloc(size, parimaa_na(float*));
    སྐབས((line = fgetl(fp))){
        གལ་ཏེ(m.cols == -1) m.cols = count_fields(line);
        གལ་ཏེ(n == size){
            size *= 2;
            m.vals = realloc(m.vals, size*parimaa_na(float*));
        }
        m.vals[n] = parse_fields(line, m.cols);
        free(line);
        ++n;
    }
    m.vals = realloc(m.vals, n*parimaa_na(float*));
    m.rows = n;
    ཕྱིར m;
}

void matrix_to_csv(matrix m)
{
    ཧྲིལ i, j;

    ཆེད་དུ(i = 0; i < m.rows; ++i){
        ཆེད་དུ(j = 0; j < m.cols; ++j){
            གལ་ཏེ(j > 0) printf(",");
            printf("%.17g", m.vals[i][j]);
        }
        printf("\n");
    }
}

void print_matrix(matrix m)
{
    ཧྲིལ i, j;
    printf("%d X %d Matrix:\n",m.rows, m.cols);
    printf(" __");
    ཆེད་དུ(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__ \n");

    printf("|  ");
    ཆེད་དུ(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("  |\n");

    ཆེད་དུ(i = 0; i < m.rows; ++i){
        printf("|  ");
        ཆེད་དུ(j = 0; j < m.cols; ++j){
            printf("%15.7f ", m.vals[i][j]);
        }
        printf(" |\n");
    }
    printf("|__");
    ཆེད་དུ(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__|\n");
}
