#جے_بنا_تعریف BLAS_H
#تعریف_کرو BLAS_H
#ضمن "darknet.h"

فراغ flatten(عائم *x, صحيح ناپ, صحيح layers, صحيح batch, صحيح forward);
فراغ pm(صحيح M, صحيح N, عائم *A);
عائم *random_میٹرکس(صحيح صفاں, صحيح خانے_گنتی);
فراغ ویلا_random_میٹرکس(صحيح TA, صحيح TB, صحيح m, صحيح k, صحيح n);
فراغ reorg_cpu(عائم *x, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم *باہرلا);

فراغ test_blas();

فراغ inter_cpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *باہرلا_2);
فراغ deinter_cpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *باہرلا_2);
فراغ mult_جوڑو_into_cpu(صحيح N, عائم *X, عائم *Y, عائم *Z);

فراغ const_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX);
فراغ constrain_gpu(صحيح N, عائم ALPHA, عائم * X, صحيح INCX);
فراغ pow_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX, عائم *Y, صحيح INCY);
فراغ mul_cpu(صحيح N, عائم *X, صحيح INCX, عائم *Y, صحيح INCY);

صحيح test_gpu_blas();
فراغ shortcut_cpu(صحيح batch, صحيح w1, صحيح h1, صحيح c1, عائم *جوڑو, صحيح w2, صحيح h2, صحيح c2, عائم s1, عائم s2, عائم *باہرلا);

فراغ اوسط_cpu(عائم *x, صحيح batch, صحيح filters, صحيح spatial, عائم *اوسط);
فراغ variance_cpu(عائم *x, عائم *اوسط, صحيح batch, صحيح filters, صحيح spatial, عائم *variance);

فراغ scale_bias(عائم *output, عائم *scales, صحيح batch, صحيح n, صحيح ناپ);
فراغ backward_scale_cpu(عائم *x_norm, عائم *delta, صحيح batch, صحيح n, صحيح ناپ, عائم *scale_updates);
فراغ اوسط_delta_cpu(عائم *delta, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *اوسط_delta);
فراغ  variance_delta_cpu(عائم *x, عائم *delta, عائم *اوسط, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *variance_delta);
فراغ normalize_delta_cpu(عائم *x, عائم *اوسط, عائم *variance, عائم *اوسط_delta, عائم *variance_delta, صحيح batch, صحيح filters, صحيح spatial, عائم *delta);
فراغ l2normalize_cpu(عائم *x, عائم *dx, صحيح batch, صحيح filters, صحيح spatial);

فراغ smooth_l1_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ l2_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ l1_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ logistic_x_ent_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ softmax_x_ent_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ weighted_جوڑ_cpu(عائم *a, عائم *b, عائم *s, صحيح عدد, عائم *c);
فراغ weighted_delta_cpu(عائم *a, عائم *b, عائم *s, عائم *da, عائم *db, عائم *ds, صحيح n, عائم *dc);

فراغ softmax(عائم *داخلہ, صحيح n, عائم عارضی, صحيح stride, عائم *output);
فراغ softmax_cpu(عائم *داخلہ, صحيح n, صحيح batch, صحيح batch_وتھ, صحيح groups, صحيح گروہ_وتھ, صحيح stride, عائم عارضی, عائم *output);
فراغ upsample_cpu(عائم *وچ, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم scale, عائم *باہرلا);

#جے_تعریف_شدہ GPU
#ضمن "cuda.h"
#ضمن "tree.h"

فراغ axpy_gpu(صحيح N, عائم ALPHA, عائم * X, صحيح INCX, عائم * Y, صحيح INCY);
فراغ axpy_gpu_وتھ(صحيح N, عائم ALPHA, عائم * X, صحيح OFFX, صحيح INCX, عائم * Y, صحيح OFFY, صحيح INCY);
فراغ copy_gpu(صحيح N, عائم * X, صحيح INCX, عائم * Y, صحيح INCY);
فراغ copy_gpu_وتھ(صحيح N, عائم * X, صحيح OFFX, صحيح INCX, عائم * Y, صحيح OFFY, صحيح INCY);
فراغ جوڑو_gpu(صحيح N, عائم ALPHA, عائم * X, صحيح INCX);
فراغ supp_gpu(صحيح N, عائم ALPHA, عائم * X, صحيح INCX);
فراغ نقاب_gpu(صحيح N, عائم * X, عائم نقاب_عدد, عائم * نقاب, عائم val);
فراغ scale_نقاب_gpu(صحيح N, عائم * X, عائم نقاب_عدد, عائم * نقاب, عائم scale);
فراغ const_gpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX);
فراغ pow_gpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX, عائم *Y, صحيح INCY);
فراغ mul_gpu(صحيح N, عائم *X, صحيح INCX, عائم *Y, صحيح INCY);

فراغ اوسط_gpu(عائم *x, صحيح batch, صحيح filters, صحيح spatial, عائم *اوسط);
فراغ variance_gpu(عائم *x, عائم *اوسط, صحيح batch, صحيح filters, صحيح spatial, عائم *variance);
فراغ normalize_gpu(عائم *x, عائم *اوسط, عائم *variance, صحيح batch, صحيح filters, صحيح spatial);
فراغ l2normalize_gpu(عائم *x, عائم *dx, صحيح batch, صحيح filters, صحيح spatial);

فراغ normalize_delta_gpu(عائم *x, عائم *اوسط, عائم *variance, عائم *اوسط_delta, عائم *variance_delta, صحيح batch, صحيح filters, صحيح spatial, عائم *delta);

فراغ fast_اوسط_delta_gpu(عائم *delta, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *اوسط_delta);
فراغ fast_variance_delta_gpu(عائم *x, عائم *delta, عائم *اوسط, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *variance_delta);

فراغ fast_variance_gpu(عائم *x, عائم *اوسط, صحيح batch, صحيح filters, صحيح spatial, عائم *variance);
فراغ fast_اوسط_gpu(عائم *x, صحيح batch, صحيح filters, صحيح spatial, عائم *اوسط);
فراغ shortcut_gpu(صحيح batch, صحيح w1, صحيح h1, صحيح c1, عائم *جوڑو, صحيح w2, صحيح h2, صحيح c2, عائم s1, عائم s2, عائم *باہرلا);
فراغ scale_bias_gpu(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح ناپ);
فراغ backward_scale_gpu(عائم *x_norm, عائم *delta, صحيح batch, صحيح n, صحيح ناپ, عائم *scale_updates);
فراغ scale_bias_gpu(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح ناپ);
فراغ جوڑو_bias_gpu(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح ناپ);
فراغ backward_bias_gpu(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح ناپ);

فراغ logistic_x_ent_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ softmax_x_ent_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ smooth_l1_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ l2_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ l1_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ wgan_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ weighted_delta_gpu(عائم *a, عائم *b, عائم *s, عائم *da, عائم *db, عائم *ds, صحيح عدد, عائم *dc);
فراغ weighted_جوڑ_gpu(عائم *a, عائم *b, عائم *s, صحيح عدد, عائم *c);
فراغ mult_جوڑو_into_gpu(صحيح عدد, عائم *a, عائم *b, عائم *c);
فراغ inter_gpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *باہرلا_2);
فراغ deinter_gpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *باہرلا_2);

فراغ reorg_gpu(عائم *x, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم *باہرلا);

فراغ softmax_gpu(عائم *داخلہ, صحيح n, صحيح batch, صحيح batch_وتھ, صحيح groups, صحيح گروہ_وتھ, صحيح stride, عائم عارضی, عائم *output);
فراغ adam_update_gpu(عائم *w, عائم *d, عائم *m, عائم *v, عائم B1, عائم B2, عائم eps, عائم decay, عائم شرح, صحيح n, صحيح batch, صحيح t);
فراغ adam_gpu(صحيح n, عائم *x, عائم *m, عائم *v, عائم B1, عائم B2, عائم شرح, عائم eps, صحيح t);

فراغ flatten_gpu(عائم *x, صحيح spatial, صحيح layers, صحيح batch, صحيح forward, عائم *باہرلا);
فراغ softmax_درخت(عائم *داخلہ, صحيح spatial, صحيح batch, صحيح stride, عائم عارضی, عائم *output, درخت hier);
فراغ upsample_gpu(عائم *وچ, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم scale, عائم *باہرلا);

#جے_انت
#جے_انت
