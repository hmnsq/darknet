#اگر_تعریف_نشده BLAS_H
#تعریف_کن BLAS_H
#ضمن "darknet.h"

فراغ flatten(عائم *x, صحيح اندازه, صحيح layers, صحيح batch, صحيح forward);
فراغ pm(صحيح M, صحيح N, عائم *A);
عائم *random_ماتریس(صحيح ردیفها, صحيح تعداد_ستون);
فراغ زمان_random_ماتریس(صحيح TA, صحيح TB, صحيح m, صحيح k, صحيح n);
فراغ reorg_cpu(عائم *x, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم *بیرون);

فراغ test_blas();

فراغ inter_cpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *بیرون_2);
فراغ deinter_cpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *بیرون_2);
فراغ mult_جمع_کن_into_cpu(صحيح N, عائم *X, عائم *Y, عائم *Z);

فراغ const_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX);
فراغ constrain_gpu(صحيح N, عائم ALPHA, عائم * X, صحيح INCX);
فراغ pow_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX, عائم *Y, صحيح INCY);
فراغ mul_cpu(صحيح N, عائم *X, صحيح INCX, عائم *Y, صحيح INCY);

صحيح test_gpu_blas();
فراغ shortcut_cpu(صحيح batch, صحيح w1, صحيح h1, صحيح c1, عائم *جمع_کن, صحيح w2, صحيح h2, صحيح c2, عائم s1, عائم s2, عائم *بیرون);

فراغ میانگین_cpu(عائم *x, صحيح batch, صحيح filters, صحيح spatial, عائم *میانگین);
فراغ variance_cpu(عائم *x, عائم *میانگین, صحيح batch, صحيح filters, صحيح spatial, عائم *variance);

فراغ scale_bias(عائم *output, عائم *scales, صحيح batch, صحيح n, صحيح اندازه);
فراغ backward_scale_cpu(عائم *x_norm, عائم *delta, صحيح batch, صحيح n, صحيح اندازه, عائم *scale_updates);
فراغ میانگین_delta_cpu(عائم *delta, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *میانگین_delta);
فراغ  variance_delta_cpu(عائم *x, عائم *delta, عائم *میانگین, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *variance_delta);
فراغ normalize_delta_cpu(عائم *x, عائم *میانگین, عائم *variance, عائم *میانگین_delta, عائم *variance_delta, صحيح batch, صحيح filters, صحيح spatial, عائم *delta);
فراغ l2normalize_cpu(عائم *x, عائم *dx, صحيح batch, صحيح filters, صحيح spatial);

فراغ smooth_l1_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ l2_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ l1_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ logistic_x_ent_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ softmax_x_ent_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ weighted_جمع_cpu(عائم *a, عائم *b, عائم *s, صحيح رقم, عائم *c);
فراغ weighted_delta_cpu(عائم *a, عائم *b, عائم *s, عائم *da, عائم *db, عائم *ds, صحيح n, عائم *dc);

فراغ softmax(عائم *درون_داد, صحيح n, عائم موقت, صحيح stride, عائم *output);
فراغ softmax_cpu(عائم *درون_داد, صحيح n, صحيح batch, صحيح batch_جابه_جایی, صحيح groups, صحيح گروه_جابه_جایی, صحيح stride, عائم موقت, عائم *output);
فراغ upsample_cpu(عائم *درون, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم scale, عائم *بیرون);

#اگر_تعریف_شده GPU
#ضمن "cuda.h"
#ضمن "tree.h"

فراغ axpy_gpu(صحيح N, عائم ALPHA, عائم * X, صحيح INCX, عائم * Y, صحيح INCY);
فراغ axpy_gpu_جابه_جایی(صحيح N, عائم ALPHA, عائم * X, صحيح OFFX, صحيح INCX, عائم * Y, صحيح OFFY, صحيح INCY);
فراغ copy_gpu(صحيح N, عائم * X, صحيح INCX, عائم * Y, صحيح INCY);
فراغ copy_gpu_جابه_جایی(صحيح N, عائم * X, صحيح OFFX, صحيح INCX, عائم * Y, صحيح OFFY, صحيح INCY);
فراغ جمع_کن_gpu(صحيح N, عائم ALPHA, عائم * X, صحيح INCX);
فراغ supp_gpu(صحيح N, عائم ALPHA, عائم * X, صحيح INCX);
فراغ ماسک_gpu(صحيح N, عائم * X, عائم ماسک_رقم, عائم * ماسک, عائم val);
فراغ scale_ماسک_gpu(صحيح N, عائم * X, عائم ماسک_رقم, عائم * ماسک, عائم scale);
فراغ const_gpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX);
فراغ pow_gpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX, عائم *Y, صحيح INCY);
فراغ mul_gpu(صحيح N, عائم *X, صحيح INCX, عائم *Y, صحيح INCY);

فراغ میانگین_gpu(عائم *x, صحيح batch, صحيح filters, صحيح spatial, عائم *میانگین);
فراغ variance_gpu(عائم *x, عائم *میانگین, صحيح batch, صحيح filters, صحيح spatial, عائم *variance);
فراغ normalize_gpu(عائم *x, عائم *میانگین, عائم *variance, صحيح batch, صحيح filters, صحيح spatial);
فراغ l2normalize_gpu(عائم *x, عائم *dx, صحيح batch, صحيح filters, صحيح spatial);

فراغ normalize_delta_gpu(عائم *x, عائم *میانگین, عائم *variance, عائم *میانگین_delta, عائم *variance_delta, صحيح batch, صحيح filters, صحيح spatial, عائم *delta);

فراغ fast_میانگین_delta_gpu(عائم *delta, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *میانگین_delta);
فراغ fast_variance_delta_gpu(عائم *x, عائم *delta, عائم *میانگین, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *variance_delta);

فراغ fast_variance_gpu(عائم *x, عائم *میانگین, صحيح batch, صحيح filters, صحيح spatial, عائم *variance);
فراغ fast_میانگین_gpu(عائم *x, صحيح batch, صحيح filters, صحيح spatial, عائم *میانگین);
فراغ shortcut_gpu(صحيح batch, صحيح w1, صحيح h1, صحيح c1, عائم *جمع_کن, صحيح w2, صحيح h2, صحيح c2, عائم s1, عائم s2, عائم *بیرون);
فراغ scale_bias_gpu(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح اندازه);
فراغ backward_scale_gpu(عائم *x_norm, عائم *delta, صحيح batch, صحيح n, صحيح اندازه, عائم *scale_updates);
فراغ scale_bias_gpu(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح اندازه);
فراغ جمع_کن_bias_gpu(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح اندازه);
فراغ backward_bias_gpu(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح اندازه);

فراغ logistic_x_ent_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ softmax_x_ent_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ smooth_l1_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ l2_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ l1_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ wgan_gpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error);
فراغ weighted_delta_gpu(عائم *a, عائم *b, عائم *s, عائم *da, عائم *db, عائم *ds, صحيح رقم, عائم *dc);
فراغ weighted_جمع_gpu(عائم *a, عائم *b, عائم *s, صحيح رقم, عائم *c);
فراغ mult_جمع_کن_into_gpu(صحيح رقم, عائم *a, عائم *b, عائم *c);
فراغ inter_gpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *بیرون_2);
فراغ deinter_gpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *بیرون_2);

فراغ reorg_gpu(عائم *x, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم *بیرون);

فراغ softmax_gpu(عائم *درون_داد, صحيح n, صحيح batch, صحيح batch_جابه_جایی, صحيح groups, صحيح گروه_جابه_جایی, صحيح stride, عائم موقت, عائم *output);
فراغ adam_update_gpu(عائم *w, عائم *d, عائم *m, عائم *v, عائم B1, عائم B2, عائم eps, عائم decay, عائم نرخ, صحيح n, صحيح batch, صحيح t);
فراغ adam_gpu(صحيح n, عائم *x, عائم *m, عائم *v, عائم B1, عائم B2, عائم نرخ, عائم eps, صحيح t);

فراغ flatten_gpu(عائم *x, صحيح spatial, صحيح layers, صحيح batch, صحيح forward, عائم *بیرون);
فراغ softmax_درخت(عائم *درون_داد, صحيح spatial, صحيح batch, صحيح stride, عائم موقت, عائم *output, درخت hier);
فراغ upsample_gpu(عائم *درون, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم scale, عائم *بیرون);

#پایان_اگر
#پایان_اگر
