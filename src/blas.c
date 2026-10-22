#include "blas.h"

#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void reorg_cpu(float *x, puur_naa_nka w, puur_naa_nka h, puur_naa_nka c, puur_naa_nka batch, puur_naa_nka stride, puur_naa_nka forward, float *out)
{
    puur_naa_nka b,i,j,k;
    puur_naa_nka out_c = c/(stride*stride);

    for(b = 0; b < batch; ++b){
        for(k = 0; k < c; ++k){
            for(j = 0; j < h; ++j){
                for(i = 0; i < w; ++i){
                    puur_naa_nka in_index  = i + w*(j + h*(k + c*b));
                    puur_naa_nka c2 = k % out_c;
                    puur_naa_nka offset = k / out_c;
                    puur_naa_nka w2 = i*stride + offset % stride;
                    puur_naa_nka h2 = j*stride + offset / stride;
                    puur_naa_nka out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    ᤀᤡ᤺(forward) out[out_index] = x[in_index];
                    ᤀᤡ᤺ᤆᤡ out[in_index] = x[out_index];
                }
            }
        }
    }
}

void flatten(float *x, puur_naa_nka size, puur_naa_nka layers, puur_naa_nka batch, puur_naa_nka forward)
{
    float *swap = calloc(size*layers*batch, parimaa_na(float));
    puur_naa_nka i,c,b;
    for(b = 0; b < batch; ++b){
        for(c = 0; c < layers; ++c){
            for(i = 0; i < size; ++i){
                puur_naa_nka i1 = b*layers*size + c*size + i;
                puur_naa_nka i2 = b*layers*size + i*layers + c;
                ᤀᤡ᤺ (forward) swap[i2] = x[i1];
                ᤀᤡ᤺ᤆᤡ swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*parimaa_na(float));
    free(swap);
}

void weighted_sum_cpu(float *a, float *b, float *s, puur_naa_nka n, float *c)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, puur_naa_nka n, float *dc)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        ᤀᤡ᤺(da) da[i] += dc[i] * s[i];
        ᤀᤡ᤺(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

void shortcut_cpu(puur_naa_nka batch, puur_naa_nka w1, puur_naa_nka h1, puur_naa_nka c1, float *add, puur_naa_nka w2, puur_naa_nka h2, puur_naa_nka c2, float s1, float s2, float *out)
{
    puur_naa_nka stride = w1/w2;
    puur_naa_nka sample = w2/w1;
    assert(stride == h1/h2);
    assert(sample == h2/h1);
    ᤀᤡ᤺(stride < 1) stride = 1;
    ᤀᤡ᤺(sample < 1) sample = 1;
    puur_naa_nka minw = (w1 < w2) ? w1 : w2;
    puur_naa_nka minh = (h1 < h2) ? h1 : h2;
    puur_naa_nka minc = (c1 < c2) ? c1 : c2;

    puur_naa_nka i,j,k,b;
    for(b = 0; b < batch; ++b){
        for(k = 0; k < minc; ++k){
            for(j = 0; j < minh; ++j){
                for(i = 0; i < minw; ++i){
                    puur_naa_nka out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    puur_naa_nka add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

void mean_cpu(float *x, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial, float *mean)
{
    float scale = 1./(batch * spatial);
    puur_naa_nka i,j,k;
    for(i = 0; i < filters; ++i){
        mean[i] = 0;
        for(j = 0; j < batch; ++j){
            for(k = 0; k < spatial; ++k){
                puur_naa_nka index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

void variance_cpu(float *x, float *mean, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial, float *variance)
{
    float scale = 1./(batch * spatial - 1);
    puur_naa_nka i,j,k;
    for(i = 0; i < filters; ++i){
        variance[i] = 0;
        for(j = 0; j < batch; ++j){
            for(k = 0; k < spatial; ++k){
                puur_naa_nka index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

void l2normalize_cpu(float *x, float *dx, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial)
{
    puur_naa_nka b,f,i;
    for(b = 0; b < batch; ++b){
        for(i = 0; i < spatial; ++i){
            float sum = 0;
            for(f = 0; f < filters; ++f){
                puur_naa_nka index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            for(f = 0; f < filters; ++f){
                puur_naa_nka index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


void normalize_cpu(float *x, float *mean, float *variance, puur_naa_nka batch, puur_naa_nka filters, puur_naa_nka spatial)
{
    puur_naa_nka b, f, i;
    for(b = 0; b < batch; ++b){
        for(f = 0; f < filters; ++f){
            for(i = 0; i < spatial; ++i){
                puur_naa_nka index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

void const_cpu(puur_naa_nka N, float ALPHA, float *X, puur_naa_nka INCX)
{
    puur_naa_nka i;
    for(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void mul_cpu(puur_naa_nka N, float *X, puur_naa_nka INCX, float *Y, puur_naa_nka INCY)
{
    puur_naa_nka i;
    for(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

void pow_cpu(puur_naa_nka N, float ALPHA, float *X, puur_naa_nka INCX, float *Y, puur_naa_nka INCY)
{
    puur_naa_nka i;
    for(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

void axpy_cpu(puur_naa_nka N, float ALPHA, float *X, puur_naa_nka INCX, float *Y, puur_naa_nka INCY)
{
    puur_naa_nka i;
    for(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

void scal_cpu(puur_naa_nka N, float ALPHA, float *X, puur_naa_nka INCX)
{
    puur_naa_nka i;
    for(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

void fill_cpu(puur_naa_nka N, float ALPHA, float *X, puur_naa_nka INCX)
{
    puur_naa_nka i;
    for(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void deinter_cpu(puur_naa_nka NX, float *X, puur_naa_nka NY, float *Y, puur_naa_nka B, float *OUT)
{
    puur_naa_nka i, j;
    puur_naa_nka index = 0;
    for(j = 0; j < B; ++j) {
        for(i = 0; i < NX; ++i){
            ᤀᤡ᤺(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        for(i = 0; i < NY; ++i){
            ᤀᤡ᤺(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

void inter_cpu(puur_naa_nka NX, float *X, puur_naa_nka NY, float *Y, puur_naa_nka B, float *OUT)
{
    puur_naa_nka i, j;
    puur_naa_nka index = 0;
    for(j = 0; j < B; ++j) {
        for(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        for(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

void copy_cpu(puur_naa_nka N, float *X, puur_naa_nka INCX, float *Y, puur_naa_nka INCY)
{
    puur_naa_nka i;
    for(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

void mult_add_into_cpu(puur_naa_nka N, float *X, float *Y, float *Z)
{
    puur_naa_nka i;
    for(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

void smooth_l1_cpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        float abs_val = fabs(diff);
        ᤀᤡ᤺(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        ᤀᤡ᤺ᤆᤡ {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

void l1_cpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

void softmax_x_ent_cpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

void logistic_x_ent_cpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

void l2_cpu(puur_naa_nka n, float *pred, float *truth, float *delta, float *error)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

float dot_cpu(puur_naa_nka N, float *X, puur_naa_nka INCX, float *Y, puur_naa_nka INCY)
{
    puur_naa_nka i;
    float dot = 0;
    for(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    ᤀᤡᤴ dot;
}

void softmax(float *input, puur_naa_nka n, float temp, puur_naa_nka stride, float *output)
{
    puur_naa_nka i;
    float sum = 0;
    float largest = -FLT_MAX;
    for(i = 0; i < n; ++i){
        ᤀᤡ᤺(input[i*stride] > largest) largest = input[i*stride];
    }
    for(i = 0; i < n; ++i){
        float e = exp(input[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    for(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


void softmax_cpu(float *input, puur_naa_nka n, puur_naa_nka batch, puur_naa_nka batch_offset, puur_naa_nka groups, puur_naa_nka group_offset, puur_naa_nka stride, float temp, float *output)
{
    puur_naa_nka g, b;
    for(b = 0; b < batch; ++b){
        for(g = 0; g < groups; ++g){
            softmax(input + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

void upsample_cpu(float *in, puur_naa_nka w, puur_naa_nka h, puur_naa_nka c, puur_naa_nka batch, puur_naa_nka stride, puur_naa_nka forward, float scale, float *out)
{
    puur_naa_nka i, j, k, b;
    for(b = 0; b < batch; ++b){
        for(k = 0; k < c; ++k){
            for(j = 0; j < h*stride; ++j){
                for(i = 0; i < w*stride; ++i){
                    puur_naa_nka in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    puur_naa_nka out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    ᤀᤡ᤺(forward) out[out_index] = scale*in[in_index];
                    ᤀᤡ᤺ᤆᤡ in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


