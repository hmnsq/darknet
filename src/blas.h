#ifndef BLAS_H
#define BLAS_H
#include "darknet.h"

void flatten(float *x, පූර්ණ_සංඛ්‍යා size, පූර්ණ_සංඛ්‍යා layers, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා forward);
void pm(පූර්ණ_සංඛ්‍යා M, පූර්ණ_සංඛ්‍යා N, float *A);
float *random_matrix(පූර්ණ_සංඛ්‍යා rows, පූර්ණ_සංඛ්‍යා cols);
void time_random_matrix(පූර්ණ_සංඛ්‍යා TA, පූර්ණ_සංඛ්‍යා TB, පූර්ණ_සංඛ්‍යා m, පූර්ණ_සංඛ්‍යා k, පූර්ණ_සංඛ්‍යා n);
void reorg_cpu(float *x, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා stride, පූර්ණ_සංඛ්‍යා forward, float *out);

void test_blas();

void inter_cpu(පූර්ණ_සංඛ්‍යා NX, float *X, පූර්ණ_සංඛ්‍යා NY, float *Y, පූර්ණ_සංඛ්‍යා B, float *OUT);
void deinter_cpu(පූර්ණ_සංඛ්‍යා NX, float *X, පූර්ණ_සංඛ්‍යා NY, float *Y, පූර්ණ_සංඛ්‍යා B, float *OUT);
void mult_add_into_cpu(පූර්ණ_සංඛ්‍යා N, float *X, float *Y, float *Z);

void const_cpu(පූර්ණ_සංඛ්‍යා N, float ALPHA, float *X, පූර්ණ_සංඛ්‍යා INCX);
void constrain_gpu(පූර්ණ_සංඛ්‍යා N, float ALPHA, float * X, පූර්ණ_සංඛ්‍යා INCX);
void pow_cpu(පූර්ණ_සංඛ්‍යා N, float ALPHA, float *X, පූර්ණ_සංඛ්‍යා INCX, float *Y, පූර්ණ_සංඛ්‍යා INCY);
void mul_cpu(පූර්ණ_සංඛ්‍යා N, float *X, පූර්ණ_සංඛ්‍යා INCX, float *Y, පූර්ණ_සංඛ්‍යා INCY);

පූර්ණ_සංඛ්‍යා test_gpu_blas();
void shortcut_cpu(පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා w1, පූර්ණ_සංඛ්‍යා h1, පූර්ණ_සංඛ්‍යා c1, float *add, පූර්ණ_සංඛ්‍යා w2, පූර්ණ_සංඛ්‍යා h2, පූර්ණ_සංඛ්‍යා c2, float s1, float s2, float *out);

void mean_cpu(float *x, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial, float *mean);
void variance_cpu(float *x, float *mean, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial, float *variance);

void scale_bias(float *output, float *scales, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා size);
void backward_scale_cpu(float *x_norm, float *delta, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා size, float *scale_updates);
void mean_delta_cpu(float *delta, float *variance, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial, float *mean_delta);
void  variance_delta_cpu(float *x, float *delta, float *mean, float *variance, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial, float *variance_delta);
void normalize_delta_cpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial, float *delta);
void l2normalize_cpu(float *x, float *dx, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial);

void smooth_l1_cpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error);
void l2_cpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error);
void l1_cpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error);
void logistic_x_ent_cpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_cpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error);
void weighted_sum_cpu(float *a, float *b, float *s, පූර්ණ_සංඛ්‍යා num, float *c);
void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, පූර්ණ_සංඛ්‍යා n, float *dc);

void softmax(float *input, පූර්ණ_සංඛ්‍යා n, float temp, පූර්ණ_සංඛ්‍යා stride, float *output);
void softmax_cpu(float *input, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා batch_offset, පූර්ණ_සංඛ්‍යා groups, පූර්ණ_සංඛ්‍යා group_offset, පූර්ණ_සංඛ්‍යා stride, float temp, float *output);
void upsample_cpu(float *in, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා stride, පූර්ණ_සංඛ්‍යා forward, float scale, float *out);

#ifdef GPU
#include "cuda.h"
#include "tree.h"

void axpy_gpu(පූර්ණ_සංඛ්‍යා N, float ALPHA, float * X, පූර්ණ_සංඛ්‍යා INCX, float * Y, පූර්ණ_සංඛ්‍යා INCY);
void axpy_gpu_offset(පූර්ණ_සංඛ්‍යා N, float ALPHA, float * X, පූර්ණ_සංඛ්‍යා OFFX, පූර්ණ_සංඛ්‍යා INCX, float * Y, පූර්ණ_සංඛ්‍යා OFFY, පූර්ණ_සංඛ්‍යා INCY);
void copy_gpu(පූර්ණ_සංඛ්‍යා N, float * X, පූර්ණ_සංඛ්‍යා INCX, float * Y, පූර්ණ_සංඛ්‍යා INCY);
void copy_gpu_offset(පූර්ණ_සංඛ්‍යා N, float * X, පූර්ණ_සංඛ්‍යා OFFX, පූර්ණ_සංඛ්‍යා INCX, float * Y, පූර්ණ_සංඛ්‍යා OFFY, පූර්ණ_සංඛ්‍යා INCY);
void add_gpu(පූර්ණ_සංඛ්‍යා N, float ALPHA, float * X, පූර්ණ_සංඛ්‍යා INCX);
void supp_gpu(පූර්ණ_සංඛ්‍යා N, float ALPHA, float * X, පූර්ණ_සංඛ්‍යා INCX);
void mask_gpu(පූර්ණ_සංඛ්‍යා N, float * X, float mask_num, float * mask, float val);
void scale_mask_gpu(පූර්ණ_සංඛ්‍යා N, float * X, float mask_num, float * mask, float scale);
void const_gpu(පූර්ණ_සංඛ්‍යා N, float ALPHA, float *X, පූර්ණ_සංඛ්‍යා INCX);
void pow_gpu(පූර්ණ_සංඛ්‍යා N, float ALPHA, float *X, පූර්ණ_සංඛ්‍යා INCX, float *Y, පූර්ණ_සංඛ්‍යා INCY);
void mul_gpu(පූර්ණ_සංඛ්‍යා N, float *X, පූර්ණ_සංඛ්‍යා INCX, float *Y, පූර්ණ_සංඛ්‍යා INCY);

void mean_gpu(float *x, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial, float *mean);
void variance_gpu(float *x, float *mean, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial, float *variance);
void normalize_gpu(float *x, float *mean, float *variance, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial);
void l2normalize_gpu(float *x, float *dx, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial);

void normalize_delta_gpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial, float *delta);

void fast_mean_delta_gpu(float *delta, float *variance, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial, float *mean_delta);
void fast_variance_delta_gpu(float *x, float *delta, float *mean, float *variance, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial, float *variance_delta);

void fast_variance_gpu(float *x, float *mean, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial, float *variance);
void fast_mean_gpu(float *x, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial, float *mean);
void shortcut_gpu(පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා w1, පූර්ණ_සංඛ්‍යා h1, පූර්ණ_සංඛ්‍යා c1, float *add, පූර්ණ_සංඛ්‍යා w2, පූර්ණ_සංඛ්‍යා h2, පූර්ණ_සංඛ්‍යා c2, float s1, float s2, float *out);
void scale_bias_gpu(float *output, float *biases, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා size);
void backward_scale_gpu(float *x_norm, float *delta, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා size, float *scale_updates);
void scale_bias_gpu(float *output, float *biases, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා size);
void add_bias_gpu(float *output, float *biases, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා size);
void backward_bias_gpu(float *bias_updates, float *delta, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා size);

void logistic_x_ent_gpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_gpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error);
void smooth_l1_gpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error);
void l2_gpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error);
void l1_gpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error);
void wgan_gpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error);
void weighted_delta_gpu(float *a, float *b, float *s, float *da, float *db, float *ds, පූර්ණ_සංඛ්‍යා num, float *dc);
void weighted_sum_gpu(float *a, float *b, float *s, පූර්ණ_සංඛ්‍යා num, float *c);
void mult_add_into_gpu(පූර්ණ_සංඛ්‍යා num, float *a, float *b, float *c);
void inter_gpu(පූර්ණ_සංඛ්‍යා NX, float *X, පූර්ණ_සංඛ්‍යා NY, float *Y, පූර්ණ_සංඛ්‍යා B, float *OUT);
void deinter_gpu(පූර්ණ_සංඛ්‍යා NX, float *X, පූර්ණ_සංඛ්‍යා NY, float *Y, පූර්ණ_සංඛ්‍යා B, float *OUT);

void reorg_gpu(float *x, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා stride, පූර්ණ_සංඛ්‍යා forward, float *out);

void softmax_gpu(float *input, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා batch_offset, පූර්ණ_සංඛ්‍යා groups, පූර්ණ_සංඛ්‍යා group_offset, පූර්ණ_සංඛ්‍යා stride, float temp, float *output);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා t);
void adam_gpu(පූර්ණ_සංඛ්‍යා n, float *x, float *m, float *v, float B1, float B2, float rate, float eps, පූර්ණ_සංඛ්‍යා t);

void flatten_gpu(float *x, පූර්ණ_සංඛ්‍යා spatial, පූර්ණ_සංඛ්‍යා layers, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා forward, float *out);
void softmax_tree(float *input, පූර්ණ_සංඛ්‍යා spatial, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා stride, float temp, float *output, tree hier);
void upsample_gpu(float *in, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා stride, පූර්ණ_සංඛ්‍යා forward, float scale, float *out);

#endif
#endif
