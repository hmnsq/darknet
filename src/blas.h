#ifndef BLAS_H
#define BLAS_H
#अन्तर्भाव "darknet.h"

शून्य flatten(दशांश *x, पूर्णांक size, पूर्णांक layers, पूर्णांक batch, पूर्णांक forward);
शून्य pm(पूर्णांक M, पूर्णांक N, दशांश *A);
दशांश *random_matrix(पूर्णांक rows, पूर्णांक cols);
शून्य time_random_matrix(पूर्णांक TA, पूर्णांक TB, पूर्णांक m, पूर्णांक k, पूर्णांक n);
शून्य reorg_cpu(दशांश *x, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक batch, पूर्णांक stride, पूर्णांक forward, दशांश *out);

शून्य test_blas();

शून्य inter_cpu(पूर्णांक NX, दशांश *X, पूर्णांक NY, दशांश *Y, पूर्णांक B, दशांश *OUT);
शून्य deinter_cpu(पूर्णांक NX, दशांश *X, पूर्णांक NY, दशांश *Y, पूर्णांक B, दशांश *OUT);
शून्य mult_add_into_cpu(पूर्णांक N, दशांश *X, दशांश *Y, दशांश *Z);

शून्य const_cpu(पूर्णांक N, दशांश ALPHA, दशांश *X, पूर्णांक INCX);
शून्य constrain_gpu(पूर्णांक N, दशांश ALPHA, दशांश * X, पूर्णांक INCX);
शून्य pow_cpu(पूर्णांक N, दशांश ALPHA, दशांश *X, पूर्णांक INCX, दशांश *Y, पूर्णांक INCY);
शून्य mul_cpu(पूर्णांक N, दशांश *X, पूर्णांक INCX, दशांश *Y, पूर्णांक INCY);

पूर्णांक test_gpu_blas();
शून्य shortcut_cpu(पूर्णांक batch, पूर्णांक w1, पूर्णांक h1, पूर्णांक c1, दशांश *add, पूर्णांक w2, पूर्णांक h2, पूर्णांक c2, दशांश s1, दशांश s2, दशांश *out);

शून्य mean_cpu(दशांश *x, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, दशांश *mean);
शून्य variance_cpu(दशांश *x, दशांश *mean, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, दशांश *variance);

शून्य scale_bias(दशांश *output, दशांश *scales, पूर्णांक batch, पूर्णांक n, पूर्णांक size);
शून्य backward_scale_cpu(दशांश *x_norm, दशांश *delta, पूर्णांक batch, पूर्णांक n, पूर्णांक size, दशांश *scale_updates);
शून्य mean_delta_cpu(दशांश *delta, दशांश *variance, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, दशांश *mean_delta);
शून्य  variance_delta_cpu(दशांश *x, दशांश *delta, दशांश *mean, दशांश *variance, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, दशांश *variance_delta);
शून्य normalize_delta_cpu(दशांश *x, दशांश *mean, दशांश *variance, दशांश *mean_delta, दशांश *variance_delta, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, दशांश *delta);
शून्य l2normalize_cpu(दशांश *x, दशांश *dx, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial);

शून्य smooth_l1_cpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error);
शून्य l2_cpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error);
शून्य l1_cpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error);
शून्य logistic_x_ent_cpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error);
शून्य softmax_x_ent_cpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error);
शून्य weighted_sum_cpu(दशांश *a, दशांश *b, दशांश *s, पूर्णांक अंक, दशांश *c);
शून्य weighted_delta_cpu(दशांश *a, दशांश *b, दशांश *s, दशांश *da, दशांश *db, दशांश *ds, पूर्णांक n, दशांश *dc);

शून्य softmax(दशांश *आदान, पूर्णांक n, दशांश temp, पूर्णांक stride, दशांश *output);
शून्य softmax_cpu(दशांश *आदान, पूर्णांक n, पूर्णांक batch, पूर्णांक batch_offset, पूर्णांक groups, पूर्णांक group_offset, पूर्णांक stride, दशांश temp, दशांश *output);
शून्य upsample_cpu(दशांश *in, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक batch, पूर्णांक stride, पूर्णांक forward, दशांश scale, दशांश *out);

#ifdef GPU
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "tree.h"

शून्य axpy_gpu(पूर्णांक N, दशांश ALPHA, दशांश * X, पूर्णांक INCX, दशांश * Y, पूर्णांक INCY);
शून्य axpy_gpu_offset(पूर्णांक N, दशांश ALPHA, दशांश * X, पूर्णांक OFFX, पूर्णांक INCX, दशांश * Y, पूर्णांक OFFY, पूर्णांक INCY);
शून्य copy_gpu(पूर्णांक N, दशांश * X, पूर्णांक INCX, दशांश * Y, पूर्णांक INCY);
शून्य copy_gpu_offset(पूर्णांक N, दशांश * X, पूर्णांक OFFX, पूर्णांक INCX, दशांश * Y, पूर्णांक OFFY, पूर्णांक INCY);
शून्य add_gpu(पूर्णांक N, दशांश ALPHA, दशांश * X, पूर्णांक INCX);
शून्य supp_gpu(पूर्णांक N, दशांश ALPHA, दशांश * X, पूर्णांक INCX);
शून्य mask_gpu(पूर्णांक N, दशांश * X, दशांश mask_अंक, दशांश * mask, दशांश val);
शून्य scale_mask_gpu(पूर्णांक N, दशांश * X, दशांश mask_अंक, दशांश * mask, दशांश scale);
शून्य const_gpu(पूर्णांक N, दशांश ALPHA, दशांश *X, पूर्णांक INCX);
शून्य pow_gpu(पूर्णांक N, दशांश ALPHA, दशांश *X, पूर्णांक INCX, दशांश *Y, पूर्णांक INCY);
शून्य mul_gpu(पूर्णांक N, दशांश *X, पूर्णांक INCX, दशांश *Y, पूर्णांक INCY);

शून्य mean_gpu(दशांश *x, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, दशांश *mean);
शून्य variance_gpu(दशांश *x, दशांश *mean, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, दशांश *variance);
शून्य normalize_gpu(दशांश *x, दशांश *mean, दशांश *variance, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial);
शून्य l2normalize_gpu(दशांश *x, दशांश *dx, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial);

शून्य normalize_delta_gpu(दशांश *x, दशांश *mean, दशांश *variance, दशांश *mean_delta, दशांश *variance_delta, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, दशांश *delta);

शून्य fast_mean_delta_gpu(दशांश *delta, दशांश *variance, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, दशांश *mean_delta);
शून्य fast_variance_delta_gpu(दशांश *x, दशांश *delta, दशांश *mean, दशांश *variance, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, दशांश *variance_delta);

शून्य fast_variance_gpu(दशांश *x, दशांश *mean, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, दशांश *variance);
शून्य fast_mean_gpu(दशांश *x, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, दशांश *mean);
शून्य shortcut_gpu(पूर्णांक batch, पूर्णांक w1, पूर्णांक h1, पूर्णांक c1, दशांश *add, पूर्णांक w2, पूर्णांक h2, पूर्णांक c2, दशांश s1, दशांश s2, दशांश *out);
शून्य scale_bias_gpu(दशांश *output, दशांश *biases, पूर्णांक batch, पूर्णांक n, पूर्णांक size);
शून्य backward_scale_gpu(दशांश *x_norm, दशांश *delta, पूर्णांक batch, पूर्णांक n, पूर्णांक size, दशांश *scale_updates);
शून्य scale_bias_gpu(दशांश *output, दशांश *biases, पूर्णांक batch, पूर्णांक n, पूर्णांक size);
शून्य add_bias_gpu(दशांश *output, दशांश *biases, पूर्णांक batch, पूर्णांक n, पूर्णांक size);
शून्य backward_bias_gpu(दशांश *bias_updates, दशांश *delta, पूर्णांक batch, पूर्णांक n, पूर्णांक size);

शून्य logistic_x_ent_gpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error);
शून्य softmax_x_ent_gpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error);
शून्य smooth_l1_gpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error);
शून्य l2_gpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error);
शून्य l1_gpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error);
शून्य wgan_gpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error);
शून्य weighted_delta_gpu(दशांश *a, दशांश *b, दशांश *s, दशांश *da, दशांश *db, दशांश *ds, पूर्णांक अंक, दशांश *dc);
शून्य weighted_sum_gpu(दशांश *a, दशांश *b, दशांश *s, पूर्णांक अंक, दशांश *c);
शून्य mult_add_into_gpu(पूर्णांक अंक, दशांश *a, दशांश *b, दशांश *c);
शून्य inter_gpu(पूर्णांक NX, दशांश *X, पूर्णांक NY, दशांश *Y, पूर्णांक B, दशांश *OUT);
शून्य deinter_gpu(पूर्णांक NX, दशांश *X, पूर्णांक NY, दशांश *Y, पूर्णांक B, दशांश *OUT);

शून्य reorg_gpu(दशांश *x, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक batch, पूर्णांक stride, पूर्णांक forward, दशांश *out);

शून्य softmax_gpu(दशांश *आदान, पूर्णांक n, पूर्णांक batch, पूर्णांक batch_offset, पूर्णांक groups, पूर्णांक group_offset, पूर्णांक stride, दशांश temp, दशांश *output);
शून्य adam_update_gpu(दशांश *w, दशांश *d, दशांश *m, दशांश *v, दशांश B1, दशांश B2, दशांश eps, दशांश decay, दशांश rate, पूर्णांक n, पूर्णांक batch, पूर्णांक t);
शून्य adam_gpu(पूर्णांक n, दशांश *x, दशांश *m, दशांश *v, दशांश B1, दशांश B2, दशांश rate, दशांश eps, पूर्णांक t);

शून्य flatten_gpu(दशांश *x, पूर्णांक spatial, पूर्णांक layers, पूर्णांक batch, पूर्णांक forward, दशांश *out);
शून्य softmax_वृक्ष(दशांश *आदान, पूर्णांक spatial, पूर्णांक batch, पूर्णांक stride, दशांश temp, दशांश *output, वृक्ष hier);
शून्य upsample_gpu(दशांश *in, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक batch, पूर्णांक stride, पूर्णांक forward, दशांश scale, दशांश *out);

#endif
#endif
