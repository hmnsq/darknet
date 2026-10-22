#ifndef BLAS_H
#define BLAS_H
#include "darknet.h"

void flatten(float *x, 整数 size, 整数 layers, 整数 batch, 整数 forward);
void pm(整数 M, 整数 N, float *A);
float *random_matrix(整数 rows, 整数 cols);
void time_random_matrix(整数 TA, 整数 TB, 整数 m, 整数 k, 整数 n);
void reorg_cpu(float *x, 整数 w, 整数 h, 整数 c, 整数 batch, 整数 stride, 整数 forward, float *out);

void test_blas();

void inter_cpu(整数 NX, float *X, 整数 NY, float *Y, 整数 B, float *OUT);
void deinter_cpu(整数 NX, float *X, 整数 NY, float *Y, 整数 B, float *OUT);
void mult_add_into_cpu(整数 N, float *X, float *Y, float *Z);

void const_cpu(整数 N, float ALPHA, float *X, 整数 INCX);
void constrain_gpu(整数 N, float ALPHA, float * X, 整数 INCX);
void pow_cpu(整数 N, float ALPHA, float *X, 整数 INCX, float *Y, 整数 INCY);
void mul_cpu(整数 N, float *X, 整数 INCX, float *Y, 整数 INCY);

整数 test_gpu_blas();
void shortcut_cpu(整数 batch, 整数 w1, 整数 h1, 整数 c1, float *add, 整数 w2, 整数 h2, 整数 c2, float s1, float s2, float *out);

void mean_cpu(float *x, 整数 batch, 整数 filters, 整数 spatial, float *mean);
void variance_cpu(float *x, float *mean, 整数 batch, 整数 filters, 整数 spatial, float *variance);

void scale_bias(float *output, float *scales, 整数 batch, 整数 n, 整数 size);
void backward_scale_cpu(float *x_norm, float *delta, 整数 batch, 整数 n, 整数 size, float *scale_updates);
void mean_delta_cpu(float *delta, float *variance, 整数 batch, 整数 filters, 整数 spatial, float *mean_delta);
void  variance_delta_cpu(float *x, float *delta, float *mean, float *variance, 整数 batch, 整数 filters, 整数 spatial, float *variance_delta);
void normalize_delta_cpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, 整数 batch, 整数 filters, 整数 spatial, float *delta);
void l2normalize_cpu(float *x, float *dx, 整数 batch, 整数 filters, 整数 spatial);

void smooth_l1_cpu(整数 n, float *pred, float *truth, float *delta, float *error);
void l2_cpu(整数 n, float *pred, float *truth, float *delta, float *error);
void l1_cpu(整数 n, float *pred, float *truth, float *delta, float *error);
void logistic_x_ent_cpu(整数 n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_cpu(整数 n, float *pred, float *truth, float *delta, float *error);
void weighted_sum_cpu(float *a, float *b, float *s, 整数 num, float *c);
void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, 整数 n, float *dc);

void softmax(float *input, 整数 n, float temp, 整数 stride, float *output);
void softmax_cpu(float *input, 整数 n, 整数 batch, 整数 batch_offset, 整数 groups, 整数 group_offset, 整数 stride, float temp, float *output);
void upsample_cpu(float *in, 整数 w, 整数 h, 整数 c, 整数 batch, 整数 stride, 整数 forward, float scale, float *out);

#ifdef GPU
#include "cuda.h"
#include "tree.h"

void axpy_gpu(整数 N, float ALPHA, float * X, 整数 INCX, float * Y, 整数 INCY);
void axpy_gpu_offset(整数 N, float ALPHA, float * X, 整数 OFFX, 整数 INCX, float * Y, 整数 OFFY, 整数 INCY);
void copy_gpu(整数 N, float * X, 整数 INCX, float * Y, 整数 INCY);
void copy_gpu_offset(整数 N, float * X, 整数 OFFX, 整数 INCX, float * Y, 整数 OFFY, 整数 INCY);
void add_gpu(整数 N, float ALPHA, float * X, 整数 INCX);
void supp_gpu(整数 N, float ALPHA, float * X, 整数 INCX);
void mask_gpu(整数 N, float * X, float mask_num, float * mask, float val);
void scale_mask_gpu(整数 N, float * X, float mask_num, float * mask, float scale);
void const_gpu(整数 N, float ALPHA, float *X, 整数 INCX);
void pow_gpu(整数 N, float ALPHA, float *X, 整数 INCX, float *Y, 整数 INCY);
void mul_gpu(整数 N, float *X, 整数 INCX, float *Y, 整数 INCY);

void mean_gpu(float *x, 整数 batch, 整数 filters, 整数 spatial, float *mean);
void variance_gpu(float *x, float *mean, 整数 batch, 整数 filters, 整数 spatial, float *variance);
void normalize_gpu(float *x, float *mean, float *variance, 整数 batch, 整数 filters, 整数 spatial);
void l2normalize_gpu(float *x, float *dx, 整数 batch, 整数 filters, 整数 spatial);

void normalize_delta_gpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, 整数 batch, 整数 filters, 整数 spatial, float *delta);

void fast_mean_delta_gpu(float *delta, float *variance, 整数 batch, 整数 filters, 整数 spatial, float *mean_delta);
void fast_variance_delta_gpu(float *x, float *delta, float *mean, float *variance, 整数 batch, 整数 filters, 整数 spatial, float *variance_delta);

void fast_variance_gpu(float *x, float *mean, 整数 batch, 整数 filters, 整数 spatial, float *variance);
void fast_mean_gpu(float *x, 整数 batch, 整数 filters, 整数 spatial, float *mean);
void shortcut_gpu(整数 batch, 整数 w1, 整数 h1, 整数 c1, float *add, 整数 w2, 整数 h2, 整数 c2, float s1, float s2, float *out);
void scale_bias_gpu(float *output, float *biases, 整数 batch, 整数 n, 整数 size);
void backward_scale_gpu(float *x_norm, float *delta, 整数 batch, 整数 n, 整数 size, float *scale_updates);
void scale_bias_gpu(float *output, float *biases, 整数 batch, 整数 n, 整数 size);
void add_bias_gpu(float *output, float *biases, 整数 batch, 整数 n, 整数 size);
void backward_bias_gpu(float *bias_updates, float *delta, 整数 batch, 整数 n, 整数 size);

void logistic_x_ent_gpu(整数 n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_gpu(整数 n, float *pred, float *truth, float *delta, float *error);
void smooth_l1_gpu(整数 n, float *pred, float *truth, float *delta, float *error);
void l2_gpu(整数 n, float *pred, float *truth, float *delta, float *error);
void l1_gpu(整数 n, float *pred, float *truth, float *delta, float *error);
void wgan_gpu(整数 n, float *pred, float *truth, float *delta, float *error);
void weighted_delta_gpu(float *a, float *b, float *s, float *da, float *db, float *ds, 整数 num, float *dc);
void weighted_sum_gpu(float *a, float *b, float *s, 整数 num, float *c);
void mult_add_into_gpu(整数 num, float *a, float *b, float *c);
void inter_gpu(整数 NX, float *X, 整数 NY, float *Y, 整数 B, float *OUT);
void deinter_gpu(整数 NX, float *X, 整数 NY, float *Y, 整数 B, float *OUT);

void reorg_gpu(float *x, 整数 w, 整数 h, 整数 c, 整数 batch, 整数 stride, 整数 forward, float *out);

void softmax_gpu(float *input, 整数 n, 整数 batch, 整数 batch_offset, 整数 groups, 整数 group_offset, 整数 stride, float temp, float *output);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, 整数 n, 整数 batch, 整数 t);
void adam_gpu(整数 n, float *x, float *m, float *v, float B1, float B2, float rate, float eps, 整数 t);

void flatten_gpu(float *x, 整数 spatial, 整数 layers, 整数 batch, 整数 forward, float *out);
void softmax_tree(float *input, 整数 spatial, 整数 batch, 整数 stride, float temp, float *output, tree hier);
void upsample_gpu(float *in, 整数 w, 整数 h, 整数 c, 整数 batch, 整数 stride, 整数 forward, float scale, float *out);

#endif
#endif
