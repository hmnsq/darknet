#ifndef BLAS_H
#define BLAS_H
#അന്തര്ഭാവ "darknet.h"

ശൂന്യം flatten(ദശാംശം *x, പൂർണ്ണസംഖ്യ size, പൂർണ്ണസംഖ്യ layers, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ forward);
ശൂന്യം pm(പൂർണ്ണസംഖ്യ M, പൂർണ്ണസംഖ്യ N, ദശാംശം *A);
ദശാംശം *random_matrix(പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols);
ശൂന്യം time_random_matrix(പൂർണ്ണസംഖ്യ TA, പൂർണ്ണസംഖ്യ TB, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ n);
ശൂന്യം reorg_cpu(ദശാംശം *x, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ forward, ദശാംശം *out);

ശൂന്യം test_blas();

ശൂന്യം inter_cpu(പൂർണ്ണസംഖ്യ NX, ദശാംശം *X, പൂർണ്ണസംഖ്യ NY, ദശാംശം *Y, പൂർണ്ണസംഖ്യ B, ദശാംശം *OUT);
ശൂന്യം deinter_cpu(പൂർണ്ണസംഖ്യ NX, ദശാംശം *X, പൂർണ്ണസംഖ്യ NY, ദശാംശം *Y, പൂർണ്ണസംഖ്യ B, ദശാംശം *OUT);
ശൂന്യം mult_add_into_cpu(പൂർണ്ണസംഖ്യ N, ദശാംശം *X, ദശാംശം *Y, ദശാംശം *Z);

ശൂന്യം const_cpu(പൂർണ്ണസംഖ്യ N, ദശാംശം ALPHA, ദശാംശം *X, പൂർണ്ണസംഖ്യ INCX);
ശൂന്യം constrain_gpu(പൂർണ്ണസംഖ്യ N, ദശാംശം ALPHA, ദശാംശം * X, പൂർണ്ണസംഖ്യ INCX);
ശൂന്യം pow_cpu(പൂർണ്ണസംഖ്യ N, ദശാംശം ALPHA, ദശാംശം *X, പൂർണ്ണസംഖ്യ INCX, ദശാംശം *Y, പൂർണ്ണസംഖ്യ INCY);
ശൂന്യം mul_cpu(പൂർണ്ണസംഖ്യ N, ദശാംശം *X, പൂർണ്ണസംഖ്യ INCX, ദശാംശം *Y, പൂർണ്ണസംഖ്യ INCY);

പൂർണ്ണസംഖ്യ test_gpu_blas();
ശൂന്യം shortcut_cpu(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w1, പൂർണ്ണസംഖ്യ h1, പൂർണ്ണസംഖ്യ c1, ദശാംശം *add, പൂർണ്ണസംഖ്യ w2, പൂർണ്ണസംഖ്യ h2, പൂർണ്ണസംഖ്യ c2, ദശാംശം s1, ദശാംശം s2, ദശാംശം *out);

ശൂന്യം mean_cpu(ദശാംശം *x, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial, ദശാംശം *mean);
ശൂന്യം variance_cpu(ദശാംശം *x, ദശാംശം *mean, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial, ദശാംശം *variance);

ശൂന്യം scale_bias(ദശാംശം *output, ദശാംശം *scales, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size);
ശൂന്യം backward_scale_cpu(ദശാംശം *x_norm, ദശാംശം *delta, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size, ദശാംശം *scale_updates);
ശൂന്യം mean_delta_cpu(ദശാംശം *delta, ദശാംശം *variance, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial, ദശാംശം *mean_delta);
ശൂന്യം  variance_delta_cpu(ദശാംശം *x, ദശാംശം *delta, ദശാംശം *mean, ദശാംശം *variance, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial, ദശാംശം *variance_delta);
ശൂന്യം normalize_delta_cpu(ദശാംശം *x, ദശാംശം *mean, ദശാംശം *variance, ദശാംശം *mean_delta, ദശാംശം *variance_delta, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial, ദശാംശം *delta);
ശൂന്യം l2normalize_cpu(ദശാംശം *x, ദശാംശം *dx, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial);

ശൂന്യം smooth_l1_cpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error);
ശൂന്യം l2_cpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error);
ശൂന്യം l1_cpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error);
ശൂന്യം logistic_x_ent_cpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error);
ശൂന്യം softmax_x_ent_cpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error);
ശൂന്യം weighted_sum_cpu(ദശാംശം *a, ദശാംശം *b, ദശാംശം *s, പൂർണ്ണസംഖ്യ അക്കം, ദശാംശം *c);
ശൂന്യം weighted_delta_cpu(ദശാംശം *a, ദശാംശം *b, ദശാംശം *s, ദശാംശം *da, ദശാംശം *db, ദശാംശം *ds, പൂർണ്ണസംഖ്യ n, ദശാംശം *dc);

ശൂന്യം softmax(ദശാംശം *നിവേശം, പൂർണ്ണസംഖ്യ n, ദശാംശം temp, പൂർണ്ണസംഖ്യ stride, ദശാംശം *output);
ശൂന്യം softmax_cpu(ദശാംശം *നിവേശം, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ batch_offset, പൂർണ്ണസംഖ്യ groups, പൂർണ്ണസംഖ്യ group_offset, പൂർണ്ണസംഖ്യ stride, ദശാംശം temp, ദശാംശം *output);
ശൂന്യം upsample_cpu(ദശാംശം *in, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ forward, ദശാംശം scale, ദശാംശം *out);

#ifdef GPU
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "tree.h"

ശൂന്യം axpy_gpu(പൂർണ്ണസംഖ്യ N, ദശാംശം ALPHA, ദശാംശം * X, പൂർണ്ണസംഖ്യ INCX, ദശാംശം * Y, പൂർണ്ണസംഖ്യ INCY);
ശൂന്യം axpy_gpu_offset(പൂർണ്ണസംഖ്യ N, ദശാംശം ALPHA, ദശാംശം * X, പൂർണ്ണസംഖ്യ OFFX, പൂർണ്ണസംഖ്യ INCX, ദശാംശം * Y, പൂർണ്ണസംഖ്യ OFFY, പൂർണ്ണസംഖ്യ INCY);
ശൂന്യം copy_gpu(പൂർണ്ണസംഖ്യ N, ദശാംശം * X, പൂർണ്ണസംഖ്യ INCX, ദശാംശം * Y, പൂർണ്ണസംഖ്യ INCY);
ശൂന്യം copy_gpu_offset(പൂർണ്ണസംഖ്യ N, ദശാംശം * X, പൂർണ്ണസംഖ്യ OFFX, പൂർണ്ണസംഖ്യ INCX, ദശാംശം * Y, പൂർണ്ണസംഖ്യ OFFY, പൂർണ്ണസംഖ്യ INCY);
ശൂന്യം add_gpu(പൂർണ്ണസംഖ്യ N, ദശാംശം ALPHA, ദശാംശം * X, പൂർണ്ണസംഖ്യ INCX);
ശൂന്യം supp_gpu(പൂർണ്ണസംഖ്യ N, ദശാംശം ALPHA, ദശാംശം * X, പൂർണ്ണസംഖ്യ INCX);
ശൂന്യം mask_gpu(പൂർണ്ണസംഖ്യ N, ദശാംശം * X, ദശാംശം mask_അക്കം, ദശാംശം * mask, ദശാംശം val);
ശൂന്യം scale_mask_gpu(പൂർണ്ണസംഖ്യ N, ദശാംശം * X, ദശാംശം mask_അക്കം, ദശാംശം * mask, ദശാംശം scale);
ശൂന്യം const_gpu(പൂർണ്ണസംഖ്യ N, ദശാംശം ALPHA, ദശാംശം *X, പൂർണ്ണസംഖ്യ INCX);
ശൂന്യം pow_gpu(പൂർണ്ണസംഖ്യ N, ദശാംശം ALPHA, ദശാംശം *X, പൂർണ്ണസംഖ്യ INCX, ദശാംശം *Y, പൂർണ്ണസംഖ്യ INCY);
ശൂന്യം mul_gpu(പൂർണ്ണസംഖ്യ N, ദശാംശം *X, പൂർണ്ണസംഖ്യ INCX, ദശാംശം *Y, പൂർണ്ണസംഖ്യ INCY);

ശൂന്യം mean_gpu(ദശാംശം *x, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial, ദശാംശം *mean);
ശൂന്യം variance_gpu(ദശാംശം *x, ദശാംശം *mean, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial, ദശാംശം *variance);
ശൂന്യം normalize_gpu(ദശാംശം *x, ദശാംശം *mean, ദശാംശം *variance, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial);
ശൂന്യം l2normalize_gpu(ദശാംശം *x, ദശാംശം *dx, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial);

ശൂന്യം normalize_delta_gpu(ദശാംശം *x, ദശാംശം *mean, ദശാംശം *variance, ദശാംശം *mean_delta, ദശാംശം *variance_delta, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial, ദശാംശം *delta);

ശൂന്യം fast_mean_delta_gpu(ദശാംശം *delta, ദശാംശം *variance, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial, ദശാംശം *mean_delta);
ശൂന്യം fast_variance_delta_gpu(ദശാംശം *x, ദശാംശം *delta, ദശാംശം *mean, ദശാംശം *variance, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial, ദശാംശം *variance_delta);

ശൂന്യം fast_variance_gpu(ദശാംശം *x, ദശാംശം *mean, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial, ദശാംശം *variance);
ശൂന്യം fast_mean_gpu(ദശാംശം *x, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial, ദശാംശം *mean);
ശൂന്യം shortcut_gpu(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w1, പൂർണ്ണസംഖ്യ h1, പൂർണ്ണസംഖ്യ c1, ദശാംശം *add, പൂർണ്ണസംഖ്യ w2, പൂർണ്ണസംഖ്യ h2, പൂർണ്ണസംഖ്യ c2, ദശാംശം s1, ദശാംശം s2, ദശാംശം *out);
ശൂന്യം scale_bias_gpu(ദശാംശം *output, ദശാംശം *biases, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size);
ശൂന്യം backward_scale_gpu(ദശാംശം *x_norm, ദശാംശം *delta, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size, ദശാംശം *scale_updates);
ശൂന്യം scale_bias_gpu(ദശാംശം *output, ദശാംശം *biases, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size);
ശൂന്യം add_bias_gpu(ദശാംശം *output, ദശാംശം *biases, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size);
ശൂന്യം backward_bias_gpu(ദശാംശം *bias_updates, ദശാംശം *delta, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size);

ശൂന്യം logistic_x_ent_gpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error);
ശൂന്യം softmax_x_ent_gpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error);
ശൂന്യം smooth_l1_gpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error);
ശൂന്യം l2_gpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error);
ശൂന്യം l1_gpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error);
ശൂന്യം wgan_gpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error);
ശൂന്യം weighted_delta_gpu(ദശാംശം *a, ദശാംശം *b, ദശാംശം *s, ദശാംശം *da, ദശാംശം *db, ദശാംശം *ds, പൂർണ്ണസംഖ്യ അക്കം, ദശാംശം *dc);
ശൂന്യം weighted_sum_gpu(ദശാംശം *a, ദശാംശം *b, ദശാംശം *s, പൂർണ്ണസംഖ്യ അക്കം, ദശാംശം *c);
ശൂന്യം mult_add_into_gpu(പൂർണ്ണസംഖ്യ അക്കം, ദശാംശം *a, ദശാംശം *b, ദശാംശം *c);
ശൂന്യം inter_gpu(പൂർണ്ണസംഖ്യ NX, ദശാംശം *X, പൂർണ്ണസംഖ്യ NY, ദശാംശം *Y, പൂർണ്ണസംഖ്യ B, ദശാംശം *OUT);
ശൂന്യം deinter_gpu(പൂർണ്ണസംഖ്യ NX, ദശാംശം *X, പൂർണ്ണസംഖ്യ NY, ദശാംശം *Y, പൂർണ്ണസംഖ്യ B, ദശാംശം *OUT);

ശൂന്യം reorg_gpu(ദശാംശം *x, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ forward, ദശാംശം *out);

ശൂന്യം softmax_gpu(ദശാംശം *നിവേശം, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ batch_offset, പൂർണ്ണസംഖ്യ groups, പൂർണ്ണസംഖ്യ group_offset, പൂർണ്ണസംഖ്യ stride, ദശാംശം temp, ദശാംശം *output);
ശൂന്യം adam_update_gpu(ദശാംശം *w, ദശാംശം *d, ദശാംശം *m, ദശാംശം *v, ദശാംശം B1, ദശാംശം B2, ദശാംശം eps, ദശാംശം decay, ദശാംശം rate, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ t);
ശൂന്യം adam_gpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *x, ദശാംശം *m, ദശാംശം *v, ദശാംശം B1, ദശാംശം B2, ദശാംശം rate, ദശാംശം eps, പൂർണ്ണസംഖ്യ t);

ശൂന്യം flatten_gpu(ദശാംശം *x, പൂർണ്ണസംഖ്യ spatial, പൂർണ്ണസംഖ്യ layers, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ forward, ദശാംശം *out);
ശൂന്യം softmax_വൃക്ഷം(ദശാംശം *നിവേശം, പൂർണ്ണസംഖ്യ spatial, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ stride, ദശാംശം temp, ദശാംശം *output, വൃക്ഷം hier);
ശൂന്യം upsample_gpu(ദശാംശം *in, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ forward, ദശാംശം scale, ദശാംശം *out);

#endif
#endif
