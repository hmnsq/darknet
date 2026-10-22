#اگر_غیر_تعریف_شدہ BLAS_H
#تعریف_کرو BLAS_H
#شامل "darknet.h"

خالی flatten(اعشاری *x, صحیح سائز, صحیح layers, صحیح batch, صحیح forward);
خالی pm(صحیح M, صحیح N, اعشاری *A);
اعشاری *random_میٹرکس(صحیح صفیں, صحیح ستون_تعداد);
خالی وقت_random_میٹرکس(صحیح TA, صحیح TB, صحیح m, صحیح k, صحیح n);
خالی reorg_cpu(اعشاری *x, صحیح w, صحیح h, صحیح c, صحیح batch, صحیح stride, صحیح forward, اعشاری *برآمد);

خالی test_blas();

خالی inter_cpu(صحیح NX, اعشاری *X, صحیح NY, اعشاری *Y, صحیح B, اعشاری *برآمد_2);
خالی deinter_cpu(صحیح NX, اعشاری *X, صحیح NY, اعشاری *Y, صحیح B, اعشاری *برآمد_2);
خالی mult_جوڑو_into_cpu(صحیح N, اعشاری *X, اعشاری *Y, اعشاری *Z);

خالی const_cpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX);
خالی constrain_gpu(صحیح N, اعشاری ALPHA, اعشاری * X, صحیح INCX);
خالی pow_cpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY);
خالی mul_cpu(صحیح N, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY);

صحیح test_gpu_blas();
خالی shortcut_cpu(صحیح batch, صحیح w1, صحیح h1, صحیح c1, اعشاری *جوڑو, صحیح w2, صحیح h2, صحیح c2, اعشاری s1, اعشاری s2, اعشاری *برآمد);

خالی اوسط_cpu(اعشاری *x, صحیح batch, صحیح filters, صحیح spatial, اعشاری *اوسط);
خالی variance_cpu(اعشاری *x, اعشاری *اوسط, صحیح batch, صحیح filters, صحیح spatial, اعشاری *variance);

خالی scale_bias(اعشاری *output, اعشاری *scales, صحیح batch, صحیح n, صحیح سائز);
خالی backward_scale_cpu(اعشاری *x_norm, اعشاری *delta, صحیح batch, صحیح n, صحیح سائز, اعشاری *scale_updates);
خالی اوسط_delta_cpu(اعشاری *delta, اعشاری *variance, صحیح batch, صحیح filters, صحیح spatial, اعشاری *اوسط_delta);
خالی  variance_delta_cpu(اعشاری *x, اعشاری *delta, اعشاری *اوسط, اعشاری *variance, صحیح batch, صحیح filters, صحیح spatial, اعشاری *variance_delta);
خالی normalize_delta_cpu(اعشاری *x, اعشاری *اوسط, اعشاری *variance, اعشاری *اوسط_delta, اعشاری *variance_delta, صحیح batch, صحیح filters, صحیح spatial, اعشاری *delta);
خالی l2normalize_cpu(اعشاری *x, اعشاری *dx, صحیح batch, صحیح filters, صحیح spatial);

خالی smooth_l1_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
خالی l2_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
خالی l1_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
خالی logistic_x_ent_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
خالی softmax_x_ent_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
خالی weighted_مجموع_cpu(اعشاری *a, اعشاری *b, اعشاری *s, صحیح عدد, اعشاری *c);
خالی weighted_delta_cpu(اعشاری *a, اعشاری *b, اعشاری *s, اعشاری *da, اعشاری *db, اعشاری *ds, صحیح n, اعشاری *dc);

خالی softmax(اعشاری *داخلہ, صحیح n, اعشاری عارضی, صحیح stride, اعشاری *output);
خالی softmax_cpu(اعشاری *داخلہ, صحیح n, صحیح batch, صحیح batch_انحراف, صحیح groups, صحیح گروہ_انحراف, صحیح stride, اعشاری عارضی, اعشاری *output);
خالی upsample_cpu(اعشاری *اندر, صحیح w, صحیح h, صحیح c, صحیح batch, صحیح stride, صحیح forward, اعشاری scale, اعشاری *برآمد);

#اگر_تعریف_شدہ GPU
#شامل "cuda.h"
#شامل "tree.h"

خالی axpy_gpu(صحیح N, اعشاری ALPHA, اعشاری * X, صحیح INCX, اعشاری * Y, صحیح INCY);
خالی axpy_gpu_انحراف(صحیح N, اعشاری ALPHA, اعشاری * X, صحیح OFFX, صحیح INCX, اعشاری * Y, صحیح OFFY, صحیح INCY);
خالی copy_gpu(صحیح N, اعشاری * X, صحیح INCX, اعشاری * Y, صحیح INCY);
خالی copy_gpu_انحراف(صحیح N, اعشاری * X, صحیح OFFX, صحیح INCX, اعشاری * Y, صحیح OFFY, صحیح INCY);
خالی جوڑو_gpu(صحیح N, اعشاری ALPHA, اعشاری * X, صحیح INCX);
خالی supp_gpu(صحیح N, اعشاری ALPHA, اعشاری * X, صحیح INCX);
خالی نقاب_gpu(صحیح N, اعشاری * X, اعشاری نقاب_عدد, اعشاری * نقاب, اعشاری val);
خالی scale_نقاب_gpu(صحیح N, اعشاری * X, اعشاری نقاب_عدد, اعشاری * نقاب, اعشاری scale);
خالی const_gpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX);
خالی pow_gpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY);
خالی mul_gpu(صحیح N, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY);

خالی اوسط_gpu(اعشاری *x, صحیح batch, صحیح filters, صحیح spatial, اعشاری *اوسط);
خالی variance_gpu(اعشاری *x, اعشاری *اوسط, صحیح batch, صحیح filters, صحیح spatial, اعشاری *variance);
خالی normalize_gpu(اعشاری *x, اعشاری *اوسط, اعشاری *variance, صحیح batch, صحیح filters, صحیح spatial);
خالی l2normalize_gpu(اعشاری *x, اعشاری *dx, صحیح batch, صحیح filters, صحیح spatial);

خالی normalize_delta_gpu(اعشاری *x, اعشاری *اوسط, اعشاری *variance, اعشاری *اوسط_delta, اعشاری *variance_delta, صحیح batch, صحیح filters, صحیح spatial, اعشاری *delta);

خالی fast_اوسط_delta_gpu(اعشاری *delta, اعشاری *variance, صحیح batch, صحیح filters, صحیح spatial, اعشاری *اوسط_delta);
خالی fast_variance_delta_gpu(اعشاری *x, اعشاری *delta, اعشاری *اوسط, اعشاری *variance, صحیح batch, صحیح filters, صحیح spatial, اعشاری *variance_delta);

خالی fast_variance_gpu(اعشاری *x, اعشاری *اوسط, صحیح batch, صحیح filters, صحیح spatial, اعشاری *variance);
خالی fast_اوسط_gpu(اعشاری *x, صحیح batch, صحیح filters, صحیح spatial, اعشاری *اوسط);
خالی shortcut_gpu(صحیح batch, صحیح w1, صحیح h1, صحیح c1, اعشاری *جوڑو, صحیح w2, صحیح h2, صحیح c2, اعشاری s1, اعشاری s2, اعشاری *برآمد);
خالی scale_bias_gpu(اعشاری *output, اعشاری *biases, صحیح batch, صحیح n, صحیح سائز);
خالی backward_scale_gpu(اعشاری *x_norm, اعشاری *delta, صحیح batch, صحیح n, صحیح سائز, اعشاری *scale_updates);
خالی scale_bias_gpu(اعشاری *output, اعشاری *biases, صحیح batch, صحیح n, صحیح سائز);
خالی جوڑو_bias_gpu(اعشاری *output, اعشاری *biases, صحیح batch, صحیح n, صحیح سائز);
خالی backward_bias_gpu(اعشاری *bias_updates, اعشاری *delta, صحیح batch, صحیح n, صحیح سائز);

خالی logistic_x_ent_gpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
خالی softmax_x_ent_gpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
خالی smooth_l1_gpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
خالی l2_gpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
خالی l1_gpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
خالی wgan_gpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error);
خالی weighted_delta_gpu(اعشاری *a, اعشاری *b, اعشاری *s, اعشاری *da, اعشاری *db, اعشاری *ds, صحیح عدد, اعشاری *dc);
خالی weighted_مجموع_gpu(اعشاری *a, اعشاری *b, اعشاری *s, صحیح عدد, اعشاری *c);
خالی mult_جوڑو_into_gpu(صحیح عدد, اعشاری *a, اعشاری *b, اعشاری *c);
خالی inter_gpu(صحیح NX, اعشاری *X, صحیح NY, اعشاری *Y, صحیح B, اعشاری *برآمد_2);
خالی deinter_gpu(صحیح NX, اعشاری *X, صحیح NY, اعشاری *Y, صحیح B, اعشاری *برآمد_2);

خالی reorg_gpu(اعشاری *x, صحیح w, صحیح h, صحیح c, صحیح batch, صحیح stride, صحیح forward, اعشاری *برآمد);

خالی softmax_gpu(اعشاری *داخلہ, صحیح n, صحیح batch, صحیح batch_انحراف, صحیح groups, صحیح گروہ_انحراف, صحیح stride, اعشاری عارضی, اعشاری *output);
خالی adam_update_gpu(اعشاری *w, اعشاری *d, اعشاری *m, اعشاری *v, اعشاری B1, اعشاری B2, اعشاری eps, اعشاری decay, اعشاری شرح, صحیح n, صحیح batch, صحیح t);
خالی adam_gpu(صحیح n, اعشاری *x, اعشاری *m, اعشاری *v, اعشاری B1, اعشاری B2, اعشاری شرح, اعشاری eps, صحیح t);

خالی flatten_gpu(اعشاری *x, صحیح spatial, صحیح layers, صحیح batch, صحیح forward, اعشاری *برآمد);
خالی softmax_درخت(اعشاری *داخلہ, صحیح spatial, صحیح batch, صحیح stride, اعشاری عارضی, اعشاری *output, درخت hier);
خالی upsample_gpu(اعشاری *اندر, صحیح w, صحیح h, صحیح c, صحیح batch, صحیح stride, صحیح forward, اعشاری scale, اعشاری *برآمد);

#اگر_ختم
#اگر_ختم
