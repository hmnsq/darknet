#ifndef BLAS_H
#define BLAS_H
#অন্তর্ভাব "darknet.h"

শূন্য flatten(দশমিক *x, পূর্ণসংখ্যা আকার_2, পূর্ণসংখ্যা layers, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা forward);
শূন্য pm(পূর্ণসংখ্যা M, পূর্ণসংখ্যা N, দশমিক *A);
দশমিক *random_matrix(পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols);
শূন্য time_random_matrix(পূর্ণসংখ্যা TA, পূর্ণসংখ্যা TB, পূর্ণসংখ্যা m, পূর্ণসংখ্যা k, পূর্ণসংখ্যা n);
শূন্য reorg_cpu(দশমিক *x, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা forward, দশমিক *out);

শূন্য test_blas();

শূন্য inter_cpu(পূর্ণসংখ্যা NX, দশমিক *X, পূর্ণসংখ্যা NY, দশমিক *Y, পূর্ণসংখ্যা B, দশমিক *OUT);
শূন্য deinter_cpu(পূর্ণসংখ্যা NX, দশমিক *X, পূর্ণসংখ্যা NY, দশমিক *Y, পূর্ণসংখ্যা B, দশমিক *OUT);
শূন্য mult_add_into_cpu(পূর্ণসংখ্যা N, দশমিক *X, দশমিক *Y, দশমিক *Z);

শূন্য const_cpu(পূর্ণসংখ্যা N, দশমিক ALPHA, দশমিক *X, পূর্ণসংখ্যা INCX);
শূন্য constrain_gpu(পূর্ণসংখ্যা N, দশমিক ALPHA, দশমিক * X, পূর্ণসংখ্যা INCX);
শূন্য pow_cpu(পূর্ণসংখ্যা N, দশমিক ALPHA, দশমিক *X, পূর্ণসংখ্যা INCX, দশমিক *Y, পূর্ণসংখ্যা INCY);
শূন্য mul_cpu(পূর্ণসংখ্যা N, দশমিক *X, পূর্ণসংখ্যা INCX, দশমিক *Y, পূর্ণসংখ্যা INCY);

পূর্ণসংখ্যা test_gpu_blas();
শূন্য shortcut_cpu(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w1, পূর্ণসংখ্যা h1, পূর্ণসংখ্যা c1, দশমিক *add, পূর্ণসংখ্যা w2, পূর্ণসংখ্যা h2, পূর্ণসংখ্যা c2, দশমিক s1, দশমিক s2, দশমিক *out);

শূন্য mean_cpu(দশমিক *x, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *mean);
শূন্য variance_cpu(দশমিক *x, দশমিক *mean, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *variance);

শূন্য scale_bias(দশমিক *output, দশমিক *scales, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2);
শূন্য backward_scale_cpu(দশমিক *x_norm, দশমিক *delta, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2, দশমিক *scale_updates);
শূন্য mean_delta_cpu(দশমিক *delta, দশমিক *variance, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *mean_delta);
শূন্য  variance_delta_cpu(দশমিক *x, দশমিক *delta, দশমিক *mean, দশমিক *variance, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *variance_delta);
শূন্য normalize_delta_cpu(দশমিক *x, দশমিক *mean, দশমিক *variance, দশমিক *mean_delta, দশমিক *variance_delta, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *delta);
শূন্য l2normalize_cpu(দশমিক *x, দশমিক *dx, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial);

শূন্য smooth_l1_cpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error);
শূন্য l2_cpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error);
শূন্য l1_cpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error);
শূন্য logistic_x_ent_cpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error);
শূন্য softmax_x_ent_cpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error);
শূন্য weighted_যোগ_cpu(দশমিক *a, দশমিক *b, দশমিক *s, পূর্ণসংখ্যা অঙ্ক, দশমিক *c);
শূন্য weighted_delta_cpu(দশমিক *a, দশমিক *b, দশমিক *s, দশমিক *da, দশমিক *db, দশমিক *ds, পূর্ণসংখ্যা n, দশমিক *dc);

শূন্য softmax(দশমিক *প্রবেশ, পূর্ণসংখ্যা n, দশমিক অস্থায়ী, পূর্ণসংখ্যা stride, দশমিক *output);
শূন্য softmax_cpu(দশমিক *প্রবেশ, পূর্ণসংখ্যা n, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা batch_offset, পূর্ণসংখ্যা groups, পূর্ণসংখ্যা group_offset, পূর্ণসংখ্যা stride, দশমিক অস্থায়ী, দশমিক *output);
শূন্য upsample_cpu(দশমিক *in, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা forward, দশমিক scale, দশমিক *out);

#ifdef GPU
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "tree.h"

শূন্য axpy_gpu(পূর্ণসংখ্যা N, দশমিক ALPHA, দশমিক * X, পূর্ণসংখ্যা INCX, দশমিক * Y, পূর্ণসংখ্যা INCY);
শূন্য axpy_gpu_offset(পূর্ণসংখ্যা N, দশমিক ALPHA, দশমিক * X, পূর্ণসংখ্যা OFFX, পূর্ণসংখ্যা INCX, দশমিক * Y, পূর্ণসংখ্যা OFFY, পূর্ণসংখ্যা INCY);
শূন্য copy_gpu(পূর্ণসংখ্যা N, দশমিক * X, পূর্ণসংখ্যা INCX, দশমিক * Y, পূর্ণসংখ্যা INCY);
শূন্য copy_gpu_offset(পূর্ণসংখ্যা N, দশমিক * X, পূর্ণসংখ্যা OFFX, পূর্ণসংখ্যা INCX, দশমিক * Y, পূর্ণসংখ্যা OFFY, পূর্ণসংখ্যা INCY);
শূন্য add_gpu(পূর্ণসংখ্যা N, দশমিক ALPHA, দশমিক * X, পূর্ণসংখ্যা INCX);
শূন্য supp_gpu(পূর্ণসংখ্যা N, দশমিক ALPHA, দশমিক * X, পূর্ণসংখ্যা INCX);
শূন্য mask_gpu(পূর্ণসংখ্যা N, দশমিক * X, দশমিক mask_অঙ্ক, দশমিক * mask, দশমিক val);
শূন্য scale_mask_gpu(পূর্ণসংখ্যা N, দশমিক * X, দশমিক mask_অঙ্ক, দশমিক * mask, দশমিক scale);
শূন্য const_gpu(পূর্ণসংখ্যা N, দশমিক ALPHA, দশমিক *X, পূর্ণসংখ্যা INCX);
শূন্য pow_gpu(পূর্ণসংখ্যা N, দশমিক ALPHA, দশমিক *X, পূর্ণসংখ্যা INCX, দশমিক *Y, পূর্ণসংখ্যা INCY);
শূন্য mul_gpu(পূর্ণসংখ্যা N, দশমিক *X, পূর্ণসংখ্যা INCX, দশমিক *Y, পূর্ণসংখ্যা INCY);

শূন্য mean_gpu(দশমিক *x, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *mean);
শূন্য variance_gpu(দশমিক *x, দশমিক *mean, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *variance);
শূন্য normalize_gpu(দশমিক *x, দশমিক *mean, দশমিক *variance, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial);
শূন্য l2normalize_gpu(দশমিক *x, দশমিক *dx, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial);

শূন্য normalize_delta_gpu(দশমিক *x, দশমিক *mean, দশমিক *variance, দশমিক *mean_delta, দশমিক *variance_delta, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *delta);

শূন্য fast_mean_delta_gpu(দশমিক *delta, দশমিক *variance, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *mean_delta);
শূন্য fast_variance_delta_gpu(দশমিক *x, দশমিক *delta, দশমিক *mean, দশমিক *variance, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *variance_delta);

শূন্য fast_variance_gpu(দশমিক *x, দশমিক *mean, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *variance);
শূন্য fast_mean_gpu(দশমিক *x, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *mean);
শূন্য shortcut_gpu(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w1, পূর্ণসংখ্যা h1, পূর্ণসংখ্যা c1, দশমিক *add, পূর্ণসংখ্যা w2, পূর্ণসংখ্যা h2, পূর্ণসংখ্যা c2, দশমিক s1, দশমিক s2, দশমিক *out);
শূন্য scale_bias_gpu(দশমিক *output, দশমিক *biases, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2);
শূন্য backward_scale_gpu(দশমিক *x_norm, দশমিক *delta, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2, দশমিক *scale_updates);
শূন্য scale_bias_gpu(দশমিক *output, দশমিক *biases, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2);
শূন্য add_bias_gpu(দশমিক *output, দশমিক *biases, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2);
শূন্য backward_bias_gpu(দশমিক *bias_updates, দশমিক *delta, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2);

শূন্য logistic_x_ent_gpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error);
শূন্য softmax_x_ent_gpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error);
শূন্য smooth_l1_gpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error);
শূন্য l2_gpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error);
শূন্য l1_gpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error);
শূন্য wgan_gpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error);
শূন্য weighted_delta_gpu(দশমিক *a, দশমিক *b, দশমিক *s, দশমিক *da, দশমিক *db, দশমিক *ds, পূর্ণসংখ্যা অঙ্ক, দশমিক *dc);
শূন্য weighted_যোগ_gpu(দশমিক *a, দশমিক *b, দশমিক *s, পূর্ণসংখ্যা অঙ্ক, দশমিক *c);
শূন্য mult_add_into_gpu(পূর্ণসংখ্যা অঙ্ক, দশমিক *a, দশমিক *b, দশমিক *c);
শূন্য inter_gpu(পূর্ণসংখ্যা NX, দশমিক *X, পূর্ণসংখ্যা NY, দশমিক *Y, পূর্ণসংখ্যা B, দশমিক *OUT);
শূন্য deinter_gpu(পূর্ণসংখ্যা NX, দশমিক *X, পূর্ণসংখ্যা NY, দশমিক *Y, পূর্ণসংখ্যা B, দশমিক *OUT);

শূন্য reorg_gpu(দশমিক *x, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা forward, দশমিক *out);

শূন্য softmax_gpu(দশমিক *প্রবেশ, পূর্ণসংখ্যা n, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা batch_offset, পূর্ণসংখ্যা groups, পূর্ণসংখ্যা group_offset, পূর্ণসংখ্যা stride, দশমিক অস্থায়ী, দশমিক *output);
শূন্য adam_update_gpu(দশমিক *w, দশমিক *d, দশমিক *m, দশমিক *v, দশমিক B1, দশমিক B2, দশমিক eps, দশমিক decay, দশমিক rate, পূর্ণসংখ্যা n, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা t);
শূন্য adam_gpu(পূর্ণসংখ্যা n, দশমিক *x, দশমিক *m, দশমিক *v, দশমিক B1, দশমিক B2, দশমিক rate, দশমিক eps, পূর্ণসংখ্যা t);

শূন্য flatten_gpu(দশমিক *x, পূর্ণসংখ্যা spatial, পূর্ণসংখ্যা layers, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা forward, দশমিক *out);
শূন্য softmax_বৃক্ষ(দশমিক *প্রবেশ, পূর্ণসংখ্যা spatial, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা stride, দশমিক অস্থায়ী, দশমিক *output, বৃক্ষ hier);
শূন্য upsample_gpu(দশমিক *in, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা forward, দশমিক scale, দশমিক *out);

#endif
#endif
