#ifndef BLAS_H
#define BLAS_H
#ಅನ್ತರ್ಭಾವ "darknet.h"

ಶೂನ್ಯ flatten(ಭಿನ್ನ *x, ಪೂರ್ಣಾಙ್ಕ size, ಪೂರ್ಣಾಙ್ಕ layers, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ forward);
ಶೂನ್ಯ pm(ಪೂರ್ಣಾಙ್ಕ M, ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ *A);
ಭಿನ್ನ *random_matrix(ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols);
ಶೂನ್ಯ time_random_matrix(ಪೂರ್ಣಾಙ್ಕ TA, ಪೂರ್ಣಾಙ್ಕ TB, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ n);
ಶೂನ್ಯ reorg_cpu(ಭಿನ್ನ *x, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ forward, ಭಿನ್ನ *out);

ಶೂನ್ಯ test_blas();

ಶೂನ್ಯ inter_cpu(ಪೂರ್ಣಾಙ್ಕ NX, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ NY, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ B, ಭಿನ್ನ *OUT);
ಶೂನ್ಯ deinter_cpu(ಪೂರ್ಣಾಙ್ಕ NX, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ NY, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ B, ಭಿನ್ನ *OUT);
ಶೂನ್ಯ mult_add_into_cpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ *X, ಭಿನ್ನ *Y, ಭಿನ್ನ *Z);

ಶೂನ್ಯ const_cpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ ALPHA, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ INCX);
ಶೂನ್ಯ constrain_gpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ ALPHA, ಭಿನ್ನ * X, ಪೂರ್ಣಾಙ್ಕ INCX);
ಶೂನ್ಯ pow_cpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ ALPHA, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ INCX, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ INCY);
ಶೂನ್ಯ mul_cpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ INCX, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ INCY);

ಪೂರ್ಣಾಙ್ಕ test_gpu_blas();
ಶೂನ್ಯ shortcut_cpu(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w1, ಪೂರ್ಣಾಙ್ಕ h1, ಪೂರ್ಣಾಙ್ಕ c1, ಭಿನ್ನ *add, ಪೂರ್ಣಾಙ್ಕ w2, ಪೂರ್ಣಾಙ್ಕ h2, ಪೂರ್ಣಾಙ್ಕ c2, ಭಿನ್ನ s1, ಭಿನ್ನ s2, ಭಿನ್ನ *out);

ಶೂನ್ಯ mean_cpu(ಭಿನ್ನ *x, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial, ಭಿನ್ನ *mean);
ಶೂನ್ಯ variance_cpu(ಭಿನ್ನ *x, ಭಿನ್ನ *mean, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial, ಭಿನ್ನ *variance);

ಶೂನ್ಯ scale_bias(ಭಿನ್ನ *output, ಭಿನ್ನ *scales, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size);
ಶೂನ್ಯ backward_scale_cpu(ಭಿನ್ನ *x_norm, ಭಿನ್ನ *delta, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size, ಭಿನ್ನ *scale_updates);
ಶೂನ್ಯ mean_delta_cpu(ಭಿನ್ನ *delta, ಭಿನ್ನ *variance, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial, ಭಿನ್ನ *mean_delta);
ಶೂನ್ಯ  variance_delta_cpu(ಭಿನ್ನ *x, ಭಿನ್ನ *delta, ಭಿನ್ನ *mean, ಭಿನ್ನ *variance, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial, ಭಿನ್ನ *variance_delta);
ಶೂನ್ಯ normalize_delta_cpu(ಭಿನ್ನ *x, ಭಿನ್ನ *mean, ಭಿನ್ನ *variance, ಭಿನ್ನ *mean_delta, ಭಿನ್ನ *variance_delta, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial, ಭಿನ್ನ *delta);
ಶೂನ್ಯ l2normalize_cpu(ಭಿನ್ನ *x, ಭಿನ್ನ *dx, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial);

ಶೂನ್ಯ smooth_l1_cpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error);
ಶೂನ್ಯ l2_cpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error);
ಶೂನ್ಯ l1_cpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error);
ಶೂನ್ಯ logistic_x_ent_cpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error);
ಶೂನ್ಯ softmax_x_ent_cpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error);
ಶೂನ್ಯ weighted_sum_cpu(ಭಿನ್ನ *a, ಭಿನ್ನ *b, ಭಿನ್ನ *s, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ, ಭಿನ್ನ *c);
ಶೂನ್ಯ weighted_delta_cpu(ಭಿನ್ನ *a, ಭಿನ್ನ *b, ಭಿನ್ನ *s, ಭಿನ್ನ *da, ಭಿನ್ನ *db, ಭಿನ್ನ *ds, ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *dc);

ಶೂನ್ಯ softmax(ಭಿನ್ನ *ಒಳಹರಿವು, ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ temp, ಪೂರ್ಣಾಙ್ಕ stride, ಭಿನ್ನ *output);
ಶೂನ್ಯ softmax_cpu(ಭಿನ್ನ *ಒಳಹರಿವು, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ batch_offset, ಪೂರ್ಣಾಙ್ಕ groups, ಪೂರ್ಣಾಙ್ಕ group_offset, ಪೂರ್ಣಾಙ್ಕ stride, ಭಿನ್ನ temp, ಭಿನ್ನ *output);
ಶೂನ್ಯ upsample_cpu(ಭಿನ್ನ *in, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ forward, ಭಿನ್ನ scale, ಭಿನ್ನ *out);

#ifdef GPU
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "tree.h"

ಶೂನ್ಯ axpy_gpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ ALPHA, ಭಿನ್ನ * X, ಪೂರ್ಣಾಙ್ಕ INCX, ಭಿನ್ನ * Y, ಪೂರ್ಣಾಙ್ಕ INCY);
ಶೂನ್ಯ axpy_gpu_offset(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ ALPHA, ಭಿನ್ನ * X, ಪೂರ್ಣಾಙ್ಕ OFFX, ಪೂರ್ಣಾಙ್ಕ INCX, ಭಿನ್ನ * Y, ಪೂರ್ಣಾಙ್ಕ OFFY, ಪೂರ್ಣಾಙ್ಕ INCY);
ಶೂನ್ಯ copy_gpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ * X, ಪೂರ್ಣಾಙ್ಕ INCX, ಭಿನ್ನ * Y, ಪೂರ್ಣಾಙ್ಕ INCY);
ಶೂನ್ಯ copy_gpu_offset(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ * X, ಪೂರ್ಣಾಙ್ಕ OFFX, ಪೂರ್ಣಾಙ್ಕ INCX, ಭಿನ್ನ * Y, ಪೂರ್ಣಾಙ್ಕ OFFY, ಪೂರ್ಣಾಙ್ಕ INCY);
ಶೂನ್ಯ add_gpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ ALPHA, ಭಿನ್ನ * X, ಪೂರ್ಣಾಙ್ಕ INCX);
ಶೂನ್ಯ supp_gpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ ALPHA, ಭಿನ್ನ * X, ಪೂರ್ಣಾಙ್ಕ INCX);
ಶೂನ್ಯ mask_gpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ * X, ಭಿನ್ನ mask_ಅಂಕೆ, ಭಿನ್ನ * mask, ಭಿನ್ನ val);
ಶೂನ್ಯ scale_mask_gpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ * X, ಭಿನ್ನ mask_ಅಂಕೆ, ಭಿನ್ನ * mask, ಭಿನ್ನ scale);
ಶೂನ್ಯ const_gpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ ALPHA, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ INCX);
ಶೂನ್ಯ pow_gpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ ALPHA, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ INCX, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ INCY);
ಶೂನ್ಯ mul_gpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ INCX, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ INCY);

ಶೂನ್ಯ mean_gpu(ಭಿನ್ನ *x, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial, ಭಿನ್ನ *mean);
ಶೂನ್ಯ variance_gpu(ಭಿನ್ನ *x, ಭಿನ್ನ *mean, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial, ಭಿನ್ನ *variance);
ಶೂನ್ಯ normalize_gpu(ಭಿನ್ನ *x, ಭಿನ್ನ *mean, ಭಿನ್ನ *variance, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial);
ಶೂನ್ಯ l2normalize_gpu(ಭಿನ್ನ *x, ಭಿನ್ನ *dx, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial);

ಶೂನ್ಯ normalize_delta_gpu(ಭಿನ್ನ *x, ಭಿನ್ನ *mean, ಭಿನ್ನ *variance, ಭಿನ್ನ *mean_delta, ಭಿನ್ನ *variance_delta, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial, ಭಿನ್ನ *delta);

ಶೂನ್ಯ fast_mean_delta_gpu(ಭಿನ್ನ *delta, ಭಿನ್ನ *variance, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial, ಭಿನ್ನ *mean_delta);
ಶೂನ್ಯ fast_variance_delta_gpu(ಭಿನ್ನ *x, ಭಿನ್ನ *delta, ಭಿನ್ನ *mean, ಭಿನ್ನ *variance, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial, ಭಿನ್ನ *variance_delta);

ಶೂನ್ಯ fast_variance_gpu(ಭಿನ್ನ *x, ಭಿನ್ನ *mean, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial, ಭಿನ್ನ *variance);
ಶೂನ್ಯ fast_mean_gpu(ಭಿನ್ನ *x, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial, ಭಿನ್ನ *mean);
ಶೂನ್ಯ shortcut_gpu(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w1, ಪೂರ್ಣಾಙ್ಕ h1, ಪೂರ್ಣಾಙ್ಕ c1, ಭಿನ್ನ *add, ಪೂರ್ಣಾಙ್ಕ w2, ಪೂರ್ಣಾಙ್ಕ h2, ಪೂರ್ಣಾಙ್ಕ c2, ಭಿನ್ನ s1, ಭಿನ್ನ s2, ಭಿನ್ನ *out);
ಶೂನ್ಯ scale_bias_gpu(ಭಿನ್ನ *output, ಭಿನ್ನ *biases, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size);
ಶೂನ್ಯ backward_scale_gpu(ಭಿನ್ನ *x_norm, ಭಿನ್ನ *delta, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size, ಭಿನ್ನ *scale_updates);
ಶೂನ್ಯ scale_bias_gpu(ಭಿನ್ನ *output, ಭಿನ್ನ *biases, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size);
ಶೂನ್ಯ add_bias_gpu(ಭಿನ್ನ *output, ಭಿನ್ನ *biases, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size);
ಶೂನ್ಯ backward_bias_gpu(ಭಿನ್ನ *bias_updates, ಭಿನ್ನ *delta, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size);

ಶೂನ್ಯ logistic_x_ent_gpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error);
ಶೂನ್ಯ softmax_x_ent_gpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error);
ಶೂನ್ಯ smooth_l1_gpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error);
ಶೂನ್ಯ l2_gpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error);
ಶೂನ್ಯ l1_gpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error);
ಶೂನ್ಯ wgan_gpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error);
ಶೂನ್ಯ weighted_delta_gpu(ಭಿನ್ನ *a, ಭಿನ್ನ *b, ಭಿನ್ನ *s, ಭಿನ್ನ *da, ಭಿನ್ನ *db, ಭಿನ್ನ *ds, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ, ಭಿನ್ನ *dc);
ಶೂನ್ಯ weighted_sum_gpu(ಭಿನ್ನ *a, ಭಿನ್ನ *b, ಭಿನ್ನ *s, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ, ಭಿನ್ನ *c);
ಶೂನ್ಯ mult_add_into_gpu(ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ, ಭಿನ್ನ *a, ಭಿನ್ನ *b, ಭಿನ್ನ *c);
ಶೂನ್ಯ inter_gpu(ಪೂರ್ಣಾಙ್ಕ NX, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ NY, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ B, ಭಿನ್ನ *OUT);
ಶೂನ್ಯ deinter_gpu(ಪೂರ್ಣಾಙ್ಕ NX, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ NY, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ B, ಭಿನ್ನ *OUT);

ಶೂನ್ಯ reorg_gpu(ಭಿನ್ನ *x, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ forward, ಭಿನ್ನ *out);

ಶೂನ್ಯ softmax_gpu(ಭಿನ್ನ *ಒಳಹರಿವು, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ batch_offset, ಪೂರ್ಣಾಙ್ಕ groups, ಪೂರ್ಣಾಙ್ಕ group_offset, ಪೂರ್ಣಾಙ್ಕ stride, ಭಿನ್ನ temp, ಭಿನ್ನ *output);
ಶೂನ್ಯ adam_update_gpu(ಭಿನ್ನ *w, ಭಿನ್ನ *d, ಭಿನ್ನ *m, ಭಿನ್ನ *v, ಭಿನ್ನ B1, ಭಿನ್ನ B2, ಭಿನ್ನ eps, ಭಿನ್ನ decay, ಭಿನ್ನ rate, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ t);
ಶೂನ್ಯ adam_gpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *x, ಭಿನ್ನ *m, ಭಿನ್ನ *v, ಭಿನ್ನ B1, ಭಿನ್ನ B2, ಭಿನ್ನ rate, ಭಿನ್ನ eps, ಪೂರ್ಣಾಙ್ಕ t);

ಶೂನ್ಯ flatten_gpu(ಭಿನ್ನ *x, ಪೂರ್ಣಾಙ್ಕ spatial, ಪೂರ್ಣಾಙ್ಕ layers, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ forward, ಭಿನ್ನ *out);
ಶೂನ್ಯ softmax_ವೃಕ್ಷ(ಭಿನ್ನ *ಒಳಹರಿವು, ಪೂರ್ಣಾಙ್ಕ spatial, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ stride, ಭಿನ್ನ temp, ಭಿನ್ನ *output, ವೃಕ್ಷ hier);
ಶೂನ್ಯ upsample_gpu(ಭಿನ್ನ *in, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ forward, ಭಿನ್ನ scale, ಭಿನ್ನ *out);

#endif
#endif
