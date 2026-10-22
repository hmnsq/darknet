#ضمن "blas.h"

#ضمن <math.h>
#ضمن <assert.h>
#ضمن <float.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>
فراغ reorg_cpu(عائم *x, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم *ٻاهريون)
{
    صحيح b,i,j,k;
    صحيح ٻاهريون_c = c/(stride*stride);

    لكل(b = 0; b < batch; ++b){
        لكل(k = 0; k < c; ++k){
            لكل(j = 0; j < h; ++j){
                لكل(i = 0; i < w; ++i){
                    صحيح اندريون_اشاريو  = i + w*(j + h*(k + c*b));
                    صحيح c2 = k % ٻاهريون_c;
                    صحيح وٿي = k / ٻاهريون_c;
                    صحيح w2 = i*stride + وٿي % stride;
                    صحيح h2 = j*stride + وٿي / stride;
                    صحيح ٻاهريون_اشاريو = w2 + w*stride*(h2 + h*stride*(c2 + ٻاهريون_c*b));
                    إذا(forward) ٻاهريون[ٻاهريون_اشاريو] = x[اندريون_اشاريو];
                    وإلا ٻاهريون[اندريون_اشاريو] = x[ٻاهريون_اشاريو];
                }
            }
        }
    }
}

فراغ flatten(عائم *x, صحيح سائيز, صحيح layers, صحيح batch, صحيح forward)
{
    عائم *مٽا_سٽا = صفر_يادگيري_ڏيو(سائيز*layers*batch, حجم(عائم));
    صحيح i,c,b;
    لكل(b = 0; b < batch; ++b){
        لكل(c = 0; c < layers; ++c){
            لكل(i = 0; i < سائيز; ++i){
                صحيح i1 = b*layers*سائيز + c*سائيز + i;
                صحيح i2 = b*layers*سائيز + i*layers + c;
                إذا (forward) مٽا_سٽا[i2] = x[i1];
                وإلا مٽا_سٽا[i1] = x[i2];
            }
        }
    }
    يادگيري_نقل(x, مٽا_سٽا, سائيز*layers*batch*حجم(عائم));
    حرر(مٽا_سٽا);
}

فراغ weighted_جوڙ_cpu(عائم *a, عائم *b, عائم *s, صحيح n, عائم *c)
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

فراغ shortcut_cpu(صحيح batch, صحيح w1, صحيح h1, صحيح c1, عائم *جوڙيو, صحيح w2, صحيح h2, صحيح c2, عائم s1, عائم s2, عائم *ٻاهريون)
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
                    صحيح ٻاهريون_اشاريو = i*sample + w2*(j*sample + h2*(k + c2*b));
                    صحيح جوڙيو_اشاريو = i*stride + w1*(j*stride + h1*(k + c1*b));
                    ٻاهريون[ٻاهريون_اشاريو] = s1*ٻاهريون[ٻاهريون_اشاريو] + s2*جوڙيو[جوڙيو_اشاريو];
                }
            }
        }
    }
}

فراغ سراسري_cpu(عائم *x, صحيح batch, صحيح filters, صحيح spatial, عائم *سراسري)
{
    عائم scale = 1./(batch * spatial);
    صحيح i,j,k;
    لكل(i = 0; i < filters; ++i){
        سراسري[i] = 0;
        لكل(j = 0; j < batch; ++j){
            لكل(k = 0; k < spatial; ++k){
                صحيح اشاريو = j*filters*spatial + i*spatial + k;
                سراسري[i] += x[اشاريو];
            }
        }
        سراسري[i] *= scale;
    }
}

فراغ variance_cpu(عائم *x, عائم *سراسري, صحيح batch, صحيح filters, صحيح spatial, عائم *variance)
{
    عائم scale = 1./(batch * spatial - 1);
    صحيح i,j,k;
    لكل(i = 0; i < filters; ++i){
        variance[i] = 0;
        لكل(j = 0; j < batch; ++j){
            لكل(k = 0; k < spatial; ++k){
                صحيح اشاريو = j*filters*spatial + i*spatial + k;
                variance[i] += قوت((x[اشاريو] - سراسري[i]), 2);
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
            عائم جوڙ = 0;
            لكل(f = 0; f < filters; ++f){
                صحيح اشاريو = b*filters*spatial + f*spatial + i;
                جوڙ += powf(x[اشاريو], 2);
            }
            جوڙ = sqrtf(جوڙ);
            لكل(f = 0; f < filters; ++f){
                صحيح اشاريو = b*filters*spatial + f*spatial + i;
                x[اشاريو] /= جوڙ;
                dx[اشاريو] = (1 - x[اشاريو]) / جوڙ;
            }
        }
    }
}


فراغ normalize_cpu(عائم *x, عائم *سراسري, عائم *variance, صحيح batch, صحيح filters, صحيح spatial)
{
    صحيح b, f, i;
    لكل(b = 0; b < batch; ++b){
        لكل(f = 0; f < filters; ++f){
            لكل(i = 0; i < spatial; ++i){
                صحيح اشاريو = b*filters*spatial + f*spatial + i;
                x[اشاريو] = (x[اشاريو] - سراسري[f])/(جذر(variance[f]) + .000001f);
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

فراغ deinter_cpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *ٻاهريون_2)
{
    صحيح i, j;
    صحيح اشاريو = 0;
    لكل(j = 0; j < B; ++j) {
        لكل(i = 0; i < NX; ++i){
            إذا(X) X[j*NX + i] += ٻاهريون_2[اشاريو];
            ++اشاريو;
        }
        لكل(i = 0; i < NY; ++i){
            إذا(Y) Y[j*NY + i] += ٻاهريون_2[اشاريو];
            ++اشاريو;
        }
    }
}

فراغ inter_cpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *ٻاهريون_2)
{
    صحيح i, j;
    صحيح اشاريو = 0;
    لكل(j = 0; j < B; ++j) {
        لكل(i = 0; i < NX; ++i){
            ٻاهريون_2[اشاريو++] = X[j*NX + i];
        }
        لكل(i = 0; i < NY; ++i){
            ٻاهريون_2[اشاريو++] = Y[j*NY + i];
        }
    }
}

فراغ copy_cpu(صحيح N, عائم *X, صحيح INCX, عائم *Y, صحيح INCY)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

فراغ mult_جوڙيو_into_cpu(صحيح N, عائم *X, عائم *Y, عائم *Z)
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
        error[i] = (t) ? -لاگرٿم(p) : 0;
        delta[i] = t-p;
    }
}

فراغ logistic_x_ent_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        عائم t = truth[i];
        عائم p = pred[i];
        error[i] = -t*لاگرٿم(p) - (1-t)*لاگرٿم(1-p);
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

فراغ softmax(عائم *داخلا, صحيح n, عائم عارضي, صحيح stride, عائم *output)
{
    صحيح i;
    عائم جوڙ = 0;
    عائم سڀ_کان_وڏو = -FLT_MAX;
    لكل(i = 0; i < n; ++i){
        إذا(داخلا[i*stride] > سڀ_کان_وڏو) سڀ_کان_وڏو = داخلا[i*stride];
    }
    لكل(i = 0; i < n; ++i){
        عائم e = قوت_نما(داخلا[i*stride]/عارضي - سڀ_کان_وڏو/عارضي);
        جوڙ += e;
        output[i*stride] = e;
    }
    لكل(i = 0; i < n; ++i){
        output[i*stride] /= جوڙ;
    }
}


فراغ softmax_cpu(عائم *داخلا, صحيح n, صحيح batch, صحيح batch_وٿي, صحيح groups, صحيح گروهه_وٿي, صحيح stride, عائم عارضي, عائم *output)
{
    صحيح g, b;
    لكل(b = 0; b < batch; ++b){
        لكل(g = 0; g < groups; ++g){
            softmax(داخلا + b*batch_وٿي + g*گروهه_وٿي, n, عارضي, stride, output + b*batch_وٿي + g*گروهه_وٿي);
        }
    }
}

فراغ upsample_cpu(عائم *اندريون, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم scale, عائم *ٻاهريون)
{
    صحيح i, j, k, b;
    لكل(b = 0; b < batch; ++b){
        لكل(k = 0; k < c; ++k){
            لكل(j = 0; j < h*stride; ++j){
                لكل(i = 0; i < w*stride; ++i){
                    صحيح اندريون_اشاريو = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    صحيح ٻاهريون_اشاريو = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    إذا(forward) ٻاهريون[ٻاهريون_اشاريو] = scale*اندريون[اندريون_اشاريو];
                    وإلا اندريون[اندريون_اشاريو] += scale*ٻاهريون[ٻاهريون_اشاريو];
                }
            }
        }
    }
}


