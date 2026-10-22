#ضمن "blas.h"

#ضمن <math.h>
#ضمن <assert.h>
#ضمن <float.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>
فراغ reorg_cpu(عائم *x, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم *باہرلا)
{
    صحيح b,i,j,k;
    صحيح باہرلا_c = c/(stride*stride);

    لكل(b = 0; b < batch; ++b){
        لكل(k = 0; k < c; ++k){
            لكل(j = 0; j < h; ++j){
                لكل(i = 0; i < w; ++i){
                    صحيح وچ_اشاریہ  = i + w*(j + h*(k + c*b));
                    صحيح c2 = k % باہرلا_c;
                    صحيح وتھ = k / باہرلا_c;
                    صحيح w2 = i*stride + وتھ % stride;
                    صحيح h2 = j*stride + وتھ / stride;
                    صحيح باہرلا_اشاریہ = w2 + w*stride*(h2 + h*stride*(c2 + باہرلا_c*b));
                    إذا(forward) باہرلا[باہرلا_اشاریہ] = x[وچ_اشاریہ];
                    وإلا باہرلا[وچ_اشاریہ] = x[باہرلا_اشاریہ];
                }
            }
        }
    }
}

فراغ flatten(عائم *x, صحيح ناپ, صحيح layers, صحيح batch, صحيح forward)
{
    عائم *وٹاندرا = صفر_حافظہ_دیو(ناپ*layers*batch, حجم(عائم));
    صحيح i,c,b;
    لكل(b = 0; b < batch; ++b){
        لكل(c = 0; c < layers; ++c){
            لكل(i = 0; i < ناپ; ++i){
                صحيح i1 = b*layers*ناپ + c*ناپ + i;
                صحيح i2 = b*layers*ناپ + i*layers + c;
                إذا (forward) وٹاندرا[i2] = x[i1];
                وإلا وٹاندرا[i1] = x[i2];
            }
        }
    }
    حافظہ_نقل(x, وٹاندرا, ناپ*layers*batch*حجم(عائم));
    حرر(وٹاندرا);
}

فراغ weighted_جوڑ_cpu(عائم *a, عائم *b, عائم *s, صحيح n, عائم *c)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

فراغ weighted_delta_cpu(عائم *a, عائم *b, عائم *s, عائم *da, عائم *db, عائم *ds, صحيح n, عائم *dc)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        إذا(da) da[i] += dc[i] * s[i];
        إذا(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

فراغ shortcut_cpu(صحيح batch, صحيح w1, صحيح h1, صحيح c1, عائم *جوڑو, صحيح w2, صحيح h2, صحيح c2, عائم s1, عائم s2, عائم *باہرلا)
{
    صحيح stride = w1/w2;
    صحيح sample = w2/w1;
    أكد(stride == h1/h2);
    أكد(sample == h2/h1);
    إذا(stride < 1) stride = 1;
    إذا(sample < 1) sample = 1;
    صحيح minw = (w1 < w2) ? w1 : w2;
    صحيح minh = (h1 < h2) ? h1 : h2;
    صحيح minc = (c1 < c2) ? c1 : c2;

    صحيح i,j,k,b;
    لكل(b = 0; b < batch; ++b){
        لكل(k = 0; k < minc; ++k){
            لكل(j = 0; j < minh; ++j){
                لكل(i = 0; i < minw; ++i){
                    صحيح باہرلا_اشاریہ = i*sample + w2*(j*sample + h2*(k + c2*b));
                    صحيح جوڑو_اشاریہ = i*stride + w1*(j*stride + h1*(k + c1*b));
                    باہرلا[باہرلا_اشاریہ] = s1*باہرلا[باہرلا_اشاریہ] + s2*جوڑو[جوڑو_اشاریہ];
                }
            }
        }
    }
}

فراغ اوسط_cpu(عائم *x, صحيح batch, صحيح filters, صحيح spatial, عائم *اوسط)
{
    عائم scale = 1./(batch * spatial);
    صحيح i,j,k;
    لكل(i = 0; i < filters; ++i){
        اوسط[i] = 0;
        لكل(j = 0; j < batch; ++j){
            لكل(k = 0; k < spatial; ++k){
                صحيح اشاریہ = j*filters*spatial + i*spatial + k;
                اوسط[i] += x[اشاریہ];
            }
        }
        اوسط[i] *= scale;
    }
}

فراغ variance_cpu(عائم *x, عائم *اوسط, صحيح batch, صحيح filters, صحيح spatial, عائم *variance)
{
    عائم scale = 1./(batch * spatial - 1);
    صحيح i,j,k;
    لكل(i = 0; i < filters; ++i){
        variance[i] = 0;
        لكل(j = 0; j < batch; ++j){
            لكل(k = 0; k < spatial; ++k){
                صحيح اشاریہ = j*filters*spatial + i*spatial + k;
                variance[i] += قوت((x[اشاریہ] - اوسط[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

فراغ l2normalize_cpu(عائم *x, عائم *dx, صحيح batch, صحيح filters, صحيح spatial)
{
    صحيح b,f,i;
    لكل(b = 0; b < batch; ++b){
        لكل(i = 0; i < spatial; ++i){
            عائم جوڑ = 0;
            لكل(f = 0; f < filters; ++f){
                صحيح اشاریہ = b*filters*spatial + f*spatial + i;
                جوڑ += powf(x[اشاریہ], 2);
            }
            جوڑ = sqrtf(جوڑ);
            لكل(f = 0; f < filters; ++f){
                صحيح اشاریہ = b*filters*spatial + f*spatial + i;
                x[اشاریہ] /= جوڑ;
                dx[اشاریہ] = (1 - x[اشاریہ]) / جوڑ;
            }
        }
    }
}


فراغ normalize_cpu(عائم *x, عائم *اوسط, عائم *variance, صحيح batch, صحيح filters, صحيح spatial)
{
    صحيح b, f, i;
    لكل(b = 0; b < batch; ++b){
        لكل(f = 0; f < filters; ++f){
            لكل(i = 0; i < spatial; ++i){
                صحيح اشاریہ = b*filters*spatial + f*spatial + i;
                x[اشاریہ] = (x[اشاریہ] - اوسط[f])/(جذر(variance[f]) + .000001f);
            }
        }
    }
}

فراغ const_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

فراغ mul_cpu(صحيح N, عائم *X, صحيح INCX, عائم *Y, صحيح INCY)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

فراغ pow_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX, عائم *Y, صحيح INCY)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) Y[i*INCY] = قوت(X[i*INCX], ALPHA);
}

فراغ axpy_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX, عائم *Y, صحيح INCY)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

فراغ scal_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

فراغ fill_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

فراغ deinter_cpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *باہرلا_2)
{
    صحيح i, j;
    صحيح اشاریہ = 0;
    لكل(j = 0; j < B; ++j) {
        لكل(i = 0; i < NX; ++i){
            إذا(X) X[j*NX + i] += باہرلا_2[اشاریہ];
            ++اشاریہ;
        }
        لكل(i = 0; i < NY; ++i){
            إذا(Y) Y[j*NY + i] += باہرلا_2[اشاریہ];
            ++اشاریہ;
        }
    }
}

فراغ inter_cpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *باہرلا_2)
{
    صحيح i, j;
    صحيح اشاریہ = 0;
    لكل(j = 0; j < B; ++j) {
        لكل(i = 0; i < NX; ++i){
            باہرلا_2[اشاریہ++] = X[j*NX + i];
        }
        لكل(i = 0; i < NY; ++i){
            باہرلا_2[اشاریہ++] = Y[j*NY + i];
        }
    }
}

فراغ copy_cpu(صحيح N, عائم *X, صحيح INCX, عائم *Y, صحيح INCY)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

فراغ mult_جوڑو_into_cpu(صحيح N, عائم *X, عائم *Y, عائم *Z)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

فراغ smooth_l1_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        عائم diff = truth[i] - pred[i];
        عائم abs_val = fabs(diff);
        إذا(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        وإلا {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

فراغ l1_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        عائم diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

فراغ softmax_x_ent_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        عائم t = truth[i];
        عائم p = pred[i];
        error[i] = (t) ? -لوگارتھم(p) : 0;
        delta[i] = t-p;
    }
}

فراغ logistic_x_ent_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        عائم t = truth[i];
        عائم p = pred[i];
        error[i] = -t*لوگارتھم(p) - (1-t)*لوگارتھم(1-p);
        delta[i] = t-p;
    }
}

فراغ l2_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        عائم diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

عائم dot_cpu(صحيح N, عائم *X, صحيح INCX, عائم *Y, صحيح INCY)
{
    صحيح i;
    عائم dot = 0;
    لكل(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    أرجع dot;
}

فراغ softmax(عائم *داخلہ, صحيح n, عائم عارضی, صحيح stride, عائم *output)
{
    صحيح i;
    عائم جوڑ = 0;
    عائم سبھ_توں_وڈا = -FLT_MAX;
    لكل(i = 0; i < n; ++i){
        إذا(داخلہ[i*stride] > سبھ_توں_وڈا) سبھ_توں_وڈا = داخلہ[i*stride];
    }
    لكل(i = 0; i < n; ++i){
        عائم e = قوت_نما(داخلہ[i*stride]/عارضی - سبھ_توں_وڈا/عارضی);
        جوڑ += e;
        output[i*stride] = e;
    }
    لكل(i = 0; i < n; ++i){
        output[i*stride] /= جوڑ;
    }
}


فراغ softmax_cpu(عائم *داخلہ, صحيح n, صحيح batch, صحيح batch_وتھ, صحيح groups, صحيح گروہ_وتھ, صحيح stride, عائم عارضی, عائم *output)
{
    صحيح g, b;
    لكل(b = 0; b < batch; ++b){
        لكل(g = 0; g < groups; ++g){
            softmax(داخلہ + b*batch_وتھ + g*گروہ_وتھ, n, عارضی, stride, output + b*batch_وتھ + g*گروہ_وتھ);
        }
    }
}

فراغ upsample_cpu(عائم *وچ, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم scale, عائم *باہرلا)
{
    صحيح i, j, k, b;
    لكل(b = 0; b < batch; ++b){
        لكل(k = 0; k < c; ++k){
            لكل(j = 0; j < h*stride; ++j){
                لكل(i = 0; i < w*stride; ++i){
                    صحيح وچ_اشاریہ = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    صحيح باہرلا_اشاریہ = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    إذا(forward) باہرلا[باہرلا_اشاریہ] = scale*وچ[وچ_اشاریہ];
                    وإلا وچ[وچ_اشاریہ] += scale*باہرلا[باہرلا_اشاریہ];
                }
            }
        }
    }
}


