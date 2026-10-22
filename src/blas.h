#ifndef BLAS_H
#define BLAS_H
#include "darknet.h"

void flatten(float *x, ကိန်းပြည့် size, ကိန်းပြည့် layers, ကိန်းပြည့် batch, ကိန်းပြည့် forward);
void pm(ကိန်းပြည့် M, ကိန်းပြည့် N, float *A);
float *random_matrix(ကိန်းပြည့် rows, ကိန်းပြည့် cols);
void time_random_matrix(ကိန်းပြည့် TA, ကိန်းပြည့် TB, ကိန်းပြည့် m, ကိန်းပြည့် k, ကိန်းပြည့် n);
void reorg_cpu(float *x, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် c, ကိန်းပြည့် batch, ကိန်းပြည့် stride, ကိန်းပြည့် forward, float *out);

void test_blas();

void inter_cpu(ကိန်းပြည့် NX, float *X, ကိန်းပြည့် NY, float *Y, ကိန်းပြည့် B, float *OUT);
void deinter_cpu(ကိန်းပြည့် NX, float *X, ကိန်းပြည့် NY, float *Y, ကိန်းပြည့် B, float *OUT);
void mult_add_into_cpu(ကိန်းပြည့် N, float *X, float *Y, float *Z);

void const_cpu(ကိန်းပြည့် N, float ALPHA, float *X, ကိန်းပြည့် INCX);
void constrain_gpu(ကိန်းပြည့် N, float ALPHA, float * X, ကိန်းပြည့် INCX);
void pow_cpu(ကိန်းပြည့် N, float ALPHA, float *X, ကိန်းပြည့် INCX, float *Y, ကိန်းပြည့် INCY);
void mul_cpu(ကိန်းပြည့် N, float *X, ကိန်းပြည့် INCX, float *Y, ကိန်းပြည့် INCY);

ကိန်းပြည့် test_gpu_blas();
void shortcut_cpu(ကိန်းပြည့် batch, ကိန်းပြည့် w1, ကိန်းပြည့် h1, ကိန်းပြည့် c1, float *add, ကိန်းပြည့် w2, ကိန်းပြည့် h2, ကိန်းပြည့် c2, float s1, float s2, float *out);

void mean_cpu(float *x, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial, float *mean);
void variance_cpu(float *x, float *mean, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial, float *variance);

void scale_bias(float *output, float *scales, ကိန်းပြည့် batch, ကိန်းပြည့် n, ကိန်းပြည့် size);
void backward_scale_cpu(float *x_norm, float *delta, ကိန်းပြည့် batch, ကိန်းပြည့် n, ကိန်းပြည့် size, float *scale_updates);
void mean_delta_cpu(float *delta, float *variance, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial, float *mean_delta);
void  variance_delta_cpu(float *x, float *delta, float *mean, float *variance, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial, float *variance_delta);
void normalize_delta_cpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial, float *delta);
void l2normalize_cpu(float *x, float *dx, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial);

void smooth_l1_cpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error);
void l2_cpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error);
void l1_cpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error);
void logistic_x_ent_cpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_cpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error);
void weighted_sum_cpu(float *a, float *b, float *s, ကိန်းပြည့် num, float *c);
void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, ကိန်းပြည့် n, float *dc);

void softmax(float *input, ကိန်းပြည့် n, float temp, ကိန်းပြည့် stride, float *output);
void softmax_cpu(float *input, ကိန်းပြည့် n, ကိန်းပြည့် batch, ကိန်းပြည့် batch_offset, ကိန်းပြည့် groups, ကိန်းပြည့် group_offset, ကိန်းပြည့် stride, float temp, float *output);
void upsample_cpu(float *in, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် c, ကိန်းပြည့် batch, ကိန်းပြည့် stride, ကိန်းပြည့် forward, float scale, float *out);

#ifdef GPU
#include "cuda.h"
#include "tree.h"

void axpy_gpu(ကိန်းပြည့် N, float ALPHA, float * X, ကိန်းပြည့် INCX, float * Y, ကိန်းပြည့် INCY);
void axpy_gpu_offset(ကိန်းပြည့် N, float ALPHA, float * X, ကိန်းပြည့် OFFX, ကိန်းပြည့် INCX, float * Y, ကိန်းပြည့် OFFY, ကိန်းပြည့် INCY);
void copy_gpu(ကိန်းပြည့် N, float * X, ကိန်းပြည့် INCX, float * Y, ကိန်းပြည့် INCY);
void copy_gpu_offset(ကိန်းပြည့် N, float * X, ကိန်းပြည့် OFFX, ကိန်းပြည့် INCX, float * Y, ကိန်းပြည့် OFFY, ကိန်းပြည့် INCY);
void add_gpu(ကိန်းပြည့် N, float ALPHA, float * X, ကိန်းပြည့် INCX);
void supp_gpu(ကိန်းပြည့် N, float ALPHA, float * X, ကိန်းပြည့် INCX);
void mask_gpu(ကိန်းပြည့် N, float * X, float mask_num, float * mask, float val);
void scale_mask_gpu(ကိန်းပြည့် N, float * X, float mask_num, float * mask, float scale);
void const_gpu(ကိန်းပြည့် N, float ALPHA, float *X, ကိန်းပြည့် INCX);
void pow_gpu(ကိန်းပြည့် N, float ALPHA, float *X, ကိန်းပြည့် INCX, float *Y, ကိန်းပြည့် INCY);
void mul_gpu(ကိန်းပြည့် N, float *X, ကိန်းပြည့် INCX, float *Y, ကိန်းပြည့် INCY);

void mean_gpu(float *x, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial, float *mean);
void variance_gpu(float *x, float *mean, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial, float *variance);
void normalize_gpu(float *x, float *mean, float *variance, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial);
void l2normalize_gpu(float *x, float *dx, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial);

void normalize_delta_gpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial, float *delta);

void fast_mean_delta_gpu(float *delta, float *variance, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial, float *mean_delta);
void fast_variance_delta_gpu(float *x, float *delta, float *mean, float *variance, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial, float *variance_delta);

void fast_variance_gpu(float *x, float *mean, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial, float *variance);
void fast_mean_gpu(float *x, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial, float *mean);
void shortcut_gpu(ကိန်းပြည့် batch, ကိန်းပြည့် w1, ကိန်းပြည့် h1, ကိန်းပြည့် c1, float *add, ကိန်းပြည့် w2, ကိန်းပြည့် h2, ကိန်းပြည့် c2, float s1, float s2, float *out);
void scale_bias_gpu(float *output, float *biases, ကိန်းပြည့် batch, ကိန်းပြည့် n, ကိန်းပြည့် size);
void backward_scale_gpu(float *x_norm, float *delta, ကိန်းပြည့် batch, ကိန်းပြည့် n, ကိန်းပြည့် size, float *scale_updates);
void scale_bias_gpu(float *output, float *biases, ကိန်းပြည့် batch, ကိန်းပြည့် n, ကိန်းပြည့် size);
void add_bias_gpu(float *output, float *biases, ကိန်းပြည့် batch, ကိန်းပြည့် n, ကိန်းပြည့် size);
void backward_bias_gpu(float *bias_updates, float *delta, ကိန်းပြည့် batch, ကိန်းပြည့် n, ကိန်းပြည့် size);

void logistic_x_ent_gpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_gpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error);
void smooth_l1_gpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error);
void l2_gpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error);
void l1_gpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error);
void wgan_gpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error);
void weighted_delta_gpu(float *a, float *b, float *s, float *da, float *db, float *ds, ကိန်းပြည့် num, float *dc);
void weighted_sum_gpu(float *a, float *b, float *s, ကိန်းပြည့် num, float *c);
void mult_add_into_gpu(ကိန်းပြည့် num, float *a, float *b, float *c);
void inter_gpu(ကိန်းပြည့် NX, float *X, ကိန်းပြည့် NY, float *Y, ကိန်းပြည့် B, float *OUT);
void deinter_gpu(ကိန်းပြည့် NX, float *X, ကိန်းပြည့် NY, float *Y, ကိန်းပြည့် B, float *OUT);

void reorg_gpu(float *x, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် c, ကိန်းပြည့် batch, ကိန်းပြည့် stride, ကိန်းပြည့် forward, float *out);

void softmax_gpu(float *input, ကိန်းပြည့် n, ကိန်းပြည့် batch, ကိန်းပြည့် batch_offset, ကိန်းပြည့် groups, ကိန်းပြည့် group_offset, ကိန်းပြည့် stride, float temp, float *output);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, ကိန်းပြည့် n, ကိန်းပြည့် batch, ကိန်းပြည့် t);
void adam_gpu(ကိန်းပြည့် n, float *x, float *m, float *v, float B1, float B2, float rate, float eps, ကိန်းပြည့် t);

void flatten_gpu(float *x, ကိန်းပြည့် spatial, ကိန်းပြည့် layers, ကိန်းပြည့် batch, ကိန်းပြည့် forward, float *out);
void softmax_tree(float *input, ကိန်းပြည့် spatial, ကိန်းပြည့် batch, ကိန်းပြည့် stride, float temp, float *output, tree hier);
void upsample_gpu(float *in, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် c, ကိန်းပြည့် batch, ကိန်းပြည့် stride, ကိန်းပြည့် forward, float scale, float *out);

#endif
#endif
