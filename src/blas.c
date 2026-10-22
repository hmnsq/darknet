#include "blas.h"

#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void reorg_cpu(float *x, целое w, целое h, целое c, целое batch, целое stride, целое forward, float *out)
{
    целое b,i,j,k;
    целое out_c = c/(stride*stride);

    для(b = 0; b < batch; ++b){
        для(k = 0; k < c; ++k){
            для(j = 0; j < h; ++j){
                для(i = 0; i < w; ++i){
                    целое in_index  = i + w*(j + h*(k + c*b));
                    целое c2 = k % out_c;
                    целое offset = k / out_c;
                    целое w2 = i*stride + offset % stride;
                    целое h2 = j*stride + offset / stride;
                    целое out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    если(forward) out[out_index] = x[in_index];
                    иначе out[in_index] = x[out_index];
                }
            }
        }
    }
}

void flatten(float *x, целое size, целое layers, целое batch, целое forward)
{
    float *swap = calloc(size*layers*batch, sizeof(float));
    целое i,c,b;
    для(b = 0; b < batch; ++b){
        для(c = 0; c < layers; ++c){
            для(i = 0; i < size; ++i){
                целое i1 = b*layers*size + c*size + i;
                целое i2 = b*layers*size + i*layers + c;
                если (forward) swap[i2] = x[i1];
                иначе swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*sizeof(float));
    free(swap);
}

void weighted_sum_cpu(float *a, float *b, float *s, целое n, float *c)
{
    целое i;
    для(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, целое n, float *dc)
{
    целое i;
    для(i = 0; i < n; ++i){
        если(da) da[i] += dc[i] * s[i];
        если(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

void shortcut_cpu(целое batch, целое w1, целое h1, целое c1, float *add, целое w2, целое h2, целое c2, float s1, float s2, float *out)
{
    целое stride = w1/w2;
    целое sample = w2/w1;
    assert(stride == h1/h2);
    assert(sample == h2/h1);
    если(stride < 1) stride = 1;
    если(sample < 1) sample = 1;
    целое minw = (w1 < w2) ? w1 : w2;
    целое minh = (h1 < h2) ? h1 : h2;
    целое minc = (c1 < c2) ? c1 : c2;

    целое i,j,k,b;
    для(b = 0; b < batch; ++b){
        для(k = 0; k < minc; ++k){
            для(j = 0; j < minh; ++j){
                для(i = 0; i < minw; ++i){
                    целое out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    целое add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

void mean_cpu(float *x, целое batch, целое filters, целое spatial, float *mean)
{
    float scale = 1./(batch * spatial);
    целое i,j,k;
    для(i = 0; i < filters; ++i){
        mean[i] = 0;
        для(j = 0; j < batch; ++j){
            для(k = 0; k < spatial; ++k){
                целое index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

void variance_cpu(float *x, float *mean, целое batch, целое filters, целое spatial, float *variance)
{
    float scale = 1./(batch * spatial - 1);
    целое i,j,k;
    для(i = 0; i < filters; ++i){
        variance[i] = 0;
        для(j = 0; j < batch; ++j){
            для(k = 0; k < spatial; ++k){
                целое index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

void l2normalize_cpu(float *x, float *dx, целое batch, целое filters, целое spatial)
{
    целое b,f,i;
    для(b = 0; b < batch; ++b){
        для(i = 0; i < spatial; ++i){
            float sum = 0;
            для(f = 0; f < filters; ++f){
                целое index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            для(f = 0; f < filters; ++f){
                целое index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


void normalize_cpu(float *x, float *mean, float *variance, целое batch, целое filters, целое spatial)
{
    целое b, f, i;
    для(b = 0; b < batch; ++b){
        для(f = 0; f < filters; ++f){
            для(i = 0; i < spatial; ++i){
                целое index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

void const_cpu(целое N, float ALPHA, float *X, целое INCX)
{
    целое i;
    для(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void mul_cpu(целое N, float *X, целое INCX, float *Y, целое INCY)
{
    целое i;
    для(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

void pow_cpu(целое N, float ALPHA, float *X, целое INCX, float *Y, целое INCY)
{
    целое i;
    для(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

void axpy_cpu(целое N, float ALPHA, float *X, целое INCX, float *Y, целое INCY)
{
    целое i;
    для(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

void scal_cpu(целое N, float ALPHA, float *X, целое INCX)
{
    целое i;
    для(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

void fill_cpu(целое N, float ALPHA, float *X, целое INCX)
{
    целое i;
    для(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void deinter_cpu(целое NX, float *X, целое NY, float *Y, целое B, float *OUT)
{
    целое i, j;
    целое index = 0;
    для(j = 0; j < B; ++j) {
        для(i = 0; i < NX; ++i){
            если(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        для(i = 0; i < NY; ++i){
            если(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

void inter_cpu(целое NX, float *X, целое NY, float *Y, целое B, float *OUT)
{
    целое i, j;
    целое index = 0;
    для(j = 0; j < B; ++j) {
        для(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        для(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

void copy_cpu(целое N, float *X, целое INCX, float *Y, целое INCY)
{
    целое i;
    для(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

void mult_add_into_cpu(целое N, float *X, float *Y, float *Z)
{
    целое i;
    для(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

void smooth_l1_cpu(целое n, float *pred, float *truth, float *delta, float *error)
{
    целое i;
    для(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        float abs_val = fabs(diff);
        если(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        иначе {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

void l1_cpu(целое n, float *pred, float *truth, float *delta, float *error)
{
    целое i;
    для(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

void softmax_x_ent_cpu(целое n, float *pred, float *truth, float *delta, float *error)
{
    целое i;
    для(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

void logistic_x_ent_cpu(целое n, float *pred, float *truth, float *delta, float *error)
{
    целое i;
    для(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

void l2_cpu(целое n, float *pred, float *truth, float *delta, float *error)
{
    целое i;
    для(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

float dot_cpu(целое N, float *X, целое INCX, float *Y, целое INCY)
{
    целое i;
    float dot = 0;
    для(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    вернуть dot;
}

void softmax(float *input, целое n, float temp, целое stride, float *output)
{
    целое i;
    float sum = 0;
    float largest = -FLT_MAX;
    для(i = 0; i < n; ++i){
        если(input[i*stride] > largest) largest = input[i*stride];
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


void softmax_cpu(float *input, целое n, целое batch, целое batch_offset, целое groups, целое group_offset, целое stride, float temp, float *output)
{
    целое g, b;
    для(b = 0; b < batch; ++b){
        для(g = 0; g < groups; ++g){
            softmax(input + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

void upsample_cpu(float *in, целое w, целое h, целое c, целое batch, целое stride, целое forward, float scale, float *out)
{
    целое i, j, k, b;
    для(b = 0; b < batch; ++b){
        для(k = 0; k < c; ++k){
            для(j = 0; j < h*stride; ++j){
                для(i = 0; i < w*stride; ++i){
                    целое in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    целое out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    если(forward) out[out_index] = scale*in[in_index];
                    иначе in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


