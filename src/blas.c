#include "blas.h"

#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void reorg_cpu(float *x, ཧྲིལ w, ཧྲིལ h, ཧྲིལ c, ཧྲིལ batch, ཧྲིལ stride, ཧྲིལ forward, float *out)
{
    ཧྲིལ b,i,j,k;
    ཧྲིལ out_c = c/(stride*stride);

    ཆེད་དུ(b = 0; b < batch; ++b){
        ཆེད་དུ(k = 0; k < c; ++k){
            ཆེད་དུ(j = 0; j < h; ++j){
                ཆེད་དུ(i = 0; i < w; ++i){
                    ཧྲིལ in_index  = i + w*(j + h*(k + c*b));
                    ཧྲིལ c2 = k % out_c;
                    ཧྲིལ offset = k / out_c;
                    ཧྲིལ w2 = i*stride + offset % stride;
                    ཧྲིལ h2 = j*stride + offset / stride;
                    ཧྲིལ out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    གལ་ཏེ(forward) out[out_index] = x[in_index];
                    དེ་མིན out[in_index] = x[out_index];
                }
            }
        }
    }
}

void flatten(float *x, ཧྲིལ size, ཧྲིལ layers, ཧྲིལ batch, ཧྲིལ forward)
{
    float *swap = calloc(size*layers*batch, parimaa_na(float));
    ཧྲིལ i,c,b;
    ཆེད་དུ(b = 0; b < batch; ++b){
        ཆེད་དུ(c = 0; c < layers; ++c){
            ཆེད་དུ(i = 0; i < size; ++i){
                ཧྲིལ i1 = b*layers*size + c*size + i;
                ཧྲིལ i2 = b*layers*size + i*layers + c;
                གལ་ཏེ (forward) swap[i2] = x[i1];
                དེ་མིན swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*parimaa_na(float));
    free(swap);
}

void weighted_sum_cpu(float *a, float *b, float *s, ཧྲིལ n, float *c)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, ཧྲིལ n, float *dc)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        གལ་ཏེ(da) da[i] += dc[i] * s[i];
        གལ་ཏེ(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

void shortcut_cpu(ཧྲིལ batch, ཧྲིལ w1, ཧྲིལ h1, ཧྲིལ c1, float *add, ཧྲིལ w2, ཧྲིལ h2, ཧྲིལ c2, float s1, float s2, float *out)
{
    ཧྲིལ stride = w1/w2;
    ཧྲིལ sample = w2/w1;
    assert(stride == h1/h2);
    assert(sample == h2/h1);
    གལ་ཏེ(stride < 1) stride = 1;
    གལ་ཏེ(sample < 1) sample = 1;
    ཧྲིལ minw = (w1 < w2) ? w1 : w2;
    ཧྲིལ minh = (h1 < h2) ? h1 : h2;
    ཧྲིལ minc = (c1 < c2) ? c1 : c2;

    ཧྲིལ i,j,k,b;
    ཆེད་དུ(b = 0; b < batch; ++b){
        ཆེད་དུ(k = 0; k < minc; ++k){
            ཆེད་དུ(j = 0; j < minh; ++j){
                ཆེད་དུ(i = 0; i < minw; ++i){
                    ཧྲིལ out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    ཧྲིལ add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

void mean_cpu(float *x, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial, float *mean)
{
    float scale = 1./(batch * spatial);
    ཧྲིལ i,j,k;
    ཆེད་དུ(i = 0; i < filters; ++i){
        mean[i] = 0;
        ཆེད་དུ(j = 0; j < batch; ++j){
            ཆེད་དུ(k = 0; k < spatial; ++k){
                ཧྲིལ index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

void variance_cpu(float *x, float *mean, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial, float *variance)
{
    float scale = 1./(batch * spatial - 1);
    ཧྲིལ i,j,k;
    ཆེད་དུ(i = 0; i < filters; ++i){
        variance[i] = 0;
        ཆེད་དུ(j = 0; j < batch; ++j){
            ཆེད་དུ(k = 0; k < spatial; ++k){
                ཧྲིལ index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

void l2normalize_cpu(float *x, float *dx, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial)
{
    ཧྲིལ b,f,i;
    ཆེད་དུ(b = 0; b < batch; ++b){
        ཆེད་དུ(i = 0; i < spatial; ++i){
            float sum = 0;
            ཆེད་དུ(f = 0; f < filters; ++f){
                ཧྲིལ index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            ཆེད་དུ(f = 0; f < filters; ++f){
                ཧྲིལ index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


void normalize_cpu(float *x, float *mean, float *variance, ཧྲིལ batch, ཧྲིལ filters, ཧྲིལ spatial)
{
    ཧྲིལ b, f, i;
    ཆེད་དུ(b = 0; b < batch; ++b){
        ཆེད་དུ(f = 0; f < filters; ++f){
            ཆེད་དུ(i = 0; i < spatial; ++i){
                ཧྲིལ index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

void const_cpu(ཧྲིལ N, float ALPHA, float *X, ཧྲིལ INCX)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void mul_cpu(ཧྲིལ N, float *X, ཧྲིལ INCX, float *Y, ཧྲིལ INCY)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

void pow_cpu(ཧྲིལ N, float ALPHA, float *X, ཧྲིལ INCX, float *Y, ཧྲིལ INCY)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

void axpy_cpu(ཧྲིལ N, float ALPHA, float *X, ཧྲིལ INCX, float *Y, ཧྲིལ INCY)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

void scal_cpu(ཧྲིལ N, float ALPHA, float *X, ཧྲིལ INCX)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

void fill_cpu(ཧྲིལ N, float ALPHA, float *X, ཧྲིལ INCX)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void deinter_cpu(ཧྲིལ NX, float *X, ཧྲིལ NY, float *Y, ཧྲིལ B, float *OUT)
{
    ཧྲིལ i, j;
    ཧྲིལ index = 0;
    ཆེད་དུ(j = 0; j < B; ++j) {
        ཆེད་དུ(i = 0; i < NX; ++i){
            གལ་ཏེ(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        ཆེད་དུ(i = 0; i < NY; ++i){
            གལ་ཏེ(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

void inter_cpu(ཧྲིལ NX, float *X, ཧྲིལ NY, float *Y, ཧྲིལ B, float *OUT)
{
    ཧྲིལ i, j;
    ཧྲིལ index = 0;
    ཆེད་དུ(j = 0; j < B; ++j) {
        ཆེད་དུ(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        ཆེད་དུ(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

void copy_cpu(ཧྲིལ N, float *X, ཧྲིལ INCX, float *Y, ཧྲིལ INCY)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

void mult_add_into_cpu(ཧྲིལ N, float *X, float *Y, float *Z)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

void smooth_l1_cpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        float abs_val = fabs(diff);
        གལ་ཏེ(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        དེ་མིན {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

void l1_cpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

void softmax_x_ent_cpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

void logistic_x_ent_cpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

void l2_cpu(ཧྲིལ n, float *pred, float *truth, float *delta, float *error)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

float dot_cpu(ཧྲིལ N, float *X, ཧྲིལ INCX, float *Y, ཧྲིལ INCY)
{
    ཧྲིལ i;
    float dot = 0;
    ཆེད་དུ(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    ཕྱིར dot;
}

void softmax(float *input, ཧྲིལ n, float temp, ཧྲིལ stride, float *output)
{
    ཧྲིལ i;
    float sum = 0;
    float largest = -FLT_MAX;
    ཆེད་དུ(i = 0; i < n; ++i){
        གལ་ཏེ(input[i*stride] > largest) largest = input[i*stride];
    }
    ཆེད་དུ(i = 0; i < n; ++i){
        float e = exp(input[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    ཆེད་དུ(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


void softmax_cpu(float *input, ཧྲིལ n, ཧྲིལ batch, ཧྲིལ batch_offset, ཧྲིལ groups, ཧྲིལ group_offset, ཧྲིལ stride, float temp, float *output)
{
    ཧྲིལ g, b;
    ཆེད་དུ(b = 0; b < batch; ++b){
        ཆེད་དུ(g = 0; g < groups; ++g){
            softmax(input + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

void upsample_cpu(float *in, ཧྲིལ w, ཧྲིལ h, ཧྲིལ c, ཧྲིལ batch, ཧྲིལ stride, ཧྲིལ forward, float scale, float *out)
{
    ཧྲིལ i, j, k, b;
    ཆེད་དུ(b = 0; b < batch; ++b){
        ཆེད་དུ(k = 0; k < c; ++k){
            ཆེད་དུ(j = 0; j < h*stride; ++j){
                ཆེད་དུ(i = 0; i < w*stride; ++i){
                    ཧྲིལ in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    ཧྲིལ out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    གལ་ཏེ(forward) out[out_index] = scale*in[in_index];
                    དེ་མིན in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


