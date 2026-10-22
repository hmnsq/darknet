#include "blas.h"

#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void reorg_cpu(float *x, ціле w, ціле h, ціле c, ціле batch, ціле stride, ціле forward, float *out)
{
    ціле b,i,j,k;
    ціле out_c = c/(stride*stride);

    для(b = 0; b < batch; ++b){
        для(k = 0; k < c; ++k){
            для(j = 0; j < h; ++j){
                для(i = 0; i < w; ++i){
                    ціле in_index  = i + w*(j + h*(k + c*b));
                    ціле c2 = k % out_c;
                    ціле offset = k / out_c;
                    ціле w2 = i*stride + offset % stride;
                    ціле h2 = j*stride + offset / stride;
                    ціле out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    якщо(forward) out[out_index] = x[in_index];
                    інакше out[in_index] = x[out_index];
                }
            }
        }
    }
}

void flatten(float *x, ціле size, ціле layers, ціле batch, ціле forward)
{
    float *swap = calloc(size*layers*batch, sizeof(float));
    ціле i,c,b;
    для(b = 0; b < batch; ++b){
        для(c = 0; c < layers; ++c){
            для(i = 0; i < size; ++i){
                ціле i1 = b*layers*size + c*size + i;
                ціле i2 = b*layers*size + i*layers + c;
                якщо (forward) swap[i2] = x[i1];
                інакше swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*sizeof(float));
    free(swap);
}

void weighted_sum_cpu(float *a, float *b, float *s, ціле n, float *c)
{
    ціле i;
    для(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, ціле n, float *dc)
{
    ціле i;
    для(i = 0; i < n; ++i){
        якщо(da) da[i] += dc[i] * s[i];
        якщо(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

void shortcut_cpu(ціле batch, ціле w1, ціле h1, ціле c1, float *add, ціле w2, ціле h2, ціле c2, float s1, float s2, float *out)
{
    ціле stride = w1/w2;
    ціле sample = w2/w1;
    assert(stride == h1/h2);
    assert(sample == h2/h1);
    якщо(stride < 1) stride = 1;
    якщо(sample < 1) sample = 1;
    ціле minw = (w1 < w2) ? w1 : w2;
    ціле minh = (h1 < h2) ? h1 : h2;
    ціле minc = (c1 < c2) ? c1 : c2;

    ціле i,j,k,b;
    для(b = 0; b < batch; ++b){
        для(k = 0; k < minc; ++k){
            для(j = 0; j < minh; ++j){
                для(i = 0; i < minw; ++i){
                    ціле out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    ціле add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

void mean_cpu(float *x, ціле batch, ціле filters, ціле spatial, float *mean)
{
    float scale = 1./(batch * spatial);
    ціле i,j,k;
    для(i = 0; i < filters; ++i){
        mean[i] = 0;
        для(j = 0; j < batch; ++j){
            для(k = 0; k < spatial; ++k){
                ціле index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

void variance_cpu(float *x, float *mean, ціле batch, ціле filters, ціле spatial, float *variance)
{
    float scale = 1./(batch * spatial - 1);
    ціле i,j,k;
    для(i = 0; i < filters; ++i){
        variance[i] = 0;
        для(j = 0; j < batch; ++j){
            для(k = 0; k < spatial; ++k){
                ціле index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

void l2normalize_cpu(float *x, float *dx, ціле batch, ціле filters, ціле spatial)
{
    ціле b,f,i;
    для(b = 0; b < batch; ++b){
        для(i = 0; i < spatial; ++i){
            float sum = 0;
            для(f = 0; f < filters; ++f){
                ціле index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            для(f = 0; f < filters; ++f){
                ціле index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


void normalize_cpu(float *x, float *mean, float *variance, ціле batch, ціле filters, ціле spatial)
{
    ціле b, f, i;
    для(b = 0; b < batch; ++b){
        для(f = 0; f < filters; ++f){
            для(i = 0; i < spatial; ++i){
                ціле index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

void const_cpu(ціле N, float ALPHA, float *X, ціле INCX)
{
    ціле i;
    для(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void mul_cpu(ціле N, float *X, ціле INCX, float *Y, ціле INCY)
{
    ціле i;
    для(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

void pow_cpu(ціле N, float ALPHA, float *X, ціле INCX, float *Y, ціле INCY)
{
    ціле i;
    для(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

void axpy_cpu(ціле N, float ALPHA, float *X, ціле INCX, float *Y, ціле INCY)
{
    ціле i;
    для(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

void scal_cpu(ціле N, float ALPHA, float *X, ціле INCX)
{
    ціле i;
    для(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

void fill_cpu(ціле N, float ALPHA, float *X, ціле INCX)
{
    ціле i;
    для(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void deinter_cpu(ціле NX, float *X, ціле NY, float *Y, ціле B, float *OUT)
{
    ціле i, j;
    ціле index = 0;
    для(j = 0; j < B; ++j) {
        для(i = 0; i < NX; ++i){
            якщо(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        для(i = 0; i < NY; ++i){
            якщо(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

void inter_cpu(ціле NX, float *X, ціле NY, float *Y, ціле B, float *OUT)
{
    ціле i, j;
    ціле index = 0;
    для(j = 0; j < B; ++j) {
        для(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        для(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

void copy_cpu(ціле N, float *X, ціле INCX, float *Y, ціле INCY)
{
    ціле i;
    для(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

void mult_add_into_cpu(ціле N, float *X, float *Y, float *Z)
{
    ціле i;
    для(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

void smooth_l1_cpu(ціле n, float *pred, float *truth, float *delta, float *error)
{
    ціле i;
    для(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        float abs_val = fabs(diff);
        якщо(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        інакше {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

void l1_cpu(ціле n, float *pred, float *truth, float *delta, float *error)
{
    ціле i;
    для(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

void softmax_x_ent_cpu(ціле n, float *pred, float *truth, float *delta, float *error)
{
    ціле i;
    для(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

void logistic_x_ent_cpu(ціле n, float *pred, float *truth, float *delta, float *error)
{
    ціле i;
    для(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

void l2_cpu(ціле n, float *pred, float *truth, float *delta, float *error)
{
    ціле i;
    для(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

float dot_cpu(ціле N, float *X, ціле INCX, float *Y, ціле INCY)
{
    ціле i;
    float dot = 0;
    для(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    повернути dot;
}

void softmax(float *input, ціле n, float temp, ціле stride, float *output)
{
    ціле i;
    float sum = 0;
    float largest = -FLT_MAX;
    для(i = 0; i < n; ++i){
        якщо(input[i*stride] > largest) largest = input[i*stride];
    }
    для(i = 0; i < n; ++i){
        float e = exp(input[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    для(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


void softmax_cpu(float *input, ціле n, ціле batch, ціле batch_offset, ціле groups, ціле group_offset, ціле stride, float temp, float *output)
{
    ціле g, b;
    для(b = 0; b < batch; ++b){
        для(g = 0; g < groups; ++g){
            softmax(input + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

void upsample_cpu(float *in, ціле w, ціле h, ціле c, ціле batch, ціле stride, ціле forward, float scale, float *out)
{
    ціле i, j, k, b;
    для(b = 0; b < batch; ++b){
        для(k = 0; k < c; ++k){
            для(j = 0; j < h*stride; ++j){
                для(i = 0; i < w*stride; ++i){
                    ціле in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    ціле out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    якщо(forward) out[out_index] = scale*in[in_index];
                    інакше in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


