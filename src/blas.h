#ifndef BLAS_H
#define BLAS_H
#include "darknet.h"

void flatten(float *x, цео size, цео layers, цео batch, цео forward);
void pm(цео M, цео N, float *A);
float *random_matrix(цео rows, цео cols);
void time_random_matrix(цео TA, цео TB, цео m, цео k, цео n);
void reorg_cpu(float *x, цео w, цео h, цео c, цео batch, цео stride, цео forward, float *out);

void test_blas();

void inter_cpu(цео NX, float *X, цео NY, float *Y, цео B, float *OUT);
void deinter_cpu(цео NX, float *X, цео NY, float *Y, цео B, float *OUT);
void mult_add_into_cpu(цео N, float *X, float *Y, float *Z);

void const_cpu(цео N, float ALPHA, float *X, цео INCX);
void constrain_gpu(цео N, float ALPHA, float * X, цео INCX);
void pow_cpu(цео N, float ALPHA, float *X, цео INCX, float *Y, цео INCY);
void mul_cpu(цео N, float *X, цео INCX, float *Y, цео INCY);

цео test_gpu_blas();
void shortcut_cpu(цео batch, цео w1, цео h1, цео c1, float *add, цео w2, цео h2, цео c2, float s1, float s2, float *out);

void mean_cpu(float *x, цео batch, цео filters, цео spatial, float *mean);
void variance_cpu(float *x, float *mean, цео batch, цео filters, цео spatial, float *variance);

void scale_bias(float *output, float *scales, цео batch, цео n, цео size);
void backward_scale_cpu(float *x_norm, float *delta, цео batch, цео n, цео size, float *scale_updates);
void mean_delta_cpu(float *delta, float *variance, цео batch, цео filters, цео spatial, float *mean_delta);
void  variance_delta_cpu(float *x, float *delta, float *mean, float *variance, цео batch, цео filters, цео spatial, float *variance_delta);
void normalize_delta_cpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, цео batch, цео filters, цео spatial, float *delta);
void l2normalize_cpu(float *x, float *dx, цео batch, цео filters, цео spatial);

void smooth_l1_cpu(цео n, float *pred, float *truth, float *delta, float *error);
void l2_cpu(цео n, float *pred, float *truth, float *delta, float *error);
void l1_cpu(цео n, float *pred, float *truth, float *delta, float *error);
void logistic_x_ent_cpu(цео n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_cpu(цео n, float *pred, float *truth, float *delta, float *error);
void weighted_sum_cpu(float *a, float *b, float *s, цео num, float *c);
void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, цео n, float *dc);

void softmax(float *input, цео n, float temp, цео stride, float *output);
void softmax_cpu(float *input, цео n, цео batch, цео batch_offset, цео groups, цео group_offset, цео stride, float temp, float *output);
void upsample_cpu(float *in, цео w, цео h, цео c, цео batch, цео stride, цео forward, float scale, float *out);

#ifdef GPU
#include "cuda.h"
#include "tree.h"

void axpy_gpu(цео N, float ALPHA, float * X, цео INCX, float * Y, цео INCY);
void axpy_gpu_offset(цео N, float ALPHA, float * X, цео OFFX, цео INCX, float * Y, цео OFFY, цео INCY);
void copy_gpu(цео N, float * X, цео INCX, float * Y, цео INCY);
void copy_gpu_offset(цео N, float * X, цео OFFX, цео INCX, float * Y, цео OFFY, цео INCY);
void add_gpu(цео N, float ALPHA, float * X, цео INCX);
void supp_gpu(цео N, float ALPHA, float * X, цео INCX);
void mask_gpu(цео N, float * X, float mask_num, float * mask, float val);
void scale_mask_gpu(цео N, float * X, float mask_num, float * mask, float scale);
void const_gpu(цео N, float ALPHA, float *X, цео INCX);
void pow_gpu(цео N, float ALPHA, float *X, цео INCX, float *Y, цео INCY);
void mul_gpu(цео N, float *X, цео INCX, float *Y, цео INCY);

void mean_gpu(float *x, цео batch, цео filters, цео spatial, float *mean);
void variance_gpu(float *x, float *mean, цео batch, цео filters, цео spatial, float *variance);
void normalize_gpu(float *x, float *mean, float *variance, цео batch, цео filters, цео spatial);
void l2normalize_gpu(float *x, float *dx, цео batch, цео filters, цео spatial);

void normalize_delta_gpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, цео batch, цео filters, цео spatial, float *delta);

void fast_mean_delta_gpu(float *delta, float *variance, цео batch, цео filters, цео spatial, float *mean_delta);
void fast_variance_delta_gpu(float *x, float *delta, float *mean, float *variance, цео batch, цео filters, цео spatial, float *variance_delta);

void fast_variance_gpu(float *x, float *mean, цео batch, цео filters, цео spatial, float *variance);
void fast_mean_gpu(float *x, цео batch, цео filters, цео spatial, float *mean);
void shortcut_gpu(цео batch, цео w1, цео h1, цео c1, float *add, цео w2, цео h2, цео c2, float s1, float s2, float *out);
void scale_bias_gpu(float *output, float *biases, цео batch, цео n, цео size);
void backward_scale_gpu(float *x_norm, float *delta, цео batch, цео n, цео size, float *scale_updates);
void scale_bias_gpu(float *output, float *biases, цео batch, цео n, цео size);
void add_bias_gpu(float *output, float *biases, цео batch, цео n, цео size);
void backward_bias_gpu(float *bias_updates, float *delta, цео batch, цео n, цео size);

void logistic_x_ent_gpu(цео n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_gpu(цео n, float *pred, float *truth, float *delta, float *error);
void smooth_l1_gpu(цео n, float *pred, float *truth, float *delta, float *error);
void l2_gpu(цео n, float *pred, float *truth, float *delta, float *error);
void l1_gpu(цео n, float *pred, float *truth, float *delta, float *error);
void wgan_gpu(цео n, float *pred, float *truth, float *delta, float *error);
void weighted_delta_gpu(float *a, float *b, float *s, float *da, float *db, float *ds, цео num, float *dc);
void weighted_sum_gpu(float *a, float *b, float *s, цео num, float *c);
void mult_add_into_gpu(цео num, float *a, float *b, float *c);
void inter_gpu(цео NX, float *X, цео NY, float *Y, цео B, float *OUT);
void deinter_gpu(цео NX, float *X, цео NY, float *Y, цео B, float *OUT);

void reorg_gpu(float *x, цео w, цео h, цео c, цео batch, цео stride, цео forward, float *out);

void softmax_gpu(float *input, цео n, цео batch, цео batch_offset, цео groups, цео group_offset, цео stride, float temp, float *output);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, цео n, цео batch, цео t);
void adam_gpu(цео n, float *x, float *m, float *v, float B1, float B2, float rate, float eps, цео t);

void flatten_gpu(float *x, цео spatial, цео layers, цео batch, цео forward, float *out);
void softmax_tree(float *input, цео spatial, цео batch, цео stride, float temp, float *output, tree hier);
void upsample_gpu(float *in, цео w, цео h, цео c, цео batch, цео stride, цео forward, float scale, float *out);

#endif
#endif
