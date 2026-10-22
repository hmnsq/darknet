#ifndef BLAS_H
#define BLAS_H
#include "darknet.h"

void flatten(float *x, 정수 size, 정수 layers, 정수 batch, 정수 forward);
void pm(정수 M, 정수 N, float *A);
float *random_matrix(정수 rows, 정수 cols);
void time_random_matrix(정수 TA, 정수 TB, 정수 m, 정수 k, 정수 n);
void reorg_cpu(float *x, 정수 w, 정수 h, 정수 c, 정수 batch, 정수 stride, 정수 forward, float *out);

void test_blas();

void inter_cpu(정수 NX, float *X, 정수 NY, float *Y, 정수 B, float *OUT);
void deinter_cpu(정수 NX, float *X, 정수 NY, float *Y, 정수 B, float *OUT);
void mult_add_into_cpu(정수 N, float *X, float *Y, float *Z);

void const_cpu(정수 N, float ALPHA, float *X, 정수 INCX);
void constrain_gpu(정수 N, float ALPHA, float * X, 정수 INCX);
void pow_cpu(정수 N, float ALPHA, float *X, 정수 INCX, float *Y, 정수 INCY);
void mul_cpu(정수 N, float *X, 정수 INCX, float *Y, 정수 INCY);

정수 test_gpu_blas();
void shortcut_cpu(정수 batch, 정수 w1, 정수 h1, 정수 c1, float *add, 정수 w2, 정수 h2, 정수 c2, float s1, float s2, float *out);

void mean_cpu(float *x, 정수 batch, 정수 filters, 정수 spatial, float *mean);
void variance_cpu(float *x, float *mean, 정수 batch, 정수 filters, 정수 spatial, float *variance);

void scale_bias(float *output, float *scales, 정수 batch, 정수 n, 정수 size);
void backward_scale_cpu(float *x_norm, float *delta, 정수 batch, 정수 n, 정수 size, float *scale_updates);
void mean_delta_cpu(float *delta, float *variance, 정수 batch, 정수 filters, 정수 spatial, float *mean_delta);
void  variance_delta_cpu(float *x, float *delta, float *mean, float *variance, 정수 batch, 정수 filters, 정수 spatial, float *variance_delta);
void normalize_delta_cpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, 정수 batch, 정수 filters, 정수 spatial, float *delta);
void l2normalize_cpu(float *x, float *dx, 정수 batch, 정수 filters, 정수 spatial);

void smooth_l1_cpu(정수 n, float *pred, float *truth, float *delta, float *error);
void l2_cpu(정수 n, float *pred, float *truth, float *delta, float *error);
void l1_cpu(정수 n, float *pred, float *truth, float *delta, float *error);
void logistic_x_ent_cpu(정수 n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_cpu(정수 n, float *pred, float *truth, float *delta, float *error);
void weighted_sum_cpu(float *a, float *b, float *s, 정수 num, float *c);
void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, 정수 n, float *dc);

void softmax(float *input, 정수 n, float temp, 정수 stride, float *output);
void softmax_cpu(float *input, 정수 n, 정수 batch, 정수 batch_offset, 정수 groups, 정수 group_offset, 정수 stride, float temp, float *output);
void upsample_cpu(float *in, 정수 w, 정수 h, 정수 c, 정수 batch, 정수 stride, 정수 forward, float scale, float *out);

#ifdef GPU
#include "cuda.h"
#include "tree.h"

void axpy_gpu(정수 N, float ALPHA, float * X, 정수 INCX, float * Y, 정수 INCY);
void axpy_gpu_offset(정수 N, float ALPHA, float * X, 정수 OFFX, 정수 INCX, float * Y, 정수 OFFY, 정수 INCY);
void copy_gpu(정수 N, float * X, 정수 INCX, float * Y, 정수 INCY);
void copy_gpu_offset(정수 N, float * X, 정수 OFFX, 정수 INCX, float * Y, 정수 OFFY, 정수 INCY);
void add_gpu(정수 N, float ALPHA, float * X, 정수 INCX);
void supp_gpu(정수 N, float ALPHA, float * X, 정수 INCX);
void mask_gpu(정수 N, float * X, float mask_num, float * mask, float val);
void scale_mask_gpu(정수 N, float * X, float mask_num, float * mask, float scale);
void const_gpu(정수 N, float ALPHA, float *X, 정수 INCX);
void pow_gpu(정수 N, float ALPHA, float *X, 정수 INCX, float *Y, 정수 INCY);
void mul_gpu(정수 N, float *X, 정수 INCX, float *Y, 정수 INCY);

void mean_gpu(float *x, 정수 batch, 정수 filters, 정수 spatial, float *mean);
void variance_gpu(float *x, float *mean, 정수 batch, 정수 filters, 정수 spatial, float *variance);
void normalize_gpu(float *x, float *mean, float *variance, 정수 batch, 정수 filters, 정수 spatial);
void l2normalize_gpu(float *x, float *dx, 정수 batch, 정수 filters, 정수 spatial);

void normalize_delta_gpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, 정수 batch, 정수 filters, 정수 spatial, float *delta);

void fast_mean_delta_gpu(float *delta, float *variance, 정수 batch, 정수 filters, 정수 spatial, float *mean_delta);
void fast_variance_delta_gpu(float *x, float *delta, float *mean, float *variance, 정수 batch, 정수 filters, 정수 spatial, float *variance_delta);

void fast_variance_gpu(float *x, float *mean, 정수 batch, 정수 filters, 정수 spatial, float *variance);
void fast_mean_gpu(float *x, 정수 batch, 정수 filters, 정수 spatial, float *mean);
void shortcut_gpu(정수 batch, 정수 w1, 정수 h1, 정수 c1, float *add, 정수 w2, 정수 h2, 정수 c2, float s1, float s2, float *out);
void scale_bias_gpu(float *output, float *biases, 정수 batch, 정수 n, 정수 size);
void backward_scale_gpu(float *x_norm, float *delta, 정수 batch, 정수 n, 정수 size, float *scale_updates);
void scale_bias_gpu(float *output, float *biases, 정수 batch, 정수 n, 정수 size);
void add_bias_gpu(float *output, float *biases, 정수 batch, 정수 n, 정수 size);
void backward_bias_gpu(float *bias_updates, float *delta, 정수 batch, 정수 n, 정수 size);

void logistic_x_ent_gpu(정수 n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_gpu(정수 n, float *pred, float *truth, float *delta, float *error);
void smooth_l1_gpu(정수 n, float *pred, float *truth, float *delta, float *error);
void l2_gpu(정수 n, float *pred, float *truth, float *delta, float *error);
void l1_gpu(정수 n, float *pred, float *truth, float *delta, float *error);
void wgan_gpu(정수 n, float *pred, float *truth, float *delta, float *error);
void weighted_delta_gpu(float *a, float *b, float *s, float *da, float *db, float *ds, 정수 num, float *dc);
void weighted_sum_gpu(float *a, float *b, float *s, 정수 num, float *c);
void mult_add_into_gpu(정수 num, float *a, float *b, float *c);
void inter_gpu(정수 NX, float *X, 정수 NY, float *Y, 정수 B, float *OUT);
void deinter_gpu(정수 NX, float *X, 정수 NY, float *Y, 정수 B, float *OUT);

void reorg_gpu(float *x, 정수 w, 정수 h, 정수 c, 정수 batch, 정수 stride, 정수 forward, float *out);

void softmax_gpu(float *input, 정수 n, 정수 batch, 정수 batch_offset, 정수 groups, 정수 group_offset, 정수 stride, float temp, float *output);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, 정수 n, 정수 batch, 정수 t);
void adam_gpu(정수 n, float *x, float *m, float *v, float B1, float B2, float rate, float eps, 정수 t);

void flatten_gpu(float *x, 정수 spatial, 정수 layers, 정수 batch, 정수 forward, float *out);
void softmax_tree(float *input, 정수 spatial, 정수 batch, 정수 stride, float temp, float *output, tree hier);
void upsample_gpu(float *in, 정수 w, 정수 h, 정수 c, 정수 batch, 정수 stride, 정수 forward, float scale, float *out);

#endif
#endif
