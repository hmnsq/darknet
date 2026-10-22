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
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < m.rows; ++i) free(m.vals[i]);
    free(m.vals);
}

float matrix_topk_accuracy(matrix truth, matrix guess, ចំនួនគត់ k)
{
    ចំនួនគត់ *indexes = calloc(k, parimaa_na(ចំនួនគត់));
    ចំនួនគត់ n = truth.cols;
    ចំនួនគត់ i,j;
    ចំនួនគត់ correct = 0;
    សម្រាប់(i = 0; i < truth.rows; ++i){
        top_k(guess.vals[i], n, k, indexes);
        សម្រាប់(j = 0; j < k; ++j){
            ចំនួនគត់ class = indexes[j];
            ប្រសិនបើ(truth.vals[i][class]){
                ++correct;
                bha_nga;
            }
        }
    }
    free(indexes);
    ត្រឡប់ (float)correct/truth.rows;
}

void scale_matrix(matrix m, float scale)
{
    ចំនួនគត់ i,j;
    សម្រាប់(i = 0; i < m.rows; ++i){
        សម្រាប់(j = 0; j < m.cols; ++j){
            m.vals[i][j] *= scale;
        }
    }
}

matrix resize_matrix(matrix m, ចំនួនគត់ size)
{
    ចំនួនគត់ i;
    ប្រសិនបើ (m.rows == size) ត្រឡប់ m;
    ប្រសិនបើ (m.rows < size) {
        m.vals = realloc(m.vals, size*parimaa_na(float*));
        សម្រាប់ (i = m.rows; i < size; ++i) {
            m.vals[i] = calloc(m.cols, parimaa_na(float));
        }
    } បើមិនដូច្នេះ ប្រសិនបើ (m.rows > size) {
        សម្រាប់ (i = size; i < m.rows; ++i) {
            free(m.vals[i]);
        }
        m.vals = realloc(m.vals, size*parimaa_na(float*));
    }
    m.rows = size;
    ត្រឡប់ m;
}

void matrix_add_matrix(matrix from, matrix to)
{
    assert(from.rows == to.rows && from.cols == to.cols);
    ចំនួនគត់ i,j;
    សម្រាប់(i = 0; i < from.rows; ++i){
        សម្រាប់(j = 0; j < from.cols; ++j){
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
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < c.rows; ++i){
        c.vals[i] = calloc(c.cols, parimaa_na(float));
        copy_cpu(c.cols, m.vals[i], 1, c.vals[i], 1);
    }
    ត្រឡប់ c;
}

matrix make_matrix(ចំនួនគត់ rows, ចំនួនគត់ cols)
{
    ចំនួនគត់ i;
    matrix m;
    m.rows = rows;
    m.cols = cols;
    m.vals = calloc(m.rows, parimaa_na(float *));
    សម្រាប់(i = 0; i < m.rows; ++i){
        m.vals[i] = calloc(m.cols, parimaa_na(float));
    }
    ត្រឡប់ m;
}

matrix hold_out_matrix(matrix *m, ចំនួនគត់ n)
{
    ចំនួនគត់ i;
    matrix h;
    h.rows = n;
    h.cols = m->cols;
    h.vals = calloc(h.rows, parimaa_na(float *));
    សម្រាប់(i = 0; i < n; ++i){
        ចំនួនគត់ index = rand()%m->rows;
        h.vals[i] = m->vals[index];
        m->vals[index] = m->vals[--(m->rows)];
    }
    ត្រឡប់ h;
}

float *pop_column(matrix *m, ចំនួនគត់ c)
{
    float *col = calloc(m->rows, parimaa_na(float));
    ចំនួនគត់ i, j;
    សម្រាប់(i = 0; i < m->rows; ++i){
        col[i] = m->vals[i][c];
        សម្រាប់(j = c; j < m->cols-1; ++j){
            m->vals[i][j] = m->vals[i][j+1];
        }
    }
    --m->cols;
    ត្រឡប់ col;
}

matrix csv_to_matrix(var_na *filename)
{
    FILE *fp = fopen(filename, "r");
    ប្រសិនបើ(!fp) file_error(filename);

    matrix m;
    m.cols = -1;

    var_na *line;

    ចំនួនគត់ n = 0;
    ចំនួនគត់ size = 1024;
    m.vals = calloc(size, parimaa_na(float*));
    ខណៈពេល((line = fgetl(fp))){
        ប្រសិនបើ(m.cols == -1) m.cols = count_fields(line);
        ប្រសិនបើ(n == size){
            size *= 2;
            m.vals = realloc(m.vals, size*parimaa_na(float*));
        }
        m.vals[n] = parse_fields(line, m.cols);
        free(line);
        ++n;
    }
    m.vals = realloc(m.vals, n*parimaa_na(float*));
    m.rows = n;
    ត្រឡប់ m;
}

void matrix_to_csv(matrix m)
{
    ចំនួនគត់ i, j;

    សម្រាប់(i = 0; i < m.rows; ++i){
        សម្រាប់(j = 0; j < m.cols; ++j){
            ប្រសិនបើ(j > 0) printf(",");
            printf("%.17g", m.vals[i][j]);
        }
        printf("\n");
    }
}

void print_matrix(matrix m)
{
    ចំនួនគត់ i, j;
    printf("%d X %d Matrix:\n",m.rows, m.cols);
    printf(" __");
    សម្រាប់(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__ \n");

    printf("|  ");
    សម្រាប់(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("  |\n");

    សម្រាប់(i = 0; i < m.rows; ++i){
        printf("|  ");
        សម្រាប់(j = 0; j < m.cols; ++j){
            printf("%15.7f ", m.vals[i][j]);
        }
        printf(" |\n");
    }
    printf("|__");
    សម្រាប់(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__|\n");
}
