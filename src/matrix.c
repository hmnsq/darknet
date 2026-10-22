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
    цяло i;
    за(i = 0; i < m.rows; ++i) free(m.vals[i]);
    free(m.vals);
}

float matrix_topk_accuracy(matrix truth, matrix guess, цяло k)
{
    цяло *indexes = calloc(k, sizeof(цяло));
    цяло n = truth.cols;
    цяло i,j;
    цяло correct = 0;
    за(i = 0; i < truth.rows; ++i){
        top_k(guess.vals[i], n, k, indexes);
        за(j = 0; j < k; ++j){
            цяло class = indexes[j];
            ако(truth.vals[i][class]){
                ++correct;
                break;
            }
        }
    }
    free(indexes);
    върни (float)correct/truth.rows;
}

void scale_matrix(matrix m, float scale)
{
    цяло i,j;
    за(i = 0; i < m.rows; ++i){
        за(j = 0; j < m.cols; ++j){
            m.vals[i][j] *= scale;
        }
    }
}

matrix resize_matrix(matrix m, цяло size)
{
    цяло i;
    ако (m.rows == size) върни m;
    ако (m.rows < size) {
        m.vals = realloc(m.vals, size*sizeof(float*));
        за (i = m.rows; i < size; ++i) {
            m.vals[i] = calloc(m.cols, sizeof(float));
        }
    } иначе ако (m.rows > size) {
        за (i = size; i < m.rows; ++i) {
            free(m.vals[i]);
        }
        m.vals = realloc(m.vals, size*sizeof(float*));
    }
    m.rows = size;
    върни m;
}

void matrix_add_matrix(matrix from, matrix to)
{
    assert(from.rows == to.rows && from.cols == to.cols);
    цяло i,j;
    за(i = 0; i < from.rows; ++i){
        за(j = 0; j < from.cols; ++j){
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
    цяло i;
    за(i = 0; i < c.rows; ++i){
        c.vals[i] = calloc(c.cols, sizeof(float));
        copy_cpu(c.cols, m.vals[i], 1, c.vals[i], 1);
    }
    върни c;
}

matrix make_matrix(цяло rows, цяло cols)
{
    цяло i;
    matrix m;
    m.rows = rows;
    m.cols = cols;
    m.vals = calloc(m.rows, sizeof(float *));
    за(i = 0; i < m.rows; ++i){
        m.vals[i] = calloc(m.cols, sizeof(float));
    }
    върни m;
}

matrix hold_out_matrix(matrix *m, цяло n)
{
    цяло i;
    matrix h;
    h.rows = n;
    h.cols = m->cols;
    h.vals = calloc(h.rows, sizeof(float *));
    за(i = 0; i < n; ++i){
        цяло index = rand()%m->rows;
        h.vals[i] = m->vals[index];
        m->vals[index] = m->vals[--(m->rows)];
    }
    върни h;
}

float *pop_column(matrix *m, цяло c)
{
    float *col = calloc(m->rows, sizeof(float));
    цяло i, j;
    за(i = 0; i < m->rows; ++i){
        col[i] = m->vals[i][c];
        за(j = c; j < m->cols-1; ++j){
            m->vals[i][j] = m->vals[i][j+1];
        }
    }
    --m->cols;
    върни col;
}

matrix csv_to_matrix(char *filename)
{
    FILE *fp = fopen(filename, "r");
    ако(!fp) file_error(filename);

    matrix m;
    m.cols = -1;

    char *line;

    цяло n = 0;
    цяло size = 1024;
    m.vals = calloc(size, sizeof(float*));
    докато((line = fgetl(fp))){
        ако(m.cols == -1) m.cols = count_fields(line);
        ако(n == size){
            size *= 2;
            m.vals = realloc(m.vals, size*sizeof(float*));
        }
        m.vals[n] = parse_fields(line, m.cols);
        free(line);
        ++n;
    }
    m.vals = realloc(m.vals, n*sizeof(float*));
    m.rows = n;
    върни m;
}

void matrix_to_csv(matrix m)
{
    цяло i, j;

    за(i = 0; i < m.rows; ++i){
        за(j = 0; j < m.cols; ++j){
            ако(j > 0) printf(",");
            printf("%.17g", m.vals[i][j]);
        }
        printf("\n");
    }
}

void print_matrix(matrix m)
{
    цяло i, j;
    printf("%d X %d Matrix:\n",m.rows, m.cols);
    printf(" __");
    за(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__ \n");

    printf("|  ");
    за(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("  |\n");

    за(i = 0; i < m.rows; ++i){
        printf("|  ");
        за(j = 0; j < m.cols; ++j){
            printf("%15.7f ", m.vals[i][j]);
        }
        printf(" |\n");
    }
    printf("|__");
    за(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__|\n");
}
