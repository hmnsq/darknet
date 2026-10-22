#अगर_अघोषित BLAS_H
#घोषणा BLAS_H
#समावेश "darknet.h"

व्योम flatten(भग्न *x, पूर्णांक आकार, पूर्णांक layers, पूर्णांक batch, पूर्णांक forward);
व्योम pm(पूर्णांक M, पूर्णांक N, भग्न *A);
भग्न *random_matrix(पूर्णांक rows, पूर्णांक cols);
व्योम time_random_matrix(पूर्णांक TA, पूर्णांक TB, पूर्णांक m, पूर्णांक k, पूर्णांक n);
व्योम reorg_cpu(भग्न *x, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक batch, पूर्णांक stride, पूर्णांक forward, भग्न *out);

व्योम test_blas();

व्योम inter_cpu(पूर्णांक NX, भग्न *X, पूर्णांक NY, भग्न *Y, पूर्णांक B, भग्न *OUT);
व्योम deinter_cpu(पूर्णांक NX, भग्न *X, पूर्णांक NY, भग्न *Y, पूर्णांक B, भग्न *OUT);
व्योम mult_add_into_cpu(पूर्णांक N, भग्न *X, भग्न *Y, भग्न *Z);

व्योम const_cpu(पूर्णांक N, भग्न ALPHA, भग्न *X, पूर्णांक INCX);
व्योम constrain_gpu(पूर्णांक N, भग्न ALPHA, भग्न * X, पूर्णांक INCX);
व्योम pow_cpu(पूर्णांक N, भग्न ALPHA, भग्न *X, पूर्णांक INCX, भग्न *Y, पूर्णांक INCY);
व्योम mul_cpu(पूर्णांक N, भग्न *X, पूर्णांक INCX, भग्न *Y, पूर्णांक INCY);

पूर्णांक test_gpu_blas();
व्योम shortcut_cpu(पूर्णांक batch, पूर्णांक w1, पूर्णांक h1, पूर्णांक c1, भग्न *add, पूर्णांक w2, पूर्णांक h2, पूर्णांक c2, भग्न s1, भग्न s2, भग्न *out);

व्योम mean_cpu(भग्न *x, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, भग्न *mean);
व्योम variance_cpu(भग्न *x, भग्न *mean, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, भग्न *variance);

व्योम scale_bias(भग्न *output, भग्न *scales, पूर्णांक batch, पूर्णांक n, पूर्णांक आकार);
व्योम backward_scale_cpu(भग्न *x_norm, भग्न *delta, पूर्णांक batch, पूर्णांक n, पूर्णांक आकार, भग्न *scale_updates);
व्योम mean_delta_cpu(भग्न *delta, भग्न *variance, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, भग्न *mean_delta);
व्योम  variance_delta_cpu(भग्न *x, भग्न *delta, भग्न *mean, भग्न *variance, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, भग्न *variance_delta);
व्योम normalize_delta_cpu(भग्न *x, भग्न *mean, भग्न *variance, भग्न *mean_delta, भग्न *variance_delta, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, भग्न *delta);
व्योम l2normalize_cpu(भग्न *x, भग्न *dx, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial);

व्योम smooth_l1_cpu(पूर्णांक n, भग्न *pred, भग्न *truth, भग्न *delta, भग्न *error);
व्योम l2_cpu(पूर्णांक n, भग्न *pred, भग्न *truth, भग्न *delta, भग्न *error);
व्योम l1_cpu(पूर्णांक n, भग्न *pred, भग्न *truth, भग्न *delta, भग्न *error);
व्योम logistic_x_ent_cpu(पूर्णांक n, भग्न *pred, भग्न *truth, भग्न *delta, भग्न *error);
व्योम softmax_x_ent_cpu(पूर्णांक n, भग्न *pred, भग्न *truth, भग्न *delta, भग्न *error);
व्योम weighted_योग_cpu(भग्न *a, भग्न *b, भग्न *s, पूर्णांक अंक, भग्न *c);
व्योम weighted_delta_cpu(भग्न *a, भग्न *b, भग्न *s, भग्न *da, भग्न *db, भग्न *ds, पूर्णांक n, भग्न *dc);

व्योम softmax(भग्न *निवेश, पूर्णांक n, भग्न अस्थायी, पूर्णांक stride, भग्न *output);
व्योम softmax_cpu(भग्न *निवेश, पूर्णांक n, पूर्णांक batch, पूर्णांक batch_offset, पूर्णांक groups, पूर्णांक group_offset, पूर्णांक stride, भग्न अस्थायी, भग्न *output);
व्योम upsample_cpu(भग्न *in, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक batch, पूर्णांक stride, पूर्णांक forward, भग्न scale, भग्न *out);

#अगर_घोषित GPU
#समावेश "cuda.h"
#समावेश "tree.h"

व्योम axpy_gpu(पूर्णांक N, भग्न ALPHA, भग्न * X, पूर्णांक INCX, भग्न * Y, पूर्णांक INCY);
व्योम axpy_gpu_offset(पूर्णांक N, भग्न ALPHA, भग्न * X, पूर्णांक OFFX, पूर्णांक INCX, भग्न * Y, पूर्णांक OFFY, पूर्णांक INCY);
व्योम copy_gpu(पूर्णांक N, भग्न * X, पूर्णांक INCX, भग्न * Y, पूर्णांक INCY);
व्योम copy_gpu_offset(पूर्णांक N, भग्न * X, पूर्णांक OFFX, पूर्णांक INCX, भग्न * Y, पूर्णांक OFFY, पूर्णांक INCY);
व्योम add_gpu(पूर्णांक N, भग्न ALPHA, भग्न * X, पूर्णांक INCX);
व्योम supp_gpu(पूर्णांक N, भग्न ALPHA, भग्न * X, पूर्णांक INCX);
व्योम mask_gpu(पूर्णांक N, भग्न * X, भग्न mask_अंक, भग्न * mask, भग्न val);
व्योम scale_mask_gpu(पूर्णांक N, भग्न * X, भग्न mask_अंक, भग्न * mask, भग्न scale);
व्योम const_gpu(पूर्णांक N, भग्न ALPHA, भग्न *X, पूर्णांक INCX);
व्योम pow_gpu(पूर्णांक N, भग्न ALPHA, भग्न *X, पूर्णांक INCX, भग्न *Y, पूर्णांक INCY);
व्योम mul_gpu(पूर्णांक N, भग्न *X, पूर्णांक INCX, भग्न *Y, पूर्णांक INCY);

व्योम mean_gpu(भग्न *x, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, भग्न *mean);
व्योम variance_gpu(भग्न *x, भग्न *mean, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, भग्न *variance);
व्योम normalize_gpu(भग्न *x, भग्न *mean, भग्न *variance, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial);
व्योम l2normalize_gpu(भग्न *x, भग्न *dx, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial);

व्योम normalize_delta_gpu(भग्न *x, भग्न *mean, भग्न *variance, भग्न *mean_delta, भग्न *variance_delta, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, भग्न *delta);

व्योम fast_mean_delta_gpu(भग्न *delta, भग्न *variance, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, भग्न *mean_delta);
व्योम fast_variance_delta_gpu(भग्न *x, भग्न *delta, भग्न *mean, भग्न *variance, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, भग्न *variance_delta);

व्योम fast_variance_gpu(भग्न *x, भग्न *mean, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, भग्न *variance);
व्योम fast_mean_gpu(भग्न *x, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, भग्न *mean);
व्योम shortcut_gpu(पूर्णांक batch, पूर्णांक w1, पूर्णांक h1, पूर्णांक c1, भग्न *add, पूर्णांक w2, पूर्णांक h2, पूर्णांक c2, भग्न s1, भग्न s2, भग्न *out);
व्योम scale_bias_gpu(भग्न *output, भग्न *biases, पूर्णांक batch, पूर्णांक n, पूर्णांक आकार);
व्योम backward_scale_gpu(भग्न *x_norm, भग्न *delta, पूर्णांक batch, पूर्णांक n, पूर्णांक आकार, भग्न *scale_updates);
व्योम scale_bias_gpu(भग्न *output, भग्न *biases, पूर्णांक batch, पूर्णांक n, पूर्णांक आकार);
व्योम add_bias_gpu(भग्न *output, भग्न *biases, पूर्णांक batch, पूर्णांक n, पूर्णांक आकार);
व्योम backward_bias_gpu(भग्न *bias_updates, भग्न *delta, पूर्णांक batch, पूर्णांक n, पूर्णांक आकार);

व्योम logistic_x_ent_gpu(पूर्णांक n, भग्न *pred, भग्न *truth, भग्न *delta, भग्न *error);
व्योम softmax_x_ent_gpu(पूर्णांक n, भग्न *pred, भग्न *truth, भग्न *delta, भग्न *error);
व्योम smooth_l1_gpu(पूर्णांक n, भग्न *pred, भग्न *truth, भग्न *delta, भग्न *error);
व्योम l2_gpu(पूर्णांक n, भग्न *pred, भग्न *truth, भग्न *delta, भग्न *error);
व्योम l1_gpu(पूर्णांक n, भग्न *pred, भग्न *truth, भग्न *delta, भग्न *error);
व्योम wgan_gpu(पूर्णांक n, भग्न *pred, भग्न *truth, भग्न *delta, भग्न *error);
व्योम weighted_delta_gpu(भग्न *a, भग्न *b, भग्न *s, भग्न *da, भग्न *db, भग्न *ds, पूर्णांक अंक, भग्न *dc);
व्योम weighted_योग_gpu(भग्न *a, भग्न *b, भग्न *s, पूर्णांक अंक, भग्न *c);
व्योम mult_add_into_gpu(पूर्णांक अंक, भग्न *a, भग्न *b, भग्न *c);
व्योम inter_gpu(पूर्णांक NX, भग्न *X, पूर्णांक NY, भग्न *Y, पूर्णांक B, भग्न *OUT);
व्योम deinter_gpu(पूर्णांक NX, भग्न *X, पूर्णांक NY, भग्न *Y, पूर्णांक B, भग्न *OUT);

व्योम reorg_gpu(भग्न *x, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक batch, पूर्णांक stride, पूर्णांक forward, भग्न *out);

व्योम softmax_gpu(भग्न *निवेश, पूर्णांक n, पूर्णांक batch, पूर्णांक batch_offset, पूर्णांक groups, पूर्णांक group_offset, पूर्णांक stride, भग्न अस्थायी, भग्न *output);
व्योम adam_update_gpu(भग्न *w, भग्न *d, भग्न *m, भग्न *v, भग्न B1, भग्न B2, भग्न eps, भग्न decay, भग्न rate, पूर्णांक n, पूर्णांक batch, पूर्णांक t);
व्योम adam_gpu(पूर्णांक n, भग्न *x, भग्न *m, भग्न *v, भग्न B1, भग्न B2, भग्न rate, भग्न eps, पूर्णांक t);

व्योम flatten_gpu(भग्न *x, पूर्णांक spatial, पूर्णांक layers, पूर्णांक batch, पूर्णांक forward, भग्न *out);
व्योम softmax_वृक्ष(भग्न *निवेश, पूर्णांक spatial, पूर्णांक batch, पूर्णांक stride, भग्न अस्थायी, भग्न *output, वृक्ष hier);
व्योम upsample_gpu(भग्न *in, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक batch, पूर्णांक stride, पूर्णांक forward, भग्न scale, भग्न *out);

#पूर्ण_अगर
#पूर्ण_अगर
