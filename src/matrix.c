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
    정수 i;
    위해(i = 0; i < m.rows; ++i) free(m.vals[i]);
    free(m.vals);
}

float matrix_topk_accuracy(matrix truth, matrix guess, 정수 k)
{
    정수 *indexes = calloc(k, sizeof(정수));
    정수 n = truth.cols;
    정수 i,j;
    정수 correct = 0;
    위해(i = 0; i < truth.rows; ++i){
        top_k(guess.vals[i], n, k, indexes);
        위해(j = 0; j < k; ++j){
            정수 class = indexes[j];
            만약(truth.vals[i][class]){
                ++correct;
                break;
            }
        }
    }
    free(indexes);
    반환 (float)correct/truth.rows;
}

void scale_matrix(matrix m, float scale)
{
    정수 i,j;
    위해(i = 0; i < m.rows; ++i){
        위해(j = 0; j < m.cols; ++j){
            m.vals[i][j] *= scale;
        }
    }
}

matrix resize_matrix(matrix m, 정수 size)
{
    정수 i;
    만약 (m.rows == size) 반환 m;
    만약 (m.rows < size) {
        m.vals = realloc(m.vals, size*sizeof(float*));
        위해 (i = m.rows; i < size; ++i) {
            m.vals[i] = calloc(m.cols, sizeof(float));
        }
    } 아니면 만약 (m.rows > size) {
        위해 (i = size; i < m.rows; ++i) {
            free(m.vals[i]);
        }
        m.vals = realloc(m.vals, size*sizeof(float*));
    }
    m.rows = size;
    반환 m;
}

void matrix_add_matrix(matrix from, matrix to)
{
    assert(from.rows == to.rows && from.cols == to.cols);
    정수 i,j;
    위해(i = 0; i < from.rows; ++i){
        위해(j = 0; j < from.cols; ++j){
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
    정수 i;
    위해(i = 0; i < c.rows; ++i){
        c.vals[i] = calloc(c.cols, sizeof(float));
        copy_cpu(c.cols, m.vals[i], 1, c.vals[i], 1);
    }
    반환 c;
}

matrix make_matrix(정수 rows, 정수 cols)
{
    정수 i;
    matrix m;
    m.rows = rows;
    m.cols = cols;
    m.vals = calloc(m.rows, sizeof(float *));
    위해(i = 0; i < m.rows; ++i){
        m.vals[i] = calloc(m.cols, sizeof(float));
    }
    반환 m;
}

matrix hold_out_matrix(matrix *m, 정수 n)
{
    정수 i;
    matrix h;
    h.rows = n;
    h.cols = m->cols;
    h.vals = calloc(h.rows, sizeof(float *));
    위해(i = 0; i < n; ++i){
        정수 index = rand()%m->rows;
        h.vals[i] = m->vals[index];
        m->vals[index] = m->vals[--(m->rows)];
    }
    반환 h;
}

float *pop_column(matrix *m, 정수 c)
{
    float *col = calloc(m->rows, sizeof(float));
    정수 i, j;
    위해(i = 0; i < m->rows; ++i){
        col[i] = m->vals[i][c];
        위해(j = c; j < m->cols-1; ++j){
            m->vals[i][j] = m->vals[i][j+1];
        }
    }
    --m->cols;
    반환 col;
}

matrix csv_to_matrix(char *filename)
{
    FILE *fp = fopen(filename, "r");
    만약(!fp) file_error(filename);

    matrix m;
    m.cols = -1;

    char *line;

    정수 n = 0;
    정수 size = 1024;
    m.vals = calloc(size, sizeof(float*));
    동안((line = fgetl(fp))){
        만약(m.cols == -1) m.cols = count_fields(line);
        만약(n == size){
            size *= 2;
            m.vals = realloc(m.vals, size*sizeof(float*));
        }
        m.vals[n] = parse_fields(line, m.cols);
        free(line);
        ++n;
    }
    m.vals = realloc(m.vals, n*sizeof(float*));
    m.rows = n;
    반환 m;
}

void matrix_to_csv(matrix m)
{
    정수 i, j;

    위해(i = 0; i < m.rows; ++i){
        위해(j = 0; j < m.cols; ++j){
            만약(j > 0) printf(",");
            printf("%.17g", m.vals[i][j]);
        }
        printf("\n");
    }
}

void print_matrix(matrix m)
{
    정수 i, j;
    printf("%d X %d Matrix:\n",m.rows, m.cols);
    printf(" __");
    위해(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__ \n");

    printf("|  ");
    위해(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("  |\n");

    위해(i = 0; i < m.rows; ++i){
        printf("|  ");
        위해(j = 0; j < m.cols; ++j){
            printf("%15.7f ", m.vals[i][j]);
        }
        printf(" |\n");
    }
    printf("|__");
    위해(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__|\n");
}
