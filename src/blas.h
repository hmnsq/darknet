#ifndef BLAS_H
#define BLAS_H
#include "darknet.h"

void flatten(float *x, ចំនួនគត់ size, ចំនួនគត់ layers, ចំនួនគត់ batch, ចំនួនគត់ forward);
void pm(ចំនួនគត់ M, ចំនួនគត់ N, float *A);
float *random_matrix(ចំនួនគត់ rows, ចំនួនគត់ cols);
void time_random_matrix(ចំនួនគត់ TA, ចំនួនគត់ TB, ចំនួនគត់ m, ចំនួនគត់ k, ចំនួនគត់ n);
void reorg_cpu(float *x, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ c, ចំនួនគត់ batch, ចំនួនគត់ stride, ចំនួនគត់ forward, float *out);

void test_blas();

void inter_cpu(ចំនួនគត់ NX, float *X, ចំនួនគត់ NY, float *Y, ចំនួនគត់ B, float *OUT);
void deinter_cpu(ចំនួនគត់ NX, float *X, ចំនួនគត់ NY, float *Y, ចំនួនគត់ B, float *OUT);
void mult_add_into_cpu(ចំនួនគត់ N, float *X, float *Y, float *Z);

void const_cpu(ចំនួនគត់ N, float ALPHA, float *X, ចំនួនគត់ INCX);
void constrain_gpu(ចំនួនគត់ N, float ALPHA, float * X, ចំនួនគត់ INCX);
void pow_cpu(ចំនួនគត់ N, float ALPHA, float *X, ចំនួនគត់ INCX, float *Y, ចំនួនគត់ INCY);
void mul_cpu(ចំនួនគត់ N, float *X, ចំនួនគត់ INCX, float *Y, ចំនួនគត់ INCY);

ចំនួនគត់ test_gpu_blas();
void shortcut_cpu(ចំនួនគត់ batch, ចំនួនគត់ w1, ចំនួនគត់ h1, ចំនួនគត់ c1, float *add, ចំនួនគត់ w2, ចំនួនគត់ h2, ចំនួនគត់ c2, float s1, float s2, float *out);

void mean_cpu(float *x, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial, float *mean);
void variance_cpu(float *x, float *mean, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial, float *variance);

void scale_bias(float *output, float *scales, ចំនួនគត់ batch, ចំនួនគត់ n, ចំនួនគត់ size);
void backward_scale_cpu(float *x_norm, float *delta, ចំនួនគត់ batch, ចំនួនគត់ n, ចំនួនគត់ size, float *scale_updates);
void mean_delta_cpu(float *delta, float *variance, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial, float *mean_delta);
void  variance_delta_cpu(float *x, float *delta, float *mean, float *variance, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial, float *variance_delta);
void normalize_delta_cpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial, float *delta);
void l2normalize_cpu(float *x, float *dx, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial);

void smooth_l1_cpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error);
void l2_cpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error);
void l1_cpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error);
void logistic_x_ent_cpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_cpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error);
void weighted_sum_cpu(float *a, float *b, float *s, ចំនួនគត់ num, float *c);
void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, ចំនួនគត់ n, float *dc);

void softmax(float *input, ចំនួនគត់ n, float temp, ចំនួនគត់ stride, float *output);
void softmax_cpu(float *input, ចំនួនគត់ n, ចំនួនគត់ batch, ចំនួនគត់ batch_offset, ចំនួនគត់ groups, ចំនួនគត់ group_offset, ចំនួនគត់ stride, float temp, float *output);
void upsample_cpu(float *in, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ c, ចំនួនគត់ batch, ចំនួនគត់ stride, ចំនួនគត់ forward, float scale, float *out);

#ifdef GPU
#include "cuda.h"
#include "tree.h"

void axpy_gpu(ចំនួនគត់ N, float ALPHA, float * X, ចំនួនគត់ INCX, float * Y, ចំនួនគត់ INCY);
void axpy_gpu_offset(ចំនួនគត់ N, float ALPHA, float * X, ចំនួនគត់ OFFX, ចំនួនគត់ INCX, float * Y, ចំនួនគត់ OFFY, ចំនួនគត់ INCY);
void copy_gpu(ចំនួនគត់ N, float * X, ចំនួនគត់ INCX, float * Y, ចំនួនគត់ INCY);
void copy_gpu_offset(ចំនួនគត់ N, float * X, ចំនួនគត់ OFFX, ចំនួនគត់ INCX, float * Y, ចំនួនគត់ OFFY, ចំនួនគត់ INCY);
void add_gpu(ចំនួនគត់ N, float ALPHA, float * X, ចំនួនគត់ INCX);
void supp_gpu(ចំនួនគត់ N, float ALPHA, float * X, ចំនួនគត់ INCX);
void mask_gpu(ចំនួនគត់ N, float * X, float mask_num, float * mask, float val);
void scale_mask_gpu(ចំនួនគត់ N, float * X, float mask_num, float * mask, float scale);
void const_gpu(ចំនួនគត់ N, float ALPHA, float *X, ចំនួនគត់ INCX);
void pow_gpu(ចំនួនគត់ N, float ALPHA, float *X, ចំនួនគត់ INCX, float *Y, ចំនួនគត់ INCY);
void mul_gpu(ចំនួនគត់ N, float *X, ចំនួនគត់ INCX, float *Y, ចំនួនគត់ INCY);

void mean_gpu(float *x, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial, float *mean);
void variance_gpu(float *x, float *mean, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial, float *variance);
void normalize_gpu(float *x, float *mean, float *variance, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial);
void l2normalize_gpu(float *x, float *dx, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial);

void normalize_delta_gpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial, float *delta);

void fast_mean_delta_gpu(float *delta, float *variance, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial, float *mean_delta);
void fast_variance_delta_gpu(float *x, float *delta, float *mean, float *variance, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial, float *variance_delta);

void fast_variance_gpu(float *x, float *mean, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial, float *variance);
void fast_mean_gpu(float *x, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial, float *mean);
void shortcut_gpu(ចំនួនគត់ batch, ចំនួនគត់ w1, ចំនួនគត់ h1, ចំនួនគត់ c1, float *add, ចំនួនគត់ w2, ចំនួនគត់ h2, ចំនួនគត់ c2, float s1, float s2, float *out);
void scale_bias_gpu(float *output, float *biases, ចំនួនគត់ batch, ចំនួនគត់ n, ចំនួនគត់ size);
void backward_scale_gpu(float *x_norm, float *delta, ចំនួនគត់ batch, ចំនួនគត់ n, ចំនួនគត់ size, float *scale_updates);
void scale_bias_gpu(float *output, float *biases, ចំនួនគត់ batch, ចំនួនគត់ n, ចំនួនគត់ size);
void add_bias_gpu(float *output, float *biases, ចំនួនគត់ batch, ចំនួនគត់ n, ចំនួនគត់ size);
void backward_bias_gpu(float *bias_updates, float *delta, ចំនួនគត់ batch, ចំនួនគត់ n, ចំនួនគត់ size);

void logistic_x_ent_gpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_gpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error);
void smooth_l1_gpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error);
void l2_gpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error);
void l1_gpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error);
void wgan_gpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error);
void weighted_delta_gpu(float *a, float *b, float *s, float *da, float *db, float *ds, ចំនួនគត់ num, float *dc);
void weighted_sum_gpu(float *a, float *b, float *s, ចំនួនគត់ num, float *c);
void mult_add_into_gpu(ចំនួនគត់ num, float *a, float *b, float *c);
void inter_gpu(ចំនួនគត់ NX, float *X, ចំនួនគត់ NY, float *Y, ចំនួនគត់ B, float *OUT);
void deinter_gpu(ចំនួនគត់ NX, float *X, ចំនួនគត់ NY, float *Y, ចំនួនគត់ B, float *OUT);

void reorg_gpu(float *x, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ c, ចំនួនគត់ batch, ចំនួនគត់ stride, ចំនួនគត់ forward, float *out);

void softmax_gpu(float *input, ចំនួនគត់ n, ចំនួនគត់ batch, ចំនួនគត់ batch_offset, ចំនួនគត់ groups, ចំនួនគត់ group_offset, ចំនួនគត់ stride, float temp, float *output);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, ចំនួនគត់ n, ចំនួនគត់ batch, ចំនួនគត់ t);
void adam_gpu(ចំនួនគត់ n, float *x, float *m, float *v, float B1, float B2, float rate, float eps, ចំនួនគត់ t);

void flatten_gpu(float *x, ចំនួនគត់ spatial, ចំនួនគត់ layers, ចំនួនគត់ batch, ចំនួនគត់ forward, float *out);
void softmax_tree(float *input, ចំនួនគត់ spatial, ចំនួនគត់ batch, ចំនួនគត់ stride, float temp, float *output, tree hier);
void upsample_gpu(float *in, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ c, ចំនួនគត់ batch, ចំនួនគត់ stride, ចំនួនគត់ forward, float scale, float *out);

#endif
#endif
