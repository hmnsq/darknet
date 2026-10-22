#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ BLAS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLAS_H
#ਅਨ੍ਤਰ੍ਭਾਵ "darknet.h"

ਖਾਲੀ flatten(ਦਸ਼ਮਲਵ *x, ਪੂਰਨ ਮਾਪ, ਪੂਰਨ layers, ਪੂਰਨ batch, ਪੂਰਨ forward);
ਖਾਲੀ pm(ਪੂਰਨ M, ਪੂਰਨ N, ਦਸ਼ਮਲਵ *A);
ਦਸ਼ਮਲਵ *random_ਮੈਟ੍ਰਿਕਸ(ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ);
ਖਾਲੀ ਸਮਾਂ_random_ਮੈਟ੍ਰਿਕਸ(ਪੂਰਨ TA, ਪੂਰਨ TB, ਪੂਰਨ m, ਪੂਰਨ k, ਪੂਰਨ n);
ਖਾਲੀ reorg_cpu(ਦਸ਼ਮਲਵ *x, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c, ਪੂਰਨ batch, ਪੂਰਨ stride, ਪੂਰਨ forward, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ);

ਖਾਲੀ test_blas();

ਖਾਲੀ inter_cpu(ਪੂਰਨ NX, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ NY, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ B, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ_2);
ਖਾਲੀ deinter_cpu(ਪੂਰਨ NX, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ NY, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ B, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ_2);
ਖਾਲੀ mult_ਜੋੜੋ_into_cpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ *X, ਦਸ਼ਮਲਵ *Y, ਦਸ਼ਮਲਵ *Z);

ਖਾਲੀ const_cpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ ALPHA, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ INCX);
ਖਾਲੀ constrain_gpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ ALPHA, ਦਸ਼ਮਲਵ * X, ਪੂਰਨ INCX);
ਖਾਲੀ pow_cpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ ALPHA, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ INCX, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ INCY);
ਖਾਲੀ mul_cpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ INCX, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ INCY);

ਪੂਰਨ test_gpu_blas();
ਖਾਲੀ shortcut_cpu(ਪੂਰਨ batch, ਪੂਰਨ w1, ਪੂਰਨ h1, ਪੂਰਨ c1, ਦਸ਼ਮਲਵ *ਜੋੜੋ, ਪੂਰਨ w2, ਪੂਰਨ h2, ਪੂਰਨ c2, ਦਸ਼ਮਲਵ s1, ਦਸ਼ਮਲਵ s2, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ);

ਖਾਲੀ ਔਸਤ_cpu(ਦਸ਼ਮਲਵ *x, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *ਔਸਤ);
ਖਾਲੀ variance_cpu(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *ਔਸਤ, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *variance);

ਖਾਲੀ scale_bias(ਦਸ਼ਮਲਵ *output, ਦਸ਼ਮਲਵ *scales, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ);
ਖਾਲੀ backward_scale_cpu(ਦਸ਼ਮਲਵ *x_norm, ਦਸ਼ਮਲਵ *delta, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ *scale_updates);
ਖਾਲੀ ਔਸਤ_delta_cpu(ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *variance, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *ਔਸਤ_delta);
ਖਾਲੀ  variance_delta_cpu(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *ਔਸਤ, ਦਸ਼ਮਲਵ *variance, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *variance_delta);
ਖਾਲੀ normalize_delta_cpu(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *ਔਸਤ, ਦਸ਼ਮਲਵ *variance, ਦਸ਼ਮਲਵ *ਔਸਤ_delta, ਦਸ਼ਮਲਵ *variance_delta, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *delta);
ਖਾਲੀ l2normalize_cpu(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *dx, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial);

ਖਾਲੀ smooth_l1_cpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error);
ਖਾਲੀ l2_cpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error);
ਖਾਲੀ l1_cpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error);
ਖਾਲੀ logistic_x_ent_cpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error);
ਖਾਲੀ softmax_x_ent_cpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error);
ਖਾਲੀ weighted_ਜੋੜ_cpu(ਦਸ਼ਮਲਵ *a, ਦਸ਼ਮਲਵ *b, ਦਸ਼ਮਲਵ *s, ਪੂਰਨ ਅੰਕ, ਦਸ਼ਮਲਵ *c);
ਖਾਲੀ weighted_delta_cpu(ਦਸ਼ਮਲਵ *a, ਦਸ਼ਮਲਵ *b, ਦਸ਼ਮਲਵ *s, ਦਸ਼ਮਲਵ *da, ਦਸ਼ਮਲਵ *db, ਦਸ਼ਮਲਵ *ds, ਪੂਰਨ n, ਦਸ਼ਮਲਵ *dc);

ਖਾਲੀ softmax(ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼, ਪੂਰਨ n, ਦਸ਼ਮਲਵ ਆਰਜ਼ੀ, ਪੂਰਨ stride, ਦਸ਼ਮਲਵ *output);
ਖਾਲੀ softmax_cpu(ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼, ਪੂਰਨ n, ਪੂਰਨ batch, ਪੂਰਨ batch_ਵਿੱਥ, ਪੂਰਨ groups, ਪੂਰਨ ਸਮੂਹ_ਵਿੱਥ, ਪੂਰਨ stride, ਦਸ਼ਮਲਵ ਆਰਜ਼ੀ, ਦਸ਼ਮਲਵ *output);
ਖਾਲੀ upsample_cpu(ਦਸ਼ਮਲਵ *ਵਿੱਚ, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c, ਪੂਰਨ batch, ਪੂਰਨ stride, ਪੂਰਨ forward, ਦਸ਼ਮਲਵ scale, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"

ਖਾਲੀ axpy_gpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ ALPHA, ਦਸ਼ਮਲਵ * X, ਪੂਰਨ INCX, ਦਸ਼ਮਲਵ * Y, ਪੂਰਨ INCY);
ਖਾਲੀ axpy_gpu_ਵਿੱਥ(ਪੂਰਨ N, ਦਸ਼ਮਲਵ ALPHA, ਦਸ਼ਮਲਵ * X, ਪੂਰਨ OFFX, ਪੂਰਨ INCX, ਦਸ਼ਮਲਵ * Y, ਪੂਰਨ OFFY, ਪੂਰਨ INCY);
ਖਾਲੀ copy_gpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ * X, ਪੂਰਨ INCX, ਦਸ਼ਮਲਵ * Y, ਪੂਰਨ INCY);
ਖਾਲੀ copy_gpu_ਵਿੱਥ(ਪੂਰਨ N, ਦਸ਼ਮਲਵ * X, ਪੂਰਨ OFFX, ਪੂਰਨ INCX, ਦਸ਼ਮਲਵ * Y, ਪੂਰਨ OFFY, ਪੂਰਨ INCY);
ਖਾਲੀ ਜੋੜੋ_gpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ ALPHA, ਦਸ਼ਮਲਵ * X, ਪੂਰਨ INCX);
ਖਾਲੀ supp_gpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ ALPHA, ਦਸ਼ਮਲਵ * X, ਪੂਰਨ INCX);
ਖਾਲੀ ਨਕਾਬ_gpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ * X, ਦਸ਼ਮਲਵ ਨਕਾਬ_ਅੰਕ, ਦਸ਼ਮਲਵ * ਨਕਾਬ, ਦਸ਼ਮਲਵ val);
ਖਾਲੀ scale_ਨਕਾਬ_gpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ * X, ਦਸ਼ਮਲਵ ਨਕਾਬ_ਅੰਕ, ਦਸ਼ਮਲਵ * ਨਕਾਬ, ਦਸ਼ਮਲਵ scale);
ਖਾਲੀ const_gpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ ALPHA, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ INCX);
ਖਾਲੀ pow_gpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ ALPHA, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ INCX, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ INCY);
ਖਾਲੀ mul_gpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ INCX, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ INCY);

ਖਾਲੀ ਔਸਤ_gpu(ਦਸ਼ਮਲਵ *x, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *ਔਸਤ);
ਖਾਲੀ variance_gpu(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *ਔਸਤ, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *variance);
ਖਾਲੀ normalize_gpu(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *ਔਸਤ, ਦਸ਼ਮਲਵ *variance, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial);
ਖਾਲੀ l2normalize_gpu(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *dx, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial);

ਖਾਲੀ normalize_delta_gpu(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *ਔਸਤ, ਦਸ਼ਮਲਵ *variance, ਦਸ਼ਮਲਵ *ਔਸਤ_delta, ਦਸ਼ਮਲਵ *variance_delta, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *delta);

ਖਾਲੀ fast_ਔਸਤ_delta_gpu(ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *variance, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *ਔਸਤ_delta);
ਖਾਲੀ fast_variance_delta_gpu(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *ਔਸਤ, ਦਸ਼ਮਲਵ *variance, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *variance_delta);

ਖਾਲੀ fast_variance_gpu(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *ਔਸਤ, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *variance);
ਖਾਲੀ fast_ਔਸਤ_gpu(ਦਸ਼ਮਲਵ *x, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *ਔਸਤ);
ਖਾਲੀ shortcut_gpu(ਪੂਰਨ batch, ਪੂਰਨ w1, ਪੂਰਨ h1, ਪੂਰਨ c1, ਦਸ਼ਮਲਵ *ਜੋੜੋ, ਪੂਰਨ w2, ਪੂਰਨ h2, ਪੂਰਨ c2, ਦਸ਼ਮਲਵ s1, ਦਸ਼ਮਲਵ s2, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ);
ਖਾਲੀ scale_bias_gpu(ਦਸ਼ਮਲਵ *output, ਦਸ਼ਮਲਵ *biases, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ);
ਖਾਲੀ backward_scale_gpu(ਦਸ਼ਮਲਵ *x_norm, ਦਸ਼ਮਲਵ *delta, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ *scale_updates);
ਖਾਲੀ scale_bias_gpu(ਦਸ਼ਮਲਵ *output, ਦਸ਼ਮਲਵ *biases, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ);
ਖਾਲੀ ਜੋੜੋ_bias_gpu(ਦਸ਼ਮਲਵ *output, ਦਸ਼ਮਲਵ *biases, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ);
ਖਾਲੀ backward_bias_gpu(ਦਸ਼ਮਲਵ *bias_updates, ਦਸ਼ਮਲਵ *delta, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ);

ਖਾਲੀ logistic_x_ent_gpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error);
ਖਾਲੀ softmax_x_ent_gpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error);
ਖਾਲੀ smooth_l1_gpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error);
ਖਾਲੀ l2_gpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error);
ਖਾਲੀ l1_gpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error);
ਖਾਲੀ wgan_gpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error);
ਖਾਲੀ weighted_delta_gpu(ਦਸ਼ਮਲਵ *a, ਦਸ਼ਮਲਵ *b, ਦਸ਼ਮਲਵ *s, ਦਸ਼ਮਲਵ *da, ਦਸ਼ਮਲਵ *db, ਦਸ਼ਮਲਵ *ds, ਪੂਰਨ ਅੰਕ, ਦਸ਼ਮਲਵ *dc);
ਖਾਲੀ weighted_ਜੋੜ_gpu(ਦਸ਼ਮਲਵ *a, ਦਸ਼ਮਲਵ *b, ਦਸ਼ਮਲਵ *s, ਪੂਰਨ ਅੰਕ, ਦਸ਼ਮਲਵ *c);
ਖਾਲੀ mult_ਜੋੜੋ_into_gpu(ਪੂਰਨ ਅੰਕ, ਦਸ਼ਮਲਵ *a, ਦਸ਼ਮਲਵ *b, ਦਸ਼ਮਲਵ *c);
ਖਾਲੀ inter_gpu(ਪੂਰਨ NX, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ NY, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ B, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ_2);
ਖਾਲੀ deinter_gpu(ਪੂਰਨ NX, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ NY, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ B, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ_2);

ਖਾਲੀ reorg_gpu(ਦਸ਼ਮਲਵ *x, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c, ਪੂਰਨ batch, ਪੂਰਨ stride, ਪੂਰਨ forward, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ);

ਖਾਲੀ softmax_gpu(ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼, ਪੂਰਨ n, ਪੂਰਨ batch, ਪੂਰਨ batch_ਵਿੱਥ, ਪੂਰਨ groups, ਪੂਰਨ ਸਮੂਹ_ਵਿੱਥ, ਪੂਰਨ stride, ਦਸ਼ਮਲਵ ਆਰਜ਼ੀ, ਦਸ਼ਮਲਵ *output);
ਖਾਲੀ adam_update_gpu(ਦਸ਼ਮਲਵ *w, ਦਸ਼ਮਲਵ *d, ਦਸ਼ਮਲਵ *m, ਦਸ਼ਮਲਵ *v, ਦਸ਼ਮਲਵ B1, ਦਸ਼ਮਲਵ B2, ਦਸ਼ਮਲਵ eps, ਦਸ਼ਮਲਵ decay, ਦਸ਼ਮਲਵ ਦਰ, ਪੂਰਨ n, ਪੂਰਨ batch, ਪੂਰਨ t);
ਖਾਲੀ adam_gpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *m, ਦਸ਼ਮਲਵ *v, ਦਸ਼ਮਲਵ B1, ਦਸ਼ਮਲਵ B2, ਦਸ਼ਮਲਵ ਦਰ, ਦਸ਼ਮਲਵ eps, ਪੂਰਨ t);

ਖਾਲੀ flatten_gpu(ਦਸ਼ਮਲਵ *x, ਪੂਰਨ spatial, ਪੂਰਨ layers, ਪੂਰਨ batch, ਪੂਰਨ forward, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ);
ਖਾਲੀ softmax_ਰੁੱਖ(ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼, ਪੂਰਨ spatial, ਪੂਰਨ batch, ਪੂਰਨ stride, ਦਸ਼ਮਲਵ ਆਰਜ਼ੀ, ਦਸ਼ਮਲਵ *output, ਰੁੱਖ hier);
ਖਾਲੀ upsample_gpu(ਦਸ਼ਮਲਵ *ਵਿੱਚ, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c, ਪੂਰਨ batch, ਪੂਰਨ stride, ਪੂਰਨ forward, ਦਸ਼ਮਲਵ scale, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ);

#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
