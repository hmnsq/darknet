#ifndef BLAS_H
#define BLAS_H
#अन्तर्भाव "darknet.h"

शून्य flatten(भिन्न *x, पूर्णाङ्क size, पूर्णाङ्क layers, पूर्णाङ्क batch, पूर्णाङ्क forward);
शून्य pm(पूर्णाङ्क M, पूर्णाङ्क N, भिन्न *A);
भिन्न *random_matrix(पूर्णाङ्क rows, पूर्णाङ्क cols);
शून्य time_random_matrix(पूर्णाङ्क TA, पूर्णाङ्क TB, पूर्णाङ्क m, पूर्णाङ्क k, पूर्णाङ्क n);
शून्य reorg_cpu(भिन्न *x, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c, पूर्णाङ्क batch, पूर्णाङ्क stride, पूर्णाङ्क forward, भिन्न *out);

शून्य test_blas();

शून्य inter_cpu(पूर्णाङ्क NX, भिन्न *X, पूर्णाङ्क NY, भिन्न *Y, पूर्णाङ्क B, भिन्न *OUT);
शून्य deinter_cpu(पूर्णाङ्क NX, भिन्न *X, पूर्णाङ्क NY, भिन्न *Y, पूर्णाङ्क B, भिन्न *OUT);
शून्य mult_add_into_cpu(पूर्णाङ्क N, भिन्न *X, भिन्न *Y, भिन्न *Z);

शून्य const_cpu(पूर्णाङ्क N, भिन्न ALPHA, भिन्न *X, पूर्णाङ्क INCX);
शून्य constrain_gpu(पूर्णाङ्क N, भिन्न ALPHA, भिन्न * X, पूर्णाङ्क INCX);
शून्य pow_cpu(पूर्णाङ्क N, भिन्न ALPHA, भिन्न *X, पूर्णाङ्क INCX, भिन्न *Y, पूर्णाङ्क INCY);
शून्य mul_cpu(पूर्णाङ्क N, भिन्न *X, पूर्णाङ्क INCX, भिन्न *Y, पूर्णाङ्क INCY);

पूर्णाङ्क test_gpu_blas();
शून्य shortcut_cpu(पूर्णाङ्क batch, पूर्णाङ्क w1, पूर्णाङ्क h1, पूर्णाङ्क c1, भिन्न *add, पूर्णाङ्क w2, पूर्णाङ्क h2, पूर्णाङ्क c2, भिन्न s1, भिन्न s2, भिन्न *out);

शून्य mean_cpu(भिन्न *x, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *mean);
शून्य variance_cpu(भिन्न *x, भिन्न *mean, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *variance);

शून्य scale_bias(भिन्न *output, भिन्न *scales, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क size);
शून्य backward_scale_cpu(भिन्न *x_norm, भिन्न *delta, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क size, भिन्न *scale_updates);
शून्य mean_delta_cpu(भिन्न *delta, भिन्न *variance, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *mean_delta);
शून्य  variance_delta_cpu(भिन्न *x, भिन्न *delta, भिन्न *mean, भिन्न *variance, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *variance_delta);
शून्य normalize_delta_cpu(भिन्न *x, भिन्न *mean, भिन्न *variance, भिन्न *mean_delta, भिन्न *variance_delta, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *delta);
शून्य l2normalize_cpu(भिन्न *x, भिन्न *dx, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial);

शून्य smooth_l1_cpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error);
शून्य l2_cpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error);
शून्य l1_cpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error);
शून्य logistic_x_ent_cpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error);
शून्य softmax_x_ent_cpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error);
शून्य weighted_sum_cpu(भिन्न *a, भिन्न *b, भिन्न *s, पूर्णाङ्क अङ्कः, भिन्न *c);
शून्य weighted_delta_cpu(भिन्न *a, भिन्न *b, भिन्न *s, भिन्न *da, भिन्न *db, भिन्न *ds, पूर्णाङ्क n, भिन्न *dc);

शून्य softmax(भिन्न *निवेशः, पूर्णाङ्क n, भिन्न temp, पूर्णाङ्क stride, भिन्न *output);
शून्य softmax_cpu(भिन्न *निवेशः, पूर्णाङ्क n, पूर्णाङ्क batch, पूर्णाङ्क batch_offset, पूर्णाङ्क groups, पूर्णाङ्क group_offset, पूर्णाङ्क stride, भिन्न temp, भिन्न *output);
शून्य upsample_cpu(भिन्न *in, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c, पूर्णाङ्क batch, पूर्णाङ्क stride, पूर्णाङ्क forward, भिन्न scale, भिन्न *out);

#ifdef GPU
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "tree.h"

शून्य axpy_gpu(पूर्णाङ्क N, भिन्न ALPHA, भिन्न * X, पूर्णाङ्क INCX, भिन्न * Y, पूर्णाङ्क INCY);
शून्य axpy_gpu_offset(पूर्णाङ्क N, भिन्न ALPHA, भिन्न * X, पूर्णाङ्क OFFX, पूर्णाङ्क INCX, भिन्न * Y, पूर्णाङ्क OFFY, पूर्णाङ्क INCY);
शून्य copy_gpu(पूर्णाङ्क N, भिन्न * X, पूर्णाङ्क INCX, भिन्न * Y, पूर्णाङ्क INCY);
शून्य copy_gpu_offset(पूर्णाङ्क N, भिन्न * X, पूर्णाङ्क OFFX, पूर्णाङ्क INCX, भिन्न * Y, पूर्णाङ्क OFFY, पूर्णाङ्क INCY);
शून्य add_gpu(पूर्णाङ्क N, भिन्न ALPHA, भिन्न * X, पूर्णाङ्क INCX);
शून्य supp_gpu(पूर्णाङ्क N, भिन्न ALPHA, भिन्न * X, पूर्णाङ्क INCX);
शून्य mask_gpu(पूर्णाङ्क N, भिन्न * X, भिन्न mask_अङ्कः, भिन्न * mask, भिन्न val);
शून्य scale_mask_gpu(पूर्णाङ्क N, भिन्न * X, भिन्न mask_अङ्कः, भिन्न * mask, भिन्न scale);
शून्य const_gpu(पूर्णाङ्क N, भिन्न ALPHA, भिन्न *X, पूर्णाङ्क INCX);
शून्य pow_gpu(पूर्णाङ्क N, भिन्न ALPHA, भिन्न *X, पूर्णाङ्क INCX, भिन्न *Y, पूर्णाङ्क INCY);
शून्य mul_gpu(पूर्णाङ्क N, भिन्न *X, पूर्णाङ्क INCX, भिन्न *Y, पूर्णाङ्क INCY);

शून्य mean_gpu(भिन्न *x, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *mean);
शून्य variance_gpu(भिन्न *x, भिन्न *mean, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *variance);
शून्य normalize_gpu(भिन्न *x, भिन्न *mean, भिन्न *variance, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial);
शून्य l2normalize_gpu(भिन्न *x, भिन्न *dx, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial);

शून्य normalize_delta_gpu(भिन्न *x, भिन्न *mean, भिन्न *variance, भिन्न *mean_delta, भिन्न *variance_delta, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *delta);

शून्य fast_mean_delta_gpu(भिन्न *delta, भिन्न *variance, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *mean_delta);
शून्य fast_variance_delta_gpu(भिन्न *x, भिन्न *delta, भिन्न *mean, भिन्न *variance, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *variance_delta);

शून्य fast_variance_gpu(भिन्न *x, भिन्न *mean, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *variance);
शून्य fast_mean_gpu(भिन्न *x, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *mean);
शून्य shortcut_gpu(पूर्णाङ्क batch, पूर्णाङ्क w1, पूर्णाङ्क h1, पूर्णाङ्क c1, भिन्न *add, पूर्णाङ्क w2, पूर्णाङ्क h2, पूर्णाङ्क c2, भिन्न s1, भिन्न s2, भिन्न *out);
शून्य scale_bias_gpu(भिन्न *output, भिन्न *biases, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क size);
शून्य backward_scale_gpu(भिन्न *x_norm, भिन्न *delta, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क size, भिन्न *scale_updates);
शून्य scale_bias_gpu(भिन्न *output, भिन्न *biases, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क size);
शून्य add_bias_gpu(भिन्न *output, भिन्न *biases, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क size);
शून्य backward_bias_gpu(भिन्न *bias_updates, भिन्न *delta, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क size);

शून्य logistic_x_ent_gpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error);
शून्य softmax_x_ent_gpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error);
शून्य smooth_l1_gpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error);
शून्य l2_gpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error);
शून्य l1_gpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error);
शून्य wgan_gpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error);
शून्य weighted_delta_gpu(भिन्न *a, भिन्न *b, भिन्न *s, भिन्न *da, भिन्न *db, भिन्न *ds, पूर्णाङ्क अङ्कः, भिन्न *dc);
शून्य weighted_sum_gpu(भिन्न *a, भिन्न *b, भिन्न *s, पूर्णाङ्क अङ्कः, भिन्न *c);
शून्य mult_add_into_gpu(पूर्णाङ्क अङ्कः, भिन्न *a, भिन्न *b, भिन्न *c);
शून्य inter_gpu(पूर्णाङ्क NX, भिन्न *X, पूर्णाङ्क NY, भिन्न *Y, पूर्णाङ्क B, भिन्न *OUT);
शून्य deinter_gpu(पूर्णाङ्क NX, भिन्न *X, पूर्णाङ्क NY, भिन्न *Y, पूर्णाङ्क B, भिन्न *OUT);

शून्य reorg_gpu(भिन्न *x, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c, पूर्णाङ्क batch, पूर्णाङ्क stride, पूर्णाङ्क forward, भिन्न *out);

शून्य softmax_gpu(भिन्न *निवेशः, पूर्णाङ्क n, पूर्णाङ्क batch, पूर्णाङ्क batch_offset, पूर्णाङ्क groups, पूर्णाङ्क group_offset, पूर्णाङ्क stride, भिन्न temp, भिन्न *output);
शून्य adam_update_gpu(भिन्न *w, भिन्न *d, भिन्न *m, भिन्न *v, भिन्न B1, भिन्न B2, भिन्न eps, भिन्न decay, भिन्न rate, पूर्णाङ्क n, पूर्णाङ्क batch, पूर्णाङ्क t);
शून्य adam_gpu(पूर्णाङ्क n, भिन्न *x, भिन्न *m, भिन्न *v, भिन्न B1, भिन्न B2, भिन्न rate, भिन्न eps, पूर्णाङ्क t);

शून्य flatten_gpu(भिन्न *x, पूर्णाङ्क spatial, पूर्णाङ्क layers, पूर्णाङ्क batch, पूर्णाङ्क forward, भिन्न *out);
शून्य softmax_वृक्षः(भिन्न *निवेशः, पूर्णाङ्क spatial, पूर्णाङ्क batch, पूर्णाङ्क stride, भिन्न temp, भिन्न *output, वृक्षः hier);
शून्य upsample_gpu(भिन्न *in, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c, पूर्णाङ्क batch, पूर्णाङ्क stride, पूर्णाङ्क forward, भिन्न scale, भिन्न *out);

#endif
#endif
