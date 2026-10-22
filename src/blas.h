#ifndef BLAS_H
#define BLAS_H
#include "darknet.h"

void flatten(float *x, puur_naa_nka size, puur_naa_nka layers, puur_naa_nka batch, puur_naa_nka forward);
void pm(puur_naa_nka M, puur_naa_nka N, float *A);
float *random_matrix(puur_naa_nka rows, puur_naa_nka cols);
void time_random_matrix(puur_naa_nka TA, puur_naa_nka TB, puur_naa_nka m, puur_naa_nka k, puur_naa_nka n);
void reorg_cpu(float *x, puur_naa_nka w, puur_naa_nka h, puur_naa_nka c, puur_naa_nka batch, puur_naa_nka stride, puur_naa_nka forward, float *out);

void test_blas();

void inter_cpu(puur_naa_nka NX, float *X, puur_naa_nka NY, float *Y, puur_naa_nka B, float *OUT);
void deinter_cpu(puur_naa_nka NX, float *X, puur_naa_nka NY, float *Y, puur_naa_nka B, float *OUT);
void mult_add_into_cpu(puur_naa_nka N, float *X, float *Y, float *Z);

void const_cpu(puur_naa_nka N, float ALPHA, float *X, puur_naa_nka INCX);
void constrain_gpu(puur_naa_nka N, float ALPHA, float * X, puur_naa_nka INCX);
void pow_cpu(puur_naa_nka N, float ALPHA, float *X, puur_naa_nka INCX, float *Y, puur_naa_nka INCY);
void mul_cpu(puur_naa_nka N, float *X, puur_naa_nka INCX, float *Y, puur_naa_nka INCY);

puur_naa_nka test_gpu_blas();
void shortcut_cpu(puur_naa_nka batch, puur_naa_nka w1, puur_naa_nka h1, puur_naa_nka c1, float *add, puur_naa_nka w2, puur_naa_nka h2, puur_naa_nka c2, float s1, float s2, float *out);

void mean_cpu(float *x, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial, float *mean);
void variance_cpu(float *x, float *mean, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial, float *variance);

void scale_bias(float *output, float *scales, puur_naa_nka batch, puur_naa_nka n, puur_naa_nka size);
void backward_scale_cpu(float *x_norm, float *delta, puur_naa_nka batch, puur_naa_nka n, puur_naa_nka size, float *scale_updates);
void mean_delta_cpu(float *delta, float *variance, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial, float *mean_delta);
void  variance_delta_cpu(float *x, float *delta, float *mean, float *variance, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial, float *variance_delta);
void normalize_delta_cpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial, float *delta);
void l2normalize_cpu(float *x, float *dx, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial);

void smooth_l1_cpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error);
void l2_cpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error);
void l1_cpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error);
void logistic_x_ent_cpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_cpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error);
void weighted_sum_cpu(float *a, float *b, float *s, puur_naa_nka num, float *c);
void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, puur_naa_nka n, float *dc);

void softmax(float *input, puur_naa_nka n, float temp, puur_naa_nka stride, float *output);
void softmax_cpu(float *input, puur_naa_nka n, puur_naa_nka batch, puur_naa_nka batch_offset, puur_naa_nka groups, puur_naa_nka group_offset, puur_naa_nka stride, float temp, float *output);
void upsample_cpu(float *in, puur_naa_nka w, puur_naa_nka h, puur_naa_nka c, puur_naa_nka batch, puur_naa_nka stride, puur_naa_nka forward, float scale, float *out);

#ifdef GPU
#include "cuda.h"
#include "tree.h"

void axpy_gpu(puur_naa_nka N, float ALPHA, float * X, puur_naa_nka INCX, float * Y, puur_naa_nka INCY);
void axpy_gpu_offset(puur_naa_nka N, float ALPHA, float * X, puur_naa_nka OFFX, puur_naa_nka INCX, float * Y, puur_naa_nka OFFY, puur_naa_nka INCY);
void copy_gpu(puur_naa_nka N, float * X, puur_naa_nka INCX, float * Y, puur_naa_nka INCY);
void copy_gpu_offset(puur_naa_nka N, float * X, puur_naa_nka OFFX, puur_naa_nka INCX, float * Y, puur_naa_nka OFFY, puur_naa_nka INCY);
void add_gpu(puur_naa_nka N, float ALPHA, float * X, puur_naa_nka INCX);
void supp_gpu(puur_naa_nka N, float ALPHA, float * X, puur_naa_nka INCX);
void mask_gpu(puur_naa_nka N, float * X, float mask_num, float * mask, float val);
void scale_mask_gpu(puur_naa_nka N, float * X, float mask_num, float * mask, float scale);
void const_gpu(puur_naa_nka N, float ALPHA, float *X, puur_naa_nka INCX);
void pow_gpu(puur_naa_nka N, float ALPHA, float *X, puur_naa_nka INCX, float *Y, puur_naa_nka INCY);
void mul_gpu(puur_naa_nka N, float *X, puur_naa_nka INCX, float *Y, puur_naa_nka INCY);

void mean_gpu(float *x, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial, float *mean);
void variance_gpu(float *x, float *mean, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial, float *variance);
void normalize_gpu(float *x, float *mean, float *variance, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial);
void l2normalize_gpu(float *x, float *dx, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial);

void normalize_delta_gpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial, float *delta);

void fast_mean_delta_gpu(float *delta, float *variance, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial, float *mean_delta);
void fast_variance_delta_gpu(float *x, float *delta, float *mean, float *variance, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial, float *variance_delta);

void fast_variance_gpu(float *x, float *mean, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial, float *variance);
void fast_mean_gpu(float *x, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial, float *mean);
void shortcut_gpu(puur_naa_nka batch, puur_naa_nka w1, puur_naa_nka h1, puur_naa_nka c1, float *add, puur_naa_nka w2, puur_naa_nka h2, puur_naa_nka c2, float s1, float s2, float *out);
void scale_bias_gpu(float *output, float *biases, puur_naa_nka batch, puur_naa_nka n, puur_naa_nka size);
void backward_scale_gpu(float *x_norm, float *delta, puur_naa_nka batch, puur_naa_nka n, puur_naa_nka size, float *scale_updates);
void scale_bias_gpu(float *output, float *biases, puur_naa_nka batch, puur_naa_nka n, puur_naa_nka size);
void add_bias_gpu(float *output, float *biases, puur_naa_nka batch, puur_naa_nka n, puur_naa_nka size);
void backward_bias_gpu(float *bias_updates, float *delta, puur_naa_nka batch, puur_naa_nka n, puur_naa_nka size);

void logistic_x_ent_gpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_gpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error);
void smooth_l1_gpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error);
void l2_gpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error);
void l1_gpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error);
void wgan_gpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error);
void weighted_delta_gpu(float *a, float *b, float *s, float *da, float *db, float *ds, puur_naa_nka num, float *dc);
void weighted_sum_gpu(float *a, float *b, float *s, puur_naa_nka num, float *c);
void mult_add_into_gpu(puur_naa_nka num, float *a, float *b, float *c);
void inter_gpu(puur_naa_nka NX, float *X, puur_naa_nka NY, float *Y, puur_naa_nka B, float *OUT);
void deinter_gpu(puur_naa_nka NX, float *X, puur_naa_nka NY, float *Y, puur_naa_nka B, float *OUT);

void reorg_gpu(float *x, puur_naa_nka w, puur_naa_nka h, puur_naa_nka c, puur_naa_nka batch, puur_naa_nka stride, puur_naa_nka forward, float *out);

void softmax_gpu(float *input, puur_naa_nka n, puur_naa_nka batch, puur_naa_nka batch_offset, puur_naa_nka groups, puur_naa_nka group_offset, puur_naa_nka stride, float temp, float *output);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, puur_naa_nka n, puur_naa_nka batch, puur_naa_nka t);
void adam_gpu(puur_naa_nka n, float *x, float *m, float *v, float B1, float B2, float rate, float eps, puur_naa_nka t);

void flatten_gpu(float *x, puur_naa_nka spatial, puur_naa_nka layers, puur_naa_nka batch, puur_naa_nka forward, float *out);
void softmax_tree(float *input, puur_naa_nka spatial, puur_naa_nka batch, puur_naa_nka stride, float temp, float *output, tree hier);
void upsample_gpu(float *in, puur_naa_nka w, puur_naa_nka h, puur_naa_nka c, puur_naa_nka batch, puur_naa_nka stride, puur_naa_nka forward, float scale, float *out);

#endif
#endif
