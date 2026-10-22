#ifndef BLAS_H
#define BLAS_H
#include "darknet.h"

void flatten(float *x, จำนวนเต็ม size, จำนวนเต็ม layers, จำนวนเต็ม batch, จำนวนเต็ม forward);
void pm(จำนวนเต็ม M, จำนวนเต็ม N, float *A);
float *random_matrix(จำนวนเต็ม rows, จำนวนเต็ม cols);
void time_random_matrix(จำนวนเต็ม TA, จำนวนเต็ม TB, จำนวนเต็ม m, จำนวนเต็ม k, จำนวนเต็ม n);
void reorg_cpu(float *x, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม c, จำนวนเต็ม batch, จำนวนเต็ม stride, จำนวนเต็ม forward, float *out);

void test_blas();

void inter_cpu(จำนวนเต็ม NX, float *X, จำนวนเต็ม NY, float *Y, จำนวนเต็ม B, float *OUT);
void deinter_cpu(จำนวนเต็ม NX, float *X, จำนวนเต็ม NY, float *Y, จำนวนเต็ม B, float *OUT);
void mult_add_into_cpu(จำนวนเต็ม N, float *X, float *Y, float *Z);

void const_cpu(จำนวนเต็ม N, float ALPHA, float *X, จำนวนเต็ม INCX);
void constrain_gpu(จำนวนเต็ม N, float ALPHA, float * X, จำนวนเต็ม INCX);
void pow_cpu(จำนวนเต็ม N, float ALPHA, float *X, จำนวนเต็ม INCX, float *Y, จำนวนเต็ม INCY);
void mul_cpu(จำนวนเต็ม N, float *X, จำนวนเต็ม INCX, float *Y, จำนวนเต็ม INCY);

จำนวนเต็ม test_gpu_blas();
void shortcut_cpu(จำนวนเต็ม batch, จำนวนเต็ม w1, จำนวนเต็ม h1, จำนวนเต็ม c1, float *add, จำนวนเต็ม w2, จำนวนเต็ม h2, จำนวนเต็ม c2, float s1, float s2, float *out);

void mean_cpu(float *x, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial, float *mean);
void variance_cpu(float *x, float *mean, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial, float *variance);

void scale_bias(float *output, float *scales, จำนวนเต็ม batch, จำนวนเต็ม n, จำนวนเต็ม size);
void backward_scale_cpu(float *x_norm, float *delta, จำนวนเต็ม batch, จำนวนเต็ม n, จำนวนเต็ม size, float *scale_updates);
void mean_delta_cpu(float *delta, float *variance, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial, float *mean_delta);
void  variance_delta_cpu(float *x, float *delta, float *mean, float *variance, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial, float *variance_delta);
void normalize_delta_cpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial, float *delta);
void l2normalize_cpu(float *x, float *dx, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial);

void smooth_l1_cpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error);
void l2_cpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error);
void l1_cpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error);
void logistic_x_ent_cpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_cpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error);
void weighted_sum_cpu(float *a, float *b, float *s, จำนวนเต็ม num, float *c);
void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, จำนวนเต็ม n, float *dc);

void softmax(float *input, จำนวนเต็ม n, float temp, จำนวนเต็ม stride, float *output);
void softmax_cpu(float *input, จำนวนเต็ม n, จำนวนเต็ม batch, จำนวนเต็ม batch_offset, จำนวนเต็ม groups, จำนวนเต็ม group_offset, จำนวนเต็ม stride, float temp, float *output);
void upsample_cpu(float *in, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม c, จำนวนเต็ม batch, จำนวนเต็ม stride, จำนวนเต็ม forward, float scale, float *out);

#ifdef GPU
#include "cuda.h"
#include "tree.h"

void axpy_gpu(จำนวนเต็ม N, float ALPHA, float * X, จำนวนเต็ม INCX, float * Y, จำนวนเต็ม INCY);
void axpy_gpu_offset(จำนวนเต็ม N, float ALPHA, float * X, จำนวนเต็ม OFFX, จำนวนเต็ม INCX, float * Y, จำนวนเต็ม OFFY, จำนวนเต็ม INCY);
void copy_gpu(จำนวนเต็ม N, float * X, จำนวนเต็ม INCX, float * Y, จำนวนเต็ม INCY);
void copy_gpu_offset(จำนวนเต็ม N, float * X, จำนวนเต็ม OFFX, จำนวนเต็ม INCX, float * Y, จำนวนเต็ม OFFY, จำนวนเต็ม INCY);
void add_gpu(จำนวนเต็ม N, float ALPHA, float * X, จำนวนเต็ม INCX);
void supp_gpu(จำนวนเต็ม N, float ALPHA, float * X, จำนวนเต็ม INCX);
void mask_gpu(จำนวนเต็ม N, float * X, float mask_num, float * mask, float val);
void scale_mask_gpu(จำนวนเต็ม N, float * X, float mask_num, float * mask, float scale);
void const_gpu(จำนวนเต็ม N, float ALPHA, float *X, จำนวนเต็ม INCX);
void pow_gpu(จำนวนเต็ม N, float ALPHA, float *X, จำนวนเต็ม INCX, float *Y, จำนวนเต็ม INCY);
void mul_gpu(จำนวนเต็ม N, float *X, จำนวนเต็ม INCX, float *Y, จำนวนเต็ม INCY);

void mean_gpu(float *x, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial, float *mean);
void variance_gpu(float *x, float *mean, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial, float *variance);
void normalize_gpu(float *x, float *mean, float *variance, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial);
void l2normalize_gpu(float *x, float *dx, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial);

void normalize_delta_gpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial, float *delta);

void fast_mean_delta_gpu(float *delta, float *variance, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial, float *mean_delta);
void fast_variance_delta_gpu(float *x, float *delta, float *mean, float *variance, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial, float *variance_delta);

void fast_variance_gpu(float *x, float *mean, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial, float *variance);
void fast_mean_gpu(float *x, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial, float *mean);
void shortcut_gpu(จำนวนเต็ม batch, จำนวนเต็ม w1, จำนวนเต็ม h1, จำนวนเต็ม c1, float *add, จำนวนเต็ม w2, จำนวนเต็ม h2, จำนวนเต็ม c2, float s1, float s2, float *out);
void scale_bias_gpu(float *output, float *biases, จำนวนเต็ม batch, จำนวนเต็ม n, จำนวนเต็ม size);
void backward_scale_gpu(float *x_norm, float *delta, จำนวนเต็ม batch, จำนวนเต็ม n, จำนวนเต็ม size, float *scale_updates);
void scale_bias_gpu(float *output, float *biases, จำนวนเต็ม batch, จำนวนเต็ม n, จำนวนเต็ม size);
void add_bias_gpu(float *output, float *biases, จำนวนเต็ม batch, จำนวนเต็ม n, จำนวนเต็ม size);
void backward_bias_gpu(float *bias_updates, float *delta, จำนวนเต็ม batch, จำนวนเต็ม n, จำนวนเต็ม size);

void logistic_x_ent_gpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_gpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error);
void smooth_l1_gpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error);
void l2_gpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error);
void l1_gpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error);
void wgan_gpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error);
void weighted_delta_gpu(float *a, float *b, float *s, float *da, float *db, float *ds, จำนวนเต็ม num, float *dc);
void weighted_sum_gpu(float *a, float *b, float *s, จำนวนเต็ม num, float *c);
void mult_add_into_gpu(จำนวนเต็ม num, float *a, float *b, float *c);
void inter_gpu(จำนวนเต็ม NX, float *X, จำนวนเต็ม NY, float *Y, จำนวนเต็ม B, float *OUT);
void deinter_gpu(จำนวนเต็ม NX, float *X, จำนวนเต็ม NY, float *Y, จำนวนเต็ม B, float *OUT);

void reorg_gpu(float *x, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม c, จำนวนเต็ม batch, จำนวนเต็ม stride, จำนวนเต็ม forward, float *out);

void softmax_gpu(float *input, จำนวนเต็ม n, จำนวนเต็ม batch, จำนวนเต็ม batch_offset, จำนวนเต็ม groups, จำนวนเต็ม group_offset, จำนวนเต็ม stride, float temp, float *output);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, จำนวนเต็ม n, จำนวนเต็ม batch, จำนวนเต็ม t);
void adam_gpu(จำนวนเต็ม n, float *x, float *m, float *v, float B1, float B2, float rate, float eps, จำนวนเต็ม t);

void flatten_gpu(float *x, จำนวนเต็ม spatial, จำนวนเต็ม layers, จำนวนเต็ม batch, จำนวนเต็ม forward, float *out);
void softmax_tree(float *input, จำนวนเต็ม spatial, จำนวนเต็ม batch, จำนวนเต็ม stride, float temp, float *output, tree hier);
void upsample_gpu(float *in, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม c, จำนวนเต็ม batch, จำนวนเต็ม stride, จำนวนเต็ม forward, float scale, float *out);

#endif
#endif
