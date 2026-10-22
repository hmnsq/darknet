#ifndef BLAS_H
#define BLAS_H
#અન્તર્ભાવ "darknet.h"

શૂન્ય flatten(દશાંશ *x, પૂર્ણાંક size, પૂર્ણાંક layers, પૂર્ણાંક batch, પૂર્ણાંક forward);
શૂન્ય pm(પૂર્ણાંક M, પૂર્ણાંક N, દશાંશ *A);
દશાંશ *random_matrix(પૂર્ણાંક rows, પૂર્ણાંક cols);
શૂન્ય time_random_matrix(પૂર્ણાંક TA, પૂર્ણાંક TB, પૂર્ણાંક m, પૂર્ણાંક k, પૂર્ણાંક n);
શૂન્ય reorg_cpu(દશાંશ *x, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c, પૂર્ણાંક batch, પૂર્ણાંક stride, પૂર્ણાંક forward, દશાંશ *out);

શૂન્ય test_blas();

શૂન્ય inter_cpu(પૂર્ણાંક NX, દશાંશ *X, પૂર્ણાંક NY, દશાંશ *Y, પૂર્ણાંક B, દશાંશ *OUT);
શૂન્ય deinter_cpu(પૂર્ણાંક NX, દશાંશ *X, પૂર્ણાંક NY, દશાંશ *Y, પૂર્ણાંક B, દશાંશ *OUT);
શૂન્ય mult_add_into_cpu(પૂર્ણાંક N, દશાંશ *X, દશાંશ *Y, દશાંશ *Z);

શૂન્ય const_cpu(પૂર્ણાંક N, દશાંશ ALPHA, દશાંશ *X, પૂર્ણાંક INCX);
શૂન્ય constrain_gpu(પૂર્ણાંક N, દશાંશ ALPHA, દશાંશ * X, પૂર્ણાંક INCX);
શૂન્ય pow_cpu(પૂર્ણાંક N, દશાંશ ALPHA, દશાંશ *X, પૂર્ણાંક INCX, દશાંશ *Y, પૂર્ણાંક INCY);
શૂન્ય mul_cpu(પૂર્ણાંક N, દશાંશ *X, પૂર્ણાંક INCX, દશાંશ *Y, પૂર્ણાંક INCY);

પૂર્ણાંક test_gpu_blas();
શૂન્ય shortcut_cpu(પૂર્ણાંક batch, પૂર્ણાંક w1, પૂર્ણાંક h1, પૂર્ણાંક c1, દશાંશ *add, પૂર્ણાંક w2, પૂર્ણાંક h2, પૂર્ણાંક c2, દશાંશ s1, દશાંશ s2, દશાંશ *out);

શૂન્ય mean_cpu(દશાંશ *x, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *mean);
શૂન્ય variance_cpu(દશાંશ *x, દશાંશ *mean, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *variance);

શૂન્ય scale_bias(દશાંશ *output, દશાંશ *scales, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size);
શૂન્ય backward_scale_cpu(દશાંશ *x_norm, દશાંશ *delta, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size, દશાંશ *scale_updates);
શૂન્ય mean_delta_cpu(દશાંશ *delta, દશાંશ *variance, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *mean_delta);
શૂન્ય  variance_delta_cpu(દશાંશ *x, દશાંશ *delta, દશાંશ *mean, દશાંશ *variance, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *variance_delta);
શૂન્ય normalize_delta_cpu(દશાંશ *x, દશાંશ *mean, દશાંશ *variance, દશાંશ *mean_delta, દશાંશ *variance_delta, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *delta);
શૂન્ય l2normalize_cpu(દશાંશ *x, દશાંશ *dx, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial);

શૂન્ય smooth_l1_cpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error);
શૂન્ય l2_cpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error);
શૂન્ય l1_cpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error);
શૂન્ય logistic_x_ent_cpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error);
શૂન્ય softmax_x_ent_cpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error);
શૂન્ય weighted_sum_cpu(દશાંશ *a, દશાંશ *b, દશાંશ *s, પૂર્ણાંક અંક, દશાંશ *c);
શૂન્ય weighted_delta_cpu(દશાંશ *a, દશાંશ *b, દશાંશ *s, દશાંશ *da, દશાંશ *db, દશાંશ *ds, પૂર્ણાંક n, દશાંશ *dc);

શૂન્ય softmax(દશાંશ *આગત, પૂર્ણાંક n, દશાંશ temp, પૂર્ણાંક stride, દશાંશ *output);
શૂન્ય softmax_cpu(દશાંશ *આગત, પૂર્ણાંક n, પૂર્ણાંક batch, પૂર્ણાંક batch_offset, પૂર્ણાંક groups, પૂર્ણાંક group_offset, પૂર્ણાંક stride, દશાંશ temp, દશાંશ *output);
શૂન્ય upsample_cpu(દશાંશ *in, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c, પૂર્ણાંક batch, પૂર્ણાંક stride, પૂર્ણાંક forward, દશાંશ scale, દશાંશ *out);

#ifdef GPU
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "tree.h"

શૂન્ય axpy_gpu(પૂર્ણાંક N, દશાંશ ALPHA, દશાંશ * X, પૂર્ણાંક INCX, દશાંશ * Y, પૂર્ણાંક INCY);
શૂન્ય axpy_gpu_offset(પૂર્ણાંક N, દશાંશ ALPHA, દશાંશ * X, પૂર્ણાંક OFFX, પૂર્ણાંક INCX, દશાંશ * Y, પૂર્ણાંક OFFY, પૂર્ણાંક INCY);
શૂન્ય copy_gpu(પૂર્ણાંક N, દશાંશ * X, પૂર્ણાંક INCX, દશાંશ * Y, પૂર્ણાંક INCY);
શૂન્ય copy_gpu_offset(પૂર્ણાંક N, દશાંશ * X, પૂર્ણાંક OFFX, પૂર્ણાંક INCX, દશાંશ * Y, પૂર્ણાંક OFFY, પૂર્ણાંક INCY);
શૂન્ય add_gpu(પૂર્ણાંક N, દશાંશ ALPHA, દશાંશ * X, પૂર્ણાંક INCX);
શૂન્ય supp_gpu(પૂર્ણાંક N, દશાંશ ALPHA, દશાંશ * X, પૂર્ણાંક INCX);
શૂન્ય mask_gpu(પૂર્ણાંક N, દશાંશ * X, દશાંશ mask_અંક, દશાંશ * mask, દશાંશ val);
શૂન્ય scale_mask_gpu(પૂર્ણાંક N, દશાંશ * X, દશાંશ mask_અંક, દશાંશ * mask, દશાંશ scale);
શૂન્ય const_gpu(પૂર્ણાંક N, દશાંશ ALPHA, દશાંશ *X, પૂર્ણાંક INCX);
શૂન્ય pow_gpu(પૂર્ણાંક N, દશાંશ ALPHA, દશાંશ *X, પૂર્ણાંક INCX, દશાંશ *Y, પૂર્ણાંક INCY);
શૂન્ય mul_gpu(પૂર્ણાંક N, દશાંશ *X, પૂર્ણાંક INCX, દશાંશ *Y, પૂર્ણાંક INCY);

શૂન્ય mean_gpu(દશાંશ *x, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *mean);
શૂન્ય variance_gpu(દશાંશ *x, દશાંશ *mean, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *variance);
શૂન્ય normalize_gpu(દશાંશ *x, દશાંશ *mean, દશાંશ *variance, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial);
શૂન્ય l2normalize_gpu(દશાંશ *x, દશાંશ *dx, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial);

શૂન્ય normalize_delta_gpu(દશાંશ *x, દશાંશ *mean, દશાંશ *variance, દશાંશ *mean_delta, દશાંશ *variance_delta, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *delta);

શૂન્ય fast_mean_delta_gpu(દશાંશ *delta, દશાંશ *variance, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *mean_delta);
શૂન્ય fast_variance_delta_gpu(દશાંશ *x, દશાંશ *delta, દશાંશ *mean, દશાંશ *variance, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *variance_delta);

શૂન્ય fast_variance_gpu(દશાંશ *x, દશાંશ *mean, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *variance);
શૂન્ય fast_mean_gpu(દશાંશ *x, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *mean);
શૂન્ય shortcut_gpu(પૂર્ણાંક batch, પૂર્ણાંક w1, પૂર્ણાંક h1, પૂર્ણાંક c1, દશાંશ *add, પૂર્ણાંક w2, પૂર્ણાંક h2, પૂર્ણાંક c2, દશાંશ s1, દશાંશ s2, દશાંશ *out);
શૂન્ય scale_bias_gpu(દશાંશ *output, દશાંશ *biases, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size);
શૂન્ય backward_scale_gpu(દશાંશ *x_norm, દશાંશ *delta, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size, દશાંશ *scale_updates);
શૂન્ય scale_bias_gpu(દશાંશ *output, દશાંશ *biases, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size);
શૂન્ય add_bias_gpu(દશાંશ *output, દશાંશ *biases, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size);
શૂન્ય backward_bias_gpu(દશાંશ *bias_updates, દશાંશ *delta, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size);

શૂન્ય logistic_x_ent_gpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error);
શૂન્ય softmax_x_ent_gpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error);
શૂન્ય smooth_l1_gpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error);
શૂન્ય l2_gpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error);
શૂન્ય l1_gpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error);
શૂન્ય wgan_gpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error);
શૂન્ય weighted_delta_gpu(દશાંશ *a, દશાંશ *b, દશાંશ *s, દશાંશ *da, દશાંશ *db, દશાંશ *ds, પૂર્ણાંક અંક, દશાંશ *dc);
શૂન્ય weighted_sum_gpu(દશાંશ *a, દશાંશ *b, દશાંશ *s, પૂર્ણાંક અંક, દશાંશ *c);
શૂન્ય mult_add_into_gpu(પૂર્ણાંક અંક, દશાંશ *a, દશાંશ *b, દશાંશ *c);
શૂન્ય inter_gpu(પૂર્ણાંક NX, દશાંશ *X, પૂર્ણાંક NY, દશાંશ *Y, પૂર્ણાંક B, દશાંશ *OUT);
શૂન્ય deinter_gpu(પૂર્ણાંક NX, દશાંશ *X, પૂર્ણાંક NY, દશાંશ *Y, પૂર્ણાંક B, દશાંશ *OUT);

શૂન્ય reorg_gpu(દશાંશ *x, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c, પૂર્ણાંક batch, પૂર્ણાંક stride, પૂર્ણાંક forward, દશાંશ *out);

શૂન્ય softmax_gpu(દશાંશ *આગત, પૂર્ણાંક n, પૂર્ણાંક batch, પૂર્ણાંક batch_offset, પૂર્ણાંક groups, પૂર્ણાંક group_offset, પૂર્ણાંક stride, દશાંશ temp, દશાંશ *output);
શૂન્ય adam_update_gpu(દશાંશ *w, દશાંશ *d, દશાંશ *m, દશાંશ *v, દશાંશ B1, દશાંશ B2, દશાંશ eps, દશાંશ decay, દશાંશ rate, પૂર્ણાંક n, પૂર્ણાંક batch, પૂર્ણાંક t);
શૂન્ય adam_gpu(પૂર્ણાંક n, દશાંશ *x, દશાંશ *m, દશાંશ *v, દશાંશ B1, દશાંશ B2, દશાંશ rate, દશાંશ eps, પૂર્ણાંક t);

શૂન્ય flatten_gpu(દશાંશ *x, પૂર્ણાંક spatial, પૂર્ણાંક layers, પૂર્ણાંક batch, પૂર્ણાંક forward, દશાંશ *out);
શૂન્ય softmax_વૃક્ષ(દશાંશ *આગત, પૂર્ણાંક spatial, પૂર્ણાંક batch, પૂર્ણાંક stride, દશાંશ temp, દશાંશ *output, વૃક્ષ hier);
શૂન્ય upsample_gpu(દશાંશ *in, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c, પૂર્ણાંક batch, પૂર્ણાંક stride, પૂર્ણાંક forward, દશાંશ scale, દશાંશ *out);

#endif
#endif
