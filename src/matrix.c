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
    số_nguyên i;
    cho(i = 0; i < m.rows; ++i) free(m.vals[i]);
    free(m.vals);
}

float matrix_topk_accuracy(matrix truth, matrix guess, số_nguyên k)
{
    số_nguyên *indexes = calloc(k, sizeof(số_nguyên));
    số_nguyên n = truth.cols;
    số_nguyên i,j;
    số_nguyên correct = 0;
    cho(i = 0; i < truth.rows; ++i){
        top_k(guess.vals[i], n, k, indexes);
        cho(j = 0; j < k; ++j){
            số_nguyên class = indexes[j];
            nếu(truth.vals[i][class]){
                ++correct;
                break;
            }
        }
    }
    free(indexes);
    trả_về (float)correct/truth.rows;
}

void scale_matrix(matrix m, float scale)
{
    số_nguyên i,j;
    cho(i = 0; i < m.rows; ++i){
        cho(j = 0; j < m.cols; ++j){
            m.vals[i][j] *= scale;
        }
    }
}

matrix resize_matrix(matrix m, số_nguyên size)
{
    số_nguyên i;
    nếu (m.rows == size) trả_về m;
    nếu (m.rows < size) {
        m.vals = realloc(m.vals, size*sizeof(float*));
        cho (i = m.rows; i < size; ++i) {
            m.vals[i] = calloc(m.cols, sizeof(float));
        }
    } không_thì nếu (m.rows > size) {
        cho (i = size; i < m.rows; ++i) {
            free(m.vals[i]);
        }
        m.vals = realloc(m.vals, size*sizeof(float*));
    }
    m.rows = size;
    trả_về m;
}

void matrix_add_matrix(matrix from, matrix to)
{
    assert(from.rows == to.rows && from.cols == to.cols);
    số_nguyên i,j;
    cho(i = 0; i < from.rows; ++i){
        cho(j = 0; j < from.cols; ++j){
            to.vals[i][j] += from.vals[i][j];
        }
    }
}

matrix copy_matrix(matrix m)
{
    matrix c = {0};
    c.rows = m.rows;
    c.cols = m.cols;
    c.vals = calloc(c.rows, sizeof(float *));
    số_nguyên i;
    cho(i = 0; i < c.rows; ++i){
        c.vals[i] = calloc(c.cols, sizeof(float));
        copy_cpu(c.cols, m.vals[i], 1, c.vals[i], 1);
    }
    trả_về c;
}

matrix make_matrix(số_nguyên rows, số_nguyên cols)
{
    số_nguyên i;
    matrix m;
    m.rows = rows;
    m.cols = cols;
    m.vals = calloc(m.rows, sizeof(float *));
    cho(i = 0; i < m.rows; ++i){
        m.vals[i] = calloc(m.cols, sizeof(float));
    }
    trả_về m;
}

matrix hold_out_matrix(matrix *m, số_nguyên n)
{
    số_nguyên i;
    matrix h;
    h.rows = n;
    h.cols = m->cols;
    h.vals = calloc(h.rows, sizeof(float *));
    cho(i = 0; i < n; ++i){
        số_nguyên index = rand()%m->rows;
        h.vals[i] = m->vals[index];
        m->vals[index] = m->vals[--(m->rows)];
    }
    trả_về h;
}

float *pop_column(matrix *m, số_nguyên c)
{
    float *col = calloc(m->rows, sizeof(float));
    số_nguyên i, j;
    cho(i = 0; i < m->rows; ++i){
        col[i] = m->vals[i][c];
        cho(j = c; j < m->cols-1; ++j){
            m->vals[i][j] = m->vals[i][j+1];
        }
    }
    --m->cols;
    trả_về col;
}

matrix csv_to_matrix(char *filename)
{
    FILE *fp = fopen(filename, "r");
    nếu(!fp) file_error(filename);

    matrix m;
    m.cols = -1;

    char *line;

    số_nguyên n = 0;
    số_nguyên size = 1024;
    m.vals = calloc(size, sizeof(float*));
    trong_khi((line = fgetl(fp))){
        nếu(m.cols == -1) m.cols = count_fields(line);
        nếu(n == size){
            size *= 2;
            m.vals = realloc(m.vals, size*sizeof(float*));
        }
        m.vals[n] = parse_fields(line, m.cols);
        free(line);
        ++n;
    }
    m.vals = realloc(m.vals, n*sizeof(float*));
    m.rows = n;
    trả_về m;
}

void matrix_to_csv(matrix m)
{
    số_nguyên i, j;

    cho(i = 0; i < m.rows; ++i){
        cho(j = 0; j < m.cols; ++j){
            nếu(j > 0) printf(",");
            printf("%.17g", m.vals[i][j]);
        }
        printf("\n");
    }
}

void print_matrix(matrix m)
{
    số_nguyên i, j;
    printf("%d X %d Matrix:\n",m.rows, m.cols);
    printf(" __");
    cho(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__ \n");

    printf("|  ");
    cho(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("  |\n");

    cho(i = 0; i < m.rows; ++i){
        printf("|  ");
        cho(j = 0; j < m.cols; ++j){
            printf("%15.7f ", m.vals[i][j]);
        }
        printf(" |\n");
    }
    printf("|__");
    cho(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__|\n");
}
