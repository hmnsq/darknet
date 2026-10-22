#ifndef BLAS_H
#define BLAS_H
#include "darknet.h"

void flatten(float *x, số_nguyên size, số_nguyên layers, số_nguyên batch, số_nguyên forward);
void pm(số_nguyên M, số_nguyên N, float *A);
float *random_matrix(số_nguyên rows, số_nguyên cols);
void time_random_matrix(số_nguyên TA, số_nguyên TB, số_nguyên m, số_nguyên k, số_nguyên n);
void reorg_cpu(float *x, số_nguyên w, số_nguyên h, số_nguyên c, số_nguyên batch, số_nguyên stride, số_nguyên forward, float *out);

void test_blas();

void inter_cpu(số_nguyên NX, float *X, số_nguyên NY, float *Y, số_nguyên B, float *OUT);
void deinter_cpu(số_nguyên NX, float *X, số_nguyên NY, float *Y, số_nguyên B, float *OUT);
void mult_add_into_cpu(số_nguyên N, float *X, float *Y, float *Z);

void const_cpu(số_nguyên N, float ALPHA, float *X, số_nguyên INCX);
void constrain_gpu(số_nguyên N, float ALPHA, float * X, số_nguyên INCX);
void pow_cpu(số_nguyên N, float ALPHA, float *X, số_nguyên INCX, float *Y, số_nguyên INCY);
void mul_cpu(số_nguyên N, float *X, số_nguyên INCX, float *Y, số_nguyên INCY);

số_nguyên test_gpu_blas();
void shortcut_cpu(số_nguyên batch, số_nguyên w1, số_nguyên h1, số_nguyên c1, float *add, số_nguyên w2, số_nguyên h2, số_nguyên c2, float s1, float s2, float *out);

void mean_cpu(float *x, số_nguyên batch, số_nguyên filters, số_nguyên spatial, float *mean);
void variance_cpu(float *x, float *mean, số_nguyên batch, số_nguyên filters, số_nguyên spatial, float *variance);

void scale_bias(float *output, float *scales, số_nguyên batch, số_nguyên n, số_nguyên size);
void backward_scale_cpu(float *x_norm, float *delta, số_nguyên batch, số_nguyên n, số_nguyên size, float *scale_updates);
void mean_delta_cpu(float *delta, float *variance, số_nguyên batch, số_nguyên filters, số_nguyên spatial, float *mean_delta);
void  variance_delta_cpu(float *x, float *delta, float *mean, float *variance, số_nguyên batch, số_nguyên filters, số_nguyên spatial, float *variance_delta);
void normalize_delta_cpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, số_nguyên batch, số_nguyên filters, số_nguyên spatial, float *delta);
void l2normalize_cpu(float *x, float *dx, số_nguyên batch, số_nguyên filters, số_nguyên spatial);

void smooth_l1_cpu(số_nguyên n, float *pred, float *truth, float *delta, float *error);
void l2_cpu(số_nguyên n, float *pred, float *truth, float *delta, float *error);
void l1_cpu(số_nguyên n, float *pred, float *truth, float *delta, float *error);
void logistic_x_ent_cpu(số_nguyên n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_cpu(số_nguyên n, float *pred, float *truth, float *delta, float *error);
void weighted_sum_cpu(float *a, float *b, float *s, số_nguyên num, float *c);
void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, số_nguyên n, float *dc);

void softmax(float *input, số_nguyên n, float temp, số_nguyên stride, float *output);
void softmax_cpu(float *input, số_nguyên n, số_nguyên batch, số_nguyên batch_offset, số_nguyên groups, số_nguyên group_offset, số_nguyên stride, float temp, float *output);
void upsample_cpu(float *in, số_nguyên w, số_nguyên h, số_nguyên c, số_nguyên batch, số_nguyên stride, số_nguyên forward, float scale, float *out);

#ifdef GPU
#include "cuda.h"
#include "tree.h"

void axpy_gpu(số_nguyên N, float ALPHA, float * X, số_nguyên INCX, float * Y, số_nguyên INCY);
void axpy_gpu_offset(số_nguyên N, float ALPHA, float * X, số_nguyên OFFX, số_nguyên INCX, float * Y, số_nguyên OFFY, số_nguyên INCY);
void copy_gpu(số_nguyên N, float * X, số_nguyên INCX, float * Y, số_nguyên INCY);
void copy_gpu_offset(số_nguyên N, float * X, số_nguyên OFFX, số_nguyên INCX, float * Y, số_nguyên OFFY, số_nguyên INCY);
void add_gpu(số_nguyên N, float ALPHA, float * X, số_nguyên INCX);
void supp_gpu(số_nguyên N, float ALPHA, float * X, số_nguyên INCX);
void mask_gpu(số_nguyên N, float * X, float mask_num, float * mask, float val);
void scale_mask_gpu(số_nguyên N, float * X, float mask_num, float * mask, float scale);
void const_gpu(số_nguyên N, float ALPHA, float *X, số_nguyên INCX);
void pow_gpu(số_nguyên N, float ALPHA, float *X, số_nguyên INCX, float *Y, số_nguyên INCY);
void mul_gpu(số_nguyên N, float *X, số_nguyên INCX, float *Y, số_nguyên INCY);

void mean_gpu(float *x, số_nguyên batch, số_nguyên filters, số_nguyên spatial, float *mean);
void variance_gpu(float *x, float *mean, số_nguyên batch, số_nguyên filters, số_nguyên spatial, float *variance);
void normalize_gpu(float *x, float *mean, float *variance, số_nguyên batch, số_nguyên filters, số_nguyên spatial);
void l2normalize_gpu(float *x, float *dx, số_nguyên batch, số_nguyên filters, số_nguyên spatial);

void normalize_delta_gpu(float *x, float *mean, float *variance, float *mean_delta, float *variance_delta, số_nguyên batch, số_nguyên filters, số_nguyên spatial, float *delta);

void fast_mean_delta_gpu(float *delta, float *variance, số_nguyên batch, số_nguyên filters, số_nguyên spatial, float *mean_delta);
void fast_variance_delta_gpu(float *x, float *delta, float *mean, float *variance, số_nguyên batch, số_nguyên filters, số_nguyên spatial, float *variance_delta);

void fast_variance_gpu(float *x, float *mean, số_nguyên batch, số_nguyên filters, số_nguyên spatial, float *variance);
void fast_mean_gpu(float *x, số_nguyên batch, số_nguyên filters, số_nguyên spatial, float *mean);
void shortcut_gpu(số_nguyên batch, số_nguyên w1, số_nguyên h1, số_nguyên c1, float *add, số_nguyên w2, số_nguyên h2, số_nguyên c2, float s1, float s2, float *out);
void scale_bias_gpu(float *output, float *biases, số_nguyên batch, số_nguyên n, số_nguyên size);
void backward_scale_gpu(float *x_norm, float *delta, số_nguyên batch, số_nguyên n, số_nguyên size, float *scale_updates);
void scale_bias_gpu(float *output, float *biases, số_nguyên batch, số_nguyên n, số_nguyên size);
void add_bias_gpu(float *output, float *biases, số_nguyên batch, số_nguyên n, số_nguyên size);
void backward_bias_gpu(float *bias_updates, float *delta, số_nguyên batch, số_nguyên n, số_nguyên size);

void logistic_x_ent_gpu(số_nguyên n, float *pred, float *truth, float *delta, float *error);
void softmax_x_ent_gpu(số_nguyên n, float *pred, float *truth, float *delta, float *error);
void smooth_l1_gpu(số_nguyên n, float *pred, float *truth, float *delta, float *error);
void l2_gpu(số_nguyên n, float *pred, float *truth, float *delta, float *error);
void l1_gpu(số_nguyên n, float *pred, float *truth, float *delta, float *error);
void wgan_gpu(số_nguyên n, float *pred, float *truth, float *delta, float *error);
void weighted_delta_gpu(float *a, float *b, float *s, float *da, float *db, float *ds, số_nguyên num, float *dc);
void weighted_sum_gpu(float *a, float *b, float *s, số_nguyên num, float *c);
void mult_add_into_gpu(số_nguyên num, float *a, float *b, float *c);
void inter_gpu(số_nguyên NX, float *X, số_nguyên NY, float *Y, số_nguyên B, float *OUT);
void deinter_gpu(số_nguyên NX, float *X, số_nguyên NY, float *Y, số_nguyên B, float *OUT);

void reorg_gpu(float *x, số_nguyên w, số_nguyên h, số_nguyên c, số_nguyên batch, số_nguyên stride, số_nguyên forward, float *out);

void softmax_gpu(float *input, số_nguyên n, số_nguyên batch, số_nguyên batch_offset, số_nguyên groups, số_nguyên group_offset, số_nguyên stride, float temp, float *output);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, số_nguyên n, số_nguyên batch, số_nguyên t);
void adam_gpu(số_nguyên n, float *x, float *m, float *v, float B1, float B2, float rate, float eps, số_nguyên t);

void flatten_gpu(float *x, số_nguyên spatial, số_nguyên layers, số_nguyên batch, số_nguyên forward, float *out);
void softmax_tree(float *input, số_nguyên spatial, số_nguyên batch, số_nguyên stride, float temp, float *output, tree hier);
void upsample_gpu(float *in, số_nguyên w, số_nguyên h, số_nguyên c, số_nguyên batch, số_nguyên stride, số_nguyên forward, float scale, float *out);

#endif
#endif
