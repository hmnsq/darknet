#ifndef BLAS_H
#define BLAS_H
#ଅନ୍ତର୍ଭାଵ "darknet.h"

ଶୂନ୍ୟ flatten(ଦଶମିକ *x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା layers, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା forward);
ଶୂନ୍ୟ pm(ପୂର୍ଣ୍ଣସଂଖ୍ୟା M, ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ *A);
ଦଶମିକ *random_matrix(ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols);
ଶୂନ୍ୟ time_random_matrix(ପୂର୍ଣ୍ଣସଂଖ୍ୟା TA, ପୂର୍ଣ୍ଣସଂଖ୍ୟା TB, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);
ଶୂନ୍ୟ reorg_cpu(ଦଶମିକ *x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା forward, ଦଶମିକ *out);

ଶୂନ୍ୟ test_blas();

ଶୂନ୍ୟ inter_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା NX, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା NY, ଦଶମିକ *Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା B, ଦଶମିକ *OUT);
ଶୂନ୍ୟ deinter_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା NX, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା NY, ଦଶମିକ *Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା B, ଦଶମିକ *OUT);
ଶୂନ୍ୟ mult_add_into_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ *X, ଦଶମିକ *Y, ଦଶମିକ *Z);

ଶୂନ୍ୟ const_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ ALPHA, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX);
ଶୂନ୍ୟ constrain_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ ALPHA, ଦଶମିକ * X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX);
ଶୂନ୍ୟ pow_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ ALPHA, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX, ଦଶମିକ *Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCY);
ଶୂନ୍ୟ mul_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX, ଦଶମିକ *Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCY);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା test_gpu_blas();
ଶୂନ୍ୟ shortcut_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c1, ଦଶମିକ *add, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c2, ଦଶମିକ s1, ଦଶମିକ s2, ଦଶମିକ *out);

ଶୂନ୍ୟ mean_cpu(ଦଶମିକ *x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *mean);
ଶୂନ୍ୟ variance_cpu(ଦଶମିକ *x, ଦଶମିକ *mean, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *variance);

ଶୂନ୍ୟ scale_bias(ଦଶମିକ *output, ଦଶମିକ *scales, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size);
ଶୂନ୍ୟ backward_scale_cpu(ଦଶମିକ *x_norm, ଦଶମିକ *delta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ *scale_updates);
ଶୂନ୍ୟ mean_delta_cpu(ଦଶମିକ *delta, ଦଶମିକ *variance, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *mean_delta);
ଶୂନ୍ୟ  variance_delta_cpu(ଦଶମିକ *x, ଦଶମିକ *delta, ଦଶମିକ *mean, ଦଶମିକ *variance, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *variance_delta);
ଶୂନ୍ୟ normalize_delta_cpu(ଦଶମିକ *x, ଦଶମିକ *mean, ଦଶମିକ *variance, ଦଶମିକ *mean_delta, ଦଶମିକ *variance_delta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *delta);
ଶୂନ୍ୟ l2normalize_cpu(ଦଶମିକ *x, ଦଶମିକ *dx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial);

ଶୂନ୍ୟ smooth_l1_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error);
ଶୂନ୍ୟ l2_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error);
ଶୂନ୍ୟ l1_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error);
ଶୂନ୍ୟ logistic_x_ent_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error);
ଶୂନ୍ୟ softmax_x_ent_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error);
ଶୂନ୍ୟ weighted_sum_cpu(ଦଶମିକ *a, ଦଶମିକ *b, ଦଶମିକ *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ, ଦଶମିକ *c);
ଶୂନ୍ୟ weighted_delta_cpu(ଦଶମିକ *a, ଦଶମିକ *b, ଦଶମିକ *s, ଦଶମିକ *da, ଦଶମିକ *db, ଦଶମିକ *ds, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *dc);

ଶୂନ୍ୟ softmax(ଦଶମିକ *ନିବେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ temp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ଦଶମିକ *output);
ଶୂନ୍ୟ softmax_cpu(ଦଶମିକ *ନିବେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_offset, ପୂର୍ଣ୍ଣସଂଖ୍ୟା groups, ପୂର୍ଣ୍ଣସଂଖ୍ୟା group_offset, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ଦଶମିକ temp, ଦଶମିକ *output);
ଶୂନ୍ୟ upsample_cpu(ଦଶମିକ *in, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା forward, ଦଶମିକ scale, ଦଶମିକ *out);

#ifdef GPU
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"

ଶୂନ୍ୟ axpy_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ ALPHA, ଦଶମିକ * X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX, ଦଶମିକ * Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCY);
ଶୂନ୍ୟ axpy_gpu_offset(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ ALPHA, ଦଶମିକ * X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା OFFX, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX, ଦଶମିକ * Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା OFFY, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCY);
ଶୂନ୍ୟ copy_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ * X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX, ଦଶମିକ * Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCY);
ଶୂନ୍ୟ copy_gpu_offset(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ * X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା OFFX, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX, ଦଶମିକ * Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା OFFY, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCY);
ଶୂନ୍ୟ add_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ ALPHA, ଦଶମିକ * X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX);
ଶୂନ୍ୟ supp_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ ALPHA, ଦଶମିକ * X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX);
ଶୂନ୍ୟ mask_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ * X, ଦଶମିକ mask_ଅଙ୍କ, ଦଶମିକ * mask, ଦଶମିକ val);
ଶୂନ୍ୟ scale_mask_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ * X, ଦଶମିକ mask_ଅଙ୍କ, ଦଶମିକ * mask, ଦଶମିକ scale);
ଶୂନ୍ୟ const_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ ALPHA, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX);
ଶୂନ୍ୟ pow_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ ALPHA, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX, ଦଶମିକ *Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCY);
ଶୂନ୍ୟ mul_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX, ଦଶମିକ *Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCY);

ଶୂନ୍ୟ mean_gpu(ଦଶମିକ *x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *mean);
ଶୂନ୍ୟ variance_gpu(ଦଶମିକ *x, ଦଶମିକ *mean, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *variance);
ଶୂନ୍ୟ normalize_gpu(ଦଶମିକ *x, ଦଶମିକ *mean, ଦଶମିକ *variance, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial);
ଶୂନ୍ୟ l2normalize_gpu(ଦଶମିକ *x, ଦଶମିକ *dx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial);

ଶୂନ୍ୟ normalize_delta_gpu(ଦଶମିକ *x, ଦଶମିକ *mean, ଦଶମିକ *variance, ଦଶମିକ *mean_delta, ଦଶମିକ *variance_delta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *delta);

ଶୂନ୍ୟ fast_mean_delta_gpu(ଦଶମିକ *delta, ଦଶମିକ *variance, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *mean_delta);
ଶୂନ୍ୟ fast_variance_delta_gpu(ଦଶମିକ *x, ଦଶମିକ *delta, ଦଶମିକ *mean, ଦଶମିକ *variance, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *variance_delta);

ଶୂନ୍ୟ fast_variance_gpu(ଦଶମିକ *x, ଦଶମିକ *mean, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *variance);
ଶୂନ୍ୟ fast_mean_gpu(ଦଶମିକ *x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *mean);
ଶୂନ୍ୟ shortcut_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c1, ଦଶମିକ *add, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c2, ଦଶମିକ s1, ଦଶମିକ s2, ଦଶମିକ *out);
ଶୂନ୍ୟ scale_bias_gpu(ଦଶମିକ *output, ଦଶମିକ *biases, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size);
ଶୂନ୍ୟ backward_scale_gpu(ଦଶମିକ *x_norm, ଦଶମିକ *delta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ *scale_updates);
ଶୂନ୍ୟ scale_bias_gpu(ଦଶମିକ *output, ଦଶମିକ *biases, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size);
ଶୂନ୍ୟ add_bias_gpu(ଦଶମିକ *output, ଦଶମିକ *biases, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size);
ଶୂନ୍ୟ backward_bias_gpu(ଦଶମିକ *bias_updates, ଦଶମିକ *delta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size);

ଶୂନ୍ୟ logistic_x_ent_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error);
ଶୂନ୍ୟ softmax_x_ent_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error);
ଶୂନ୍ୟ smooth_l1_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error);
ଶୂନ୍ୟ l2_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error);
ଶୂନ୍ୟ l1_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error);
ଶୂନ୍ୟ wgan_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error);
ଶୂନ୍ୟ weighted_delta_gpu(ଦଶମିକ *a, ଦଶମିକ *b, ଦଶମିକ *s, ଦଶମିକ *da, ଦଶମିକ *db, ଦଶମିକ *ds, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ, ଦଶମିକ *dc);
ଶୂନ୍ୟ weighted_sum_gpu(ଦଶମିକ *a, ଦଶମିକ *b, ଦଶମିକ *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ, ଦଶମିକ *c);
ଶୂନ୍ୟ mult_add_into_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ, ଦଶମିକ *a, ଦଶମିକ *b, ଦଶମିକ *c);
ଶୂନ୍ୟ inter_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା NX, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା NY, ଦଶମିକ *Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା B, ଦଶମିକ *OUT);
ଶୂନ୍ୟ deinter_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା NX, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା NY, ଦଶମିକ *Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା B, ଦଶମିକ *OUT);

ଶୂନ୍ୟ reorg_gpu(ଦଶମିକ *x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା forward, ଦଶମିକ *out);

ଶୂନ୍ୟ softmax_gpu(ଦଶମିକ *ନିବେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_offset, ପୂର୍ଣ୍ଣସଂଖ୍ୟା groups, ପୂର୍ଣ୍ଣସଂଖ୍ୟା group_offset, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ଦଶମିକ temp, ଦଶମିକ *output);
ଶୂନ୍ୟ adam_update_gpu(ଦଶମିକ *w, ଦଶମିକ *d, ଦଶମିକ *m, ଦଶମିକ *v, ଦଶମିକ B1, ଦଶମିକ B2, ଦଶମିକ eps, ଦଶମିକ decay, ଦଶମିକ rate, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା t);
ଶୂନ୍ୟ adam_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *x, ଦଶମିକ *m, ଦଶମିକ *v, ଦଶମିକ B1, ଦଶମିକ B2, ଦଶମିକ rate, ଦଶମିକ eps, ପୂର୍ଣ୍ଣସଂଖ୍ୟା t);

ଶୂନ୍ୟ flatten_gpu(ଦଶମିକ *x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ପୂର୍ଣ୍ଣସଂଖ୍ୟା layers, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା forward, ଦଶମିକ *out);
ଶୂନ୍ୟ softmax_ବୃକ୍ଷ(ଦଶମିକ *ନିବେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ଦଶମିକ temp, ଦଶମିକ *output, ବୃକ୍ଷ hier);
ଶୂନ୍ୟ upsample_gpu(ଦଶମିକ *in, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା forward, ଦଶମିକ scale, ଦଶମିକ *out);

#endif
#endif
