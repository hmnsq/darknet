#include "blas.h"

#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void reorg_cpu(float *x, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ stride, ຈຳນວນເຕັມ forward, float *out)
{
    ຈຳນວນເຕັມ b,i,j,k;
    ຈຳນວນເຕັມ out_c = c/(stride*stride);

    ສຳລັບ(b = 0; b < batch; ++b){
        ສຳລັບ(k = 0; k < c; ++k){
            ສຳລັບ(j = 0; j < h; ++j){
                ສຳລັບ(i = 0; i < w; ++i){
                    ຈຳນວນເຕັມ in_index  = i + w*(j + h*(k + c*b));
                    ຈຳນວນເຕັມ c2 = k % out_c;
                    ຈຳນວນເຕັມ offset = k / out_c;
                    ຈຳນວນເຕັມ w2 = i*stride + offset % stride;
                    ຈຳນວນເຕັມ h2 = j*stride + offset / stride;
                    ຈຳນວນເຕັມ out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    ຖ້າ(forward) out[out_index] = x[in_index];
                    ບໍ່ດັ່ງນັ້ນ out[in_index] = x[out_index];
                }
            }
        }
    }
}

void flatten(float *x, ຈຳນວນເຕັມ size, ຈຳນວນເຕັມ layers, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ forward)
{
    float *swap = calloc(size*layers*batch, parimaa_na(float));
    ຈຳນວນເຕັມ i,c,b;
    ສຳລັບ(b = 0; b < batch; ++b){
        ສຳລັບ(c = 0; c < layers; ++c){
            ສຳລັບ(i = 0; i < size; ++i){
                ຈຳນວນເຕັມ i1 = b*layers*size + c*size + i;
                ຈຳນວນເຕັມ i2 = b*layers*size + i*layers + c;
                ຖ້າ (forward) swap[i2] = x[i1];
                ບໍ່ດັ່ງນັ້ນ swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*parimaa_na(float));
    free(swap);
}

void weighted_sum_cpu(float *a, float *b, float *s, ຈຳນວນເຕັມ n, float *c)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

void weighted_delta_cpu(float *a, float *b, float *s, float *da, float *db, float *ds, ຈຳນວນເຕັມ n, float *dc)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < n; ++i){
        ຖ້າ(da) da[i] += dc[i] * s[i];
        ຖ້າ(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

void shortcut_cpu(ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ w1, ຈຳນວນເຕັມ h1, ຈຳນວນເຕັມ c1, float *add, ຈຳນວນເຕັມ w2, ຈຳນວນເຕັມ h2, ຈຳນວນເຕັມ c2, float s1, float s2, float *out)
{
    ຈຳນວນເຕັມ stride = w1/w2;
    ຈຳນວນເຕັມ sample = w2/w1;
    assert(stride == h1/h2);
    assert(sample == h2/h1);
    ຖ້າ(stride < 1) stride = 1;
    ຖ້າ(sample < 1) sample = 1;
    ຈຳນວນເຕັມ minw = (w1 < w2) ? w1 : w2;
    ຈຳນວນເຕັມ minh = (h1 < h2) ? h1 : h2;
    ຈຳນວນເຕັມ minc = (c1 < c2) ? c1 : c2;

    ຈຳນວນເຕັມ i,j,k,b;
    ສຳລັບ(b = 0; b < batch; ++b){
        ສຳລັບ(k = 0; k < minc; ++k){
            ສຳລັບ(j = 0; j < minh; ++j){
                ສຳລັບ(i = 0; i < minw; ++i){
                    ຈຳນວນເຕັມ out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    ຈຳນວນເຕັມ add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

void mean_cpu(float *x, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial, float *mean)
{
    float scale = 1./(batch * spatial);
    ຈຳນວນເຕັມ i,j,k;
    ສຳລັບ(i = 0; i < filters; ++i){
        mean[i] = 0;
        ສຳລັບ(j = 0; j < batch; ++j){
            ສຳລັບ(k = 0; k < spatial; ++k){
                ຈຳນວນເຕັມ index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

void variance_cpu(float *x, float *mean, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial, float *variance)
{
    float scale = 1./(batch * spatial - 1);
    ຈຳນວນເຕັມ i,j,k;
    ສຳລັບ(i = 0; i < filters; ++i){
        variance[i] = 0;
        ສຳລັບ(j = 0; j < batch; ++j){
            ສຳລັບ(k = 0; k < spatial; ++k){
                ຈຳນວນເຕັມ index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

void l2normalize_cpu(float *x, float *dx, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial)
{
    ຈຳນວນເຕັມ b,f,i;
    ສຳລັບ(b = 0; b < batch; ++b){
        ສຳລັບ(i = 0; i < spatial; ++i){
            float sum = 0;
            ສຳລັບ(f = 0; f < filters; ++f){
                ຈຳນວນເຕັມ index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            ສຳລັບ(f = 0; f < filters; ++f){
                ຈຳນວນເຕັມ index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


void normalize_cpu(float *x, float *mean, float *variance, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ filters, ຈຳນວນເຕັມ spatial)
{
    ຈຳນວນເຕັມ b, f, i;
    ສຳລັບ(b = 0; b < batch; ++b){
        ສຳລັບ(f = 0; f < filters; ++f){
            ສຳລັບ(i = 0; i < spatial; ++i){
                ຈຳນວນເຕັມ index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

void const_cpu(ຈຳນວນເຕັມ N, float ALPHA, float *X, ຈຳນວນເຕັມ INCX)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void mul_cpu(ຈຳນວນເຕັມ N, float *X, ຈຳນວນເຕັມ INCX, float *Y, ຈຳນວນເຕັມ INCY)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

void pow_cpu(ຈຳນວນເຕັມ N, float ALPHA, float *X, ຈຳນວນເຕັມ INCX, float *Y, ຈຳນວນເຕັມ INCY)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

void axpy_cpu(ຈຳນວນເຕັມ N, float ALPHA, float *X, ຈຳນວນເຕັມ INCX, float *Y, ຈຳນວນເຕັມ INCY)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

void scal_cpu(ຈຳນວນເຕັມ N, float ALPHA, float *X, ຈຳນວນເຕັມ INCX)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

void fill_cpu(ຈຳນວນເຕັມ N, float ALPHA, float *X, ຈຳນວນເຕັມ INCX)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

void deinter_cpu(ຈຳນວນເຕັມ NX, float *X, ຈຳນວນເຕັມ NY, float *Y, ຈຳນວນເຕັມ B, float *OUT)
{
    ຈຳນວນເຕັມ i, j;
    ຈຳນວນເຕັມ index = 0;
    ສຳລັບ(j = 0; j < B; ++j) {
        ສຳລັບ(i = 0; i < NX; ++i){
            ຖ້າ(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        ສຳລັບ(i = 0; i < NY; ++i){
            ຖ້າ(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

void inter_cpu(ຈຳນວນເຕັມ NX, float *X, ຈຳນວນເຕັມ NY, float *Y, ຈຳນວນເຕັມ B, float *OUT)
{
    ຈຳນວນເຕັມ i, j;
    ຈຳນວນເຕັມ index = 0;
    ສຳລັບ(j = 0; j < B; ++j) {
        ສຳລັບ(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        ສຳລັບ(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

void copy_cpu(ຈຳນວນເຕັມ N, float *X, ຈຳນວນເຕັມ INCX, float *Y, ຈຳນວນເຕັມ INCY)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

void mult_add_into_cpu(ຈຳນວນເຕັມ N, float *X, float *Y, float *Z)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

void smooth_l1_cpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        float abs_val = fabs(diff);
        ຖ້າ(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        ບໍ່ດັ່ງນັ້ນ {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

void l1_cpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

void softmax_x_ent_cpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

void logistic_x_ent_cpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < n; ++i){
        float t = truth[i];
        float p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

void l2_cpu(ຈຳນວນເຕັມ n, float *pred, float *truth, float *delta, float *error)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < n; ++i){
        float diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

float dot_cpu(ຈຳນວນເຕັມ N, float *X, ຈຳນວນເຕັມ INCX, float *Y, ຈຳນວນເຕັມ INCY)
{
    ຈຳນວນເຕັມ i;
    float dot = 0;
    ສຳລັບ(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    ສົ່ງຄືນ dot;
}

void softmax(float *input, ຈຳນວນເຕັມ n, float temp, ຈຳນວນເຕັມ stride, float *output)
{
    ຈຳນວນເຕັມ i;
    float sum = 0;
    float largest = -FLT_MAX;
    ສຳລັບ(i = 0; i < n; ++i){
        ຖ້າ(input[i*stride] > largest) largest = input[i*stride];
    }
    ສຳລັບ(i = 0; i < n; ++i){
        float e = exp(input[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    ສຳລັບ(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


void softmax_cpu(float *input, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ batch_offset, ຈຳນວນເຕັມ groups, ຈຳນວນເຕັມ group_offset, ຈຳນວນເຕັມ stride, float temp, float *output)
{
    ຈຳນວນເຕັມ g, b;
    ສຳລັບ(b = 0; b < batch; ++b){
        ສຳລັບ(g = 0; g < groups; ++g){
            softmax(input + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

void upsample_cpu(float *in, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ stride, ຈຳນວນເຕັມ forward, float scale, float *out)
{
    ຈຳນວນເຕັມ i, j, k, b;
    ສຳລັບ(b = 0; b < batch; ++b){
        ສຳລັບ(k = 0; k < c; ++k){
            ສຳລັບ(j = 0; j < h*stride; ++j){
                ສຳລັບ(i = 0; i < w*stride; ++i){
                    ຈຳນວນເຕັມ in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    ຈຳນວນເຕັມ out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    ຖ້າ(forward) out[out_index] = scale*in[in_index];
                    ບໍ່ດັ່ງນັ້ນ in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


