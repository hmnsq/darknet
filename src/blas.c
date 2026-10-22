#অন্তর্ভাব "blas.h"

#অন্তর্ভাব <math.h>
#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <float.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>
শূন্য reorg_cpu(দশমিক *x, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা forward, দশমিক *out)
{
    পূর্ণসংখ্যা b,i,j,k;
    পূর্ণসংখ্যা out_c = c/(stride*stride);

    জন্য(b = 0; b < batch; ++b){
        জন্য(k = 0; k < c; ++k){
            জন্য(j = 0; j < h; ++j){
                জন্য(i = 0; i < w; ++i){
                    পূর্ণসংখ্যা in_সূচক  = i + w*(j + h*(k + c*b));
                    পূর্ণসংখ্যা c2 = k % out_c;
                    পূর্ণসংখ্যা offset = k / out_c;
                    পূর্ণসংখ্যা w2 = i*stride + offset % stride;
                    পূর্ণসংখ্যা h2 = j*stride + offset / stride;
                    পূর্ণসংখ্যা out_সূচক = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    যদি(forward) out[out_সূচক] = x[in_সূচক];
                    অন্যথা out[in_সূচক] = x[out_সূচক];
                }
            }
        }
    }
}

শূন্য flatten(দশমিক *x, পূর্ণসংখ্যা আকার_2, পূর্ণসংখ্যা layers, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা forward)
{
    দশমিক *swap = calloc(আকার_2*layers*batch, আকার(দশমিক));
    পূর্ণসংখ্যা i,c,b;
    জন্য(b = 0; b < batch; ++b){
        জন্য(c = 0; c < layers; ++c){
            জন্য(i = 0; i < আকার_2; ++i){
                পূর্ণসংখ্যা i1 = b*layers*আকার_2 + c*আকার_2 + i;
                পূর্ণসংখ্যা i2 = b*layers*আকার_2 + i*layers + c;
                যদি (forward) swap[i2] = x[i1];
                অন্যথা swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, আকার_2*layers*batch*আকার(দশমিক));
    মুক্ত(swap);
}

শূন্য weighted_যোগ_cpu(দশমিক *a, দশমিক *b, দশমিক *s, পূর্ণসংখ্যা n, দশমিক *c)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

শূন্য weighted_delta_cpu(দশমিক *a, দশমিক *b, দশমিক *s, দশমিক *da, দশমিক *db, দশমিক *ds, পূর্ণসংখ্যা n, দশমিক *dc)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        যদি(da) da[i] += dc[i] * s[i];
        যদি(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

শূন্য shortcut_cpu(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w1, পূর্ণসংখ্যা h1, পূর্ণসংখ্যা c1, দশমিক *add, পূর্ণসংখ্যা w2, পূর্ণসংখ্যা h2, পূর্ণসংখ্যা c2, দশমিক s1, দশমিক s2, দশমিক *out)
{
    পূর্ণসংখ্যা stride = w1/w2;
    পূর্ণসংখ্যা sample = w2/w1;
    প্রতিজ্ঞা(stride == h1/h2);
    প্রতিজ্ঞা(sample == h2/h1);
    যদি(stride < 1) stride = 1;
    যদি(sample < 1) sample = 1;
    পূর্ণসংখ্যা minw = (w1 < w2) ? w1 : w2;
    পূর্ণসংখ্যা minh = (h1 < h2) ? h1 : h2;
    পূর্ণসংখ্যা minc = (c1 < c2) ? c1 : c2;

    পূর্ণসংখ্যা i,j,k,b;
    জন্য(b = 0; b < batch; ++b){
        জন্য(k = 0; k < minc; ++k){
            জন্য(j = 0; j < minh; ++j){
                জন্য(i = 0; i < minw; ++i){
                    পূর্ণসংখ্যা out_সূচক = i*sample + w2*(j*sample + h2*(k + c2*b));
                    পূর্ণসংখ্যা add_সূচক = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_সূচক] = s1*out[out_সূচক] + s2*add[add_সূচক];
                }
            }
        }
    }
}

শূন্য mean_cpu(দশমিক *x, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *mean)
{
    দশমিক scale = 1./(batch * spatial);
    পূর্ণসংখ্যা i,j,k;
    জন্য(i = 0; i < filters; ++i){
        mean[i] = 0;
        জন্য(j = 0; j < batch; ++j){
            জন্য(k = 0; k < spatial; ++k){
                পূর্ণসংখ্যা সূচক = j*filters*spatial + i*spatial + k;
                mean[i] += x[সূচক];
            }
        }
        mean[i] *= scale;
    }
}

শূন্য variance_cpu(দশমিক *x, দশমিক *mean, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *variance)
{
    দশমিক scale = 1./(batch * spatial - 1);
    পূর্ণসংখ্যা i,j,k;
    জন্য(i = 0; i < filters; ++i){
        variance[i] = 0;
        জন্য(j = 0; j < batch; ++j){
            জন্য(k = 0; k < spatial; ++k){
                পূর্ণসংখ্যা সূচক = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[সূচক] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

শূন্য l2normalize_cpu(দশমিক *x, দশমিক *dx, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial)
{
    পূর্ণসংখ্যা b,f,i;
    জন্য(b = 0; b < batch; ++b){
        জন্য(i = 0; i < spatial; ++i){
            দশমিক যোগ = 0;
            জন্য(f = 0; f < filters; ++f){
                পূর্ণসংখ্যা সূচক = b*filters*spatial + f*spatial + i;
                যোগ += powf(x[সূচক], 2);
            }
            যোগ = sqrtf(যোগ);
            জন্য(f = 0; f < filters; ++f){
                পূর্ণসংখ্যা সূচক = b*filters*spatial + f*spatial + i;
                x[সূচক] /= যোগ;
                dx[সূচক] = (1 - x[সূচক]) / যোগ;
            }
        }
    }
}


শূন্য normalize_cpu(দশমিক *x, দশমিক *mean, দশমিক *variance, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial)
{
    পূর্ণসংখ্যা b, f, i;
    জন্য(b = 0; b < batch; ++b){
        জন্য(f = 0; f < filters; ++f){
            জন্য(i = 0; i < spatial; ++i){
                পূর্ণসংখ্যা সূচক = b*filters*spatial + f*spatial + i;
                x[সূচক] = (x[সূচক] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

শূন্য const_cpu(পূর্ণসংখ্যা N, দশমিক ALPHA, দশমিক *X, পূর্ণসংখ্যা INCX)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

শূন্য mul_cpu(পূর্ণসংখ্যা N, দশমিক *X, পূর্ণসংখ্যা INCX, দশমিক *Y, পূর্ণসংখ্যা INCY)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

শূন্য pow_cpu(পূর্ণসংখ্যা N, দশমিক ALPHA, দশমিক *X, পূর্ণসংখ্যা INCX, দশমিক *Y, পূর্ণসংখ্যা INCY)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

শূন্য axpy_cpu(পূর্ণসংখ্যা N, দশমিক ALPHA, দশমিক *X, পূর্ণসংখ্যা INCX, দশমিক *Y, পূর্ণসংখ্যা INCY)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

শূন্য scal_cpu(পূর্ণসংখ্যা N, দশমিক ALPHA, দশমিক *X, পূর্ণসংখ্যা INCX)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

শূন্য fill_cpu(পূর্ণসংখ্যা N, দশমিক ALPHA, দশমিক *X, পূর্ণসংখ্যা INCX)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

শূন্য deinter_cpu(পূর্ণসংখ্যা NX, দশমিক *X, পূর্ণসংখ্যা NY, দশমিক *Y, পূর্ণসংখ্যা B, দশমিক *OUT)
{
    পূর্ণসংখ্যা i, j;
    পূর্ণসংখ্যা সূচক = 0;
    জন্য(j = 0; j < B; ++j) {
        জন্য(i = 0; i < NX; ++i){
            যদি(X) X[j*NX + i] += OUT[সূচক];
            ++সূচক;
        }
        জন্য(i = 0; i < NY; ++i){
            যদি(Y) Y[j*NY + i] += OUT[সূচক];
            ++সূচক;
        }
    }
}

শূন্য inter_cpu(পূর্ণসংখ্যা NX, দশমিক *X, পূর্ণসংখ্যা NY, দশমিক *Y, পূর্ণসংখ্যা B, দশমিক *OUT)
{
    পূর্ণসংখ্যা i, j;
    পূর্ণসংখ্যা সূচক = 0;
    জন্য(j = 0; j < B; ++j) {
        জন্য(i = 0; i < NX; ++i){
            OUT[সূচক++] = X[j*NX + i];
        }
        জন্য(i = 0; i < NY; ++i){
            OUT[সূচক++] = Y[j*NY + i];
        }
    }
}

শূন্য copy_cpu(পূর্ণসংখ্যা N, দশমিক *X, পূর্ণসংখ্যা INCX, দশমিক *Y, পূর্ণসংখ্যা INCY)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

শূন্য mult_add_into_cpu(পূর্ণসংখ্যা N, দশমিক *X, দশমিক *Y, দশমিক *Z)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

শূন্য smooth_l1_cpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        দশমিক diff = truth[i] - pred[i];
        দশমিক abs_val = fabs(diff);
        যদি(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        অন্যথা {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

শূন্য l1_cpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        দশমিক diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

শূন্য softmax_x_ent_cpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        দশমিক t = truth[i];
        দশমিক p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

শূন্য logistic_x_ent_cpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        দশমিক t = truth[i];
        দশমিক p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

শূন্য l2_cpu(পূর্ণসংখ্যা n, দশমিক *pred, দশমিক *truth, দশমিক *delta, দশমিক *error)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        দশমিক diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

দশমিক dot_cpu(পূর্ণসংখ্যা N, দশমিক *X, পূর্ণসংখ্যা INCX, দশমিক *Y, পূর্ণসংখ্যা INCY)
{
    পূর্ণসংখ্যা i;
    দশমিক dot = 0;
    জন্য(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    ফেরত dot;
}

শূন্য softmax(দশমিক *প্রবেশ, পূর্ণসংখ্যা n, দশমিক অস্থায়ী, পূর্ণসংখ্যা stride, দশমিক *output)
{
    পূর্ণসংখ্যা i;
    দশমিক যোগ = 0;
    দশমিক largest = -FLT_MAX;
    জন্য(i = 0; i < n; ++i){
        যদি(প্রবেশ[i*stride] > largest) largest = প্রবেশ[i*stride];
    }
    জন্য(i = 0; i < n; ++i){
        দশমিক e = exp(প্রবেশ[i*stride]/অস্থায়ী - largest/অস্থায়ী);
        যোগ += e;
        output[i*stride] = e;
    }
    জন্য(i = 0; i < n; ++i){
        output[i*stride] /= যোগ;
    }
}


শূন্য softmax_cpu(দশমিক *প্রবেশ, পূর্ণসংখ্যা n, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা batch_offset, পূর্ণসংখ্যা groups, পূর্ণসংখ্যা group_offset, পূর্ণসংখ্যা stride, দশমিক অস্থায়ী, দশমিক *output)
{
    পূর্ণসংখ্যা g, b;
    জন্য(b = 0; b < batch; ++b){
        জন্য(g = 0; g < groups; ++g){
            softmax(প্রবেশ + b*batch_offset + g*group_offset, n, অস্থায়ী, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

শূন্য upsample_cpu(দশমিক *in, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা forward, দশমিক scale, দশমিক *out)
{
    পূর্ণসংখ্যা i, j, k, b;
    জন্য(b = 0; b < batch; ++b){
        জন্য(k = 0; k < c; ++k){
            জন্য(j = 0; j < h*stride; ++j){
                জন্য(i = 0; i < w*stride; ++i){
                    পূর্ণসংখ্যা in_সূচক = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    পূর্ণসংখ্যা out_সূচক = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    যদি(forward) out[out_সূচক] = scale*in[in_সূচক];
                    অন্যথা in[in_সূচক] += scale*out[out_সূচক];
                }
            }
        }
    }
}


