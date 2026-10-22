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
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < m.rows; ++i) free(m.vals[i]);
    free(m.vals);
}

float matrix_topk_accuracy(matrix truth, matrix guess, ကိန်းပြည့် k)
{
    ကိန်းပြည့် *indexes = calloc(k, parimaa_na(ကိန်းပြည့်));
    ကိန်းပြည့် n = truth.cols;
    ကိန်းပြည့် i,j;
    ကိန်းပြည့် correct = 0;
    အတွက်(i = 0; i < truth.rows; ++i){
        top_k(guess.vals[i], n, k, indexes);
        အတွက်(j = 0; j < k; ++j){
            ကိန်းပြည့် class = indexes[j];
            အကယ်၍(truth.vals[i][class]){
                ++correct;
                bha_nga;
            }
        }
    }
    free(indexes);
    ပြန်ပေး (float)correct/truth.rows;
}

void scale_matrix(matrix m, float scale)
{
    ကိန်းပြည့် i,j;
    အတွက်(i = 0; i < m.rows; ++i){
        အတွက်(j = 0; j < m.cols; ++j){
            m.vals[i][j] *= scale;
        }
    }
}

matrix resize_matrix(matrix m, ကိန်းပြည့် size)
{
    ကိန်းပြည့် i;
    အကယ်၍ (m.rows == size) ပြန်ပေး m;
    အကယ်၍ (m.rows < size) {
        m.vals = realloc(m.vals, size*parimaa_na(float*));
        အတွက် (i = m.rows; i < size; ++i) {
            m.vals[i] = calloc(m.cols, parimaa_na(float));
        }
    } မဟုတ်ပါက အကယ်၍ (m.rows > size) {
        အတွက် (i = size; i < m.rows; ++i) {
            free(m.vals[i]);
        }
        m.vals = realloc(m.vals, size*parimaa_na(float*));
    }
    m.rows = size;
    ပြန်ပေး m;
}

void matrix_add_matrix(matrix from, matrix to)
{
    assert(from.rows == to.rows && from.cols == to.cols);
    ကိန်းပြည့် i,j;
    အတွက်(i = 0; i < from.rows; ++i){
        အတွက်(j = 0; j < from.cols; ++j){
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
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < c.rows; ++i){
        c.vals[i] = calloc(c.cols, parimaa_na(float));
        copy_cpu(c.cols, m.vals[i], 1, c.vals[i], 1);
    }
    ပြန်ပေး c;
}

matrix make_matrix(ကိန်းပြည့် rows, ကိန်းပြည့် cols)
{
    ကိန်းပြည့် i;
    matrix m;
    m.rows = rows;
    m.cols = cols;
    m.vals = calloc(m.rows, parimaa_na(float *));
    အတွက်(i = 0; i < m.rows; ++i){
        m.vals[i] = calloc(m.cols, parimaa_na(float));
    }
    ပြန်ပေး m;
}

matrix hold_out_matrix(matrix *m, ကိန်းပြည့် n)
{
    ကိန်းပြည့် i;
    matrix h;
    h.rows = n;
    h.cols = m->cols;
    h.vals = calloc(h.rows, parimaa_na(float *));
    အတွက်(i = 0; i < n; ++i){
        ကိန်းပြည့် index = rand()%m->rows;
        h.vals[i] = m->vals[index];
        m->vals[index] = m->vals[--(m->rows)];
    }
    ပြန်ပေး h;
}

float *pop_column(matrix *m, ကိန်းပြည့် c)
{
    float *col = calloc(m->rows, parimaa_na(float));
    ကိန်းပြည့် i, j;
    အတွက်(i = 0; i < m->rows; ++i){
        col[i] = m->vals[i][c];
        အတွက်(j = c; j < m->cols-1; ++j){
            m->vals[i][j] = m->vals[i][j+1];
        }
    }
    --m->cols;
    ပြန်ပေး col;
}

matrix csv_to_matrix(var_na *filename)
{
    FILE *fp = fopen(filename, "r");
    အကယ်၍(!fp) file_error(filename);

    matrix m;
    m.cols = -1;

    var_na *line;

    ကိန်းပြည့် n = 0;
    ကိန်းပြည့် size = 1024;
    m.vals = calloc(size, parimaa_na(float*));
    နေစဉ်((line = fgetl(fp))){
        အကယ်၍(m.cols == -1) m.cols = count_fields(line);
        အကယ်၍(n == size){
            size *= 2;
            m.vals = realloc(m.vals, size*parimaa_na(float*));
        }
        m.vals[n] = parse_fields(line, m.cols);
        free(line);
        ++n;
    }
    m.vals = realloc(m.vals, n*parimaa_na(float*));
    m.rows = n;
    ပြန်ပေး m;
}

void matrix_to_csv(matrix m)
{
    ကိန်းပြည့် i, j;

    အတွက်(i = 0; i < m.rows; ++i){
        အတွက်(j = 0; j < m.cols; ++j){
            အကယ်၍(j > 0) printf(",");
            printf("%.17g", m.vals[i][j]);
        }
        printf("\n");
    }
}

void print_matrix(matrix m)
{
    ကိန်းပြည့် i, j;
    printf("%d X %d Matrix:\n",m.rows, m.cols);
    printf(" __");
    အတွက်(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__ \n");

    printf("|  ");
    အတွက်(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("  |\n");

    အတွက်(i = 0; i < m.rows; ++i){
        printf("|  ");
        အတွက်(j = 0; j < m.cols; ++j){
            printf("%15.7f ", m.vals[i][j]);
        }
        printf(" |\n");
    }
    printf("|__");
    အတွက်(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__|\n");
}
