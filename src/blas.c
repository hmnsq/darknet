#شامل "blas.h"

#شامل <math.h>
#شامل <assert.h>
#شامل <float.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>
تهی reorg_cpu(اعشاری *x, صحیح w, صحیح h, صحیح c, صحیح batch, صحیح stride, صحیح forward, اعشاری *out)
{
    صحیح b,i,j,k;
    صحیح out_c = c/(stride*stride);

    برای(b = 0; b < batch; ++b){
        برای(k = 0; k < c; ++k){
            برای(j = 0; j < h; ++j){
                برای(i = 0; i < w; ++i){
                    صحیح in_index  = i + w*(j + h*(k + c*b));
                    صحیح c2 = k % out_c;
                    صحیح offset = k / out_c;
                    صحیح w2 = i*stride + offset % stride;
                    صحیح h2 = j*stride + offset / stride;
                    صحیح out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    اگر(forward) out[out_index] = x[in_index];
                    وگرنه out[in_index] = x[out_index];
                }
            }
        }
    }
}

تهی flatten(اعشاری *x, صحیح size, صحیح layers, صحیح batch, صحیح forward)
{
    اعشاری *swap = calloc(size*layers*batch, اندازه(اعشاری));
    صحیح i,c,b;
    برای(b = 0; b < batch; ++b){
        برای(c = 0; c < layers; ++c){
            برای(i = 0; i < size; ++i){
                صحیح i1 = b*layers*size + c*size + i;
                صحیح i2 = b*layers*size + i*layers + c;
                اگر (forward) swap[i2] = x[i1];
                وگرنه swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*اندازه(اعشاری));
    آزاد(swap);
}

تهی weighted_sum_cpu(اعشاری *a, اعشاری *b, اعشاری *s, صحیح n, اعشاری *c)
{
    صحیح i;
    برای(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

تهی weighted_delta_cpu(اعشاری *a, اعشاری *b, اعشاری *s, اعشاری *da, اعشاری *db, اعشاری *ds, صحیح n, اعشاری *dc)
{
    صحیح i;
    برای(i = 0; i < n; ++i){
        اگر(da) da[i] += dc[i] * s[i];
        اگر(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

تهی shortcut_cpu(صحیح batch, صحیح w1, صحیح h1, صحیح c1, اعشاری *add, صحیح w2, صحیح h2, صحیح c2, اعشاری s1, اعشاری s2, اعشاری *out)
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
    برای(b = 0; b < batch; ++b){
        برای(k = 0; k < minc; ++k){
            برای(j = 0; j < minh; ++j){
                برای(i = 0; i < minw; ++i){
                    صحیح out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    صحیح add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

تهی mean_cpu(اعشاری *x, صحیح batch, صحیح filters, صحیح spatial, اعشاری *mean)
{
    اعشاری scale = 1./(batch * spatial);
    صحیح i,j,k;
    برای(i = 0; i < filters; ++i){
        mean[i] = 0;
        برای(j = 0; j < batch; ++j){
            برای(k = 0; k < spatial; ++k){
                صحیح index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

تهی variance_cpu(اعشاری *x, اعشاری *mean, صحیح batch, صحیح filters, صحیح spatial, اعشاری *variance)
{
    اعشاری scale = 1./(batch * spatial - 1);
    صحیح i,j,k;
    برای(i = 0; i < filters; ++i){
        variance[i] = 0;
        برای(j = 0; j < batch; ++j){
            برای(k = 0; k < spatial; ++k){
                صحیح index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

تهی l2normalize_cpu(اعشاری *x, اعشاری *dx, صحیح batch, صحیح filters, صحیح spatial)
{
    صحیح b,f,i;
    برای(b = 0; b < batch; ++b){
        برای(i = 0; i < spatial; ++i){
            اعشاری sum = 0;
            برای(f = 0; f < filters; ++f){
                صحیح index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            برای(f = 0; f < filters; ++f){
                صحیح index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


تهی normalize_cpu(اعشاری *x, اعشاری *mean, اعشاری *variance, صحیح batch, صحیح filters, صحیح spatial)
{
    صحیح b, f, i;
    برای(b = 0; b < batch; ++b){
        برای(f = 0; f < filters; ++f){
            برای(i = 0; i < spatial; ++i){
                صحیح index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

تهی const_cpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX)
{
    صحیح i;
    برای(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

تهی mul_cpu(صحیح N, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY)
{
    صحیح i;
    برای(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

تهی pow_cpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY)
{
    صحیح i;
    برای(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

تهی axpy_cpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY)
{
    صحیح i;
    برای(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

تهی scal_cpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX)
{
    صحیح i;
    برای(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

تهی fill_cpu(صحیح N, اعشاری ALPHA, اعشاری *X, صحیح INCX)
{
    صحیح i;
    برای(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

تهی deinter_cpu(صحیح NX, اعشاری *X, صحیح NY, اعشاری *Y, صحیح B, اعشاری *OUT)
{
    صحیح i, j;
    صحیح index = 0;
    برای(j = 0; j < B; ++j) {
        برای(i = 0; i < NX; ++i){
            اگر(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        برای(i = 0; i < NY; ++i){
            اگر(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

تهی inter_cpu(صحیح NX, اعشاری *X, صحیح NY, اعشاری *Y, صحیح B, اعشاری *OUT)
{
    صحیح i, j;
    صحیح index = 0;
    برای(j = 0; j < B; ++j) {
        برای(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        برای(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

تهی copy_cpu(صحیح N, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY)
{
    صحیح i;
    برای(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

تهی mult_add_into_cpu(صحیح N, اعشاری *X, اعشاری *Y, اعشاری *Z)
{
    صحیح i;
    برای(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

تهی smooth_l1_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error)
{
    صحیح i;
    برای(i = 0; i < n; ++i){
        اعشاری diff = truth[i] - pred[i];
        اعشاری abs_val = fabs(diff);
        اگر(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        وگرنه {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

تهی l1_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error)
{
    صحیح i;
    برای(i = 0; i < n; ++i){
        اعشاری diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

تهی softmax_x_ent_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error)
{
    صحیح i;
    برای(i = 0; i < n; ++i){
        اعشاری t = truth[i];
        اعشاری p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

تهی logistic_x_ent_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error)
{
    صحیح i;
    برای(i = 0; i < n; ++i){
        اعشاری t = truth[i];
        اعشاری p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

تهی l2_cpu(صحیح n, اعشاری *pred, اعشاری *truth, اعشاری *delta, اعشاری *error)
{
    صحیح i;
    برای(i = 0; i < n; ++i){
        اعشاری diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

اعشاری dot_cpu(صحیح N, اعشاری *X, صحیح INCX, اعشاری *Y, صحیح INCY)
{
    صحیح i;
    اعشاری dot = 0;
    برای(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    بازگشت dot;
}

تهی softmax(اعشاری *درون_داد, صحیح n, اعشاری temp, صحیح stride, اعشاری *output)
{
    صحیح i;
    اعشاری sum = 0;
    اعشاری largest = -FLT_MAX;
    برای(i = 0; i < n; ++i){
        اگر(درون_داد[i*stride] > largest) largest = درون_داد[i*stride];
    }
    برای(i = 0; i < n; ++i){
        اعشاری e = exp(درون_داد[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    برای(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


تهی softmax_cpu(اعشاری *درون_داد, صحیح n, صحیح batch, صحیح batch_offset, صحیح groups, صحیح group_offset, صحیح stride, اعشاری temp, اعشاری *output)
{
    صحیح g, b;
    برای(b = 0; b < batch; ++b){
        برای(g = 0; g < groups; ++g){
            softmax(درون_داد + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

تهی upsample_cpu(اعشاری *in, صحیح w, صحیح h, صحیح c, صحیح batch, صحیح stride, صحیح forward, اعشاری scale, اعشاری *out)
{
    صحیح i, j, k, b;
    برای(b = 0; b < batch; ++b){
        برای(k = 0; k < c; ++k){
            برای(j = 0; j < h*stride; ++j){
                برای(i = 0; i < w*stride; ++i){
                    صحیح in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    صحیح out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    اگر(forward) out[out_index] = scale*in[in_index];
                    وگرنه in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


