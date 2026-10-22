#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <float.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
ਖਾਲੀ reorg_cpu(ਦਸ਼ਮਲਵ *x, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c, ਪੂਰਨ batch, ਪੂਰਨ stride, ਪੂਰਨ forward, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ)
{
    ਪੂਰਨ b,i,j,k;
    ਪੂਰਨ ਬਾਹਰਲਾ_c = c/(stride*stride);

    ਲਈ(b = 0; b < batch; ++b){
        ਲਈ(k = 0; k < c; ++k){
            ਲਈ(j = 0; j < h; ++j){
                ਲਈ(i = 0; i < w; ++i){
                    ਪੂਰਨ ਵਿੱਚ_ਸੂਚਕਾਂਕ  = i + w*(j + h*(k + c*b));
                    ਪੂਰਨ c2 = k % ਬਾਹਰਲਾ_c;
                    ਪੂਰਨ ਵਿੱਥ = k / ਬਾਹਰਲਾ_c;
                    ਪੂਰਨ w2 = i*stride + ਵਿੱਥ % stride;
                    ਪੂਰਨ h2 = j*stride + ਵਿੱਥ / stride;
                    ਪੂਰਨ ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ = w2 + w*stride*(h2 + h*stride*(c2 + ਬਾਹਰਲਾ_c*b));
                    ਜੇ(forward) ਬਾਹਰਲਾ[ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ] = x[ਵਿੱਚ_ਸੂਚਕਾਂਕ];
                    ਨਹੀਂਤਾਂ ਬਾਹਰਲਾ[ਵਿੱਚ_ਸੂਚਕਾਂਕ] = x[ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ];
                }
            }
        }
    }
}

ਖਾਲੀ flatten(ਦਸ਼ਮਲਵ *x, ਪੂਰਨ ਮਾਪ, ਪੂਰਨ layers, ਪੂਰਨ batch, ਪੂਰਨ forward)
{
    ਦਸ਼ਮਲਵ *ਅਦਲਾ_ਬਦਲੀ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਮਾਪ*layers*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਪੂਰਨ i,c,b;
    ਲਈ(b = 0; b < batch; ++b){
        ਲਈ(c = 0; c < layers; ++c){
            ਲਈ(i = 0; i < ਮਾਪ; ++i){
                ਪੂਰਨ i1 = b*layers*ਮਾਪ + c*ਮਾਪ + i;
                ਪੂਰਨ i2 = b*layers*ਮਾਪ + i*layers + c;
                ਜੇ (forward) ਅਦਲਾ_ਬਦਲੀ[i2] = x[i1];
                ਨਹੀਂਤਾਂ ਅਦਲਾ_ਬਦਲੀ[i1] = x[i2];
            }
        }
    }
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(x, ਅਦਲਾ_ਬਦਲੀ, ਮਾਪ*layers*batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਮੁਕਤ(ਅਦਲਾ_ਬਦਲੀ);
}

ਖਾਲੀ weighted_ਜੋੜ_cpu(ਦਸ਼ਮਲਵ *a, ਦਸ਼ਮਲਵ *b, ਦਸ਼ਮਲਵ *s, ਪੂਰਨ n, ਦਸ਼ਮਲਵ *c)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

ਖਾਲੀ weighted_delta_cpu(ਦਸ਼ਮਲਵ *a, ਦਸ਼ਮਲਵ *b, ਦਸ਼ਮਲਵ *s, ਦਸ਼ਮਲਵ *da, ਦਸ਼ਮਲਵ *db, ਦਸ਼ਮਲਵ *ds, ਪੂਰਨ n, ਦਸ਼ਮਲਵ *dc)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        ਜੇ(da) da[i] += dc[i] * s[i];
        ਜੇ(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

ਖਾਲੀ shortcut_cpu(ਪੂਰਨ batch, ਪੂਰਨ w1, ਪੂਰਨ h1, ਪੂਰਨ c1, ਦਸ਼ਮਲਵ *ਜੋੜੋ, ਪੂਰਨ w2, ਪੂਰਨ h2, ਪੂਰਨ c2, ਦਸ਼ਮਲਵ s1, ਦਸ਼ਮਲਵ s2, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ)
{
    ਪੂਰਨ stride = w1/w2;
    ਪੂਰਨ sample = w2/w1;
    ਪ੍ਰਤਿਜ੍ਞਾ(stride == h1/h2);
    ਪ੍ਰਤਿਜ੍ਞਾ(sample == h2/h1);
    ਜੇ(stride < 1) stride = 1;
    ਜੇ(sample < 1) sample = 1;
    ਪੂਰਨ minw = (w1 < w2) ? w1 : w2;
    ਪੂਰਨ minh = (h1 < h2) ? h1 : h2;
    ਪੂਰਨ minc = (c1 < c2) ? c1 : c2;

    ਪੂਰਨ i,j,k,b;
    ਲਈ(b = 0; b < batch; ++b){
        ਲਈ(k = 0; k < minc; ++k){
            ਲਈ(j = 0; j < minh; ++j){
                ਲਈ(i = 0; i < minw; ++i){
                    ਪੂਰਨ ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ = i*sample + w2*(j*sample + h2*(k + c2*b));
                    ਪੂਰਨ ਜੋੜੋ_ਸੂਚਕਾਂਕ = i*stride + w1*(j*stride + h1*(k + c1*b));
                    ਬਾਹਰਲਾ[ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ] = s1*ਬਾਹਰਲਾ[ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ] + s2*ਜੋੜੋ[ਜੋੜੋ_ਸੂਚਕਾਂਕ];
                }
            }
        }
    }
}

ਖਾਲੀ ਔਸਤ_cpu(ਦਸ਼ਮਲਵ *x, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *ਔਸਤ)
{
    ਦਸ਼ਮਲਵ scale = 1./(batch * spatial);
    ਪੂਰਨ i,j,k;
    ਲਈ(i = 0; i < filters; ++i){
        ਔਸਤ[i] = 0;
        ਲਈ(j = 0; j < batch; ++j){
            ਲਈ(k = 0; k < spatial; ++k){
                ਪੂਰਨ ਸੂਚਕਾਂਕ = j*filters*spatial + i*spatial + k;
                ਔਸਤ[i] += x[ਸੂਚਕਾਂਕ];
            }
        }
        ਔਸਤ[i] *= scale;
    }
}

ਖਾਲੀ variance_cpu(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *ਔਸਤ, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *variance)
{
    ਦਸ਼ਮਲਵ scale = 1./(batch * spatial - 1);
    ਪੂਰਨ i,j,k;
    ਲਈ(i = 0; i < filters; ++i){
        variance[i] = 0;
        ਲਈ(j = 0; j < batch; ++j){
            ਲਈ(k = 0; k < spatial; ++k){
                ਪੂਰਨ ਸੂਚਕਾਂਕ = j*filters*spatial + i*spatial + k;
                variance[i] += ਘਾਤ((x[ਸੂਚਕਾਂਕ] - ਔਸਤ[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

ਖਾਲੀ l2normalize_cpu(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *dx, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial)
{
    ਪੂਰਨ b,f,i;
    ਲਈ(b = 0; b < batch; ++b){
        ਲਈ(i = 0; i < spatial; ++i){
            ਦਸ਼ਮਲਵ ਜੋੜ = 0;
            ਲਈ(f = 0; f < filters; ++f){
                ਪੂਰਨ ਸੂਚਕਾਂਕ = b*filters*spatial + f*spatial + i;
                ਜੋੜ += powf(x[ਸੂਚਕਾਂਕ], 2);
            }
            ਜੋੜ = sqrtf(ਜੋੜ);
            ਲਈ(f = 0; f < filters; ++f){
                ਪੂਰਨ ਸੂਚਕਾਂਕ = b*filters*spatial + f*spatial + i;
                x[ਸੂਚਕਾਂਕ] /= ਜੋੜ;
                dx[ਸੂਚਕਾਂਕ] = (1 - x[ਸੂਚਕਾਂਕ]) / ਜੋੜ;
            }
        }
    }
}


ਖਾਲੀ normalize_cpu(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *ਔਸਤ, ਦਸ਼ਮਲਵ *variance, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial)
{
    ਪੂਰਨ b, f, i;
    ਲਈ(b = 0; b < batch; ++b){
        ਲਈ(f = 0; f < filters; ++f){
            ਲਈ(i = 0; i < spatial; ++i){
                ਪੂਰਨ ਸੂਚਕਾਂਕ = b*filters*spatial + f*spatial + i;
                x[ਸੂਚਕਾਂਕ] = (x[ਸੂਚਕਾਂਕ] - ਔਸਤ[f])/(ਵਰਗਮੂਲ(variance[f]) + .000001f);
            }
        }
    }
}

ਖਾਲੀ const_cpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ ALPHA, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ INCX)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

ਖਾਲੀ mul_cpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ INCX, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ INCY)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

ਖਾਲੀ pow_cpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ ALPHA, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ INCX, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ INCY)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < N; ++i) Y[i*INCY] = ਘਾਤ(X[i*INCX], ALPHA);
}

ਖਾਲੀ axpy_cpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ ALPHA, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ INCX, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ INCY)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

ਖਾਲੀ scal_cpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ ALPHA, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ INCX)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

ਖਾਲੀ fill_cpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ ALPHA, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ INCX)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

ਖਾਲੀ deinter_cpu(ਪੂਰਨ NX, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ NY, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ B, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ_2)
{
    ਪੂਰਨ i, j;
    ਪੂਰਨ ਸੂਚਕਾਂਕ = 0;
    ਲਈ(j = 0; j < B; ++j) {
        ਲਈ(i = 0; i < NX; ++i){
            ਜੇ(X) X[j*NX + i] += ਬਾਹਰਲਾ_2[ਸੂਚਕਾਂਕ];
            ++ਸੂਚਕਾਂਕ;
        }
        ਲਈ(i = 0; i < NY; ++i){
            ਜੇ(Y) Y[j*NY + i] += ਬਾਹਰਲਾ_2[ਸੂਚਕਾਂਕ];
            ++ਸੂਚਕਾਂਕ;
        }
    }
}

ਖਾਲੀ inter_cpu(ਪੂਰਨ NX, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ NY, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ B, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ_2)
{
    ਪੂਰਨ i, j;
    ਪੂਰਨ ਸੂਚਕਾਂਕ = 0;
    ਲਈ(j = 0; j < B; ++j) {
        ਲਈ(i = 0; i < NX; ++i){
            ਬਾਹਰਲਾ_2[ਸੂਚਕਾਂਕ++] = X[j*NX + i];
        }
        ਲਈ(i = 0; i < NY; ++i){
            ਬਾਹਰਲਾ_2[ਸੂਚਕਾਂਕ++] = Y[j*NY + i];
        }
    }
}

ਖਾਲੀ copy_cpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ INCX, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ INCY)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

ਖਾਲੀ mult_ਜੋੜੋ_into_cpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ *X, ਦਸ਼ਮਲਵ *Y, ਦਸ਼ਮਲਵ *Z)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

ਖਾਲੀ smooth_l1_cpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        ਦਸ਼ਮਲਵ diff = truth[i] - pred[i];
        ਦਸ਼ਮਲਵ abs_val = fabs(diff);
        ਜੇ(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        ਨਹੀਂਤਾਂ {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

ਖਾਲੀ l1_cpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        ਦਸ਼ਮਲਵ diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

ਖਾਲੀ softmax_x_ent_cpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        ਦਸ਼ਮਲਵ t = truth[i];
        ਦਸ਼ਮਲਵ p = pred[i];
        error[i] = (t) ? -ਲਘੂਗਣਕ(p) : 0;
        delta[i] = t-p;
    }
}

ਖਾਲੀ logistic_x_ent_cpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        ਦਸ਼ਮਲਵ t = truth[i];
        ਦਸ਼ਮਲਵ p = pred[i];
        error[i] = -t*ਲਘੂਗਣਕ(p) - (1-t)*ਲਘੂਗਣਕ(1-p);
        delta[i] = t-p;
    }
}

ਖਾਲੀ l2_cpu(ਪੂਰਨ n, ਦਸ਼ਮਲਵ *pred, ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *error)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        ਦਸ਼ਮਲਵ diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

ਦਸ਼ਮਲਵ dot_cpu(ਪੂਰਨ N, ਦਸ਼ਮਲਵ *X, ਪੂਰਨ INCX, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ INCY)
{
    ਪੂਰਨ i;
    ਦਸ਼ਮਲਵ dot = 0;
    ਲਈ(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    ਵਾਪਸ dot;
}

ਖਾਲੀ softmax(ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼, ਪੂਰਨ n, ਦਸ਼ਮਲਵ ਆਰਜ਼ੀ, ਪੂਰਨ stride, ਦਸ਼ਮਲਵ *output)
{
    ਪੂਰਨ i;
    ਦਸ਼ਮਲਵ ਜੋੜ = 0;
    ਦਸ਼ਮਲਵ ਸਭ_ਤੋਂ_ਵੱਡਾ = -FLT_MAX;
    ਲਈ(i = 0; i < n; ++i){
        ਜੇ(ਨਿਵੇਸ਼[i*stride] > ਸਭ_ਤੋਂ_ਵੱਡਾ) ਸਭ_ਤੋਂ_ਵੱਡਾ = ਨਿਵੇਸ਼[i*stride];
    }
    ਲਈ(i = 0; i < n; ++i){
        ਦਸ਼ਮਲਵ e = ਘਾਤਾਂਕ(ਨਿਵੇਸ਼[i*stride]/ਆਰਜ਼ੀ - ਸਭ_ਤੋਂ_ਵੱਡਾ/ਆਰਜ਼ੀ);
        ਜੋੜ += e;
        output[i*stride] = e;
    }
    ਲਈ(i = 0; i < n; ++i){
        output[i*stride] /= ਜੋੜ;
    }
}


ਖਾਲੀ softmax_cpu(ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼, ਪੂਰਨ n, ਪੂਰਨ batch, ਪੂਰਨ batch_ਵਿੱਥ, ਪੂਰਨ groups, ਪੂਰਨ ਸਮੂਹ_ਵਿੱਥ, ਪੂਰਨ stride, ਦਸ਼ਮਲਵ ਆਰਜ਼ੀ, ਦਸ਼ਮਲਵ *output)
{
    ਪੂਰਨ g, b;
    ਲਈ(b = 0; b < batch; ++b){
        ਲਈ(g = 0; g < groups; ++g){
            softmax(ਨਿਵੇਸ਼ + b*batch_ਵਿੱਥ + g*ਸਮੂਹ_ਵਿੱਥ, n, ਆਰਜ਼ੀ, stride, output + b*batch_ਵਿੱਥ + g*ਸਮੂਹ_ਵਿੱਥ);
        }
    }
}

ਖਾਲੀ upsample_cpu(ਦਸ਼ਮਲਵ *ਵਿੱਚ, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c, ਪੂਰਨ batch, ਪੂਰਨ stride, ਪੂਰਨ forward, ਦਸ਼ਮਲਵ scale, ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ)
{
    ਪੂਰਨ i, j, k, b;
    ਲਈ(b = 0; b < batch; ++b){
        ਲਈ(k = 0; k < c; ++k){
            ਲਈ(j = 0; j < h*stride; ++j){
                ਲਈ(i = 0; i < w*stride; ++i){
                    ਪੂਰਨ ਵਿੱਚ_ਸੂਚਕਾਂਕ = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    ਪੂਰਨ ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    ਜੇ(forward) ਬਾਹਰਲਾ[ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ] = scale*ਵਿੱਚ[ਵਿੱਚ_ਸੂਚਕਾਂਕ];
                    ਨਹੀਂਤਾਂ ਵਿੱਚ[ਵਿੱਚ_ਸੂਚਕਾਂਕ] += scale*ਬਾਹਰਲਾ[ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ];
                }
            }
        }
    }
}


