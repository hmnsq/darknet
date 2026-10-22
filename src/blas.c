#include "blas.h"

#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void reorg_cpu(float *x, số_nguyên w, số_nguyên h, số_nguyên c, số_nguyên batch, số_nguyên stride, số_nguyên forward, float *out)
{
    số_nguyên b,i,j,k;
    số_nguyên out_c = c/(stride*stride);

    cho(b = 0; b < batch; ++b){
        cho(k = 0; k < c; ++k){
            cho(j = 0; j < h; ++j){
                cho(i = 0; i < w; ++i){
                    số_nguyên in_index  = i + w*(j + h*(k + c*b));
                    số_nguyên c2 = k % out_c;
                    số_nguyên offset = k / out_c;
                    số_nguyên w2 = i*stride + offset % stride;
                    số_nguyên h2 = j*stride + offset / stride;
                    số_nguyên out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    nếu(forward) out[out_index] = x[in_index];
                    không_thì out[in_index] = x[out_index];
                }
            }
        }
    }
}

void flatten(float *x, số_nguyên size, số_nguyên layers, số_nguyên batch, số_nguyên forward)
{
    float *swap = calloc(size*layers*batch, sizeof(float));
    số_nguyên i,c,b;
    cho(b = 0; b < batch; ++b){
        cho(c = 0; c < layers; ++c){
            cho(i = 0; i < size; ++i){
                số_nguyên i1 = b*layers*size + c*size + i;
                số_nguyên i2 = b*layers*size + i*layers + c;
                nếu (forward) swap[i2] = x[i1];
                không_thì swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*sizeof(float));
    free(swap);
}

void weighted_sum_cpu(float *a, float *b, float *s, số_nguyên n, float *c)
{
    số_nguyên i;
    cho(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, số_nguyên n, float *dc)
{
    số_nguyên i;
    cho(i = 0; i < n; ++i){
        nếu(da) da[i] += dc[i] * s[i];
        nếu(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

void shortcut_cpu(số_nguyên batch, số_nguyên w1, số_nguyên h1, số_nguyên c1, float *add, số_nguyên w2, số_nguyên h2, số_nguyên c2, float s1, float s2, float *out)
{
    số_nguyên stride = w1/w2;
    số_nguyên sample = w2/w1;
    assert(stride == h1/h2);
    assert(sample == h2/h1);
    nếu(stride < 1) stride = 1;
    nếu(sample < 1) sample = 1;
    số_nguyên minw = (w1 < w2) ? w1 : w2;
    số_nguyên minh = (h1 < h2) ? h1 : h2;
    số_nguyên minc = (c1 < c2) ? c1 : c2;

    số_nguyên i,j,k,b;
    cho(b = 0; b < batch; ++b){
        cho(k = 0; k < minc; ++k){
            cho(j = 0; j < minh; ++j){
                cho(i = 0; i < minw; ++i){
                    số_nguyên out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    số_nguyên add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

void mean_cpu(float *x, số_nguyên batch, số_nguyên filters, số_nguyên spatial, float *mean)
{
    float scale = 1./(batch * spatial);
    số_nguyên i,j,k;
    cho(i = 0; i < filters; ++i){
        mean[i] = 0;
        cho(j = 0; j < batch; ++j){
            cho(k = 0; k < spatial; ++k){
                số_nguyên index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

void variance_cpu(float *x, float *mean, số_nguyên batch, số_nguyên filters, số_nguyên spatial, float *variance)
{
    float scale = 1./(batch * spatial - 1);
    số_nguyên i,j,k;
    cho(i = 0; i < filters; ++i){
        variance[i] = 0;
        cho(j = 0; j < batch; ++j){
            cho(k = 0; k < spatial; ++k){
                số_nguyên index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

void l2normalize_cpu(float *x, float *dx, số_nguyên batch, số_nguyên filters, số_nguyên spatial)
{
    số_nguyên b,f,i;
    cho(b = 0; b < batch; ++b){
        cho(i = 0; i < spatial; ++i){
            float sum = 0;
            cho(f = 0; f < filters; ++f){
                số_nguyên index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            cho(f = 0; f < filters; ++f){
                số_nguyên index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


void normalize_cpu(float *x, float *mean, float *variance, số_nguyên batch, số_nguyên filters, số_nguyên spatial)
{
    số_nguyên b, f, i;
    cho(b = 0; b < batch; ++b){
        cho(f = 0; f < filters; ++f){
            cho(i = 0; i < spatial; ++i){
                số_nguyên index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

void const_cpu(số_nguyên N, float ALPHA, float *X, số_nguyên INCX)
{
    số_nguyên i;
    cho(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void mul_cpu(số_nguyên N, float *X, số_nguyên INCX, float *Y, số_nguyên INCY)
{
    số_nguyên i;
    cho(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

void pow_cpu(số_nguyên N, float ALPHA, float *X, số_nguyên INCX, float *Y, số_nguyên INCY)
{
    số_nguyên i;
    cho(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

void axpy_cpu(số_nguyên N, float ALPHA, float *X, số_nguyên INCX, float *Y, số_nguyên INCY)
{
    số_nguyên i;
    cho(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

void scal_cpu(số_nguyên N, float ALPHA, float *X, số_nguyên INCX)
{
    số_nguyên i;
    cho(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

void fill_cpu(số_nguyên N, float ALPHA, float *X, số_nguyên INCX)
{
    số_nguyên i;
    cho(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void deinter_cpu(số_nguyên NX, float *X, số_nguyên NY, float *Y, số_nguyên B, float *OUT)
{
    số_nguyên i, j;
    số_nguyên index = 0;
    cho(j = 0; j < B; ++j) {
        cho(i = 0; i < NX; ++i){
            nếu(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        cho(i = 0; i < NY; ++i){
            nếu(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

void inter_cpu(số_nguyên NX, float *X, số_nguyên NY, float *Y, số_nguyên B, float *OUT)
{
    số_nguyên i, j;
    số_nguyên index = 0;
    cho(j = 0; j < B; ++j) {
        cho(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        cho(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

void copy_cpu(số_nguyên N, float *X, số_nguyên INCX, float *Y, số_nguyên INCY)
{
    số_nguyên i;
    cho(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

void mult_add_into_cpu(số_nguyên N, float *X, float *Y, float *Z)
{
    số_nguyên i;
    cho(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

void smooth_l1_cpu(số_nguyên n, float *pred, float *truth, float *delta, float *error)
{
    số_nguyên i;
    cho(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        float abs_val = fabs(diff);
        nếu(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        không_thì {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

void l1_cpu(số_nguyên n, float *pred, float *truth, float *delta, float *error)
{
    số_nguyên i;
    cho(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

void softmax_x_ent_cpu(số_nguyên n, float *pred, float *truth, float *delta, float *error)
{
    số_nguyên i;
    cho(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

void logistic_x_ent_cpu(số_nguyên n, float *pred, float *truth, float *delta, float *error)
{
    số_nguyên i;
    cho(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

void l2_cpu(số_nguyên n, float *pred, float *truth, float *delta, float *error)
{
    số_nguyên i;
    cho(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

float dot_cpu(số_nguyên N, float *X, số_nguyên INCX, float *Y, số_nguyên INCY)
{
    số_nguyên i;
    float dot = 0;
    cho(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    trả_về dot;
}

void softmax(float *input, số_nguyên n, float temp, số_nguyên stride, float *output)
{
    số_nguyên i;
    float sum = 0;
    float largest = -FLT_MAX;
    cho(i = 0; i < n; ++i){
        nếu(input[i*stride] > largest) largest = input[i*stride];
    }
    cho(i = 0; i < n; ++i){
        float e = exp(input[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    cho(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


void softmax_cpu(float *input, số_nguyên n, số_nguyên batch, số_nguyên batch_offset, số_nguyên groups, số_nguyên group_offset, số_nguyên stride, float temp, float *output)
{
    số_nguyên g, b;
    cho(b = 0; b < batch; ++b){
        cho(g = 0; g < groups; ++g){
            softmax(input + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

void upsample_cpu(float *in, số_nguyên w, số_nguyên h, số_nguyên c, số_nguyên batch, số_nguyên stride, số_nguyên forward, float scale, float *out)
{
    số_nguyên i, j, k, b;
    cho(b = 0; b < batch; ++b){
        cho(k = 0; k < c; ++k){
            cho(j = 0; j < h*stride; ++j){
                cho(i = 0; i < w*stride; ++i){
                    số_nguyên in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    số_nguyên out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    nếu(forward) out[out_index] = scale*in[in_index];
                    không_thì in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


