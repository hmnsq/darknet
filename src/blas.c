#include "blas.h"

#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void reorg_cpu(float *x, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม c, จำนวนเต็ม batch, จำนวนเต็ม stride, จำนวนเต็ม forward, float *out)
{
    จำนวนเต็ม b,i,j,k;
    จำนวนเต็ม out_c = c/(stride*stride);

    สำหรับ(b = 0; b < batch; ++b){
        สำหรับ(k = 0; k < c; ++k){
            สำหรับ(j = 0; j < h; ++j){
                สำหรับ(i = 0; i < w; ++i){
                    จำนวนเต็ม in_index  = i + w*(j + h*(k + c*b));
                    จำนวนเต็ม c2 = k % out_c;
                    จำนวนเต็ม offset = k / out_c;
                    จำนวนเต็ม w2 = i*stride + offset % stride;
                    จำนวนเต็ม h2 = j*stride + offset / stride;
                    จำนวนเต็ม out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    ถ้า(forward) out[out_index] = x[in_index];
                    มิฉะนั้น out[in_index] = x[out_index];
                }
            }
        }
    }
}

void flatten(float *x, จำนวนเต็ม size, จำนวนเต็ม layers, จำนวนเต็ม batch, จำนวนเต็ม forward)
{
    float *swap = calloc(size*layers*batch, parimaa_na(float));
    จำนวนเต็ม i,c,b;
    สำหรับ(b = 0; b < batch; ++b){
        สำหรับ(c = 0; c < layers; ++c){
            สำหรับ(i = 0; i < size; ++i){
                จำนวนเต็ม i1 = b*layers*size + c*size + i;
                จำนวนเต็ม i2 = b*layers*size + i*layers + c;
                ถ้า (forward) swap[i2] = x[i1];
                มิฉะนั้น swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*parimaa_na(float));
    free(swap);
}

void weighted_sum_cpu(float *a, float *b, float *s, จำนวนเต็ม n, float *c)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, จำนวนเต็ม n, float *dc)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i){
        ถ้า(da) da[i] += dc[i] * s[i];
        ถ้า(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

void shortcut_cpu(จำนวนเต็ม batch, จำนวนเต็ม w1, จำนวนเต็ม h1, จำนวนเต็ม c1, float *add, จำนวนเต็ม w2, จำนวนเต็ม h2, จำนวนเต็ม c2, float s1, float s2, float *out)
{
    จำนวนเต็ม stride = w1/w2;
    จำนวนเต็ม sample = w2/w1;
    assert(stride == h1/h2);
    assert(sample == h2/h1);
    ถ้า(stride < 1) stride = 1;
    ถ้า(sample < 1) sample = 1;
    จำนวนเต็ม minw = (w1 < w2) ? w1 : w2;
    จำนวนเต็ม minh = (h1 < h2) ? h1 : h2;
    จำนวนเต็ม minc = (c1 < c2) ? c1 : c2;

    จำนวนเต็ม i,j,k,b;
    สำหรับ(b = 0; b < batch; ++b){
        สำหรับ(k = 0; k < minc; ++k){
            สำหรับ(j = 0; j < minh; ++j){
                สำหรับ(i = 0; i < minw; ++i){
                    จำนวนเต็ม out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    จำนวนเต็ม add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

void mean_cpu(float *x, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial, float *mean)
{
    float scale = 1./(batch * spatial);
    จำนวนเต็ม i,j,k;
    สำหรับ(i = 0; i < filters; ++i){
        mean[i] = 0;
        สำหรับ(j = 0; j < batch; ++j){
            สำหรับ(k = 0; k < spatial; ++k){
                จำนวนเต็ม index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

void variance_cpu(float *x, float *mean, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial, float *variance)
{
    float scale = 1./(batch * spatial - 1);
    จำนวนเต็ม i,j,k;
    สำหรับ(i = 0; i < filters; ++i){
        variance[i] = 0;
        สำหรับ(j = 0; j < batch; ++j){
            สำหรับ(k = 0; k < spatial; ++k){
                จำนวนเต็ม index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

void l2normalize_cpu(float *x, float *dx, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial)
{
    จำนวนเต็ม b,f,i;
    สำหรับ(b = 0; b < batch; ++b){
        สำหรับ(i = 0; i < spatial; ++i){
            float sum = 0;
            สำหรับ(f = 0; f < filters; ++f){
                จำนวนเต็ม index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            สำหรับ(f = 0; f < filters; ++f){
                จำนวนเต็ม index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


void normalize_cpu(float *x, float *mean, float *variance, จำนวนเต็ม batch, จำนวนเต็ม filters, จำนวนเต็ม spatial)
{
    จำนวนเต็ม b, f, i;
    สำหรับ(b = 0; b < batch; ++b){
        สำหรับ(f = 0; f < filters; ++f){
            สำหรับ(i = 0; i < spatial; ++i){
                จำนวนเต็ม index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

void const_cpu(จำนวนเต็ม N, float ALPHA, float *X, จำนวนเต็ม INCX)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void mul_cpu(จำนวนเต็ม N, float *X, จำนวนเต็ม INCX, float *Y, จำนวนเต็ม INCY)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

void pow_cpu(จำนวนเต็ม N, float ALPHA, float *X, จำนวนเต็ม INCX, float *Y, จำนวนเต็ม INCY)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

void axpy_cpu(จำนวนเต็ม N, float ALPHA, float *X, จำนวนเต็ม INCX, float *Y, จำนวนเต็ม INCY)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

void scal_cpu(จำนวนเต็ม N, float ALPHA, float *X, จำนวนเต็ม INCX)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

void fill_cpu(จำนวนเต็ม N, float ALPHA, float *X, จำนวนเต็ม INCX)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void deinter_cpu(จำนวนเต็ม NX, float *X, จำนวนเต็ม NY, float *Y, จำนวนเต็ม B, float *OUT)
{
    จำนวนเต็ม i, j;
    จำนวนเต็ม index = 0;
    สำหรับ(j = 0; j < B; ++j) {
        สำหรับ(i = 0; i < NX; ++i){
            ถ้า(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        สำหรับ(i = 0; i < NY; ++i){
            ถ้า(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

void inter_cpu(จำนวนเต็ม NX, float *X, จำนวนเต็ม NY, float *Y, จำนวนเต็ม B, float *OUT)
{
    จำนวนเต็ม i, j;
    จำนวนเต็ม index = 0;
    สำหรับ(j = 0; j < B; ++j) {
        สำหรับ(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        สำหรับ(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

void copy_cpu(จำนวนเต็ม N, float *X, จำนวนเต็ม INCX, float *Y, จำนวนเต็ม INCY)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

void mult_add_into_cpu(จำนวนเต็ม N, float *X, float *Y, float *Z)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

void smooth_l1_cpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        float abs_val = fabs(diff);
        ถ้า(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        มิฉะนั้น {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

void l1_cpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

void softmax_x_ent_cpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

void logistic_x_ent_cpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

void l2_cpu(จำนวนเต็ม n, float *pred, float *truth, float *delta, float *error)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

float dot_cpu(จำนวนเต็ม N, float *X, จำนวนเต็ม INCX, float *Y, จำนวนเต็ม INCY)
{
    จำนวนเต็ม i;
    float dot = 0;
    สำหรับ(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    คืน dot;
}

void softmax(float *input, จำนวนเต็ม n, float temp, จำนวนเต็ม stride, float *output)
{
    จำนวนเต็ม i;
    float sum = 0;
    float largest = -FLT_MAX;
    สำหรับ(i = 0; i < n; ++i){
        ถ้า(input[i*stride] > largest) largest = input[i*stride];
    }
    สำหรับ(i = 0; i < n; ++i){
        float e = exp(input[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    สำหรับ(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


void softmax_cpu(float *input, จำนวนเต็ม n, จำนวนเต็ม batch, จำนวนเต็ม batch_offset, จำนวนเต็ม groups, จำนวนเต็ม group_offset, จำนวนเต็ม stride, float temp, float *output)
{
    จำนวนเต็ม g, b;
    สำหรับ(b = 0; b < batch; ++b){
        สำหรับ(g = 0; g < groups; ++g){
            softmax(input + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

void upsample_cpu(float *in, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม c, จำนวนเต็ม batch, จำนวนเต็ม stride, จำนวนเต็ม forward, float scale, float *out)
{
    จำนวนเต็ม i, j, k, b;
    สำหรับ(b = 0; b < batch; ++b){
        สำหรับ(k = 0; k < c; ++k){
            สำหรับ(j = 0; j < h*stride; ++j){
                สำหรับ(i = 0; i < w*stride; ++i){
                    จำนวนเต็ม in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    จำนวนเต็ม out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    ถ้า(forward) out[out_index] = scale*in[in_index];
                    มิฉะนั้น in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


