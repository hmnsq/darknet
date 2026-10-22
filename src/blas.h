#ifndef BLAS_H
#define BLAS_H
#include "darknet.h"

void flatten(float *x, ຈຳນວນເຕັມ size, ຈຳນວນເຕັມ layers, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ forward);
void pm(ຈຳນວນເຕັມ M, ຈຳນວນເຕັມ N, float *A);
float *random_matrix(ຈຳນວນເຕັມ rows, ຈຳນວນເຕັມ cols);
void time_random_matrix(ຈຳນວນເຕັມ TA, ຈຳນວນເຕັມ TB, ຈຳນວນເຕັມ m, ຈຳນວນເຕັມ k, ຈຳນວນເຕັມ n);
void reorg_cpu(float *x, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ stride, ຈຳນວນເຕັມ forward, float *out);

void test_blas();

void inter_cpu(ຈຳນວນເຕັມ NX, float *X, ຈຳນວນເຕັມ NY, float *Y, ຈຳນວນເຕັມ B, float *OUT);
void deinter_cpu(ຈຳນວນເຕັມ NX, float *X, ຈຳນວນເຕັມ NY, float *Y, ຈຳນວນເຕັມ B, float *OUT);
void mult_add_into_cpu(ຈຳນວນເຕັມ N, float *X, float *Y, float *Z);

void const_cpu(ຈຳນວນເຕັມ N, float ALPHA, float *X, ຈຳນວນເຕັມ INCX);
void constrain_gpu(ຈຳນວນເຕັມ N, float ALPHA, float * X, ຈຳນວນເຕັມ INCX);
void pow_cpu(ຈຳນວນເຕັມ N, float ALPHA, float *X, ຈຳນວນເຕັມ INCX, float *Y, ຈຳນວນເຕັມ INCY);
void mul_cpu(ຈຳນວນເຕັມ N, float *X, ຈຳນວນເຕັມ INCX, float *Y, ຈຳນວນເຕັມ INCY);

ຈຳນວນເຕັມ test_gpu_blas();
void shortcut_cpu(ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ w1, ຈຳນວນເຕັມ h1, ຈຳນວນເຕັມ c1, float *add, ຈຳນວນເຕັມ w2, ຈຳນວນເຕັມ h2, ຈຳນວນເຕັມ c2, float s1, float s2, float *out);

void mean_cpu(float *x, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial, float *mean);
void variance_cpu(float *x, float *mean, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial, float *variance);

void scale_bias(float *output, float *scales, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ size);
void backward_scale_cpu(float *x_norm, float *delta, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ size, float *scale_updates);
void mean_delta_cpu(float *delta, float *variance, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial, float *mean_delta);
void  variance_delta_cpu(float *x, float *delta, float *mean, float *variance, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial, float *variance_delta);
void normalize_delta_cpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial, float *delta);
void l2normalize_cpu(float *x, float *dx, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial);

void smooth_l1_cpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error);
void l2_cpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error);
void l1_cpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error);
void logistic_x_ent_cpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_cpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error);
void weighted_sum_cpu(float *a, float *b, float *s, ຈຳນວນເຕັມ num, float *c);
void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, ຈຳນວນເຕັມ n, float *dc);

void softmax(float *input, ຈຳນວນເຕັມ n, float temp, ຈຳນວນເຕັມ stride, float *output);
void softmax_cpu(float *input, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ batch_offset, ຈຳນວນເຕັມ groups, ຈຳນວນເຕັມ group_offset, ຈຳນວນເຕັມ stride, float temp, float *output);
void upsample_cpu(float *in, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ stride, ຈຳນວນເຕັມ forward, float scale, float *out);

#ifdef GPU
#include "cuda.h"
#include "tree.h"

void axpy_gpu(ຈຳນວນເຕັມ N, float ALPHA, float * X, ຈຳນວນເຕັມ INCX, float * Y, ຈຳນວນເຕັມ INCY);
void axpy_gpu_offset(ຈຳນວນເຕັມ N, float ALPHA, float * X, ຈຳນວນເຕັມ OFFX, ຈຳນວນເຕັມ INCX, float * Y, ຈຳນວນເຕັມ OFFY, ຈຳນວນເຕັມ INCY);
void copy_gpu(ຈຳນວນເຕັມ N, float * X, ຈຳນວນເຕັມ INCX, float * Y, ຈຳນວນເຕັມ INCY);
void copy_gpu_offset(ຈຳນວນເຕັມ N, float * X, ຈຳນວນເຕັມ OFFX, ຈຳນວນເຕັມ INCX, float * Y, ຈຳນວນເຕັມ OFFY, ຈຳນວນເຕັມ INCY);
void add_gpu(ຈຳນວນເຕັມ N, float ALPHA, float * X, ຈຳນວນເຕັມ INCX);
void supp_gpu(ຈຳນວນເຕັມ N, float ALPHA, float * X, ຈຳນວນເຕັມ INCX);
void mask_gpu(ຈຳນວນເຕັມ N, float * X, float mask_num, float * mask, float val);
void scale_mask_gpu(ຈຳນວນເຕັມ N, float * X, float mask_num, float * mask, float scale);
void const_gpu(ຈຳນວນເຕັມ N, float ALPHA, float *X, ຈຳນວນເຕັມ INCX);
void pow_gpu(ຈຳນວນເຕັມ N, float ALPHA, float *X, ຈຳນວນເຕັມ INCX, float *Y, ຈຳນວນເຕັມ INCY);
void mul_gpu(ຈຳນວນເຕັມ N, float *X, ຈຳນວນເຕັມ INCX, float *Y, ຈຳນວນເຕັມ INCY);

void mean_gpu(float *x, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial, float *mean);
void variance_gpu(float *x, float *mean, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial, float *variance);
void normalize_gpu(float *x, float *mean, float *variance, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial);
void l2normalize_gpu(float *x, float *dx, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial);

void normalize_delta_gpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial, float *delta);

void fast_mean_delta_gpu(float *delta, float *variance, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial, float *mean_delta);
void fast_variance_delta_gpu(float *x, float *delta, float *mean, float *variance, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial, float *variance_delta);

void fast_variance_gpu(float *x, float *mean, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial, float *variance);
void fast_mean_gpu(float *x, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial, float *mean);
void shortcut_gpu(ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ w1, ຈຳນວນເຕັມ h1, ຈຳນວນເຕັມ c1, float *add, ຈຳນວນເຕັມ w2, ຈຳນວນເຕັມ h2, ຈຳນວນເຕັມ c2, float s1, float s2, float *out);
void scale_bias_gpu(float *output, float *biases, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ size);
void backward_scale_gpu(float *x_norm, float *delta, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ size, float *scale_updates);
void scale_bias_gpu(float *output, float *biases, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ size);
void add_bias_gpu(float *output, float *biases, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ size);
void backward_bias_gpu(float *bias_updates, float *delta, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ size);

void logistic_x_ent_gpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_gpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error);
void smooth_l1_gpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error);
void l2_gpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error);
void l1_gpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error);
void wgan_gpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error);
void weighted_delta_gpu(float *a, float *b, float *s, float *da, float *db, float *ds, ຈຳນວນເຕັມ num, float *dc);
void weighted_sum_gpu(float *a, float *b, float *s, ຈຳນວນເຕັມ num, float *c);
void mult_add_into_gpu(ຈຳນວນເຕັມ num, float *a, float *b, float *c);
void inter_gpu(ຈຳນວນເຕັມ NX, float *X, ຈຳນວນເຕັມ NY, float *Y, ຈຳນວນເຕັມ B, float *OUT);
void deinter_gpu(ຈຳນວນເຕັມ NX, float *X, ຈຳນວນເຕັມ NY, float *Y, ຈຳນວນເຕັມ B, float *OUT);

void reorg_gpu(float *x, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ stride, ຈຳນວນເຕັມ forward, float *out);

void softmax_gpu(float *input, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ batch_offset, ຈຳນວນເຕັມ groups, ຈຳນວນເຕັມ group_offset, ຈຳນວນເຕັມ stride, float temp, float *output);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ t);
void adam_gpu(ຈຳນວນເຕັມ n, float *x, float *m, float *v, float B1, float B2, float rate, float eps, ຈຳນວນເຕັມ t);

void flatten_gpu(float *x, ຈຳນວນເຕັມ spatial, ຈຳນວນເຕັມ layers, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ forward, float *out);
void softmax_tree(float *input, ຈຳນວນເຕັມ spatial, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ stride, float temp, float *output, tree hier);
void upsample_gpu(float *in, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ stride, ຈຳນວນເຕັມ forward, float scale, float *out);

#endif
#endif
