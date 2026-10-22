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
    puur_naa_nka i;
    for(i = 0; i < m.rows; ++i) free(m.vals[i]);
    free(m.vals);
}

float matrix_topk_accuracy(matrix truth, matrix guess, puur_naa_nka k)
{
    puur_naa_nka *indexes = calloc(k, parimaa_na(puur_naa_nka));
    puur_naa_nka n = truth.cols;
    puur_naa_nka i,j;
    puur_naa_nka correct = 0;
    for(i = 0; i < truth.rows; ++i){
        top_k(guess.vals[i], n, k, indexes);
        for(j = 0; j < k; ++j){
            puur_naa_nka class = indexes[j];
            ᮚᮦᮔ᮪(truth.vals[i][class]){
                ++correct;
                bha_nga;
            }
        }
    }
    free(indexes);
    ᮝᮜᮤ (float)correct/truth.rows;
}

void scale_matrix(matrix m, float scale)
{
    puur_naa_nka i,j;
    for(i = 0; i < m.rows; ++i){
        for(j = 0; j < m.cols; ++j){
            m.vals[i][j] *= scale;
        }
    }
}

matrix resize_matrix(matrix m, puur_naa_nka size)
{
    puur_naa_nka i;
    ᮚᮦᮔ᮪ (m.rows == size) ᮝᮜᮤ m;
    ᮚᮦᮔ᮪ (m.rows < size) {
        m.vals = realloc(m.vals, size*parimaa_na(float*));
        for (i = m.rows; i < size; ++i) {
            m.vals[i] = calloc(m.cols, parimaa_na(float));
        }
    } ᮜᮤᮚ ᮚᮦᮔ᮪ (m.rows > size) {
        for (i = size; i < m.rows; ++i) {
            free(m.vals[i]);
        }
        m.vals = realloc(m.vals, size*parimaa_na(float*));
    }
    m.rows = size;
    ᮝᮜᮤ m;
}

void matrix_add_matrix(matrix from, matrix to)
{
    assert(from.rows == to.rows && from.cols == to.cols);
    puur_naa_nka i,j;
    for(i = 0; i < from.rows; ++i){
        for(j = 0; j < from.cols; ++j){
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
    puur_naa_nka i;
    for(i = 0; i < c.rows; ++i){
        c.vals[i] = calloc(c.cols, parimaa_na(float));
        copy_cpu(c.cols, m.vals[i], 1, c.vals[i], 1);
    }
    ᮝᮜᮤ c;
}

matrix make_matrix(puur_naa_nka rows, puur_naa_nka cols)
{
    puur_naa_nka i;
    matrix m;
    m.rows = rows;
    m.cols = cols;
    m.vals = calloc(m.rows, parimaa_na(float *));
    for(i = 0; i < m.rows; ++i){
        m.vals[i] = calloc(m.cols, parimaa_na(float));
    }
    ᮝᮜᮤ m;
}

matrix hold_out_matrix(matrix *m, puur_naa_nka n)
{
    puur_naa_nka i;
    matrix h;
    h.rows = n;
    h.cols = m->cols;
    h.vals = calloc(h.rows, parimaa_na(float *));
    for(i = 0; i < n; ++i){
        puur_naa_nka index = rand()%m->rows;
        h.vals[i] = m->vals[index];
        m->vals[index] = m->vals[--(m->rows)];
    }
    ᮝᮜᮤ h;
}

float *pop_column(matrix *m, puur_naa_nka c)
{
    float *col = calloc(m->rows, parimaa_na(float));
    puur_naa_nka i, j;
    for(i = 0; i < m->rows; ++i){
        col[i] = m->vals[i][c];
        for(j = c; j < m->cols-1; ++j){
            m->vals[i][j] = m->vals[i][j+1];
        }
    }
    --m->cols;
    ᮝᮜᮤ col;
}

matrix csv_to_matrix(var_na *filename)
{
    FILE *fp = fopen(filename, "r");
    ᮚᮦᮔ᮪(!fp) file_error(filename);

    matrix m;
    m.cols = -1;

    var_na *line;

    puur_naa_nka n = 0;
    puur_naa_nka size = 1024;
    m.vals = calloc(size, parimaa_na(float*));
    ᮞᮨᮓᮨᮜ((line = fgetl(fp))){
        ᮚᮦᮔ᮪(m.cols == -1) m.cols = count_fields(line);
        ᮚᮦᮔ᮪(n == size){
            size *= 2;
            m.vals = realloc(m.vals, size*parimaa_na(float*));
        }
        m.vals[n] = parse_fields(line, m.cols);
        free(line);
        ++n;
    }
    m.vals = realloc(m.vals, n*parimaa_na(float*));
    m.rows = n;
    ᮝᮜᮤ m;
}

void matrix_to_csv(matrix m)
{
    puur_naa_nka i, j;

    for(i = 0; i < m.rows; ++i){
        for(j = 0; j < m.cols; ++j){
            ᮚᮦᮔ᮪(j > 0) printf(",");
            printf("%.17g", m.vals[i][j]);
        }
        printf("\n");
    }
}

void print_matrix(matrix m)
{
    puur_naa_nka i, j;
    printf("%d X %d Matrix:\n",m.rows, m.cols);
    printf(" __");
    for(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__ \n");

    printf("|  ");
    for(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("  |\n");

    for(i = 0; i < m.rows; ++i){
        printf("|  ");
        for(j = 0; j < m.cols; ++j){
            printf("%15.7f ", m.vals[i][j]);
        }
        printf(" |\n");
    }
    printf("|__");
    for(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__|\n");
}
