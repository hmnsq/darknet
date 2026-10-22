#ifndef BLAS_H
#define BLAS_H
#include "darknet.h"

void flatten(float *x, ціле size, ціле layers, ціле batch, ціле forward);
void pm(ціле M, ціле N, float *A);
float *random_matrix(ціле rows, ціле cols);
void time_random_matrix(ціле TA, ціле TB, ціле m, ціле k, ціле n);
void reorg_cpu(float *x, ціле w, ціле h, ціле c, ціле batch, ціле stride, ціле forward, float *out);

void test_blas();

void inter_cpu(ціле NX, float *X, ціле NY, float *Y, ціле B, float *OUT);
void deinter_cpu(ціле NX, float *X, ціле NY, float *Y, ціле B, float *OUT);
void mult_add_into_cpu(ціле N, float *X, float *Y, float *Z);

void const_cpu(ціле N, float ALPHA, float *X, ціле INCX);
void constrain_gpu(ціле N, float ALPHA, float * X, ціле INCX);
void pow_cpu(ціле N, float ALPHA, float *X, ціле INCX, float *Y, ціле INCY);
void mul_cpu(ціле N, float *X, ціле INCX, float *Y, ціле INCY);

ціле test_gpu_blas();
void shortcut_cpu(ціле batch, ціле w1, ціле h1, ціле c1, float *add, ціле w2, ціле h2, ціле c2, float s1, float s2, float *out);

void mean_cpu(float *x, ціле batch, ціле filters, ціле spatial, float *mean);
void variance_cpu(float *x, float *mean, ціле batch, ціле filters, ціле spatial, float *variance);

void scale_bias(float *output, float *scales, ціле batch, ціле n, ціле size);
void backward_scale_cpu(float *x_norm, float *delta, ціле batch, ціле n, ціле size, float *scale_updates);
void mean_delta_cpu(float *delta, float *variance, ціле batch, ціле filters, ціле spatial, float *mean_delta);
void  variance_delta_cpu(float *x, float *delta, float *mean, float *variance, ціле batch, ціле filters, ціле spatial, float *variance_delta);
void normalize_delta_cpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, ціле batch, ціле filters, ціле spatial, float *delta);
void l2normalize_cpu(float *x, float *dx, ціле batch, ціле filters, ціле spatial);

void smooth_l1_cpu(ціле n, float *pred, float *truth, float *delta, float *error);
void l2_cpu(ціле n, float *pred, float *truth, float *delta, float *error);
void l1_cpu(ціле n, float *pred, float *truth, float *delta, float *error);
void logistic_x_ent_cpu(ціле n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_cpu(ціле n, float *pred, float *truth, float *delta, float *error);
void weighted_sum_cpu(float *a, float *b, float *s, ціле num, float *c);
void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, ціле n, float *dc);

void softmax(float *input, ціле n, float temp, ціле stride, float *output);
void softmax_cpu(float *input, ціле n, ціле batch, ціле batch_offset, ціле groups, ціле group_offset, ціле stride, float temp, float *output);
void upsample_cpu(float *in, ціле w, ціле h, ціле c, ціле batch, ціле stride, ціле forward, float scale, float *out);

#ifdef GPU
#include "cuda.h"
#include "tree.h"

void axpy_gpu(ціле N, float ALPHA, float * X, ціле INCX, float * Y, ціле INCY);
void axpy_gpu_offset(ціле N, float ALPHA, float * X, ціле OFFX, ціле INCX, float * Y, ціле OFFY, ціле INCY);
void copy_gpu(ціле N, float * X, ціле INCX, float * Y, ціле INCY);
void copy_gpu_offset(ціле N, float * X, ціле OFFX, ціле INCX, float * Y, ціле OFFY, ціле INCY);
void add_gpu(ціле N, float ALPHA, float * X, ціле INCX);
void supp_gpu(ціле N, float ALPHA, float * X, ціле INCX);
void mask_gpu(ціле N, float * X, float mask_num, float * mask, float val);
void scale_mask_gpu(ціле N, float * X, float mask_num, float * mask, float scale);
void const_gpu(ціле N, float ALPHA, float *X, ціле INCX);
void pow_gpu(ціле N, float ALPHA, float *X, ціле INCX, float *Y, ціле INCY);
void mul_gpu(ціле N, float *X, ціле INCX, float *Y, ціле INCY);

void mean_gpu(float *x, ціле batch, ціле filters, ціле spatial, float *mean);
void variance_gpu(float *x, float *mean, ціле batch, ціле filters, ціле spatial, float *variance);
void normalize_gpu(float *x, float *mean, float *variance, ціле batch, ціле filters, ціле spatial);
void l2normalize_gpu(float *x, float *dx, ціле batch, ціле filters, ціле spatial);

void normalize_delta_gpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, ціле batch, ціле filters, ціле spatial, float *delta);

void fast_mean_delta_gpu(float *delta, float *variance, ціле batch, ціле filters, ціле spatial, float *mean_delta);
void fast_variance_delta_gpu(float *x, float *delta, float *mean, float *variance, ціле batch, ціле filters, ціле spatial, float *variance_delta);

void fast_variance_gpu(float *x, float *mean, ціле batch, ціле filters, ціле spatial, float *variance);
void fast_mean_gpu(float *x, ціле batch, ціле filters, ціле spatial, float *mean);
void shortcut_gpu(ціле batch, ціле w1, ціле h1, ціле c1, float *add, ціле w2, ціле h2, ціле c2, float s1, float s2, float *out);
void scale_bias_gpu(float *output, float *biases, ціле batch, ціле n, ціле size);
void backward_scale_gpu(float *x_norm, float *delta, ціле batch, ціле n, ціле size, float *scale_updates);
void scale_bias_gpu(float *output, float *biases, ціле batch, ціле n, ціле size);
void add_bias_gpu(float *output, float *biases, ціле batch, ціле n, ціле size);
void backward_bias_gpu(float *bias_updates, float *delta, ціле batch, ціле n, ціле size);

void logistic_x_ent_gpu(ціле n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_gpu(ціле n, float *pred, float *truth, float *delta, float *error);
void smooth_l1_gpu(ціле n, float *pred, float *truth, float *delta, float *error);
void l2_gpu(ціле n, float *pred, float *truth, float *delta, float *error);
void l1_gpu(ціле n, float *pred, float *truth, float *delta, float *error);
void wgan_gpu(ціле n, float *pred, float *truth, float *delta, float *error);
void weighted_delta_gpu(float *a, float *b, float *s, float *da, float *db, float *ds, ціле num, float *dc);
void weighted_sum_gpu(float *a, float *b, float *s, ціле num, float *c);
void mult_add_into_gpu(ціле num, float *a, float *b, float *c);
void inter_gpu(ціле NX, float *X, ціле NY, float *Y, ціле B, float *OUT);
void deinter_gpu(ціле NX, float *X, ціле NY, float *Y, ціле B, float *OUT);

void reorg_gpu(float *x, ціле w, ціле h, ціле c, ціле batch, ціле stride, ціле forward, float *out);

void softmax_gpu(float *input, ціле n, ціле batch, ціле batch_offset, ціле groups, ціле group_offset, ціле stride, float temp, float *output);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, ціле n, ціле batch, ціле t);
void adam_gpu(ціле n, float *x, float *m, float *v, float B1, float B2, float rate, float eps, ціле t);

void flatten_gpu(float *x, ціле spatial, ціле layers, ціле batch, ціле forward, float *out);
void softmax_tree(float *input, ціле spatial, ціле batch, ціле stride, float temp, float *output, tree hier);
void upsample_gpu(float *in, ціле w, ціле h, ціле c, ціле batch, ціле stride, ціле forward, float scale, float *out);

#endif
#endif
