#include "blas.h"

#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void reorg_cpu(float *x, 整数 w, 整数 h, 整数 c, 整数 batch, 整数 stride, 整数 forward, float *out)
{
    整数 b,i,j,k;
    整数 out_c = c/(stride*stride);

    为(b = 0; b < batch; ++b){
        为(k = 0; k < c; ++k){
            为(j = 0; j < h; ++j){
                为(i = 0; i < w; ++i){
                    整数 in_index  = i + w*(j + h*(k + c*b));
                    整数 c2 = k % out_c;
                    整数 offset = k / out_c;
                    整数 w2 = i*stride + offset % stride;
                    整数 h2 = j*stride + offset / stride;
                    整数 out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    如果(forward) out[out_index] = x[in_index];
                    否则 out[in_index] = x[out_index];
                }
            }
        }
    }
}

void flatten(float *x, 整数 size, 整数 layers, 整数 batch, 整数 forward)
{
    float *swap = calloc(size*layers*batch, sizeof(float));
    整数 i,c,b;
    为(b = 0; b < batch; ++b){
        为(c = 0; c < layers; ++c){
            为(i = 0; i < size; ++i){
                整数 i1 = b*layers*size + c*size + i;
                整数 i2 = b*layers*size + i*layers + c;
                如果 (forward) swap[i2] = x[i1];
                否则 swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*sizeof(float));
    free(swap);
}

void weighted_sum_cpu(float *a, float *b, float *s, 整数 n, float *c)
{
    整数 i;
    为(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, 整数 n, float *dc)
{
    整数 i;
    为(i = 0; i < n; ++i){
        如果(da) da[i] += dc[i] * s[i];
        如果(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

void shortcut_cpu(整数 batch, 整数 w1, 整数 h1, 整数 c1, float *add, 整数 w2, 整数 h2, 整数 c2, float s1, float s2, float *out)
{
    整数 stride = w1/w2;
    整数 sample = w2/w1;
    assert(stride == h1/h2);
    assert(sample == h2/h1);
    如果(stride < 1) stride = 1;
    如果(sample < 1) sample = 1;
    整数 minw = (w1 < w2) ? w1 : w2;
    整数 minh = (h1 < h2) ? h1 : h2;
    整数 minc = (c1 < c2) ? c1 : c2;

    整数 i,j,k,b;
    为(b = 0; b < batch; ++b){
        为(k = 0; k < minc; ++k){
            为(j = 0; j < minh; ++j){
                为(i = 0; i < minw; ++i){
                    整数 out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    整数 add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

void mean_cpu(float *x, 整数 batch, 整数 filters, 整数 spatial, float *mean)
{
    float scale = 1./(batch * spatial);
    整数 i,j,k;
    为(i = 0; i < filters; ++i){
        mean[i] = 0;
        为(j = 0; j < batch; ++j){
            为(k = 0; k < spatial; ++k){
                整数 index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

void variance_cpu(float *x, float *mean, 整数 batch, 整数 filters, 整数 spatial, float *variance)
{
    float scale = 1./(batch * spatial - 1);
    整数 i,j,k;
    为(i = 0; i < filters; ++i){
        variance[i] = 0;
        为(j = 0; j < batch; ++j){
            为(k = 0; k < spatial; ++k){
                整数 index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

void l2normalize_cpu(float *x, float *dx, 整数 batch, 整数 filters, 整数 spatial)
{
    整数 b,f,i;
    为(b = 0; b < batch; ++b){
        为(i = 0; i < spatial; ++i){
            float sum = 0;
            为(f = 0; f < filters; ++f){
                整数 index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            为(f = 0; f < filters; ++f){
                整数 index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


void normalize_cpu(float *x, float *mean, float *variance, 整数 batch, 整数 filters, 整数 spatial)
{
    整数 b, f, i;
    为(b = 0; b < batch; ++b){
        为(f = 0; f < filters; ++f){
            为(i = 0; i < spatial; ++i){
                整数 index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

void const_cpu(整数 N, float ALPHA, float *X, 整数 INCX)
{
    整数 i;
    为(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void mul_cpu(整数 N, float *X, 整数 INCX, float *Y, 整数 INCY)
{
    整数 i;
    为(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

void pow_cpu(整数 N, float ALPHA, float *X, 整数 INCX, float *Y, 整数 INCY)
{
    整数 i;
    为(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

void axpy_cpu(整数 N, float ALPHA, float *X, 整数 INCX, float *Y, 整数 INCY)
{
    整数 i;
    为(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

void scal_cpu(整数 N, float ALPHA, float *X, 整数 INCX)
{
    整数 i;
    为(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

void fill_cpu(整数 N, float ALPHA, float *X, 整数 INCX)
{
    整数 i;
    为(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void deinter_cpu(整数 NX, float *X, 整数 NY, float *Y, 整数 B, float *OUT)
{
    整数 i, j;
    整数 index = 0;
    为(j = 0; j < B; ++j) {
        为(i = 0; i < NX; ++i){
            如果(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        为(i = 0; i < NY; ++i){
            如果(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

void inter_cpu(整数 NX, float *X, 整数 NY, float *Y, 整数 B, float *OUT)
{
    整数 i, j;
    整数 index = 0;
    为(j = 0; j < B; ++j) {
        为(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        为(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

void copy_cpu(整数 N, float *X, 整数 INCX, float *Y, 整数 INCY)
{
    整数 i;
    为(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

void mult_add_into_cpu(整数 N, float *X, float *Y, float *Z)
{
    整数 i;
    为(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

void smooth_l1_cpu(整数 n, float *pred, float *truth, float *delta, float *error)
{
    整数 i;
    为(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        float abs_val = fabs(diff);
        如果(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        否则 {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

void l1_cpu(整数 n, float *pred, float *truth, float *delta, float *error)
{
    整数 i;
    为(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

void softmax_x_ent_cpu(整数 n, float *pred, float *truth, float *delta, float *error)
{
    整数 i;
    为(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

void logistic_x_ent_cpu(整数 n, float *pred, float *truth, float *delta, float *error)
{
    整数 i;
    为(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

void l2_cpu(整数 n, float *pred, float *truth, float *delta, float *error)
{
    整数 i;
    为(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

float dot_cpu(整数 N, float *X, 整数 INCX, float *Y, 整数 INCY)
{
    整数 i;
    float dot = 0;
    为(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    返回 dot;
}

void softmax(float *input, 整数 n, float temp, 整数 stride, float *output)
{
    整数 i;
    float sum = 0;
    float largest = -FLT_MAX;
    为(i = 0; i < n; ++i){
        如果(input[i*stride] > largest) largest = input[i*stride];
    }
    为(i = 0; i < n; ++i){
        float e = exp(input[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    为(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


void softmax_cpu(float *input, 整数 n, 整数 batch, 整数 batch_offset, 整数 groups, 整数 group_offset, 整数 stride, float temp, float *output)
{
    整数 g, b;
    为(b = 0; b < batch; ++b){
        为(g = 0; g < groups; ++g){
            softmax(input + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

void upsample_cpu(float *in, 整数 w, 整数 h, 整数 c, 整数 batch, 整数 stride, 整数 forward, float scale, float *out)
{
    整数 i, j, k, b;
    为(b = 0; b < batch; ++b){
        为(k = 0; k < c; ++k){
            为(j = 0; j < h*stride; ++j){
                为(i = 0; i < w*stride; ++i){
                    整数 in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    整数 out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    如果(forward) out[out_index] = scale*in[in_index];
                    否则 in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


