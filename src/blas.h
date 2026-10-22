#ifndef BLAS_H
#define BLAS_H
#include "darknet.h"

void flatten(float *x, ཧྲིལ size, ཧྲིལ layers, ཧྲིལ batch, ཧྲིལ forward);
void pm(ཧྲིལ M, ཧྲིལ N, float *A);
float *random_matrix(ཧྲིལ rows, ཧྲིལ cols);
void time_random_matrix(ཧྲིལ TA, ཧྲིལ TB, ཧྲིལ m, ཧྲིལ k, ཧྲིལ n);
void reorg_cpu(float *x, ཧྲིལ w, ཧྲིལ h, ཧྲིལ c, ཧྲིལ batch, ཧྲིལ stride, ཧྲིལ forward, float *out);

void test_blas();

void inter_cpu(ཧྲིལ NX, float *X, ཧྲིལ NY, float *Y, ཧྲིལ B, float *OUT);
void deinter_cpu(ཧྲིལ NX, float *X, ཧྲིལ NY, float *Y, ཧྲིལ B, float *OUT);
void mult_add_into_cpu(ཧྲིལ N, float *X, float *Y, float *Z);

void const_cpu(ཧྲིལ N, float ALPHA, float *X, ཧྲིལ INCX);
void constrain_gpu(ཧྲིལ N, float ALPHA, float * X, ཧྲིལ INCX);
void pow_cpu(ཧྲིལ N, float ALPHA, float *X, ཧྲིལ INCX, float *Y, ཧྲིལ INCY);
void mul_cpu(ཧྲིལ N, float *X, ཧྲིལ INCX, float *Y, ཧྲིལ INCY);

ཧྲིལ test_gpu_blas();
void shortcut_cpu(ཧྲིལ batch, ཧྲིལ w1, ཧྲིལ h1, ཧྲིལ c1, float *add, ཧྲིལ w2, ཧྲིལ h2, ཧྲིལ c2, float s1, float s2, float *out);

void mean_cpu(float *x, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial, float *mean);
void variance_cpu(float *x, float *mean, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial, float *variance);

void scale_bias(float *output, float *scales, ཧྲིལ batch, ཧྲིལ n, ཧྲིལ size);
void backward_scale_cpu(float *x_norm, float *delta, ཧྲིལ batch, ཧྲིལ n, ཧྲིལ size, float *scale_updates);
void mean_delta_cpu(float *delta, float *variance, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial, float *mean_delta);
void  variance_delta_cpu(float *x, float *delta, float *mean, float *variance, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial, float *variance_delta);
void normalize_delta_cpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial, float *delta);
void l2normalize_cpu(float *x, float *dx, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial);

void smooth_l1_cpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error);
void l2_cpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error);
void l1_cpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error);
void logistic_x_ent_cpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_cpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error);
void weighted_sum_cpu(float *a, float *b, float *s, ཧྲིལ num, float *c);
void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, ཧྲིལ n, float *dc);

void softmax(float *input, ཧྲིལ n, float temp, ཧྲིལ stride, float *output);
void softmax_cpu(float *input, ཧྲིལ n, ཧྲིལ batch, ཧྲིལ batch_offset, ཧྲིལ groups, ཧྲིལ group_offset, ཧྲིལ stride, float temp, float *output);
void upsample_cpu(float *in, ཧྲིལ w, ཧྲིལ h, ཧྲིལ c, ཧྲིལ batch, ཧྲིལ stride, ཧྲིལ forward, float scale, float *out);

#ifdef GPU
#include "cuda.h"
#include "tree.h"

void axpy_gpu(ཧྲིལ N, float ALPHA, float * X, ཧྲིལ INCX, float * Y, ཧྲིལ INCY);
void axpy_gpu_offset(ཧྲིལ N, float ALPHA, float * X, ཧྲིལ OFFX, ཧྲིལ INCX, float * Y, ཧྲིལ OFFY, ཧྲིལ INCY);
void copy_gpu(ཧྲིལ N, float * X, ཧྲིལ INCX, float * Y, ཧྲིལ INCY);
void copy_gpu_offset(ཧྲིལ N, float * X, ཧྲིལ OFFX, ཧྲིལ INCX, float * Y, ཧྲིལ OFFY, ཧྲིལ INCY);
void add_gpu(ཧྲིལ N, float ALPHA, float * X, ཧྲིལ INCX);
void supp_gpu(ཧྲིལ N, float ALPHA, float * X, ཧྲིལ INCX);
void mask_gpu(ཧྲིལ N, float * X, float mask_num, float * mask, float val);
void scale_mask_gpu(ཧྲིལ N, float * X, float mask_num, float * mask, float scale);
void const_gpu(ཧྲིལ N, float ALPHA, float *X, ཧྲིལ INCX);
void pow_gpu(ཧྲིལ N, float ALPHA, float *X, ཧྲིལ INCX, float *Y, ཧྲིལ INCY);
void mul_gpu(ཧྲིལ N, float *X, ཧྲིལ INCX, float *Y, ཧྲིལ INCY);

void mean_gpu(float *x, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial, float *mean);
void variance_gpu(float *x, float *mean, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial, float *variance);
void normalize_gpu(float *x, float *mean, float *variance, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial);
void l2normalize_gpu(float *x, float *dx, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial);

void normalize_delta_gpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial, float *delta);

void fast_mean_delta_gpu(float *delta, float *variance, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial, float *mean_delta);
void fast_variance_delta_gpu(float *x, float *delta, float *mean, float *variance, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial, float *variance_delta);

void fast_variance_gpu(float *x, float *mean, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial, float *variance);
void fast_mean_gpu(float *x, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial, float *mean);
void shortcut_gpu(ཧྲིལ batch, ཧྲིལ w1, ཧྲིལ h1, ཧྲིལ c1, float *add, ཧྲིལ w2, ཧྲིལ h2, ཧྲིལ c2, float s1, float s2, float *out);
void scale_bias_gpu(float *output, float *biases, ཧྲིལ batch, ཧྲིལ n, ཧྲིལ size);
void backward_scale_gpu(float *x_norm, float *delta, ཧྲིལ batch, ཧྲིལ n, ཧྲིལ size, float *scale_updates);
void scale_bias_gpu(float *output, float *biases, ཧྲིལ batch, ཧྲིལ n, ཧྲིལ size);
void add_bias_gpu(float *output, float *biases, ཧྲིལ batch, ཧྲིལ n, ཧྲིལ size);
void backward_bias_gpu(float *bias_updates, float *delta, ཧྲིལ batch, ཧྲིལ n, ཧྲིལ size);

void logistic_x_ent_gpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_gpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error);
void smooth_l1_gpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error);
void l2_gpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error);
void l1_gpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error);
void wgan_gpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error);
void weighted_delta_gpu(float *a, float *b, float *s, float *da, float *db, float *ds, ཧྲིལ num, float *dc);
void weighted_sum_gpu(float *a, float *b, float *s, ཧྲིལ num, float *c);
void mult_add_into_gpu(ཧྲིལ num, float *a, float *b, float *c);
void inter_gpu(ཧྲིལ NX, float *X, ཧྲིལ NY, float *Y, ཧྲིལ B, float *OUT);
void deinter_gpu(ཧྲིལ NX, float *X, ཧྲིལ NY, float *Y, ཧྲིལ B, float *OUT);

void reorg_gpu(float *x, ཧྲིལ w, ཧྲིལ h, ཧྲིལ c, ཧྲིལ batch, ཧྲིལ stride, ཧྲིལ forward, float *out);

void softmax_gpu(float *input, ཧྲིལ n, ཧྲིལ batch, ཧྲིལ batch_offset, ཧྲིལ groups, ཧྲིལ group_offset, ཧྲིལ stride, float temp, float *output);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, ཧྲིལ n, ཧྲིལ batch, ཧྲིལ t);
void adam_gpu(ཧྲིལ n, float *x, float *m, float *v, float B1, float B2, float rate, float eps, ཧྲིལ t);

void flatten_gpu(float *x, ཧྲིལ spatial, ཧྲིལ layers, ཧྲིལ batch, ཧྲིལ forward, float *out);
void softmax_tree(float *input, ཧྲིལ spatial, ཧྲིལ batch, ཧྲིལ stride, float temp, float *output, tree hier);
void upsample_gpu(float *in, ཧྲིལ w, ཧྲིལ h, ཧྲིལ c, ཧྲིལ batch, ཧྲིལ stride, ཧྲིལ forward, float scale, float *out);

#endif
#endif
