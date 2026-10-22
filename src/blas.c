#شامل "blas.h"

#شامل <math.h>
#شامل <assert.h>
#شامل <float.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>
خالی reorg_cpu(اعشاری *x, صحیح w, صحیح h, صحیح c, صحیح batch, صحیح stride, صحیح forward, اعشاری *برآمد)
{
    صحیح b,i,j,k;
    صحیح برآمد_c = c/(stride*stride);

    برائے(b = 0; b < batch; ++b){
        برائے(k = 0; k < c; ++k){
            برائے(j = 0; j < h; ++j){
                برائے(i = 0; i < w; ++i){
                    صحیح اندر_اشاریہ  = i + w*(j + h*(k + c*b));
                    صحیح c2 = k % برآمد_c;
                    صحیح انحراف = k / برآمد_c;
                    صحیح w2 = i*stride + انحراف % stride;
                    صحیح h2 = j*stride + انحراف / stride;
                    صحیح برآمد_اشاریہ = w2 + w*stride*(h2 + h*stride*(c2 + برآمد_c*b));
                    اگر(forward) برآمد[برآمد_اشاریہ] = x[اندر_اشاریہ];
                    ورنہ برآمد[اندر_اشاریہ] = x[برآمد_اشاریہ];
                }
            }
        }
    }
}

خالی flatten(اعشاری *x, صحیح سائز, صحیح layers, صحیح batch, صحیح forward)
{
    اعشاری *تبادلہ = صفر_حافظہ_دو(سائز*layers*batch, حجم(اعشاری));
    صحیح i,c,b;
    برائے(b = 0; b < batch; ++b){
        برائے(c = 0; c < layers; ++c){
            برائے(i = 0; i < سائز; ++i){
                صحیح i1 = b*layers*سائز + c*سائز + i;
                صحیح i2 = b*layers*سائز + i*layers + c;
                اگر (forward) تبادلہ[i2] = x[i1];
                ورنہ تبادلہ[i1] = x[i2];
            }
        }
    }
    حافظہ_نقل(x, تبادلہ, سائز*layers*batch*حجم(اعشاری));
    آزاد(تبادلہ);
}

خالی weighted_مجموع_cpu(اعشاری *a, اعشاری *b, اعشاری *s, صحیح n, اعشاری *c)
{
    صحیح i;
    برائے(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

خالی weighted_delta_cpu(اعشاری *a, اعشاری *b, اعشاری *s, اعشاری *da, اعشاری *db, اعشاری *ds, صحیح n, اعشاری *dc)
{
    صحیح i;
    برائے(i = 0; i < n; ++i){
        اگر(da) da[i] += dc[i] * s[i];
        اگر(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

خالی shortcut_cpu(صحیح batch, صحیح w1, صحیح h1, صحیح c1, اعشاری *جوڑو, صحیح w2, صحیح h2, صحیح c2, اعشاری s1, اعشاری s2, اعشاری *برآمد)
{
    صحیح stride = w1/w2;
    صحیح sample = w2/w1;
    أكد(stride == h1/h2);
    أكد(sample == h2/h1);
    اگر(stride < 1) stride = 1;
    اگر(sample < 1) sample = 1;
    صحیح minw = (w1 < w2) ? w1 : w2;
    صحیح minh = (h1 < h2) ? h1 : h2;
    صحیح minc = (c1 < c2) ? c1 : c2;

    صحیح i,j,k,b;
    برائے(b = 0; b < batch; ++b){
        برائے(k = 0; k < minc; ++k){
            برائے(j = 0; j < minh; ++j){
                برائے(i = 0; i < minw; ++i){
                    صحیح برآمد_اشاریہ = i*sample + w2*(j*sample + h2*(k + c2*b));
                    صحیح جوڑو_اشاریہ = i*stride + w1*(j*stride + h1*(k + c1*b));
                    برآمد[برآمد_اشاریہ] = s1*برآمد[برآمد_اشاریہ] + s2*جوڑو[جوڑو_اشاریہ];
                }
            }
        }
    }
}

خالی اوسط_cpu(اعشاری *x, صحیح batch, صحیح filters, صحیح spatial, اعشاری *اوسط)
{
    اعشاری scale = 1./(batch * spatial);
    صحیح i,j,k;
    برائے(i = 0; i < filters; ++i){
        اوسط[i] = 0;
        برائے(j = 0; j < batch; ++j){
            برائے(k = 0; k < spatial; ++k){
                صحیح اشاریہ = j*filters*spatial + i*spatial + k;
                اوسط[i] += x[اشاریہ];
            }
        }
        اوسط[i] *= scale;
    }
}

خالی variance_cpu(اعشاری *x, اعشاری *اوسط, صحیح batch, صحیح filters, صحیح spatial, اعشاری *variance)
{
    اعشاری scale = 1./(batch * spatial - 1);
    صحیح i,j,k;
    برائے(i = 0; i < filters; ++i){
        variance[i] = 0;
        برائے(j = 0; j < batch; ++j){
            برائے(k = 0; k < spatial; ++k){
                صحیح اشاریہ = j*filters*spatial + i*spatial + k;
                variance[i] += قوت((x[اشاریہ] - اوسط[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

خالی l2normalize_cpu(اعشاری *x, اعشاری *dx, صحیح batch, صحیح filters, صحیح spatial)
{
    صحیح b,f,i;
    برائے(b = 0; b < batch; ++b){
        برائے(i = 0; i < spatial; ++i){
            اعشاری مجموع = 0;
            برائے(f = 0; f < filters; ++f){
                صحیح اشاریہ = b*filters*spatial + f*spatial + i;
                مجموع += powf(x[اشاریہ], 2);
            }
            مجموع = sqrtf(مجموع);
            برائے(f = 0; f < filters; ++f){
                صحیح اشاریہ = b*filters*spatial + f*spatial + i;
                x[اشاریہ] /= مجموع;
                dx[اشاریہ] = (1 - x[اشاریہ]) / مجموع;
            }
        }
    }
}


خالی normalize_cpu(اعشاری *x, اعشاری *اوسط, اعشاری *variance, صحیح batch, صحیح filters, صحیح spatial)
{
    صحیح b, f, i;
    برائے(b = 0; b < batch; ++b){
        برائے(f = 0; f < filters; ++f){
            برائے(i = 0; i < spatial; ++i){
                صحیح اشاریہ = b*filters*spatial + f*spatial + i;
                x[اشاریہ] = (x[اشاریہ] - اوسط[f])/(جذر(variance[f]) + .000001f);
            }
        }
    }
}

خالی const_cpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX)
{
    صحیح i;
    برائے(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

خالی mul_cpu(صحیح N, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY)
{
    صحیح i;
    برائے(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

خالی pow_cpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY)
{
    صحیح i;
    برائے(i = 0; i < N; ++i) Y[i*INCY] = قوت(X[i*INCX], ALPHA);
}

خالی axpy_cpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY)
{
    صحیح i;
    برائے(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

خالی scal_cpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX)
{
    صحیح i;
    برائے(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

خالی fill_cpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX)
{
    صحیح i;
    برائے(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

خالی deinter_cpu(صحیح NX, اعشاری *X, صحیح NY, اعشاری *Y, صحیح B, اعشاری *برآمد_2)
{
    صحیح i, j;
    صحیح اشاریہ = 0;
    برائے(j = 0; j < B; ++j) {
        برائے(i = 0; i < NX; ++i){
            اگر(X) X[j*NX + i] += برآمد_2[اشاریہ];
            ++اشاریہ;
        }
        برائے(i = 0; i < NY; ++i){
            اگر(Y) Y[j*NY + i] += برآمد_2[اشاریہ];
            ++اشاریہ;
        }
    }
}

خالی inter_cpu(صحیح NX, اعشاری *X, صحیح NY, اعشاری *Y, صحیح B, اعشاری *برآمد_2)
{
    صحیح i, j;
    صحیح اشاریہ = 0;
    برائے(j = 0; j < B; ++j) {
        برائے(i = 0; i < NX; ++i){
            برآمد_2[اشاریہ++] = X[j*NX + i];
        }
        برائے(i = 0; i < NY; ++i){
            برآمد_2[اشاریہ++] = Y[j*NY + i];
        }
    }
}

خالی copy_cpu(صحیح N, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY)
{
    صحیح i;
    برائے(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

خالی mult_جوڑو_into_cpu(صحیح N, اعشاری *X, اعشاری *Y, اعشاری *Z)
{
    صحیح i;
    برائے(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

خالی smooth_l1_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error)
{
    صحیح i;
    برائے(i = 0; i < n; ++i){
        اعشاری diff = truth[i] - pred[i];
        اعشاری abs_val = fabs(diff);
        اگر(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        ورنہ {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

خالی l1_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error)
{
    صحیح i;
    برائے(i = 0; i < n; ++i){
        اعشاری diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

خالی softmax_x_ent_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error)
{
    صحیح i;
    برائے(i = 0; i < n; ++i){
        اعشاری t = truth[i];
        اعشاری p = pred[i];
        error[i] = (t) ? -لوگارتھم(p) : 0;
        delta[i] = t-p;
    }
}

خالی logistic_x_ent_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error)
{
    صحیح i;
    برائے(i = 0; i < n; ++i){
        اعشاری t = truth[i];
        اعشاری p = pred[i];
        error[i] = -t*لوگارتھم(p) - (1-t)*لوگارتھم(1-p);
        delta[i] = t-p;
    }
}

خالی l2_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error)
{
    صحیح i;
    برائے(i = 0; i < n; ++i){
        اعشاری diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

اعشاری dot_cpu(صحیح N, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY)
{
    صحیح i;
    اعشاری dot = 0;
    برائے(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    واپس dot;
}

خالی softmax(اعشاری *داخلہ, صحیح n, اعشاری عارضی, صحیح stride, اعشاری *output)
{
    صحیح i;
    اعشاری مجموع = 0;
    اعشاری سب_سے_بڑا = -FLT_MAX;
    برائے(i = 0; i < n; ++i){
        اگر(داخلہ[i*stride] > سب_سے_بڑا) سب_سے_بڑا = داخلہ[i*stride];
    }
    برائے(i = 0; i < n; ++i){
        اعشاری e = قوت_نما(داخلہ[i*stride]/عارضی - سب_سے_بڑا/عارضی);
        مجموع += e;
        output[i*stride] = e;
    }
    برائے(i = 0; i < n; ++i){
        output[i*stride] /= مجموع;
    }
}


خالی softmax_cpu(اعشاری *داخلہ, صحیح n, صحیح batch, صحیح batch_انحراف, صحیح groups, صحیح گروہ_انحراف, صحیح stride, اعشاری عارضی, اعشاری *output)
{
    صحیح g, b;
    برائے(b = 0; b < batch; ++b){
        برائے(g = 0; g < groups; ++g){
            softmax(داخلہ + b*batch_انحراف + g*گروہ_انحراف, n, عارضی, stride, output + b*batch_انحراف + g*گروہ_انحراف);
        }
    }
}

خالی upsample_cpu(اعشاری *اندر, صحیح w, صحیح h, صحیح c, صحیح batch, صحیح stride, صحیح forward, اعشاری scale, اعشاری *برآمد)
{
    صحیح i, j, k, b;
    برائے(b = 0; b < batch; ++b){
        برائے(k = 0; k < c; ++k){
            برائے(j = 0; j < h*stride; ++j){
                برائے(i = 0; i < w*stride; ++i){
                    صحیح اندر_اشاریہ = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    صحیح برآمد_اشاریہ = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    اگر(forward) برآمد[برآمد_اشاریہ] = scale*اندر[اندر_اشاریہ];
                    ورنہ اندر[اندر_اشاریہ] += scale*برآمد[برآمد_اشاریہ];
                }
            }
        }
    }
}


