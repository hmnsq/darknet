#ضمن "blas.h"

#ضمن <math.h>
#ضمن <assert.h>
#ضمن <float.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>
فراغ reorg_cpu(عائم *x, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم *بیرون)
{
    صحيح b,i,j,k;
    صحيح بیرون_c = c/(stride*stride);

    لكل(b = 0; b < batch; ++b){
        لكل(k = 0; k < c; ++k){
            لكل(j = 0; j < h; ++j){
                لكل(i = 0; i < w; ++i){
                    صحيح درون_شاخص  = i + w*(j + h*(k + c*b));
                    صحيح c2 = k % بیرون_c;
                    صحيح جابه_جایی = k / بیرون_c;
                    صحيح w2 = i*stride + جابه_جایی % stride;
                    صحيح h2 = j*stride + جابه_جایی / stride;
                    صحيح بیرون_شاخص = w2 + w*stride*(h2 + h*stride*(c2 + بیرون_c*b));
                    إذا(forward) بیرون[بیرون_شاخص] = x[درون_شاخص];
                    وإلا بیرون[درون_شاخص] = x[بیرون_شاخص];
                }
            }
        }
    }
}

فراغ flatten(عائم *x, صحيح اندازه, صحيح layers, صحيح batch, صحيح forward)
{
    عائم *جابجا = حافظه_صفر_بده(اندازه*layers*batch, حجم(عائم));
    صحيح i,c,b;
    لكل(b = 0; b < batch; ++b){
        لكل(c = 0; c < layers; ++c){
            لكل(i = 0; i < اندازه; ++i){
                صحيح i1 = b*layers*اندازه + c*اندازه + i;
                صحيح i2 = b*layers*اندازه + i*layers + c;
                إذا (forward) جابجا[i2] = x[i1];
                وإلا جابجا[i1] = x[i2];
            }
        }
    }
    کپی_حافظه(x, جابجا, اندازه*layers*batch*حجم(عائم));
    حرر(جابجا);
}

فراغ weighted_جمع_cpu(عائم *a, عائم *b, عائم *s, صحيح n, عائم *c)
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

فراغ shortcut_cpu(صحيح batch, صحيح w1, صحيح h1, صحيح c1, عائم *جمع_کن, صحيح w2, صحيح h2, صحيح c2, عائم s1, عائم s2, عائم *بیرون)
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
                    صحيح بیرون_شاخص = i*sample + w2*(j*sample + h2*(k + c2*b));
                    صحيح جمع_کن_شاخص = i*stride + w1*(j*stride + h1*(k + c1*b));
                    بیرون[بیرون_شاخص] = s1*بیرون[بیرون_شاخص] + s2*جمع_کن[جمع_کن_شاخص];
                }
            }
        }
    }
}

فراغ میانگین_cpu(عائم *x, صحيح batch, صحيح filters, صحيح spatial, عائم *میانگین)
{
    عائم scale = 1./(batch * spatial);
    صحيح i,j,k;
    لكل(i = 0; i < filters; ++i){
        میانگین[i] = 0;
        لكل(j = 0; j < batch; ++j){
            لكل(k = 0; k < spatial; ++k){
                صحيح شاخص = j*filters*spatial + i*spatial + k;
                میانگین[i] += x[شاخص];
            }
        }
        میانگین[i] *= scale;
    }
}

فراغ variance_cpu(عائم *x, عائم *میانگین, صحيح batch, صحيح filters, صحيح spatial, عائم *variance)
{
    عائم scale = 1./(batch * spatial - 1);
    صحيح i,j,k;
    لكل(i = 0; i < filters; ++i){
        variance[i] = 0;
        لكل(j = 0; j < batch; ++j){
            لكل(k = 0; k < spatial; ++k){
                صحيح شاخص = j*filters*spatial + i*spatial + k;
                variance[i] += توان((x[شاخص] - میانگین[i]), 2);
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
            عائم جمع = 0;
            لكل(f = 0; f < filters; ++f){
                صحيح شاخص = b*filters*spatial + f*spatial + i;
                جمع += powf(x[شاخص], 2);
            }
            جمع = sqrtf(جمع);
            لكل(f = 0; f < filters; ++f){
                صحيح شاخص = b*filters*spatial + f*spatial + i;
                x[شاخص] /= جمع;
                dx[شاخص] = (1 - x[شاخص]) / جمع;
            }
        }
    }
}


فراغ normalize_cpu(عائم *x, عائم *میانگین, عائم *variance, صحيح batch, صحيح filters, صحيح spatial)
{
    صحيح b, f, i;
    لكل(b = 0; b < batch; ++b){
        لكل(f = 0; f < filters; ++f){
            لكل(i = 0; i < spatial; ++i){
                صحيح شاخص = b*filters*spatial + f*spatial + i;
                x[شاخص] = (x[شاخص] - میانگین[f])/(جذر(variance[f]) + .000001f);
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
    لكل(i = 0; i < N; ++i) Y[i*INCY] = توان(X[i*INCX], ALPHA);
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

فراغ deinter_cpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *بیرون_2)
{
    صحيح i, j;
    صحيح شاخص = 0;
    لكل(j = 0; j < B; ++j) {
        لكل(i = 0; i < NX; ++i){
            إذا(X) X[j*NX + i] += بیرون_2[شاخص];
            ++شاخص;
        }
        لكل(i = 0; i < NY; ++i){
            إذا(Y) Y[j*NY + i] += بیرون_2[شاخص];
            ++شاخص;
        }
    }
}

فراغ inter_cpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *بیرون_2)
{
    صحيح i, j;
    صحيح شاخص = 0;
    لكل(j = 0; j < B; ++j) {
        لكل(i = 0; i < NX; ++i){
            بیرون_2[شاخص++] = X[j*NX + i];
        }
        لكل(i = 0; i < NY; ++i){
            بیرون_2[شاخص++] = Y[j*NY + i];
        }
    }
}

فراغ copy_cpu(صحيح N, عائم *X, صحيح INCX, عائم *Y, صحيح INCY)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

فراغ mult_جمع_کن_into_cpu(صحيح N, عائم *X, عائم *Y, عائم *Z)
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
        error[i] = (t) ? -لگاریتم(p) : 0;
        delta[i] = t-p;
    }
}

فراغ logistic_x_ent_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        عائم t = truth[i];
        عائم p = pred[i];
        error[i] = -t*لگاریتم(p) - (1-t)*لگاریتم(1-p);
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

فراغ softmax(عائم *درون_داد, صحيح n, عائم موقت, صحيح stride, عائم *output)
{
    صحيح i;
    عائم جمع = 0;
    عائم بزرگترین = -FLT_MAX;
    لكل(i = 0; i < n; ++i){
        إذا(درون_داد[i*stride] > بزرگترین) بزرگترین = درون_داد[i*stride];
    }
    لكل(i = 0; i < n; ++i){
        عائم e = نمایی_تابع(درون_داد[i*stride]/موقت - بزرگترین/موقت);
        جمع += e;
        output[i*stride] = e;
    }
    لكل(i = 0; i < n; ++i){
        output[i*stride] /= جمع;
    }
}


فراغ softmax_cpu(عائم *درون_داد, صحيح n, صحيح batch, صحيح batch_جابه_جایی, صحيح groups, صحيح گروه_جابه_جایی, صحيح stride, عائم موقت, عائم *output)
{
    صحيح g, b;
    لكل(b = 0; b < batch; ++b){
        لكل(g = 0; g < groups; ++g){
            softmax(درون_داد + b*batch_جابه_جایی + g*گروه_جابه_جایی, n, موقت, stride, output + b*batch_جابه_جایی + g*گروه_جابه_جایی);
        }
    }
}

فراغ upsample_cpu(عائم *درون, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم scale, عائم *بیرون)
{
    صحيح i, j, k, b;
    لكل(b = 0; b < batch; ++b){
        لكل(k = 0; k < c; ++k){
            لكل(j = 0; j < h*stride; ++j){
                لكل(i = 0; i < w*stride; ++i){
                    صحيح درون_شاخص = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    صحيح بیرون_شاخص = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    إذا(forward) بیرون[بیرون_شاخص] = scale*درون[درون_شاخص];
                    وإلا درون[درون_شاخص] += scale*بیرون[بیرون_شاخص];
                }
            }
        }
    }
}


