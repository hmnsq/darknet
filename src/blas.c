#include "blas.h"

#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void reorg_cpu(float *x, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် c, ကိန်းပြည့် batch, ကိန်းပြည့် stride, ကိန်းပြည့် forward, float *out)
{
    ကိန်းပြည့် b,i,j,k;
    ကိန်းပြည့် out_c = c/(stride*stride);

    အတွက်(b = 0; b < batch; ++b){
        အတွက်(k = 0; k < c; ++k){
            အတွက်(j = 0; j < h; ++j){
                အတွက်(i = 0; i < w; ++i){
                    ကိန်းပြည့် in_index  = i + w*(j + h*(k + c*b));
                    ကိန်းပြည့် c2 = k % out_c;
                    ကိန်းပြည့် offset = k / out_c;
                    ကိန်းပြည့် w2 = i*stride + offset % stride;
                    ကိန်းပြည့် h2 = j*stride + offset / stride;
                    ကိန်းပြည့် out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    အကယ်၍(forward) out[out_index] = x[in_index];
                    မဟုတ်ပါက out[in_index] = x[out_index];
                }
            }
        }
    }
}

void flatten(float *x, ကိန်းပြည့် size, ကိန်းပြည့် layers, ကိန်းပြည့် batch, ကိန်းပြည့် forward)
{
    float *swap = calloc(size*layers*batch, parimaa_na(float));
    ကိန်းပြည့် i,c,b;
    အတွက်(b = 0; b < batch; ++b){
        အတွက်(c = 0; c < layers; ++c){
            အတွက်(i = 0; i < size; ++i){
                ကိန်းပြည့် i1 = b*layers*size + c*size + i;
                ကိန်းပြည့် i2 = b*layers*size + i*layers + c;
                အကယ်၍ (forward) swap[i2] = x[i1];
                မဟုတ်ပါက swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*parimaa_na(float));
    free(swap);
}

void weighted_sum_cpu(float *a, float *b, float *s, ကိန်းပြည့် n, float *c)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, ကိန်းပြည့် n, float *dc)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i){
        အကယ်၍(da) da[i] += dc[i] * s[i];
        အကယ်၍(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

void shortcut_cpu(ကိန်းပြည့် batch, ကိန်းပြည့် w1, ကိန်းပြည့် h1, ကိန်းပြည့် c1, float *add, ကိန်းပြည့် w2, ကိန်းပြည့် h2, ကိန်းပြည့် c2, float s1, float s2, float *out)
{
    ကိန်းပြည့် stride = w1/w2;
    ကိန်းပြည့် sample = w2/w1;
    assert(stride == h1/h2);
    assert(sample == h2/h1);
    အကယ်၍(stride < 1) stride = 1;
    အကယ်၍(sample < 1) sample = 1;
    ကိန်းပြည့် minw = (w1 < w2) ? w1 : w2;
    ကိန်းပြည့် minh = (h1 < h2) ? h1 : h2;
    ကိန်းပြည့် minc = (c1 < c2) ? c1 : c2;

    ကိန်းပြည့် i,j,k,b;
    အတွက်(b = 0; b < batch; ++b){
        အတွက်(k = 0; k < minc; ++k){
            အတွက်(j = 0; j < minh; ++j){
                အတွက်(i = 0; i < minw; ++i){
                    ကိန်းပြည့် out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    ကိန်းပြည့် add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

void mean_cpu(float *x, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial, float *mean)
{
    float scale = 1./(batch * spatial);
    ကိန်းပြည့် i,j,k;
    အတွက်(i = 0; i < filters; ++i){
        mean[i] = 0;
        အတွက်(j = 0; j < batch; ++j){
            အတွက်(k = 0; k < spatial; ++k){
                ကိန်းပြည့် index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

void variance_cpu(float *x, float *mean, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial, float *variance)
{
    float scale = 1./(batch * spatial - 1);
    ကိန်းပြည့် i,j,k;
    အတွက်(i = 0; i < filters; ++i){
        variance[i] = 0;
        အတွက်(j = 0; j < batch; ++j){
            အတွက်(k = 0; k < spatial; ++k){
                ကိန်းပြည့် index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

void l2normalize_cpu(float *x, float *dx, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial)
{
    ကိန်းပြည့် b,f,i;
    အတွက်(b = 0; b < batch; ++b){
        အတွက်(i = 0; i < spatial; ++i){
            float sum = 0;
            အတွက်(f = 0; f < filters; ++f){
                ကိန်းပြည့် index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            အတွက်(f = 0; f < filters; ++f){
                ကိန်းပြည့် index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


void normalize_cpu(float *x, float *mean, float *variance, ကိန်းပြည့် batch, ကိန်းပြည့် filters, ကိန်းပြည့် spatial)
{
    ကိန်းပြည့် b, f, i;
    အတွက်(b = 0; b < batch; ++b){
        အတွက်(f = 0; f < filters; ++f){
            အတွက်(i = 0; i < spatial; ++i){
                ကိန်းပြည့် index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

void const_cpu(ကိန်းပြည့် N, float ALPHA, float *X, ကိန်းပြည့် INCX)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void mul_cpu(ကိန်းပြည့် N, float *X, ကိန်းပြည့် INCX, float *Y, ကိန်းပြည့် INCY)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

void pow_cpu(ကိန်းပြည့် N, float ALPHA, float *X, ကိန်းပြည့် INCX, float *Y, ကိန်းပြည့် INCY)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

void axpy_cpu(ကိန်းပြည့် N, float ALPHA, float *X, ကိန်းပြည့် INCX, float *Y, ကိန်းပြည့် INCY)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

void scal_cpu(ကိန်းပြည့် N, float ALPHA, float *X, ကိန်းပြည့် INCX)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

void fill_cpu(ကိန်းပြည့် N, float ALPHA, float *X, ကိန်းပြည့် INCX)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void deinter_cpu(ကိန်းပြည့် NX, float *X, ကိန်းပြည့် NY, float *Y, ကိန်းပြည့် B, float *OUT)
{
    ကိန်းပြည့် i, j;
    ကိန်းပြည့် index = 0;
    အတွက်(j = 0; j < B; ++j) {
        အတွက်(i = 0; i < NX; ++i){
            အကယ်၍(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        အတွက်(i = 0; i < NY; ++i){
            အကယ်၍(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

void inter_cpu(ကိန်းပြည့် NX, float *X, ကိန်းပြည့် NY, float *Y, ကိန်းပြည့် B, float *OUT)
{
    ကိန်းပြည့် i, j;
    ကိန်းပြည့် index = 0;
    အတွက်(j = 0; j < B; ++j) {
        အတွက်(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        အတွက်(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

void copy_cpu(ကိန်းပြည့် N, float *X, ကိန်းပြည့် INCX, float *Y, ကိန်းပြည့် INCY)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

void mult_add_into_cpu(ကိန်းပြည့် N, float *X, float *Y, float *Z)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

void smooth_l1_cpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        float abs_val = fabs(diff);
        အကယ်၍(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        မဟုတ်ပါက {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

void l1_cpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

void softmax_x_ent_cpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

void logistic_x_ent_cpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

void l2_cpu(ကိန်းပြည့် n, float *pred, float *truth, float *delta, float *error)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

float dot_cpu(ကိန်းပြည့် N, float *X, ကိန်းပြည့် INCX, float *Y, ကိန်းပြည့် INCY)
{
    ကိန်းပြည့် i;
    float dot = 0;
    အတွက်(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    ပြန်ပေး dot;
}

void softmax(float *input, ကိန်းပြည့် n, float temp, ကိန်းပြည့် stride, float *output)
{
    ကိန်းပြည့် i;
    float sum = 0;
    float largest = -FLT_MAX;
    အတွက်(i = 0; i < n; ++i){
        အကယ်၍(input[i*stride] > largest) largest = input[i*stride];
    }
    အတွက်(i = 0; i < n; ++i){
        float e = exp(input[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    အတွက်(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


void softmax_cpu(float *input, ကိန်းပြည့် n, ကိန်းပြည့် batch, ကိန်းပြည့် batch_offset, ကိန်းပြည့် groups, ကိန်းပြည့် group_offset, ကိန်းပြည့် stride, float temp, float *output)
{
    ကိန်းပြည့် g, b;
    အတွက်(b = 0; b < batch; ++b){
        အတွက်(g = 0; g < groups; ++g){
            softmax(input + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

void upsample_cpu(float *in, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် c, ကိန်းပြည့် batch, ကိန်းပြည့် stride, ကိန်းပြည့် forward, float scale, float *out)
{
    ကိန်းပြည့် i, j, k, b;
    အတွက်(b = 0; b < batch; ++b){
        အတွက်(k = 0; k < c; ++k){
            အတွက်(j = 0; j < h*stride; ++j){
                အတွက်(i = 0; i < w*stride; ++i){
                    ကိန်းပြည့် in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    ကိန်းပြည့် out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    အကယ်၍(forward) out[out_index] = scale*in[in_index];
                    မဟုတ်ပါက in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


