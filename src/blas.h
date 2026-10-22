#ifndef BLAS_H
#define BLAS_H
#অন্তর্ভাব "darknet.h"

শূন্য flatten(ভিন্ন *x, পূর্ণাঙ্ক size, পূর্ণাঙ্ক layers, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক forward);
শূন্য pm(পূর্ণাঙ্ক M, পূর্ণাঙ্ক N, ভিন্ন *A);
ভিন্ন *random_matrix(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols);
শূন্য time_random_matrix(পূর্ণাঙ্ক TA, পূর্ণাঙ্ক TB, পূর্ণাঙ্ক m, পূর্ণাঙ্ক k, পূর্ণাঙ্ক n);
শূন্য reorg_cpu(ভিন্ন *x, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক forward, ভিন্ন *out);

শূন্য test_blas();

শূন্য inter_cpu(পূর্ণাঙ্ক NX, ভিন্ন *X, পূর্ণাঙ্ক NY, ভিন্ন *Y, পূর্ণাঙ্ক B, ভিন্ন *OUT);
শূন্য deinter_cpu(পূর্ণাঙ্ক NX, ভিন্ন *X, পূর্ণাঙ্ক NY, ভিন্ন *Y, পূর্ণাঙ্ক B, ভিন্ন *OUT);
শূন্য mult_add_into_cpu(পূর্ণাঙ্ক N, ভিন্ন *X, ভিন্ন *Y, ভিন্ন *Z);

শূন্য const_cpu(পূর্ণাঙ্ক N, ভিন্ন ALPHA, ভিন্ন *X, পূর্ণাঙ্ক INCX);
শূন্য constrain_gpu(পূর্ণাঙ্ক N, ভিন্ন ALPHA, ভিন্ন * X, পূর্ণাঙ্ক INCX);
শূন্য pow_cpu(পূর্ণাঙ্ক N, ভিন্ন ALPHA, ভিন্ন *X, পূর্ণাঙ্ক INCX, ভিন্ন *Y, পূর্ণাঙ্ক INCY);
শূন্য mul_cpu(পূর্ণাঙ্ক N, ভিন্ন *X, পূর্ণাঙ্ক INCX, ভিন্ন *Y, পূর্ণাঙ্ক INCY);

পূর্ণাঙ্ক test_gpu_blas();
শূন্য shortcut_cpu(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w1, পূর্ণাঙ্ক h1, পূর্ণাঙ্ক c1, ভিন্ন *add, পূর্ণাঙ্ক w2, পূর্ণাঙ্ক h2, পূর্ণাঙ্ক c2, ভিন্ন s1, ভিন্ন s2, ভিন্ন *out);

শূন্য mean_cpu(ভিন্ন *x, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial, ভিন্ন *mean);
শূন্য variance_cpu(ভিন্ন *x, ভিন্ন *mean, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial, ভিন্ন *variance);

শূন্য scale_bias(ভিন্ন *output, ভিন্ন *scales, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size);
শূন্য backward_scale_cpu(ভিন্ন *x_norm, ভিন্ন *delta, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size, ভিন্ন *scale_updates);
শূন্য mean_delta_cpu(ভিন্ন *delta, ভিন্ন *variance, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial, ভিন্ন *mean_delta);
শূন্য  variance_delta_cpu(ভিন্ন *x, ভিন্ন *delta, ভিন্ন *mean, ভিন্ন *variance, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial, ভিন্ন *variance_delta);
শূন্য normalize_delta_cpu(ভিন্ন *x, ভিন্ন *mean, ভিন্ন *variance, ভিন্ন *mean_delta, ভিন্ন *variance_delta, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial, ভিন্ন *delta);
শূন্য l2normalize_cpu(ভিন্ন *x, ভিন্ন *dx, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial);

শূন্য smooth_l1_cpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error);
শূন্য l2_cpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error);
শূন্য l1_cpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error);
শূন্য logistic_x_ent_cpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error);
শূন্য softmax_x_ent_cpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error);
শূন্য weighted_sum_cpu(ভিন্ন *a, ভিন্ন *b, ভিন্ন *s, পূর্ণাঙ্ক অঙ্ক, ভিন্ন *c);
শূন্য weighted_delta_cpu(ভিন্ন *a, ভিন্ন *b, ভিন্ন *s, ভিন্ন *da, ভিন্ন *db, ভিন্ন *ds, পূর্ণাঙ্ক n, ভিন্ন *dc);

শূন্য softmax(ভিন্ন *প্রবেশ, পূর্ণাঙ্ক n, ভিন্ন temp, পূর্ণাঙ্ক stride, ভিন্ন *output);
শূন্য softmax_cpu(ভিন্ন *প্রবেশ, পূর্ণাঙ্ক n, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক batch_offset, পূর্ণাঙ্ক groups, পূর্ণাঙ্ক group_offset, পূর্ণাঙ্ক stride, ভিন্ন temp, ভিন্ন *output);
শূন্য upsample_cpu(ভিন্ন *in, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক forward, ভিন্ন scale, ভিন্ন *out);

#ifdef GPU
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "tree.h"

শূন্য axpy_gpu(পূর্ণাঙ্ক N, ভিন্ন ALPHA, ভিন্ন * X, পূর্ণাঙ্ক INCX, ভিন্ন * Y, পূর্ণাঙ্ক INCY);
শূন্য axpy_gpu_offset(পূর্ণাঙ্ক N, ভিন্ন ALPHA, ভিন্ন * X, পূর্ণাঙ্ক OFFX, পূর্ণাঙ্ক INCX, ভিন্ন * Y, পূর্ণাঙ্ক OFFY, পূর্ণাঙ্ক INCY);
শূন্য copy_gpu(পূর্ণাঙ্ক N, ভিন্ন * X, পূর্ণাঙ্ক INCX, ভিন্ন * Y, পূর্ণাঙ্ক INCY);
শূন্য copy_gpu_offset(পূর্ণাঙ্ক N, ভিন্ন * X, পূর্ণাঙ্ক OFFX, পূর্ণাঙ্ক INCX, ভিন্ন * Y, পূর্ণাঙ্ক OFFY, পূর্ণাঙ্ক INCY);
শূন্য add_gpu(পূর্ণাঙ্ক N, ভিন্ন ALPHA, ভিন্ন * X, পূর্ণাঙ্ক INCX);
শূন্য supp_gpu(পূর্ণাঙ্ক N, ভিন্ন ALPHA, ভিন্ন * X, পূর্ণাঙ্ক INCX);
শূন্য mask_gpu(পূর্ণাঙ্ক N, ভিন্ন * X, ভিন্ন mask_অঙ্ক, ভিন্ন * mask, ভিন্ন val);
শূন্য scale_mask_gpu(পূর্ণাঙ্ক N, ভিন্ন * X, ভিন্ন mask_অঙ্ক, ভিন্ন * mask, ভিন্ন scale);
শূন্য const_gpu(পূর্ণাঙ্ক N, ভিন্ন ALPHA, ভিন্ন *X, পূর্ণাঙ্ক INCX);
শূন্য pow_gpu(পূর্ণাঙ্ক N, ভিন্ন ALPHA, ভিন্ন *X, পূর্ণাঙ্ক INCX, ভিন্ন *Y, পূর্ণাঙ্ক INCY);
শূন্য mul_gpu(পূর্ণাঙ্ক N, ভিন্ন *X, পূর্ণাঙ্ক INCX, ভিন্ন *Y, পূর্ণাঙ্ক INCY);

শূন্য mean_gpu(ভিন্ন *x, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial, ভিন্ন *mean);
শূন্য variance_gpu(ভিন্ন *x, ভিন্ন *mean, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial, ভিন্ন *variance);
শূন্য normalize_gpu(ভিন্ন *x, ভিন্ন *mean, ভিন্ন *variance, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial);
শূন্য l2normalize_gpu(ভিন্ন *x, ভিন্ন *dx, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial);

শূন্য normalize_delta_gpu(ভিন্ন *x, ভিন্ন *mean, ভিন্ন *variance, ভিন্ন *mean_delta, ভিন্ন *variance_delta, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial, ভিন্ন *delta);

শূন্য fast_mean_delta_gpu(ভিন্ন *delta, ভিন্ন *variance, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial, ভিন্ন *mean_delta);
শূন্য fast_variance_delta_gpu(ভিন্ন *x, ভিন্ন *delta, ভিন্ন *mean, ভিন্ন *variance, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial, ভিন্ন *variance_delta);

শূন্য fast_variance_gpu(ভিন্ন *x, ভিন্ন *mean, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial, ভিন্ন *variance);
শূন্য fast_mean_gpu(ভিন্ন *x, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial, ভিন্ন *mean);
শূন্য shortcut_gpu(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w1, পূর্ণাঙ্ক h1, পূর্ণাঙ্ক c1, ভিন্ন *add, পূর্ণাঙ্ক w2, পূর্ণাঙ্ক h2, পূর্ণাঙ্ক c2, ভিন্ন s1, ভিন্ন s2, ভিন্ন *out);
শূন্য scale_bias_gpu(ভিন্ন *output, ভিন্ন *biases, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size);
শূন্য backward_scale_gpu(ভিন্ন *x_norm, ভিন্ন *delta, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size, ভিন্ন *scale_updates);
শূন্য scale_bias_gpu(ভিন্ন *output, ভিন্ন *biases, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size);
শূন্য add_bias_gpu(ভিন্ন *output, ভিন্ন *biases, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size);
শূন্য backward_bias_gpu(ভিন্ন *bias_updates, ভিন্ন *delta, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size);

শূন্য logistic_x_ent_gpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error);
শূন্য softmax_x_ent_gpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error);
শূন্য smooth_l1_gpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error);
শূন্য l2_gpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error);
শূন্য l1_gpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error);
শূন্য wgan_gpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error);
শূন্য weighted_delta_gpu(ভিন্ন *a, ভিন্ন *b, ভিন্ন *s, ভিন্ন *da, ভিন্ন *db, ভিন্ন *ds, পূর্ণাঙ্ক অঙ্ক, ভিন্ন *dc);
শূন্য weighted_sum_gpu(ভিন্ন *a, ভিন্ন *b, ভিন্ন *s, পূর্ণাঙ্ক অঙ্ক, ভিন্ন *c);
শূন্য mult_add_into_gpu(পূর্ণাঙ্ক অঙ্ক, ভিন্ন *a, ভিন্ন *b, ভিন্ন *c);
শূন্য inter_gpu(পূর্ণাঙ্ক NX, ভিন্ন *X, পূর্ণাঙ্ক NY, ভিন্ন *Y, পূর্ণাঙ্ক B, ভিন্ন *OUT);
শূন্য deinter_gpu(পূর্ণাঙ্ক NX, ভিন্ন *X, পূর্ণাঙ্ক NY, ভিন্ন *Y, পূর্ণাঙ্ক B, ভিন্ন *OUT);

শূন্য reorg_gpu(ভিন্ন *x, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক forward, ভিন্ন *out);

শূন্য softmax_gpu(ভিন্ন *প্রবেশ, পূর্ণাঙ্ক n, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক batch_offset, পূর্ণাঙ্ক groups, পূর্ণাঙ্ক group_offset, পূর্ণাঙ্ক stride, ভিন্ন temp, ভিন্ন *output);
শূন্য adam_update_gpu(ভিন্ন *w, ভিন্ন *d, ভিন্ন *m, ভিন্ন *v, ভিন্ন B1, ভিন্ন B2, ভিন্ন eps, ভিন্ন decay, ভিন্ন rate, পূর্ণাঙ্ক n, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক t);
শূন্য adam_gpu(পূর্ণাঙ্ক n, ভিন্ন *x, ভিন্ন *m, ভিন্ন *v, ভিন্ন B1, ভিন্ন B2, ভিন্ন rate, ভিন্ন eps, পূর্ণাঙ্ক t);

শূন্য flatten_gpu(ভিন্ন *x, পূর্ণাঙ্ক spatial, পূর্ণাঙ্ক layers, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক forward, ভিন্ন *out);
শূন্য softmax_বৃক্ষ(ভিন্ন *প্রবেশ, পূর্ণাঙ্ক spatial, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক stride, ভিন্ন temp, ভিন্ন *output, বৃক্ষ hier);
শূন্য upsample_gpu(ভিন্ন *in, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক forward, ভিন্ন scale, ভিন্ন *out);

#endif
#endif
