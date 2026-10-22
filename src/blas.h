#ifndef BLAS_H
#define BLAS_H
#ضمن "darknet.h"

فراغ flatten(عائم *x, صحيح size, صحيح layers, صحيح batch, صحيح forward);
فراغ pm(صحيح M, صحيح N, عائم *A);
عائم *random_matrix(صحيح rows, صحيح cols);
فراغ time_random_matrix(صحيح TA, صحيح TB, صحيح m, صحيح k, صحيح n);
فراغ reorg_cpu(عائم *x, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم *out);

فراغ test_blas();

فراغ inter_cpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *OUT);
فراغ deinter_cpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *OUT);
فراغ mult_add_into_cpu(صحيح N, عائم *X, عائم *Y, عائم *Z);

فراغ const_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX);
فراغ constrain_gpu(صحيح N, عائم ALPHA, عائم * X, صحيح INCX);
فراغ pow_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX, عائم *Y, صحيح INCY);
فراغ mul_cpu(صحيح N, عائم *X, صحيح INCX, عائم *Y, صحيح INCY);

صحيح test_gpu_blas();
فراغ shortcut_cpu(صحيح batch, صحيح w1, صحيح h1, صحيح c1, عائم *add, صحيح w2, صحيح h2, صحيح c2, عائم s1, عائم s2, عائم *out);

فراغ mean_cpu(عائم *x, صحيح batch, صحيح filters, صحيح spatial, عائم *mean);
فراغ variance_cpu(عائم *x, عائم *mean, صحيح batch, صحيح filters, صحيح spatial, عائم *variance);

فراغ scale_bias(عائم *output, عائم *scales, صحيح batch, صحيح n, صحيح size);
فراغ backward_scale_cpu(عائم *x_norm, عائم *delta, صحيح batch, صحيح n, صحيح size, عائم *scale_updates);
فراغ mean_delta_cpu(عائم *delta, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *mean_delta);
فراغ  variance_delta_cpu(عائم *x, عائم *delta, عائم *mean, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *variance_delta);
فراغ normalize_delta_cpu(عائم *x, عائم *mean, عائم *variance, عائم *mean_delta, عائم *variance_delta, صحيح batch, صحيح filters, صحيح spatial, عائم *delta);
فراغ l2normalize_cpu(عائم *x, عائم *dx, صحيح batch, صحيح filters, صحيح spatial);

فراغ smooth_l1_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ l2_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ l1_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ logistic_x_ent_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ softmax_x_ent_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ weighted_sum_cpu(عائم *a, عائم *b, عائم *s, صحيح لمب, عائم *c);
فراغ weighted_delta_cpu(عائم *a, عائم *b, عائم *s, عائم *da, عائم *db, عائم *ds, صحيح n, عائم *dc);

فراغ softmax(عائم *شغرو, صحيح n, عائم temp, صحيح stride, عائم *output);
فراغ softmax_cpu(عائم *شغرو, صحيح n, صحيح batch, صحيح batch_offset, صحيح groups, صحيح group_offset, صحيح stride, عائم temp, عائم *output);
فراغ upsample_cpu(عائم *in, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم scale, عائم *out);

#ifdef GPU
#ضمن "cuda.h"
#ضمن "tree.h"

فراغ axpy_gpu(صحيح N, عائم ALPHA, عائم * X, صحيح INCX, عائم * Y, صحيح INCY);
فراغ axpy_gpu_offset(صحيح N, عائم ALPHA, عائم * X, صحيح OFFX, صحيح INCX, عائم * Y, صحيح OFFY, صحيح INCY);
فراغ copy_gpu(صحيح N, عائم * X, صحيح INCX, عائم * Y, صحيح INCY);
فراغ copy_gpu_offset(صحيح N, عائم * X, صحيح OFFX, صحيح INCX, عائم * Y, صحيح OFFY, صحيح INCY);
فراغ add_gpu(صحيح N, عائم ALPHA, عائم * X, صحيح INCX);
فراغ supp_gpu(صحيح N, عائم ALPHA, عائم * X, صحيح INCX);
فراغ mask_gpu(صحيح N, عائم * X, عائم mask_لمب, عائم * mask, عائم val);
فراغ scale_mask_gpu(صحيح N, عائم * X, عائم mask_لمب, عائم * mask, عائم scale);
فراغ const_gpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX);
فراغ pow_gpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX, عائم *Y, صحيح INCY);
فراغ mul_gpu(صحيح N, عائم *X, صحيح INCX, عائم *Y, صحيح INCY);

فراغ mean_gpu(عائم *x, صحيح batch, صحيح filters, صحيح spatial, عائم *mean);
فراغ variance_gpu(عائم *x, عائم *mean, صحيح batch, صحيح filters, صحيح spatial, عائم *variance);
فراغ normalize_gpu(عائم *x, عائم *mean, عائم *variance, صحيح batch, صحيح filters, صحيح spatial);
فراغ l2normalize_gpu(عائم *x, عائم *dx, صحيح batch, صحيح filters, صحيح spatial);

فراغ normalize_delta_gpu(عائم *x, عائم *mean, عائم *variance, عائم *mean_delta, عائم *variance_delta, صحيح batch, صحيح filters, صحيح spatial, عائم *delta);

فراغ fast_mean_delta_gpu(عائم *delta, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *mean_delta);
فراغ fast_variance_delta_gpu(عائم *x, عائم *delta, عائم *mean, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *variance_delta);

فراغ fast_variance_gpu(عائم *x, عائم *mean, صحيح batch, صحيح filters, صحيح spatial, عائم *variance);
فراغ fast_mean_gpu(عائم *x, صحيح batch, صحيح filters, صحيح spatial, عائم *mean);
فراغ shortcut_gpu(صحيح batch, صحيح w1, صحيح h1, صحيح c1, عائم *add, صحيح w2, صحيح h2, صحيح c2, عائم s1, عائم s2, عائم *out);
فراغ scale_bias_gpu(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح size);
فراغ backward_scale_gpu(عائم *x_norm, عائم *delta, صحيح batch, صحيح n, صحيح size, عائم *scale_updates);
فراغ scale_bias_gpu(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح size);
فراغ add_bias_gpu(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح size);
فراغ backward_bias_gpu(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح size);

فراغ logistic_x_ent_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ softmax_x_ent_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ smooth_l1_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ l2_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ l1_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ wgan_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ weighted_delta_gpu(عائم *a, عائم *b, عائم *s, عائم *da, عائم *db, عائم *ds, صحيح لمب, عائم *dc);
فراغ weighted_sum_gpu(عائم *a, عائم *b, عائم *s, صحيح لمب, عائم *c);
فراغ mult_add_into_gpu(صحيح لمب, عائم *a, عائم *b, عائم *c);
فراغ inter_gpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *OUT);
فراغ deinter_gpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *OUT);

فراغ reorg_gpu(عائم *x, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم *out);

فراغ softmax_gpu(عائم *شغرو, صحيح n, صحيح batch, صحيح batch_offset, صحيح groups, صحيح group_offset, صحيح stride, عائم temp, عائم *output);
فراغ adam_update_gpu(عائم *w, عائم *d, عائم *m, عائم *v, عائم B1, عائم B2, عائم eps, عائم decay, عائم rate, صحيح n, صحيح batch, صحيح t);
فراغ adam_gpu(صحيح n, عائم *x, عائم *m, عائم *v, عائم B1, عائم B2, عائم rate, عائم eps, صحيح t);

فراغ flatten_gpu(عائم *x, صحيح spatial, صحيح layers, صحيح batch, صحيح forward, عائم *out);
فراغ softmax_بشيا(عائم *شغرو, صحيح spatial, صحيح batch, صحيح stride, عائم temp, عائم *output, بشيا hier);
فراغ upsample_gpu(عائم *in, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم scale, عائم *out);

#endif
#endif
