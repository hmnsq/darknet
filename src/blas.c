#include "blas.h"

#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void reorg_cpu(float *x, цео w, цео h, цео c, цео batch, цео stride, цео forward, float *out)
{
    цео b,i,j,k;
    цео out_c = c/(stride*stride);

    за(b = 0; b < batch; ++b){
        за(k = 0; k < c; ++k){
            за(j = 0; j < h; ++j){
                за(i = 0; i < w; ++i){
                    цео in_index  = i + w*(j + h*(k + c*b));
                    цео c2 = k % out_c;
                    цео offset = k / out_c;
                    цео w2 = i*stride + offset % stride;
                    цео h2 = j*stride + offset / stride;
                    цео out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    ако(forward) out[out_index] = x[in_index];
                    инако out[in_index] = x[out_index];
                }
            }
        }
    }
}

void flatten(float *x, цео size, цео layers, цео batch, цео forward)
{
    float *swap = calloc(size*layers*batch, sizeof(float));
    цео i,c,b;
    за(b = 0; b < batch; ++b){
        за(c = 0; c < layers; ++c){
            за(i = 0; i < size; ++i){
                цео i1 = b*layers*size + c*size + i;
                цео i2 = b*layers*size + i*layers + c;
                ако (forward) swap[i2] = x[i1];
                инако swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*sizeof(float));
    free(swap);
}

void weighted_sum_cpu(float *a, float *b, float *s, цео n, float *c)
{
    цео i;
    за(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, цео n, float *dc)
{
    цео i;
    за(i = 0; i < n; ++i){
        ако(da) da[i] += dc[i] * s[i];
        ако(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

void shortcut_cpu(цео batch, цео w1, цео h1, цео c1, float *add, цео w2, цео h2, цео c2, float s1, float s2, float *out)
{
    цео stride = w1/w2;
    цео sample = w2/w1;
    assert(stride == h1/h2);
    assert(sample == h2/h1);
    ако(stride < 1) stride = 1;
    ако(sample < 1) sample = 1;
    цео minw = (w1 < w2) ? w1 : w2;
    цео minh = (h1 < h2) ? h1 : h2;
    цео minc = (c1 < c2) ? c1 : c2;

    цео i,j,k,b;
    за(b = 0; b < batch; ++b){
        за(k = 0; k < minc; ++k){
            за(j = 0; j < minh; ++j){
                за(i = 0; i < minw; ++i){
                    цео out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    цео add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

void mean_cpu(float *x, цео batch, цео filters, цео spatial, float *mean)
{
    float scale = 1./(batch * spatial);
    цео i,j,k;
    за(i = 0; i < filters; ++i){
        mean[i] = 0;
        за(j = 0; j < batch; ++j){
            за(k = 0; k < spatial; ++k){
                цео index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

void variance_cpu(float *x, float *mean, цео batch, цео filters, цео spatial, float *variance)
{
    float scale = 1./(batch * spatial - 1);
    цео i,j,k;
    за(i = 0; i < filters; ++i){
        variance[i] = 0;
        за(j = 0; j < batch; ++j){
            за(k = 0; k < spatial; ++k){
                цео index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

void l2normalize_cpu(float *x, float *dx, цео batch, цео filters, цео spatial)
{
    цео b,f,i;
    за(b = 0; b < batch; ++b){
        за(i = 0; i < spatial; ++i){
            float sum = 0;
            за(f = 0; f < filters; ++f){
                цео index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            за(f = 0; f < filters; ++f){
                цео index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


void normalize_cpu(float *x, float *mean, float *variance, цео batch, цео filters, цео spatial)
{
    цео b, f, i;
    за(b = 0; b < batch; ++b){
        за(f = 0; f < filters; ++f){
            за(i = 0; i < spatial; ++i){
                цео index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

void const_cpu(цео N, float ALPHA, float *X, цео INCX)
{
    цео i;
    за(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void mul_cpu(цео N, float *X, цео INCX, float *Y, цео INCY)
{
    цео i;
    за(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

void pow_cpu(цео N, float ALPHA, float *X, цео INCX, float *Y, цео INCY)
{
    цео i;
    за(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

void axpy_cpu(цео N, float ALPHA, float *X, цео INCX, float *Y, цео INCY)
{
    цео i;
    за(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

void scal_cpu(цео N, float ALPHA, float *X, цео INCX)
{
    цео i;
    за(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

void fill_cpu(цео N, float ALPHA, float *X, цео INCX)
{
    цео i;
    за(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void deinter_cpu(цео NX, float *X, цео NY, float *Y, цео B, float *OUT)
{
    цео i, j;
    цео index = 0;
    за(j = 0; j < B; ++j) {
        за(i = 0; i < NX; ++i){
            ако(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        за(i = 0; i < NY; ++i){
            ако(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

void inter_cpu(цео NX, float *X, цео NY, float *Y, цео B, float *OUT)
{
    цео i, j;
    цео index = 0;
    за(j = 0; j < B; ++j) {
        за(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        за(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

void copy_cpu(цео N, float *X, цео INCX, float *Y, цео INCY)
{
    цео i;
    за(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

void mult_add_into_cpu(цео N, float *X, float *Y, float *Z)
{
    цео i;
    за(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

void smooth_l1_cpu(цео n, float *pred, float *truth, float *delta, float *error)
{
    цео i;
    за(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        float abs_val = fabs(diff);
        ако(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        инако {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

void l1_cpu(цео n, float *pred, float *truth, float *delta, float *error)
{
    цео i;
    за(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

void softmax_x_ent_cpu(цео n, float *pred, float *truth, float *delta, float *error)
{
    цео i;
    за(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

void logistic_x_ent_cpu(цео n, float *pred, float *truth, float *delta, float *error)
{
    цео i;
    за(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

void l2_cpu(цео n, float *pred, float *truth, float *delta, float *error)
{
    цео i;
    за(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

float dot_cpu(цео N, float *X, цео INCX, float *Y, цео INCY)
{
    цео i;
    float dot = 0;
    за(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    врати dot;
}

void softmax(float *input, цео n, float temp, цео stride, float *output)
{
    цео i;
    float sum = 0;
    float largest = -FLT_MAX;
    за(i = 0; i < n; ++i){
        ако(input[i*stride] > largest) largest = input[i*stride];
    }
    за(i = 0; i < n; ++i){
        float e = exp(input[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    за(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


void softmax_cpu(float *input, цео n, цео batch, цео batch_offset, цео groups, цео group_offset, цео stride, float temp, float *output)
{
    цео g, b;
    за(b = 0; b < batch; ++b){
        за(g = 0; g < groups; ++g){
            softmax(input + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

void upsample_cpu(float *in, цео w, цео h, цео c, цео batch, цео stride, цео forward, float scale, float *out)
{
    цео i, j, k, b;
    за(b = 0; b < batch; ++b){
        за(k = 0; k < c; ++k){
            за(j = 0; j < h*stride; ++j){
                за(i = 0; i < w*stride; ++i){
                    цео in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    цео out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    ако(forward) out[out_index] = scale*in[in_index];
                    инако in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


