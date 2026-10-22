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
    དོན་ལུ(i = 0; i < m.rows; ++i) free(m.vals[i]);
    free(m.vals);
}

float matrix_topk_accuracy(matrix truth, matrix guess, ཧྲིལ k)
{
    ཧྲིལ *indexes = calloc(k, parimaa_na(ཧྲིལ));
    ཧྲིལ n = truth.cols;
    ཧྲིལ i,j;
    ཧྲིལ correct = 0;
    དོན་ལུ(i = 0; i < truth.rows; ++i){
        top_k(guess.vals[i], n, k, indexes);
        དོན་ལུ(j = 0; j < k; ++j){
            ཧྲིལ class = indexes[j];
            གལ་སྲིད(truth.vals[i][class]){
                ++correct;
                bha_nga;
            }
        }
    }
    free(indexes);
    ལོག (float)correct/truth.rows;
}

void scale_matrix(matrix m, float scale)
{
    ཧྲིལ i,j;
    དོན་ལུ(i = 0; i < m.rows; ++i){
        དོན་ལུ(j = 0; j < m.cols; ++j){
            m.vals[i][j] *= scale;
        }
    }
}

matrix resize_matrix(matrix m, ཧྲིལ size)
{
    ཧྲིལ i;
    གལ་སྲིད (m.rows == size) ལོག m;
    གལ་སྲིད (m.rows < size) {
        m.vals = realloc(m.vals, size*parimaa_na(float*));
        དོན་ལུ (i = m.rows; i < size; ++i) {
            m.vals[i] = calloc(m.cols, parimaa_na(float));
        }
    } དེ་མིན གལ་སྲིད (m.rows > size) {
        དོན་ལུ (i = size; i < m.rows; ++i) {
            free(m.vals[i]);
        }
        m.vals = realloc(m.vals, size*parimaa_na(float*));
    }
    m.rows = size;
    ལོག m;
}

void matrix_add_matrix(matrix from, matrix to)
{
    assert(from.rows == to.rows && from.cols == to.cols);
    ཧྲིལ i,j;
    དོན་ལུ(i = 0; i < from.rows; ++i){
        དོན་ལུ(j = 0; j < from.cols; ++j){
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
    དོན་ལུ(i = 0; i < c.rows; ++i){
        c.vals[i] = calloc(c.cols, parimaa_na(float));
        copy_cpu(c.cols, m.vals[i], 1, c.vals[i], 1);
    }
    ལོག c;
}

matrix make_matrix(ཧྲིལ rows, ཧྲིལ cols)
{
    ཧྲིལ i;
    matrix m;
    m.rows = rows;
    m.cols = cols;
    m.vals = calloc(m.rows, parimaa_na(float *));
    དོན་ལུ(i = 0; i < m.rows; ++i){
        m.vals[i] = calloc(m.cols, parimaa_na(float));
    }
    ལོག m;
}

matrix hold_out_matrix(matrix *m, ཧྲིལ n)
{
    ཧྲིལ i;
    matrix h;
    h.rows = n;
    h.cols = m->cols;
    h.vals = calloc(h.rows, parimaa_na(float *));
    དོན་ལུ(i = 0; i < n; ++i){
        ཧྲིལ index = rand()%m->rows;
        h.vals[i] = m->vals[index];
        m->vals[index] = m->vals[--(m->rows)];
    }
    ལོག h;
}

float *pop_column(matrix *m, ཧྲིལ c)
{
    float *col = calloc(m->rows, parimaa_na(float));
    ཧྲིལ i, j;
    དོན་ལུ(i = 0; i < m->rows; ++i){
        col[i] = m->vals[i][c];
        དོན་ལུ(j = c; j < m->cols-1; ++j){
            m->vals[i][j] = m->vals[i][j+1];
        }
    }
    --m->cols;
    ལོག col;
}

matrix csv_to_matrix(var_na *filename)
{
    FILE *fp = fopen(filename, "r");
    གལ་སྲིད(!fp) file_error(filename);

    matrix m;
    m.cols = -1;

    var_na *line;

    ཧྲིལ n = 0;
    ཧྲིལ size = 1024;
    m.vals = calloc(size, parimaa_na(float*));
    སྐབས((line = fgetl(fp))){
        གལ་སྲིད(m.cols == -1) m.cols = count_fields(line);
        གལ་སྲིད(n == size){
            size *= 2;
            m.vals = realloc(m.vals, size*parimaa_na(float*));
        }
        m.vals[n] = parse_fields(line, m.cols);
        free(line);
        ++n;
    }
    m.vals = realloc(m.vals, n*parimaa_na(float*));
    m.rows = n;
    ལོག m;
}

void matrix_to_csv(matrix m)
{
    ཧྲིལ i, j;

    དོན་ལུ(i = 0; i < m.rows; ++i){
        དོན་ལུ(j = 0; j < m.cols; ++j){
            གལ་སྲིད(j > 0) printf(",");
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
    དོན་ལུ(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__ \n");

    printf("|  ");
    དོན་ལུ(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("  |\n");

    དོན་ལུ(i = 0; i < m.rows; ++i){
        printf("|  ");
        དོན་ལུ(j = 0; j < m.cols; ++j){
            printf("%15.7f ", m.vals[i][j]);
        }
        printf(" |\n");
    }
    printf("|__");
    དོན་ལུ(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__|\n");
}
