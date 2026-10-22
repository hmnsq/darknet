#include "blas.h"

#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void reorg_cpu(float *x, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා stride, පූර්ණ_සංඛ්‍යා forward, float *out)
{
    පූර්ණ_සංඛ්‍යා b,i,j,k;
    පූර්ණ_සංඛ්‍යා out_c = c/(stride*stride);

    සඳහා(b = 0; b < batch; ++b){
        සඳහා(k = 0; k < c; ++k){
            සඳහා(j = 0; j < h; ++j){
                සඳහා(i = 0; i < w; ++i){
                    පූර්ණ_සංඛ්‍යා in_index  = i + w*(j + h*(k + c*b));
                    පූර්ණ_සංඛ්‍යා c2 = k % out_c;
                    පූර්ණ_සංඛ්‍යා offset = k / out_c;
                    පූර්ණ_සංඛ්‍යා w2 = i*stride + offset % stride;
                    පූර්ණ_සංඛ්‍යා h2 = j*stride + offset / stride;
                    පූර්ණ_සංඛ්‍යා out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    නම්(forward) out[out_index] = x[in_index];
                    නැතහොත් out[in_index] = x[out_index];
                }
            }
        }
    }
}

void flatten(float *x, පූර්ණ_සංඛ්‍යා size, පූර්ණ_සංඛ්‍යා layers, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා forward)
{
    float *swap = calloc(size*layers*batch, parimaa_na(float));
    පූර්ණ_සංඛ්‍යා i,c,b;
    සඳහා(b = 0; b < batch; ++b){
        සඳහා(c = 0; c < layers; ++c){
            සඳහා(i = 0; i < size; ++i){
                පූර්ණ_සංඛ්‍යා i1 = b*layers*size + c*size + i;
                පූර්ණ_සංඛ්‍යා i2 = b*layers*size + i*layers + c;
                නම් (forward) swap[i2] = x[i1];
                නැතහොත් swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*parimaa_na(float));
    free(swap);
}

void weighted_sum_cpu(float *a, float *b, float *s, පූර්ණ_සංඛ්‍යා n, float *c)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, පූර්ණ_සංඛ්‍යා n, float *dc)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < n; ++i){
        නම්(da) da[i] += dc[i] * s[i];
        නම්(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

void shortcut_cpu(පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා w1, පූර්ණ_සංඛ්‍යා h1, පූර්ණ_සංඛ්‍යා c1, float *add, පූර්ණ_සංඛ්‍යා w2, පූර්ණ_සංඛ්‍යා h2, පූර්ණ_සංඛ්‍යා c2, float s1, float s2, float *out)
{
    පූර්ණ_සංඛ්‍යා stride = w1/w2;
    පූර්ණ_සංඛ්‍යා sample = w2/w1;
    assert(stride == h1/h2);
    assert(sample == h2/h1);
    නම්(stride < 1) stride = 1;
    නම්(sample < 1) sample = 1;
    පූර්ණ_සංඛ්‍යා minw = (w1 < w2) ? w1 : w2;
    පූර්ණ_සංඛ්‍යා minh = (h1 < h2) ? h1 : h2;
    පූර්ණ_සංඛ්‍යා minc = (c1 < c2) ? c1 : c2;

    පූර්ණ_සංඛ්‍යා i,j,k,b;
    සඳහා(b = 0; b < batch; ++b){
        සඳහා(k = 0; k < minc; ++k){
            සඳහා(j = 0; j < minh; ++j){
                සඳහා(i = 0; i < minw; ++i){
                    පූර්ණ_සංඛ්‍යා out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    පූර්ණ_සංඛ්‍යා add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

void mean_cpu(float *x, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial, float *mean)
{
    float scale = 1./(batch * spatial);
    පූර්ණ_සංඛ්‍යා i,j,k;
    සඳහා(i = 0; i < filters; ++i){
        mean[i] = 0;
        සඳහා(j = 0; j < batch; ++j){
            සඳහා(k = 0; k < spatial; ++k){
                පූර්ණ_සංඛ්‍යා index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

void variance_cpu(float *x, float *mean, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial, float *variance)
{
    float scale = 1./(batch * spatial - 1);
    පූර්ණ_සංඛ්‍යා i,j,k;
    සඳහා(i = 0; i < filters; ++i){
        variance[i] = 0;
        සඳහා(j = 0; j < batch; ++j){
            සඳහා(k = 0; k < spatial; ++k){
                පූර්ණ_සංඛ්‍යා index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

void l2normalize_cpu(float *x, float *dx, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial)
{
    පූර්ණ_සංඛ්‍යා b,f,i;
    සඳහා(b = 0; b < batch; ++b){
        සඳහා(i = 0; i < spatial; ++i){
            float sum = 0;
            සඳහා(f = 0; f < filters; ++f){
                පූර්ණ_සංඛ්‍යා index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            සඳහා(f = 0; f < filters; ++f){
                පූර්ණ_සංඛ්‍යා index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


void normalize_cpu(float *x, float *mean, float *variance, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා filters, පූර්ණ_සංඛ්‍යා spatial)
{
    පූර්ණ_සංඛ්‍යා b, f, i;
    සඳහා(b = 0; b < batch; ++b){
        සඳහා(f = 0; f < filters; ++f){
            සඳහා(i = 0; i < spatial; ++i){
                පූර්ණ_සංඛ්‍යා index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

void const_cpu(පූර්ණ_සංඛ්‍යා N, float ALPHA, float *X, පූර්ණ_සංඛ්‍යා INCX)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void mul_cpu(පූර්ණ_සංඛ්‍යා N, float *X, පූර්ණ_සංඛ්‍යා INCX, float *Y, පූර්ණ_සංඛ්‍යා INCY)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

void pow_cpu(පූර්ණ_සංඛ්‍යා N, float ALPHA, float *X, පූර්ණ_සංඛ්‍යා INCX, float *Y, පූර්ණ_සංඛ්‍යා INCY)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

void axpy_cpu(පූර්ණ_සංඛ්‍යා N, float ALPHA, float *X, පූර්ණ_සංඛ්‍යා INCX, float *Y, පූර්ණ_සංඛ්‍යා INCY)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

void scal_cpu(පූර්ණ_සංඛ්‍යා N, float ALPHA, float *X, පූර්ණ_සංඛ්‍යා INCX)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

void fill_cpu(පූර්ණ_සංඛ්‍යා N, float ALPHA, float *X, පූර්ණ_සංඛ්‍යා INCX)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void deinter_cpu(පූර්ණ_සංඛ්‍යා NX, float *X, පූර්ණ_සංඛ්‍යා NY, float *Y, පූර්ණ_සංඛ්‍යා B, float *OUT)
{
    පූර්ණ_සංඛ්‍යා i, j;
    පූර්ණ_සංඛ්‍යා index = 0;
    සඳහා(j = 0; j < B; ++j) {
        සඳහා(i = 0; i < NX; ++i){
            නම්(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        සඳහා(i = 0; i < NY; ++i){
            නම්(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

void inter_cpu(පූර්ණ_සංඛ්‍යා NX, float *X, පූර්ණ_සංඛ්‍යා NY, float *Y, පූර්ණ_සංඛ්‍යා B, float *OUT)
{
    පූර්ණ_සංඛ්‍යා i, j;
    පූර්ණ_සංඛ්‍යා index = 0;
    සඳහා(j = 0; j < B; ++j) {
        සඳහා(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        සඳහා(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

void copy_cpu(පූර්ණ_සංඛ්‍යා N, float *X, පූර්ණ_සංඛ්‍යා INCX, float *Y, පූර්ණ_සංඛ්‍යා INCY)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

void mult_add_into_cpu(පූර්ණ_සංඛ්‍යා N, float *X, float *Y, float *Z)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

void smooth_l1_cpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        float abs_val = fabs(diff);
        නම්(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        නැතහොත් {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

void l1_cpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

void softmax_x_ent_cpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

void logistic_x_ent_cpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

void l2_cpu(පූර්ණ_සංඛ්‍යා n, float *pred, float *truth, float *delta, float *error)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

float dot_cpu(පූර්ණ_සංඛ්‍යා N, float *X, පූර්ණ_සංඛ්‍යා INCX, float *Y, පූර්ණ_සංඛ්‍යා INCY)
{
    පූර්ණ_සංඛ්‍යා i;
    float dot = 0;
    සඳහා(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    ආපසු dot;
}

void softmax(float *input, පූර්ණ_සංඛ්‍යා n, float temp, පූර්ණ_සංඛ්‍යා stride, float *output)
{
    පූර්ණ_සංඛ්‍යා i;
    float sum = 0;
    float largest = -FLT_MAX;
    සඳහා(i = 0; i < n; ++i){
        නම්(input[i*stride] > largest) largest = input[i*stride];
    }
    සඳහා(i = 0; i < n; ++i){
        float e = exp(input[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    සඳහා(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


void softmax_cpu(float *input, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා batch_offset, පූර්ණ_සංඛ්‍යා groups, පූර්ණ_සංඛ්‍යා group_offset, පූර්ණ_සංඛ්‍යා stride, float temp, float *output)
{
    පූර්ණ_සංඛ්‍යා g, b;
    සඳහා(b = 0; b < batch; ++b){
        සඳහා(g = 0; g < groups; ++g){
            softmax(input + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

void upsample_cpu(float *in, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා stride, පූර්ණ_සංඛ්‍යා forward, float scale, float *out)
{
    පූර්ණ_සංඛ්‍යා i, j, k, b;
    සඳහා(b = 0; b < batch; ++b){
        සඳහා(k = 0; k < c; ++k){
            සඳහා(j = 0; j < h*stride; ++j){
                සඳහා(i = 0; i < w*stride; ++i){
                    පූර්ණ_සංඛ්‍යා in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    පූර්ණ_සංඛ්‍යා out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    නම්(forward) out[out_index] = scale*in[in_index];
                    නැතහොත් in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


