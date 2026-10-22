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
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < m.rows; ++i) free(m.vals[i]);
    free(m.vals);
}

float matrix_topk_accuracy(matrix truth, matrix guess, ຈຳນວນເຕັມ k)
{
    ຈຳນວນເຕັມ *indexes = calloc(k, parimaa_na(ຈຳນວນເຕັມ));
    ຈຳນວນເຕັມ n = truth.cols;
    ຈຳນວນເຕັມ i,j;
    ຈຳນວນເຕັມ correct = 0;
    ສຳລັບ(i = 0; i < truth.rows; ++i){
        top_k(guess.vals[i], n, k, indexes);
        ສຳລັບ(j = 0; j < k; ++j){
            ຈຳນວນເຕັມ class = indexes[j];
            ຖ້າ(truth.vals[i][class]){
                ++correct;
                bha_nga;
            }
        }
    }
    free(indexes);
    ສົ່ງຄືນ (float)correct/truth.rows;
}

void scale_matrix(matrix m, float scale)
{
    ຈຳນວນເຕັມ i,j;
    ສຳລັບ(i = 0; i < m.rows; ++i){
        ສຳລັບ(j = 0; j < m.cols; ++j){
            m.vals[i][j] *= scale;
        }
    }
}

matrix resize_matrix(matrix m, ຈຳນວນເຕັມ size)
{
    ຈຳນວນເຕັມ i;
    ຖ້າ (m.rows == size) ສົ່ງຄືນ m;
    ຖ້າ (m.rows < size) {
        m.vals = realloc(m.vals, size*parimaa_na(float*));
        ສຳລັບ (i = m.rows; i < size; ++i) {
            m.vals[i] = calloc(m.cols, parimaa_na(float));
        }
    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (m.rows > size) {
        ສຳລັບ (i = size; i < m.rows; ++i) {
            free(m.vals[i]);
        }
        m.vals = realloc(m.vals, size*parimaa_na(float*));
    }
    m.rows = size;
    ສົ່ງຄືນ m;
}

void matrix_add_matrix(matrix from, matrix to)
{
    assert(from.rows == to.rows && from.cols == to.cols);
    ຈຳນວນເຕັມ i,j;
    ສຳລັບ(i = 0; i < from.rows; ++i){
        ສຳລັບ(j = 0; j < from.cols; ++j){
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
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < c.rows; ++i){
        c.vals[i] = calloc(c.cols, parimaa_na(float));
        copy_cpu(c.cols, m.vals[i], 1, c.vals[i], 1);
    }
    ສົ່ງຄືນ c;
}

matrix make_matrix(ຈຳນວນເຕັມ rows, ຈຳນວນເຕັມ cols)
{
    ຈຳນວນເຕັມ i;
    matrix m;
    m.rows = rows;
    m.cols = cols;
    m.vals = calloc(m.rows, parimaa_na(float *));
    ສຳລັບ(i = 0; i < m.rows; ++i){
        m.vals[i] = calloc(m.cols, parimaa_na(float));
    }
    ສົ່ງຄືນ m;
}

matrix hold_out_matrix(matrix *m, ຈຳນວນເຕັມ n)
{
    ຈຳນວນເຕັມ i;
    matrix h;
    h.rows = n;
    h.cols = m->cols;
    h.vals = calloc(h.rows, parimaa_na(float *));
    ສຳລັບ(i = 0; i < n; ++i){
        ຈຳນວນເຕັມ index = rand()%m->rows;
        h.vals[i] = m->vals[index];
        m->vals[index] = m->vals[--(m->rows)];
    }
    ສົ່ງຄືນ h;
}

float *pop_column(matrix *m, ຈຳນວນເຕັມ c)
{
    float *col = calloc(m->rows, parimaa_na(float));
    ຈຳນວນເຕັມ i, j;
    ສຳລັບ(i = 0; i < m->rows; ++i){
        col[i] = m->vals[i][c];
        ສຳລັບ(j = c; j < m->cols-1; ++j){
            m->vals[i][j] = m->vals[i][j+1];
        }
    }
    --m->cols;
    ສົ່ງຄືນ col;
}

matrix csv_to_matrix(var_na *filename)
{
    FILE *fp = fopen(filename, "r");
    ຖ້າ(!fp) file_error(filename);

    matrix m;
    m.cols = -1;

    var_na *line;

    ຈຳນວນເຕັມ n = 0;
    ຈຳນວນເຕັມ size = 1024;
    m.vals = calloc(size, parimaa_na(float*));
    ໃນຂະນະທີ່((line = fgetl(fp))){
        ຖ້າ(m.cols == -1) m.cols = count_fields(line);
        ຖ້າ(n == size){
            size *= 2;
            m.vals = realloc(m.vals, size*parimaa_na(float*));
        }
        m.vals[n] = parse_fields(line, m.cols);
        free(line);
        ++n;
    }
    m.vals = realloc(m.vals, n*parimaa_na(float*));
    m.rows = n;
    ສົ່ງຄືນ m;
}

void matrix_to_csv(matrix m)
{
    ຈຳນວນເຕັມ i, j;

    ສຳລັບ(i = 0; i < m.rows; ++i){
        ສຳລັບ(j = 0; j < m.cols; ++j){
            ຖ້າ(j > 0) printf(",");
            printf("%.17g", m.vals[i][j]);
        }
        printf("\n");
    }
}

void print_matrix(matrix m)
{
    ຈຳນວນເຕັມ i, j;
    printf("%d X %d Matrix:\n",m.rows, m.cols);
    printf(" __");
    ສຳລັບ(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__ \n");

    printf("|  ");
    ສຳລັບ(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("  |\n");

    ສຳລັບ(i = 0; i < m.rows; ++i){
        printf("|  ");
        ສຳລັບ(j = 0; j < m.cols; ++j){
            printf("%15.7f ", m.vals[i][j]);
        }
        printf(" |\n");
    }
    printf("|__");
    ສຳລັບ(j = 0; j < 16*m.cols-1; ++j) printf(" ");
    printf("__|\n");
}
