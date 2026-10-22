#ifndef BLAS_H
#define BLAS_H
#include "darknet.h"

void flatten(float *x, цяло size, цяло layers, цяло batch, цяло forward);
void pm(цяло M, цяло N, float *A);
float *random_matrix(цяло rows, цяло cols);
void time_random_matrix(цяло TA, цяло TB, цяло m, цяло k, цяло n);
void reorg_cpu(float *x, цяло w, цяло h, цяло c, цяло batch, цяло stride, цяло forward, float *out);

void test_blas();

void inter_cpu(цяло NX, float *X, цяло NY, float *Y, цяло B, float *OUT);
void deinter_cpu(цяло NX, float *X, цяло NY, float *Y, цяло B, float *OUT);
void mult_add_into_cpu(цяло N, float *X, float *Y, float *Z);

void const_cpu(цяло N, float ALPHA, float *X, цяло INCX);
void constrain_gpu(цяло N, float ALPHA, float * X, цяло INCX);
void pow_cpu(цяло N, float ALPHA, float *X, цяло INCX, float *Y, цяло INCY);
void mul_cpu(цяло N, float *X, цяло INCX, float *Y, цяло INCY);

цяло test_gpu_blas();
void shortcut_cpu(цяло batch, цяло w1, цяло h1, цяло c1, float *add, цяло w2, цяло h2, цяло c2, float s1, float s2, float *out);

void mean_cpu(float *x, цяло batch, цяло filters, цяло spatial, float *mean);
void variance_cpu(float *x, float *mean, цяло batch, цяло filters, цяло spatial, float *variance);

void scale_bias(float *output, float *scales, цяло batch, цяло n, цяло size);
void backward_scale_cpu(float *x_norm, float *delta, цяло batch, цяло n, цяло size, float *scale_updates);
void mean_delta_cpu(float *delta, float *variance, цяло batch, цяло filters, цяло spatial, float *mean_delta);
void  variance_delta_cpu(float *x, float *delta, float *mean, float *variance, цяло batch, цяло filters, цяло spatial, float *variance_delta);
void normalize_delta_cpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, цяло batch, цяло filters, цяло spatial, float *delta);
void l2normalize_cpu(float *x, float *dx, цяло batch, цяло filters, цяло spatial);

void smooth_l1_cpu(цяло n, float *pred, float *truth, float *delta, float *error);
void l2_cpu(цяло n, float *pred, float *truth, float *delta, float *error);
void l1_cpu(цяло n, float *pred, float *truth, float *delta, float *error);
void logistic_x_ent_cpu(цяло n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_cpu(цяло n, float *pred, float *truth, float *delta, float *error);
void weighted_sum_cpu(float *a, float *b, float *s, цяло num, float *c);
void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, цяло n, float *dc);

void softmax(float *input, цяло n, float temp, цяло stride, float *output);
void softmax_cpu(float *input, цяло n, цяло batch, цяло batch_offset, цяло groups, цяло group_offset, цяло stride, float temp, float *output);
void upsample_cpu(float *in, цяло w, цяло h, цяло c, цяло batch, цяло stride, цяло forward, float scale, float *out);

#ifdef GPU
#include "cuda.h"
#include "tree.h"

void axpy_gpu(цяло N, float ALPHA, float * X, цяло INCX, float * Y, цяло INCY);
void axpy_gpu_offset(цяло N, float ALPHA, float * X, цяло OFFX, цяло INCX, float * Y, цяло OFFY, цяло INCY);
void copy_gpu(цяло N, float * X, цяло INCX, float * Y, цяло INCY);
void copy_gpu_offset(цяло N, float * X, цяло OFFX, цяло INCX, float * Y, цяло OFFY, цяло INCY);
void add_gpu(цяло N, float ALPHA, float * X, цяло INCX);
void supp_gpu(цяло N, float ALPHA, float * X, цяло INCX);
void mask_gpu(цяло N, float * X, float mask_num, float * mask, float val);
void scale_mask_gpu(цяло N, float * X, float mask_num, float * mask, float scale);
void const_gpu(цяло N, float ALPHA, float *X, цяло INCX);
void pow_gpu(цяло N, float ALPHA, float *X, цяло INCX, float *Y, цяло INCY);
void mul_gpu(цяло N, float *X, цяло INCX, float *Y, цяло INCY);

void mean_gpu(float *x, цяло batch, цяло filters, цяло spatial, float *mean);
void variance_gpu(float *x, float *mean, цяло batch, цяло filters, цяло spatial, float *variance);
void normalize_gpu(float *x, float *mean, float *variance, цяло batch, цяло filters, цяло spatial);
void l2normalize_gpu(float *x, float *dx, цяло batch, цяло filters, цяло spatial);

void normalize_delta_gpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, цяло batch, цяло filters, цяло spatial, float *delta);

void fast_mean_delta_gpu(float *delta, float *variance, цяло batch, цяло filters, цяло spatial, float *mean_delta);
void fast_variance_delta_gpu(float *x, float *delta, float *mean, float *variance, цяло batch, цяло filters, цяло spatial, float *variance_delta);

void fast_variance_gpu(float *x, float *mean, цяло batch, цяло filters, цяло spatial, float *variance);
void fast_mean_gpu(float *x, цяло batch, цяло filters, цяло spatial, float *mean);
void shortcut_gpu(цяло batch, цяло w1, цяло h1, цяло c1, float *add, цяло w2, цяло h2, цяло c2, float s1, float s2, float *out);
void scale_bias_gpu(float *output, float *biases, цяло batch, цяло n, цяло size);
void backward_scale_gpu(float *x_norm, float *delta, цяло batch, цяло n, цяло size, float *scale_updates);
void scale_bias_gpu(float *output, float *biases, цяло batch, цяло n, цяло size);
void add_bias_gpu(float *output, float *biases, цяло batch, цяло n, цяло size);
void backward_bias_gpu(float *bias_updates, float *delta, цяло batch, цяло n, цяло size);

void logistic_x_ent_gpu(цяло n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_gpu(цяло n, float *pred, float *truth, float *delta, float *error);
void smooth_l1_gpu(цяло n, float *pred, float *truth, float *delta, float *error);
void l2_gpu(цяло n, float *pred, float *truth, float *delta, float *error);
void l1_gpu(цяло n, float *pred, float *truth, float *delta, float *error);
void wgan_gpu(цяло n, float *pred, float *truth, float *delta, float *error);
void weighted_delta_gpu(float *a, float *b, float *s, float *da, float *db, float *ds, цяло num, float *dc);
void weighted_sum_gpu(float *a, float *b, float *s, цяло num, float *c);
void mult_add_into_gpu(цяло num, float *a, float *b, float *c);
void inter_gpu(цяло NX, float *X, цяло NY, float *Y, цяло B, float *OUT);
void deinter_gpu(цяло NX, float *X, цяло NY, float *Y, цяло B, float *OUT);

void reorg_gpu(float *x, цяло w, цяло h, цяло c, цяло batch, цяло stride, цяло forward, float *out);

void softmax_gpu(float *input, цяло n, цяло batch, цяло batch_offset, цяло groups, цяло group_offset, цяло stride, float temp, float *output);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, цяло n, цяло batch, цяло t);
void adam_gpu(цяло n, float *x, float *m, float *v, float B1, float B2, float rate, float eps, цяло t);

void flatten_gpu(float *x, цяло spatial, цяло layers, цяло batch, цяло forward, float *out);
void softmax_tree(float *input, цяло spatial, цяло batch, цяло stride, float temp, float *output, tree hier);
void upsample_gpu(float *in, цяло w, цяло h, цяло c, цяло batch, цяло stride, цяло forward, float scale, float *out);

#endif
#endif
