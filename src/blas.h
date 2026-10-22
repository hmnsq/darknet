#ifndef BLAS_H
#define BLAS_H
#அந்தர்பாவ "darknet.h"

வெறுமை flatten(தசம *x, முழு size, முழு layers, முழு batch, முழு forward);
வெறுமை pm(முழு M, முழு N, தசம *A);
தசம *random_matrix(முழு rows, முழு cols);
வெறுமை time_random_matrix(முழு TA, முழு TB, முழு m, முழு k, முழு n);
வெறுமை reorg_cpu(தசம *x, முழு w, முழு h, முழு c, முழு batch, முழு stride, முழு forward, தசம *out);

வெறுமை test_blas();

வெறுமை inter_cpu(முழு NX, தசம *X, முழு NY, தசம *Y, முழு B, தசம *OUT);
வெறுமை deinter_cpu(முழு NX, தசம *X, முழு NY, தசம *Y, முழு B, தசம *OUT);
வெறுமை mult_add_into_cpu(முழு N, தசம *X, தசம *Y, தசம *Z);

வெறுமை const_cpu(முழு N, தசம ALPHA, தசம *X, முழு INCX);
வெறுமை constrain_gpu(முழு N, தசம ALPHA, தசம * X, முழு INCX);
வெறுமை pow_cpu(முழு N, தசம ALPHA, தசம *X, முழு INCX, தசம *Y, முழு INCY);
வெறுமை mul_cpu(முழு N, தசம *X, முழு INCX, தசம *Y, முழு INCY);

முழு test_gpu_blas();
வெறுமை shortcut_cpu(முழு batch, முழு w1, முழு h1, முழு c1, தசம *add, முழு w2, முழு h2, முழு c2, தசம s1, தசம s2, தசம *out);

வெறுமை mean_cpu(தசம *x, முழு batch, முழு filters, முழு spatial, தசம *mean);
வெறுமை variance_cpu(தசம *x, தசம *mean, முழு batch, முழு filters, முழு spatial, தசம *variance);

வெறுமை scale_bias(தசம *output, தசம *scales, முழு batch, முழு n, முழு size);
வெறுமை backward_scale_cpu(தசம *x_norm, தசம *delta, முழு batch, முழு n, முழு size, தசம *scale_updates);
வெறுமை mean_delta_cpu(தசம *delta, தசம *variance, முழு batch, முழு filters, முழு spatial, தசம *mean_delta);
வெறுமை  variance_delta_cpu(தசம *x, தசம *delta, தசம *mean, தசம *variance, முழு batch, முழு filters, முழு spatial, தசம *variance_delta);
வெறுமை normalize_delta_cpu(தசம *x, தசம *mean, தசம *variance, தசம *mean_delta, தசம *variance_delta, முழு batch, முழு filters, முழு spatial, தசம *delta);
வெறுமை l2normalize_cpu(தசம *x, தசம *dx, முழு batch, முழு filters, முழு spatial);

வெறுமை smooth_l1_cpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error);
வெறுமை l2_cpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error);
வெறுமை l1_cpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error);
வெறுமை logistic_x_ent_cpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error);
வெறுமை softmax_x_ent_cpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error);
வெறுமை weighted_sum_cpu(தசம *a, தசம *b, தசம *s, முழு இலக்கம், தசம *c);
வெறுமை weighted_delta_cpu(தசம *a, தசம *b, தசம *s, தசம *da, தசம *db, தசம *ds, முழு n, தசம *dc);

வெறுமை softmax(தசம *உள்ளீடு, முழு n, தசம temp, முழு stride, தசம *output);
வெறுமை softmax_cpu(தசம *உள்ளீடு, முழு n, முழு batch, முழு batch_offset, முழு groups, முழு group_offset, முழு stride, தசம temp, தசம *output);
வெறுமை upsample_cpu(தசம *in, முழு w, முழு h, முழு c, முழு batch, முழு stride, முழு forward, தசம scale, தசம *out);

#ifdef GPU
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "tree.h"

வெறுமை axpy_gpu(முழு N, தசம ALPHA, தசம * X, முழு INCX, தசம * Y, முழு INCY);
வெறுமை axpy_gpu_offset(முழு N, தசம ALPHA, தசம * X, முழு OFFX, முழு INCX, தசம * Y, முழு OFFY, முழு INCY);
வெறுமை copy_gpu(முழு N, தசம * X, முழு INCX, தசம * Y, முழு INCY);
வெறுமை copy_gpu_offset(முழு N, தசம * X, முழு OFFX, முழு INCX, தசம * Y, முழு OFFY, முழு INCY);
வெறுமை add_gpu(முழு N, தசம ALPHA, தசம * X, முழு INCX);
வெறுமை supp_gpu(முழு N, தசம ALPHA, தசம * X, முழு INCX);
வெறுமை mask_gpu(முழு N, தசம * X, தசம mask_இலக்கம், தசம * mask, தசம val);
வெறுமை scale_mask_gpu(முழு N, தசம * X, தசம mask_இலக்கம், தசம * mask, தசம scale);
வெறுமை const_gpu(முழு N, தசம ALPHA, தசம *X, முழு INCX);
வெறுமை pow_gpu(முழு N, தசம ALPHA, தசம *X, முழு INCX, தசம *Y, முழு INCY);
வெறுமை mul_gpu(முழு N, தசம *X, முழு INCX, தசம *Y, முழு INCY);

வெறுமை mean_gpu(தசம *x, முழு batch, முழு filters, முழு spatial, தசம *mean);
வெறுமை variance_gpu(தசம *x, தசம *mean, முழு batch, முழு filters, முழு spatial, தசம *variance);
வெறுமை normalize_gpu(தசம *x, தசம *mean, தசம *variance, முழு batch, முழு filters, முழு spatial);
வெறுமை l2normalize_gpu(தசம *x, தசம *dx, முழு batch, முழு filters, முழு spatial);

வெறுமை normalize_delta_gpu(தசம *x, தசம *mean, தசம *variance, தசம *mean_delta, தசம *variance_delta, முழு batch, முழு filters, முழு spatial, தசம *delta);

வெறுமை fast_mean_delta_gpu(தசம *delta, தசம *variance, முழு batch, முழு filters, முழு spatial, தசம *mean_delta);
வெறுமை fast_variance_delta_gpu(தசம *x, தசம *delta, தசம *mean, தசம *variance, முழு batch, முழு filters, முழு spatial, தசம *variance_delta);

வெறுமை fast_variance_gpu(தசம *x, தசம *mean, முழு batch, முழு filters, முழு spatial, தசம *variance);
வெறுமை fast_mean_gpu(தசம *x, முழு batch, முழு filters, முழு spatial, தசம *mean);
வெறுமை shortcut_gpu(முழு batch, முழு w1, முழு h1, முழு c1, தசம *add, முழு w2, முழு h2, முழு c2, தசம s1, தசம s2, தசம *out);
வெறுமை scale_bias_gpu(தசம *output, தசம *biases, முழு batch, முழு n, முழு size);
வெறுமை backward_scale_gpu(தசம *x_norm, தசம *delta, முழு batch, முழு n, முழு size, தசம *scale_updates);
வெறுமை scale_bias_gpu(தசம *output, தசம *biases, முழு batch, முழு n, முழு size);
வெறுமை add_bias_gpu(தசம *output, தசம *biases, முழு batch, முழு n, முழு size);
வெறுமை backward_bias_gpu(தசம *bias_updates, தசம *delta, முழு batch, முழு n, முழு size);

வெறுமை logistic_x_ent_gpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error);
வெறுமை softmax_x_ent_gpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error);
வெறுமை smooth_l1_gpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error);
வெறுமை l2_gpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error);
வெறுமை l1_gpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error);
வெறுமை wgan_gpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error);
வெறுமை weighted_delta_gpu(தசம *a, தசம *b, தசம *s, தசம *da, தசம *db, தசம *ds, முழு இலக்கம், தசம *dc);
வெறுமை weighted_sum_gpu(தசம *a, தசம *b, தசம *s, முழு இலக்கம், தசம *c);
வெறுமை mult_add_into_gpu(முழு இலக்கம், தசம *a, தசம *b, தசம *c);
வெறுமை inter_gpu(முழு NX, தசம *X, முழு NY, தசம *Y, முழு B, தசம *OUT);
வெறுமை deinter_gpu(முழு NX, தசம *X, முழு NY, தசம *Y, முழு B, தசம *OUT);

வெறுமை reorg_gpu(தசம *x, முழு w, முழு h, முழு c, முழு batch, முழு stride, முழு forward, தசம *out);

வெறுமை softmax_gpu(தசம *உள்ளீடு, முழு n, முழு batch, முழு batch_offset, முழு groups, முழு group_offset, முழு stride, தசம temp, தசம *output);
வெறுமை adam_update_gpu(தசம *w, தசம *d, தசம *m, தசம *v, தசம B1, தசம B2, தசம eps, தசம decay, தசம rate, முழு n, முழு batch, முழு t);
வெறுமை adam_gpu(முழு n, தசம *x, தசம *m, தசம *v, தசம B1, தசம B2, தசம rate, தசம eps, முழு t);

வெறுமை flatten_gpu(தசம *x, முழு spatial, முழு layers, முழு batch, முழு forward, தசம *out);
வெறுமை softmax_மரம்(தசம *உள்ளீடு, முழு spatial, முழு batch, முழு stride, தசம temp, தசம *output, மரம் hier);
வெறுமை upsample_gpu(தசம *in, முழு w, முழு h, முழு c, முழு batch, முழு stride, முழு forward, தசம scale, தசம *out);

#endif
#endif
