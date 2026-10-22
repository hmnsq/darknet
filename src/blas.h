#ifndef BLAS_H
#define BLAS_H
#include "darknet.h"

void flatten(float *x, целое size, целое layers, целое batch, целое forward);
void pm(целое M, целое N, float *A);
float *random_matrix(целое rows, целое cols);
void time_random_matrix(целое TA, целое TB, целое m, целое k, целое n);
void reorg_cpu(float *x, целое w, целое h, целое c, целое batch, целое stride, целое forward, float *out);

void test_blas();

void inter_cpu(целое NX, float *X, целое NY, float *Y, целое B, float *OUT);
void deinter_cpu(целое NX, float *X, целое NY, float *Y, целое B, float *OUT);
void mult_add_into_cpu(целое N, float *X, float *Y, float *Z);

void const_cpu(целое N, float ALPHA, float *X, целое INCX);
void constrain_gpu(целое N, float ALPHA, float * X, целое INCX);
void pow_cpu(целое N, float ALPHA, float *X, целое INCX, float *Y, целое INCY);
void mul_cpu(целое N, float *X, целое INCX, float *Y, целое INCY);

целое test_gpu_blas();
void shortcut_cpu(целое batch, целое w1, целое h1, целое c1, float *add, целое w2, целое h2, целое c2, float s1, float s2, float *out);

void mean_cpu(float *x, целое batch, целое filters, целое spatial, float *mean);
void variance_cpu(float *x, float *mean, целое batch, целое filters, целое spatial, float *variance);

void scale_bias(float *output, float *scales, целое batch, целое n, целое size);
void backward_scale_cpu(float *x_norm, float *delta, целое batch, целое n, целое size, float *scale_updates);
void mean_delta_cpu(float *delta, float *variance, целое batch, целое filters, целое spatial, float *mean_delta);
void  variance_delta_cpu(float *x, float *delta, float *mean, float *variance, целое batch, целое filters, целое spatial, float *variance_delta);
void normalize_delta_cpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, целое batch, целое filters, целое spatial, float *delta);
void l2normalize_cpu(float *x, float *dx, целое batch, целое filters, целое spatial);

void smooth_l1_cpu(целое n, float *pred, float *truth, float *delta, float *error);
void l2_cpu(целое n, float *pred, float *truth, float *delta, float *error);
void l1_cpu(целое n, float *pred, float *truth, float *delta, float *error);
void logistic_x_ent_cpu(целое n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_cpu(целое n, float *pred, float *truth, float *delta, float *error);
void weighted_sum_cpu(float *a, float *b, float *s, целое num, float *c);
void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, целое n, float *dc);

void softmax(float *input, целое n, float temp, целое stride, float *output);
void softmax_cpu(float *input, целое n, целое batch, целое batch_offset, целое groups, целое group_offset, целое stride, float temp, float *output);
void upsample_cpu(float *in, целое w, целое h, целое c, целое batch, целое stride, целое forward, float scale, float *out);

#ifdef GPU
#include "cuda.h"
#include "tree.h"

void axpy_gpu(целое N, float ALPHA, float * X, целое INCX, float * Y, целое INCY);
void axpy_gpu_offset(целое N, float ALPHA, float * X, целое OFFX, целое INCX, float * Y, целое OFFY, целое INCY);
void copy_gpu(целое N, float * X, целое INCX, float * Y, целое INCY);
void copy_gpu_offset(целое N, float * X, целое OFFX, целое INCX, float * Y, целое OFFY, целое INCY);
void add_gpu(целое N, float ALPHA, float * X, целое INCX);
void supp_gpu(целое N, float ALPHA, float * X, целое INCX);
void mask_gpu(целое N, float * X, float mask_num, float * mask, float val);
void scale_mask_gpu(целое N, float * X, float mask_num, float * mask, float scale);
void const_gpu(целое N, float ALPHA, float *X, целое INCX);
void pow_gpu(целое N, float ALPHA, float *X, целое INCX, float *Y, целое INCY);
void mul_gpu(целое N, float *X, целое INCX, float *Y, целое INCY);

void mean_gpu(float *x, целое batch, целое filters, целое spatial, float *mean);
void variance_gpu(float *x, float *mean, целое batch, целое filters, целое spatial, float *variance);
void normalize_gpu(float *x, float *mean, float *variance, целое batch, целое filters, целое spatial);
void l2normalize_gpu(float *x, float *dx, целое batch, целое filters, целое spatial);

void normalize_delta_gpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, целое batch, целое filters, целое spatial, float *delta);

void fast_mean_delta_gpu(float *delta, float *variance, целое batch, целое filters, целое spatial, float *mean_delta);
void fast_variance_delta_gpu(float *x, float *delta, float *mean, float *variance, целое batch, целое filters, целое spatial, float *variance_delta);

void fast_variance_gpu(float *x, float *mean, целое batch, целое filters, целое spatial, float *variance);
void fast_mean_gpu(float *x, целое batch, целое filters, целое spatial, float *mean);
void shortcut_gpu(целое batch, целое w1, целое h1, целое c1, float *add, целое w2, целое h2, целое c2, float s1, float s2, float *out);
void scale_bias_gpu(float *output, float *biases, целое batch, целое n, целое size);
void backward_scale_gpu(float *x_norm, float *delta, целое batch, целое n, целое size, float *scale_updates);
void scale_bias_gpu(float *output, float *biases, целое batch, целое n, целое size);
void add_bias_gpu(float *output, float *biases, целое batch, целое n, целое size);
void backward_bias_gpu(float *bias_updates, float *delta, целое batch, целое n, целое size);

void logistic_x_ent_gpu(целое n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_gpu(целое n, float *pred, float *truth, float *delta, float *error);
void smooth_l1_gpu(целое n, float *pred, float *truth, float *delta, float *error);
void l2_gpu(целое n, float *pred, float *truth, float *delta, float *error);
void l1_gpu(целое n, float *pred, float *truth, float *delta, float *error);
void wgan_gpu(целое n, float *pred, float *truth, float *delta, float *error);
void weighted_delta_gpu(float *a, float *b, float *s, float *da, float *db, float *ds, целое num, float *dc);
void weighted_sum_gpu(float *a, float *b, float *s, целое num, float *c);
void mult_add_into_gpu(целое num, float *a, float *b, float *c);
void inter_gpu(целое NX, float *X, целое NY, float *Y, целое B, float *OUT);
void deinter_gpu(целое NX, float *X, целое NY, float *Y, целое B, float *OUT);

void reorg_gpu(float *x, целое w, целое h, целое c, целое batch, целое stride, целое forward, float *out);

void softmax_gpu(float *input, целое n, целое batch, целое batch_offset, целое groups, целое group_offset, целое stride, float temp, float *output);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, целое n, целое batch, целое t);
void adam_gpu(целое n, float *x, float *m, float *v, float B1, float B2, float rate, float eps, целое t);

void flatten_gpu(float *x, целое spatial, целое layers, целое batch, целое forward, float *out);
void softmax_tree(float *input, целое spatial, целое batch, целое stride, float temp, float *output, tree hier);
void upsample_gpu(float *in, целое w, целое h, целое c, целое batch, целое stride, целое forward, float scale, float *out);

#endif
#endif
