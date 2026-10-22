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
    ціле i;
    для(i = 0; i < m.rows; ++i) free(m.vals[i]);
    free(m.vals);
}

float matrix_topk_accuracy(matrix truth, matrix guess, ціле k)
{
    ціле *indexes = calloc(k, sizeof(ціле));
    ціле n = truth.cols;
    ціле i,j;
    ціле correct = 0;
    для(i = 0; i < truth.rows; ++i){
        top_k(guess.vals[i], n, k, indexes);
        для(j = 0; j < k; ++j){
            ціле class = indexes[j];
            якщо(truth.vals[i][class]){
                ++correct;
                break;
            }
        }
    }
    free(indexes);
    повернути (float)correct/truth.rows;
}

void scale_matrix(matrix m, float scale)
{
    ціле i,j;
    для(i = 0; i < m.rows; ++i){
        для(j = 0; j < m.cols; ++j){
            m.vals[i][j] *= scale;
        }
    }
}

matrix resize_matrix(matrix m, ціле size)
{
    ціле i;
    якщо (m.rows == size) повернути m;
    якщо (m.rows < size) {
        m.vals = realloc(m.vals, size*sizeof(float*));
        для (i = m.rows; i < size; ++i) {
            m.vals[i] = calloc(m.cols, sizeof(float));
        }
    } інакше якщо (m.rows > size) {
        для (i = size; i < m.rows; ++i) {
            free(m.vals[i]);
        }
        m.vals = realloc(m.vals, size*sizeof(float*));
    }
    m.rows = size;
    повернути m;
}

void matrix_add_matrix(matrix from, matrix to)
{
    assert(from.rows == to.rows && from.cols == to.cols);
    ціле i,j;
    для(i = 0; i < from.rows; ++i){
        для(j = 0; j < from.cols; ++j){
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
    ціле i;
    для(i = 0; i < c.rows; ++i){
        c.vals[i] = calloc(c.cols, sizeof(float));
        copy_cpu(c.cols, m.vals[i], 1, c.vals[i], 1);
    }
    повернути c;
}

matrix make_matrix(ціле rows, ціле cols)
{
    ціле i;
    matrix m;
    m.rows = rows;
    m.cols = cols;
    m.vals = calloc(m.rows, sizeof(float *));
    для(i = 0; i < m.rows; ++i){
        m.vals[i] = calloc(m.cols, sizeof(float));
    }
    повернути m;
}

matrix hold_out_matrix(matrix *m, ціле n)
{
    ціле i;
    matrix h;
    h.rows = n;
    h.cols = m->cols;
    h.vals = calloc(h.rows, sizeof(float *));
    для(i = 0; i < n; ++i){
        ціле index = rand()%m->rows;
        h.vals[i] = m->vals[index];
        m->vals[index] = m->vals[--(m->rows)];
    }
    повернути h;
}

float *pop_column(matrix *m, ціле c)
{
    float *col = calloc(m->rows, sizeof(float));
    ціле i, j;
    для(i = 0; i < m->rows; ++i){
        col[i] = m->vals[i][c];
        для(j = c; j < m->cols-1; ++j){
            m->vals[i][j] = m->vals[i][j+1];
        }
    }
    --m->cols;
    повернути col;
}

matrix csv_to_matrix(char *filename)
{
    FILE *fp = fopen(filename, "r");
    якщо(!fp) file_error(filename);

    matrix m;
    m.cols = -1;

    char *line;

    ціле n = 0;
    ціле size = 1024;
    m.vals = calloc(size, sizeof(float*));
    поки((line = fgetl(fp))){
        якщо(m.cols == -1) m.cols = count_fields(line);
        якщо(n == size){
            size *= 2;
            m.vals = realloc(m.vals, size*sizeof(float*));
        }
        m.vals[n] = parse_fields(line, m.cols);
        free(line);
        ++n;
    }
    m.vals = realloc(m.vals, n*sizeof(float*));
    m.rows = n;
    повернути m;
}

void matrix_to_csv(matrix m)
{
    ціле i, j;

    для(i = 0; i < m.rows; ++i){
        для(j = 0; j < m.cols; ++j){
            якщо(j > 0) printf(",");
            printf("%.17g", m.vals[i][j]);
        }
        printf("\n");
    }
}

void print_matrix(matrix m)
{
    ціле i, j;
    printf("%d X %d Matrix:\n",m.rows, m.cols);
    printf(" __");
    для(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__ \n");

    printf("|  ");
    для(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("  |\n");

    для(i = 0; i < m.rows; ++i){
        printf("|  ");
        для(j = 0; j < m.cols; ++j){
            printf("%15.7f ", m.vals[i][j]);
        }
        printf(" |\n");
    }
    printf("|__");
    для(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__|\n");
}
