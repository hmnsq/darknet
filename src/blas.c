#include "blas.h"

#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void reorg_cpu(float *x, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ c, ចំនួនគត់ batch, ចំនួនគត់ stride, ចំនួនគត់ forward, float *out)
{
    ចំនួនគត់ b,i,j,k;
    ចំនួនគត់ out_c = c/(stride*stride);

    សម្រាប់(b = 0; b < batch; ++b){
        សម្រាប់(k = 0; k < c; ++k){
            សម្រាប់(j = 0; j < h; ++j){
                សម្រាប់(i = 0; i < w; ++i){
                    ចំនួនគត់ in_index  = i + w*(j + h*(k + c*b));
                    ចំនួនគត់ c2 = k % out_c;
                    ចំនួនគត់ offset = k / out_c;
                    ចំនួនគត់ w2 = i*stride + offset % stride;
                    ចំនួនគត់ h2 = j*stride + offset / stride;
                    ចំនួនគត់ out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    ប្រសិនបើ(forward) out[out_index] = x[in_index];
                    បើមិនដូច្នេះ out[in_index] = x[out_index];
                }
            }
        }
    }
}

void flatten(float *x, ចំនួនគត់ size, ចំនួនគត់ layers, ចំនួនគត់ batch, ចំនួនគត់ forward)
{
    float *swap = calloc(size*layers*batch, parimaa_na(float));
    ចំនួនគត់ i,c,b;
    សម្រាប់(b = 0; b < batch; ++b){
        សម្រាប់(c = 0; c < layers; ++c){
            សម្រាប់(i = 0; i < size; ++i){
                ចំនួនគត់ i1 = b*layers*size + c*size + i;
                ចំនួនគត់ i2 = b*layers*size + i*layers + c;
                ប្រសិនបើ (forward) swap[i2] = x[i1];
                បើមិនដូច្នេះ swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*parimaa_na(float));
    free(swap);
}

void weighted_sum_cpu(float *a, float *b, float *s, ចំនួនគត់ n, float *c)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, ចំនួនគត់ n, float *dc)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i){
        ប្រសិនបើ(da) da[i] += dc[i] * s[i];
        ប្រសិនបើ(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

void shortcut_cpu(ចំនួនគត់ batch, ចំនួនគត់ w1, ចំនួនគត់ h1, ចំនួនគត់ c1, float *add, ចំនួនគត់ w2, ចំនួនគត់ h2, ចំនួនគត់ c2, float s1, float s2, float *out)
{
    ចំនួនគត់ stride = w1/w2;
    ចំនួនគត់ sample = w2/w1;
    assert(stride == h1/h2);
    assert(sample == h2/h1);
    ប្រសិនបើ(stride < 1) stride = 1;
    ប្រសិនបើ(sample < 1) sample = 1;
    ចំនួនគត់ minw = (w1 < w2) ? w1 : w2;
    ចំនួនគត់ minh = (h1 < h2) ? h1 : h2;
    ចំនួនគត់ minc = (c1 < c2) ? c1 : c2;

    ចំនួនគត់ i,j,k,b;
    សម្រាប់(b = 0; b < batch; ++b){
        សម្រាប់(k = 0; k < minc; ++k){
            សម្រាប់(j = 0; j < minh; ++j){
                សម្រាប់(i = 0; i < minw; ++i){
                    ចំនួនគត់ out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    ចំនួនគត់ add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

void mean_cpu(float *x, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial, float *mean)
{
    float scale = 1./(batch * spatial);
    ចំនួនគត់ i,j,k;
    សម្រាប់(i = 0; i < filters; ++i){
        mean[i] = 0;
        សម្រាប់(j = 0; j < batch; ++j){
            សម្រាប់(k = 0; k < spatial; ++k){
                ចំនួនគត់ index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

void variance_cpu(float *x, float *mean, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial, float *variance)
{
    float scale = 1./(batch * spatial - 1);
    ចំនួនគត់ i,j,k;
    សម្រាប់(i = 0; i < filters; ++i){
        variance[i] = 0;
        សម្រាប់(j = 0; j < batch; ++j){
            សម្រាប់(k = 0; k < spatial; ++k){
                ចំនួនគត់ index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

void l2normalize_cpu(float *x, float *dx, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial)
{
    ចំនួនគត់ b,f,i;
    សម្រាប់(b = 0; b < batch; ++b){
        សម្រាប់(i = 0; i < spatial; ++i){
            float sum = 0;
            សម្រាប់(f = 0; f < filters; ++f){
                ចំនួនគត់ index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            សម្រាប់(f = 0; f < filters; ++f){
                ចំនួនគត់ index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


void normalize_cpu(float *x, float *mean, float *variance, ចំនួនគត់ batch, ចំនួនគត់ filters, ចំនួនគត់ spatial)
{
    ចំនួនគត់ b, f, i;
    សម្រាប់(b = 0; b < batch; ++b){
        សម្រាប់(f = 0; f < filters; ++f){
            សម្រាប់(i = 0; i < spatial; ++i){
                ចំនួនគត់ index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

void const_cpu(ចំនួនគត់ N, float ALPHA, float *X, ចំនួនគត់ INCX)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void mul_cpu(ចំនួនគត់ N, float *X, ចំនួនគត់ INCX, float *Y, ចំនួនគត់ INCY)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

void pow_cpu(ចំនួនគត់ N, float ALPHA, float *X, ចំនួនគត់ INCX, float *Y, ចំនួនគត់ INCY)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

void axpy_cpu(ចំនួនគត់ N, float ALPHA, float *X, ចំនួនគត់ INCX, float *Y, ចំនួនគត់ INCY)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

void scal_cpu(ចំនួនគត់ N, float ALPHA, float *X, ចំនួនគត់ INCX)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

void fill_cpu(ចំនួនគត់ N, float ALPHA, float *X, ចំនួនគត់ INCX)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void deinter_cpu(ចំនួនគត់ NX, float *X, ចំនួនគត់ NY, float *Y, ចំនួនគត់ B, float *OUT)
{
    ចំនួនគត់ i, j;
    ចំនួនគត់ index = 0;
    សម្រាប់(j = 0; j < B; ++j) {
        សម្រាប់(i = 0; i < NX; ++i){
            ប្រសិនបើ(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        សម្រាប់(i = 0; i < NY; ++i){
            ប្រសិនបើ(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

void inter_cpu(ចំនួនគត់ NX, float *X, ចំនួនគត់ NY, float *Y, ចំនួនគត់ B, float *OUT)
{
    ចំនួនគត់ i, j;
    ចំនួនគត់ index = 0;
    សម្រាប់(j = 0; j < B; ++j) {
        សម្រាប់(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        សម្រាប់(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

void copy_cpu(ចំនួនគត់ N, float *X, ចំនួនគត់ INCX, float *Y, ចំនួនគត់ INCY)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

void mult_add_into_cpu(ចំនួនគត់ N, float *X, float *Y, float *Z)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

void smooth_l1_cpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        float abs_val = fabs(diff);
        ប្រសិនបើ(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        បើមិនដូច្នេះ {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

void l1_cpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

void softmax_x_ent_cpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

void logistic_x_ent_cpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

void l2_cpu(ចំនួនគត់ n, float *pred, float *truth, float *delta, float *error)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

float dot_cpu(ចំនួនគត់ N, float *X, ចំនួនគត់ INCX, float *Y, ចំនួនគត់ INCY)
{
    ចំនួនគត់ i;
    float dot = 0;
    សម្រាប់(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    ត្រឡប់ dot;
}

void softmax(float *input, ចំនួនគត់ n, float temp, ចំនួនគត់ stride, float *output)
{
    ចំនួនគត់ i;
    float sum = 0;
    float largest = -FLT_MAX;
    សម្រាប់(i = 0; i < n; ++i){
        ប្រសិនបើ(input[i*stride] > largest) largest = input[i*stride];
    }
    សម្រាប់(i = 0; i < n; ++i){
        float e = exp(input[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    សម្រាប់(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


void softmax_cpu(float *input, ចំនួនគត់ n, ចំនួនគត់ batch, ចំនួនគត់ batch_offset, ចំនួនគត់ groups, ចំនួនគត់ group_offset, ចំនួនគត់ stride, float temp, float *output)
{
    ចំនួនគត់ g, b;
    សម្រាប់(b = 0; b < batch; ++b){
        សម្រាប់(g = 0; g < groups; ++g){
            softmax(input + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

void upsample_cpu(float *in, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ c, ចំនួនគត់ batch, ចំនួនគត់ stride, ចំនួនគត់ forward, float scale, float *out)
{
    ចំនួនគត់ i, j, k, b;
    សម្រាប់(b = 0; b < batch; ++b){
        សម្រាប់(k = 0; k < c; ++k){
            សម្រាប់(j = 0; j < h*stride; ++j){
                សម្រាប់(i = 0; i < w*stride; ++i){
                    ចំនួនគត់ in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    ចំនួនគត់ out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    ប្រសិនបើ(forward) out[out_index] = scale*in[in_index];
                    បើមិនដូច្នេះ in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


