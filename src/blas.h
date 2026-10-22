#ifndef BLAS_H
#define BLAS_H
#شامل "darknet.h"

تهی flatten(اعشاری *x, صحیح size, صحیح layers, صحیح batch, صحیح forward);
تهی pm(صحیح M, صحیح N, اعشاری *A);
اعشاری *random_matrix(صحیح rows, صحیح cols);
تهی time_random_matrix(صحیح TA, صحیح TB, صحیح m, صحیح k, صحیح n);
تهی reorg_cpu(اعشاری *x, صحیح w, صحیح h, صحیح c, صحیح batch, صحیح stride, صحیح forward, اعشاری *out);

تهی test_blas();

تهی inter_cpu(صحیح NX, اعشاری *X, صحیح NY, اعشاری *Y, صحیح B, اعشاری *OUT);
تهی deinter_cpu(صحیح NX, اعشاری *X, صحیح NY, اعشاری *Y, صحیح B, اعشاری *OUT);
تهی mult_add_into_cpu(صحیح N, اعشاری *X, اعشاری *Y, اعشاری *Z);

تهی const_cpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX);
تهی constrain_gpu(صحیح N, اعشاری ALPHA, اعشاری * X, صحیح INCX);
تهی pow_cpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY);
تهی mul_cpu(صحیح N, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY);

صحیح test_gpu_blas();
تهی shortcut_cpu(صحیح batch, صحیح w1, صحیح h1, صحیح c1, اعشاری *add, صحیح w2, صحیح h2, صحیح c2, اعشاری s1, اعشاری s2, اعشاری *out);

تهی mean_cpu(اعشاری *x, صحیح batch, صحیح filters, صحیح spatial, اعشاری *mean);
تهی variance_cpu(اعشاری *x, اعشاری *mean, صحیح batch, صحیح filters, صحیح spatial, اعشاری *variance);

تهی scale_bias(اعشاری *output, اعشاری *scales, صحیح batch, صحیح n, صحیح size);
تهی backward_scale_cpu(اعشاری *x_norm, اعشاری *delta, صحیح batch, صحیح n, صحیح size, اعشاری *scale_updates);
تهی mean_delta_cpu(اعشاری *delta, اعشاری *variance, صحیح batch, صحیح filters, صحیح spatial, اعشاری *mean_delta);
تهی  variance_delta_cpu(اعشاری *x, اعشاری *delta, اعشاری *mean, اعشاری *variance, صحیح batch, صحیح filters, صحیح spatial, اعشاری *variance_delta);
تهی normalize_delta_cpu(اعشاری *x, اعشاری *mean, اعشاری *variance, اعشاری *mean_delta, اعشاری *variance_delta, صحیح batch, صحیح filters, صحیح spatial, اعشاری *delta);
تهی l2normalize_cpu(اعشاری *x, اعشاری *dx, صحیح batch, صحیح filters, صحیح spatial);

تهی smooth_l1_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
تهی l2_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
تهی l1_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
تهی logistic_x_ent_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
تهی softmax_x_ent_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
تهی weighted_sum_cpu(اعشاری *a, اعشاری *b, اعشاری *s, صحیح رقم, اعشاری *c);
تهی weighted_delta_cpu(اعشاری *a, اعشاری *b, اعشاری *s, اعشاری *da, اعشاری *db, اعشاری *ds, صحیح n, اعشاری *dc);

تهی softmax(اعشاری *درون_داد, صحیح n, اعشاری temp, صحیح stride, اعشاری *output);
تهی softmax_cpu(اعشاری *درون_داد, صحیح n, صحیح batch, صحیح batch_offset, صحیح groups, صحیح group_offset, صحیح stride, اعشاری temp, اعشاری *output);
تهی upsample_cpu(اعشاری *in, صحیح w, صحیح h, صحیح c, صحیح batch, صحیح stride, صحیح forward, اعشاری scale, اعشاری *out);

#ifdef GPU
#شامل "cuda.h"
#شامل "tree.h"

تهی axpy_gpu(صحیح N, اعشاری ALPHA, اعشاری * X, صحیح INCX, اعشاری * Y, صحیح INCY);
تهی axpy_gpu_offset(صحیح N, اعشاری ALPHA, اعشاری * X, صحیح OFFX, صحیح INCX, اعشاری * Y, صحیح OFFY, صحیح INCY);
تهی copy_gpu(صحیح N, اعشاری * X, صحیح INCX, اعشاری * Y, صحیح INCY);
تهی copy_gpu_offset(صحیح N, اعشاری * X, صحیح OFFX, صحیح INCX, اعشاری * Y, صحیح OFFY, صحیح INCY);
تهی add_gpu(صحیح N, اعشاری ALPHA, اعشاری * X, صحیح INCX);
تهی supp_gpu(صحیح N, اعشاری ALPHA, اعشاری * X, صحیح INCX);
تهی mask_gpu(صحیح N, اعشاری * X, اعشاری mask_رقم, اعشاری * mask, اعشاری val);
تهی scale_mask_gpu(صحیح N, اعشاری * X, اعشاری mask_رقم, اعشاری * mask, اعشاری scale);
تهی const_gpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX);
تهی pow_gpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY);
تهی mul_gpu(صحیح N, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY);

تهی mean_gpu(اعشاری *x, صحیح batch, صحیح filters, صحیح spatial, اعشاری *mean);
تهی variance_gpu(اعشاری *x, اعشاری *mean, صحیح batch, صحیح filters, صحیح spatial, اعشاری *variance);
تهی normalize_gpu(اعشاری *x, اعشاری *mean, اعشاری *variance, صحیح batch, صحیح filters, صحیح spatial);
تهی l2normalize_gpu(اعشاری *x, اعشاری *dx, صحیح batch, صحیح filters, صحیح spatial);

تهی normalize_delta_gpu(اعشاری *x, اعشاری *mean, اعشاری *variance, اعشاری *mean_delta, اعشاری *variance_delta, صحیح batch, صحیح filters, صحیح spatial, اعشاری *delta);

تهی fast_mean_delta_gpu(اعشاری *delta, اعشاری *variance, صحیح batch, صحیح filters, صحیح spatial, اعشاری *mean_delta);
تهی fast_variance_delta_gpu(اعشاری *x, اعشاری *delta, اعشاری *mean, اعشاری *variance, صحیح batch, صحیح filters, صحیح spatial, اعشاری *variance_delta);

تهی fast_variance_gpu(اعشاری *x, اعشاری *mean, صحیح batch, صحیح filters, صحیح spatial, اعشاری *variance);
تهی fast_mean_gpu(اعشاری *x, صحیح batch, صحیح filters, صحیح spatial, اعشاری *mean);
تهی shortcut_gpu(صحیح batch, صحیح w1, صحیح h1, صحیح c1, اعشاری *add, صحیح w2, صحیح h2, صحیح c2, اعشاری s1, اعشاری s2, اعشاری *out);
تهی scale_bias_gpu(اعشاری *output, اعشاری *biases, صحیح batch, صحیح n, صحیح size);
تهی backward_scale_gpu(اعشاری *x_norm, اعشاری *delta, صحیح batch, صحیح n, صحیح size, اعشاری *scale_updates);
تهی scale_bias_gpu(اعشاری *output, اعشاری *biases, صحیح batch, صحیح n, صحیح size);
تهی add_bias_gpu(اعشاری *output, اعشاری *biases, صحیح batch, صحیح n, صحیح size);
تهی backward_bias_gpu(اعشاری *bias_updates, اعشاری *delta, صحیح batch, صحیح n, صحیح size);

تهی logistic_x_ent_gpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
تهی softmax_x_ent_gpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
تهی smooth_l1_gpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
تهی l2_gpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
تهی l1_gpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
تهی wgan_gpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
تهی weighted_delta_gpu(اعشاری *a, اعشاری *b, اعشاری *s, اعشاری *da, اعشاری *db, اعشاری *ds, صحیح رقم, اعشاری *dc);
تهی weighted_sum_gpu(اعشاری *a, اعشاری *b, اعشاری *s, صحیح رقم, اعشاری *c);
تهی mult_add_into_gpu(صحیح رقم, اعشاری *a, اعشاری *b, اعشاری *c);
تهی inter_gpu(صحیح NX, اعشاری *X, صحیح NY, اعشاری *Y, صحیح B, اعشاری *OUT);
تهی deinter_gpu(صحیح NX, اعشاری *X, صحیح NY, اعشاری *Y, صحیح B, اعشاری *OUT);

تهی reorg_gpu(اعشاری *x, صحیح w, صحیح h, صحیح c, صحیح batch, صحیح stride, صحیح forward, اعشاری *out);

تهی softmax_gpu(اعشاری *درون_داد, صحیح n, صحیح batch, صحیح batch_offset, صحیح groups, صحیح group_offset, صحیح stride, اعشاری temp, اعشاری *output);
تهی adam_update_gpu(اعشاری *w, اعشاری *d, اعشاری *m, اعشاری *v, اعشاری B1, اعشاری B2, اعشاری eps, اعشاری decay, اعشاری rate, صحیح n, صحیح batch, صحیح t);
تهی adam_gpu(صحیح n, اعشاری *x, اعشاری *m, اعشاری *v, اعشاری B1, اعشاری B2, اعشاری rate, اعشاری eps, صحیح t);

تهی flatten_gpu(اعشاری *x, صحیح spatial, صحیح layers, صحیح batch, صحیح forward, اعشاری *out);
تهی softmax_درخت(اعشاری *درون_داد, صحیح spatial, صحیح batch, صحیح stride, اعشاری temp, اعشاری *output, درخت hier);
تهی upsample_gpu(اعشاری *in, صحیح w, صحیح h, صحیح c, صحیح batch, صحیح stride, صحیح forward, اعشاری scale, اعشاری *out);

#endif
#endif
