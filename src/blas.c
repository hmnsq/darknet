#include "blas.h"

#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void reorg_cpu(float *x, 정수 w, 정수 h, 정수 c, 정수 batch, 정수 stride, 정수 forward, float *out)
{
    정수 b,i,j,k;
    정수 out_c = c/(stride*stride);

    위해(b = 0; b < batch; ++b){
        위해(k = 0; k < c; ++k){
            위해(j = 0; j < h; ++j){
                위해(i = 0; i < w; ++i){
                    정수 in_index  = i + w*(j + h*(k + c*b));
                    정수 c2 = k % out_c;
                    정수 offset = k / out_c;
                    정수 w2 = i*stride + offset % stride;
                    정수 h2 = j*stride + offset / stride;
                    정수 out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    만약(forward) out[out_index] = x[in_index];
                    아니면 out[in_index] = x[out_index];
                }
            }
        }
    }
}

void flatten(float *x, 정수 size, 정수 layers, 정수 batch, 정수 forward)
{
    float *swap = calloc(size*layers*batch, sizeof(float));
    정수 i,c,b;
    위해(b = 0; b < batch; ++b){
        위해(c = 0; c < layers; ++c){
            위해(i = 0; i < size; ++i){
                정수 i1 = b*layers*size + c*size + i;
                정수 i2 = b*layers*size + i*layers + c;
                만약 (forward) swap[i2] = x[i1];
                아니면 swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*sizeof(float));
    free(swap);
}

void weighted_sum_cpu(float *a, float *b, float *s, 정수 n, float *c)
{
    정수 i;
    위해(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, 정수 n, float *dc)
{
    정수 i;
    위해(i = 0; i < n; ++i){
        만약(da) da[i] += dc[i] * s[i];
        만약(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

void shortcut_cpu(정수 batch, 정수 w1, 정수 h1, 정수 c1, float *add, 정수 w2, 정수 h2, 정수 c2, float s1, float s2, float *out)
{
    정수 stride = w1/w2;
    정수 sample = w2/w1;
    assert(stride == h1/h2);
    assert(sample == h2/h1);
    만약(stride < 1) stride = 1;
    만약(sample < 1) sample = 1;
    정수 minw = (w1 < w2) ? w1 : w2;
    정수 minh = (h1 < h2) ? h1 : h2;
    정수 minc = (c1 < c2) ? c1 : c2;

    정수 i,j,k,b;
    위해(b = 0; b < batch; ++b){
        위해(k = 0; k < minc; ++k){
            위해(j = 0; j < minh; ++j){
                위해(i = 0; i < minw; ++i){
                    정수 out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    정수 add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

void mean_cpu(float *x, 정수 batch, 정수 filters, 정수 spatial, float *mean)
{
    float scale = 1./(batch * spatial);
    정수 i,j,k;
    위해(i = 0; i < filters; ++i){
        mean[i] = 0;
        위해(j = 0; j < batch; ++j){
            위해(k = 0; k < spatial; ++k){
                정수 index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

void variance_cpu(float *x, float *mean, 정수 batch, 정수 filters, 정수 spatial, float *variance)
{
    float scale = 1./(batch * spatial - 1);
    정수 i,j,k;
    위해(i = 0; i < filters; ++i){
        variance[i] = 0;
        위해(j = 0; j < batch; ++j){
            위해(k = 0; k < spatial; ++k){
                정수 index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

void l2normalize_cpu(float *x, float *dx, 정수 batch, 정수 filters, 정수 spatial)
{
    정수 b,f,i;
    위해(b = 0; b < batch; ++b){
        위해(i = 0; i < spatial; ++i){
            float sum = 0;
            위해(f = 0; f < filters; ++f){
                정수 index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            위해(f = 0; f < filters; ++f){
                정수 index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


void normalize_cpu(float *x, float *mean, float *variance, 정수 batch, 정수 filters, 정수 spatial)
{
    정수 b, f, i;
    위해(b = 0; b < batch; ++b){
        위해(f = 0; f < filters; ++f){
            위해(i = 0; i < spatial; ++i){
                정수 index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

void const_cpu(정수 N, float ALPHA, float *X, 정수 INCX)
{
    정수 i;
    위해(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void mul_cpu(정수 N, float *X, 정수 INCX, float *Y, 정수 INCY)
{
    정수 i;
    위해(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

void pow_cpu(정수 N, float ALPHA, float *X, 정수 INCX, float *Y, 정수 INCY)
{
    정수 i;
    위해(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

void axpy_cpu(정수 N, float ALPHA, float *X, 정수 INCX, float *Y, 정수 INCY)
{
    정수 i;
    위해(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

void scal_cpu(정수 N, float ALPHA, float *X, 정수 INCX)
{
    정수 i;
    위해(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

void fill_cpu(정수 N, float ALPHA, float *X, 정수 INCX)
{
    정수 i;
    위해(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void deinter_cpu(정수 NX, float *X, 정수 NY, float *Y, 정수 B, float *OUT)
{
    정수 i, j;
    정수 index = 0;
    위해(j = 0; j < B; ++j) {
        위해(i = 0; i < NX; ++i){
            만약(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        위해(i = 0; i < NY; ++i){
            만약(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

void inter_cpu(정수 NX, float *X, 정수 NY, float *Y, 정수 B, float *OUT)
{
    정수 i, j;
    정수 index = 0;
    위해(j = 0; j < B; ++j) {
        위해(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        위해(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

void copy_cpu(정수 N, float *X, 정수 INCX, float *Y, 정수 INCY)
{
    정수 i;
    위해(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

void mult_add_into_cpu(정수 N, float *X, float *Y, float *Z)
{
    정수 i;
    위해(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

void smooth_l1_cpu(정수 n, float *pred, float *truth, float *delta, float *error)
{
    정수 i;
    위해(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        float abs_val = fabs(diff);
        만약(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        아니면 {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

void l1_cpu(정수 n, float *pred, float *truth, float *delta, float *error)
{
    정수 i;
    위해(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

void softmax_x_ent_cpu(정수 n, float *pred, float *truth, float *delta, float *error)
{
    정수 i;
    위해(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

void logistic_x_ent_cpu(정수 n, float *pred, float *truth, float *delta, float *error)
{
    정수 i;
    위해(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

void l2_cpu(정수 n, float *pred, float *truth, float *delta, float *error)
{
    정수 i;
    위해(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

float dot_cpu(정수 N, float *X, 정수 INCX, float *Y, 정수 INCY)
{
    정수 i;
    float dot = 0;
    위해(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    반환 dot;
}

void softmax(float *input, 정수 n, float temp, 정수 stride, float *output)
{
    정수 i;
    float sum = 0;
    float largest = -FLT_MAX;
    위해(i = 0; i < n; ++i){
        만약(input[i*stride] > largest) largest = input[i*stride];
    }
    위해(i = 0; i < n; ++i){
        float e = exp(input[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    위해(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


void softmax_cpu(float *input, 정수 n, 정수 batch, 정수 batch_offset, 정수 groups, 정수 group_offset, 정수 stride, float temp, float *output)
{
    정수 g, b;
    위해(b = 0; b < batch; ++b){
        위해(g = 0; g < groups; ++g){
            softmax(input + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

void upsample_cpu(float *in, 정수 w, 정수 h, 정수 c, 정수 batch, 정수 stride, 정수 forward, float scale, float *out)
{
    정수 i, j, k, b;
    위해(b = 0; b < batch; ++b){
        위해(k = 0; k < c; ++k){
            위해(j = 0; j < h*stride; ++j){
                위해(i = 0; i < w*stride; ++i){
                    정수 in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    정수 out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    만약(forward) out[out_index] = scale*in[in_index];
                    아니면 in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


