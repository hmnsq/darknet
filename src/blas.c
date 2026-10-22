#ಅನ್ತರ್ಭಾವ "blas.h"

#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <float.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
ಶೂನ್ಯ reorg_cpu(ಭಿನ್ನ *x, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ forward, ಭಿನ್ನ *out)
{
    ಪೂರ್ಣಾಙ್ಕ b,i,j,k;
    ಪೂರ್ಣಾಙ್ಕ out_c = c/(stride*stride);

    ಪ್ರತಿ(b = 0; b < batch; ++b){
        ಪ್ರತಿ(k = 0; k < c; ++k){
            ಪ್ರತಿ(j = 0; j < h; ++j){
                ಪ್ರತಿ(i = 0; i < w; ++i){
                    ಪೂರ್ಣಾಙ್ಕ in_index  = i + w*(j + h*(k + c*b));
                    ಪೂರ್ಣಾಙ್ಕ c2 = k % out_c;
                    ಪೂರ್ಣಾಙ್ಕ offset = k / out_c;
                    ಪೂರ್ಣಾಙ್ಕ w2 = i*stride + offset % stride;
                    ಪೂರ್ಣಾಙ್ಕ h2 = j*stride + offset / stride;
                    ಪೂರ್ಣಾಙ್ಕ out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    ಯದಿ(forward) out[out_index] = x[in_index];
                    ಅನ್ಯಥಾ out[in_index] = x[out_index];
                }
            }
        }
    }
}

ಶೂನ್ಯ flatten(ಭಿನ್ನ *x, ಪೂರ್ಣಾಙ್ಕ size, ಪೂರ್ಣಾಙ್ಕ layers, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ forward)
{
    ಭಿನ್ನ *swap = calloc(size*layers*batch, ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಪೂರ್ಣಾಙ್ಕ i,c,b;
    ಪ್ರತಿ(b = 0; b < batch; ++b){
        ಪ್ರತಿ(c = 0; c < layers; ++c){
            ಪ್ರತಿ(i = 0; i < size; ++i){
                ಪೂರ್ಣಾಙ್ಕ i1 = b*layers*size + c*size + i;
                ಪೂರ್ಣಾಙ್ಕ i2 = b*layers*size + i*layers + c;
                ಯದಿ (forward) swap[i2] = x[i1];
                ಅನ್ಯಥಾ swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಮುಕ್ತ(swap);
}

ಶೂನ್ಯ weighted_sum_cpu(ಭಿನ್ನ *a, ಭಿನ್ನ *b, ಭಿನ್ನ *s, ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *c)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

ಶೂನ್ಯ weighted_delta_cpu(ಭಿನ್ನ *a, ಭಿನ್ನ *b, ಭಿನ್ನ *s, ಭಿನ್ನ *da, ಭಿನ್ನ *db, ಭಿನ್ನ *ds, ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *dc)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಯದಿ(da) da[i] += dc[i] * s[i];
        ಯದಿ(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

ಶೂನ್ಯ shortcut_cpu(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w1, ಪೂರ್ಣಾಙ್ಕ h1, ಪೂರ್ಣಾಙ್ಕ c1, ಭಿನ್ನ *add, ಪೂರ್ಣಾಙ್ಕ w2, ಪೂರ್ಣಾಙ್ಕ h2, ಪೂರ್ಣಾಙ್ಕ c2, ಭಿನ್ನ s1, ಭಿನ್ನ s2, ಭಿನ್ನ *out)
{
    ಪೂರ್ಣಾಙ್ಕ stride = w1/w2;
    ಪೂರ್ಣಾಙ್ಕ sample = w2/w1;
    ಪ್ರತಿಜ್ಞಾ(stride == h1/h2);
    ಪ್ರತಿಜ್ಞಾ(sample == h2/h1);
    ಯದಿ(stride < 1) stride = 1;
    ಯದಿ(sample < 1) sample = 1;
    ಪೂರ್ಣಾಙ್ಕ minw = (w1 < w2) ? w1 : w2;
    ಪೂರ್ಣಾಙ್ಕ minh = (h1 < h2) ? h1 : h2;
    ಪೂರ್ಣಾಙ್ಕ minc = (c1 < c2) ? c1 : c2;

    ಪೂರ್ಣಾಙ್ಕ i,j,k,b;
    ಪ್ರತಿ(b = 0; b < batch; ++b){
        ಪ್ರತಿ(k = 0; k < minc; ++k){
            ಪ್ರತಿ(j = 0; j < minh; ++j){
                ಪ್ರತಿ(i = 0; i < minw; ++i){
                    ಪೂರ್ಣಾಙ್ಕ out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    ಪೂರ್ಣಾಙ್ಕ add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

ಶೂನ್ಯ mean_cpu(ಭಿನ್ನ *x, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial, ಭಿನ್ನ *mean)
{
    ಭಿನ್ನ scale = 1./(batch * spatial);
    ಪೂರ್ಣಾಙ್ಕ i,j,k;
    ಪ್ರತಿ(i = 0; i < filters; ++i){
        mean[i] = 0;
        ಪ್ರತಿ(j = 0; j < batch; ++j){
            ಪ್ರತಿ(k = 0; k < spatial; ++k){
                ಪೂರ್ಣಾಙ್ಕ index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

ಶೂನ್ಯ variance_cpu(ಭಿನ್ನ *x, ಭಿನ್ನ *mean, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial, ಭಿನ್ನ *variance)
{
    ಭಿನ್ನ scale = 1./(batch * spatial - 1);
    ಪೂರ್ಣಾಙ್ಕ i,j,k;
    ಪ್ರತಿ(i = 0; i < filters; ++i){
        variance[i] = 0;
        ಪ್ರತಿ(j = 0; j < batch; ++j){
            ಪ್ರತಿ(k = 0; k < spatial; ++k){
                ಪೂರ್ಣಾಙ್ಕ index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

ಶೂನ್ಯ l2normalize_cpu(ಭಿನ್ನ *x, ಭಿನ್ನ *dx, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial)
{
    ಪೂರ್ಣಾಙ್ಕ b,f,i;
    ಪ್ರತಿ(b = 0; b < batch; ++b){
        ಪ್ರತಿ(i = 0; i < spatial; ++i){
            ಭಿನ್ನ sum = 0;
            ಪ್ರತಿ(f = 0; f < filters; ++f){
                ಪೂರ್ಣಾಙ್ಕ index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            ಪ್ರತಿ(f = 0; f < filters; ++f){
                ಪೂರ್ಣಾಙ್ಕ index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


ಶೂನ್ಯ normalize_cpu(ಭಿನ್ನ *x, ಭಿನ್ನ *mean, ಭಿನ್ನ *variance, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ filters, ಪೂರ್ಣಾಙ್ಕ spatial)
{
    ಪೂರ್ಣಾಙ್ಕ b, f, i;
    ಪ್ರತಿ(b = 0; b < batch; ++b){
        ಪ್ರತಿ(f = 0; f < filters; ++f){
            ಪ್ರತಿ(i = 0; i < spatial; ++i){
                ಪೂರ್ಣಾಙ್ಕ index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

ಶೂನ್ಯ const_cpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ ALPHA, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ INCX)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

ಶೂನ್ಯ mul_cpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ INCX, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ INCY)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

ಶೂನ್ಯ pow_cpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ ALPHA, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ INCX, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ INCY)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

ಶೂನ್ಯ axpy_cpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ ALPHA, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ INCX, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ INCY)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

ಶೂನ್ಯ scal_cpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ ALPHA, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ INCX)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

ಶೂನ್ಯ fill_cpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ ALPHA, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ INCX)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

ಶೂನ್ಯ deinter_cpu(ಪೂರ್ಣಾಙ್ಕ NX, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ NY, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ B, ಭಿನ್ನ *OUT)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪೂರ್ಣಾಙ್ಕ index = 0;
    ಪ್ರತಿ(j = 0; j < B; ++j) {
        ಪ್ರತಿ(i = 0; i < NX; ++i){
            ಯದಿ(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        ಪ್ರತಿ(i = 0; i < NY; ++i){
            ಯದಿ(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

ಶೂನ್ಯ inter_cpu(ಪೂರ್ಣಾಙ್ಕ NX, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ NY, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ B, ಭಿನ್ನ *OUT)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪೂರ್ಣಾಙ್ಕ index = 0;
    ಪ್ರತಿ(j = 0; j < B; ++j) {
        ಪ್ರತಿ(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        ಪ್ರತಿ(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

ಶೂನ್ಯ copy_cpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ INCX, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ INCY)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

ಶೂನ್ಯ mult_add_into_cpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ *X, ಭಿನ್ನ *Y, ಭಿನ್ನ *Z)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

ಶೂನ್ಯ smooth_l1_cpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಭಿನ್ನ diff = truth[i] - pred[i];
        ಭಿನ್ನ abs_val = fabs(diff);
        ಯದಿ(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        ಅನ್ಯಥಾ {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

ಶೂನ್ಯ l1_cpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಭಿನ್ನ diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

ಶೂನ್ಯ softmax_x_ent_cpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಭಿನ್ನ t = truth[i];
        ಭಿನ್ನ p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

ಶೂನ್ಯ logistic_x_ent_cpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಭಿನ್ನ t = truth[i];
        ಭಿನ್ನ p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

ಶೂನ್ಯ l2_cpu(ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *pred, ಭಿನ್ನ *truth, ಭಿನ್ನ *delta, ಭಿನ್ನ *error)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಭಿನ್ನ diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

ಭಿನ್ನ dot_cpu(ಪೂರ್ಣಾಙ್ಕ N, ಭಿನ್ನ *X, ಪೂರ್ಣಾಙ್ಕ INCX, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ INCY)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಭಿನ್ನ dot = 0;
    ಪ್ರತಿ(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    ಪ್ರತ್ಯಾಗಮ dot;
}

ಶೂನ್ಯ softmax(ಭಿನ್ನ *ಒಳಹರಿವು, ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ temp, ಪೂರ್ಣಾಙ್ಕ stride, ಭಿನ್ನ *output)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಭಿನ್ನ sum = 0;
    ಭಿನ್ನ largest = -FLT_MAX;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಯದಿ(ಒಳಹರಿವು[i*stride] > largest) largest = ಒಳಹರಿವು[i*stride];
    }
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಭಿನ್ನ e = exp(ಒಳಹರಿವು[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    ಪ್ರತಿ(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


ಶೂನ್ಯ softmax_cpu(ಭಿನ್ನ *ಒಳಹರಿವು, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ batch_offset, ಪೂರ್ಣಾಙ್ಕ groups, ಪೂರ್ಣಾಙ್ಕ group_offset, ಪೂರ್ಣಾಙ್ಕ stride, ಭಿನ್ನ temp, ಭಿನ್ನ *output)
{
    ಪೂರ್ಣಾಙ್ಕ g, b;
    ಪ್ರತಿ(b = 0; b < batch; ++b){
        ಪ್ರತಿ(g = 0; g < groups; ++g){
            softmax(ಒಳಹರಿವು + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

ಶೂನ್ಯ upsample_cpu(ಭಿನ್ನ *in, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ forward, ಭಿನ್ನ scale, ಭಿನ್ನ *out)
{
    ಪೂರ್ಣಾಙ್ಕ i, j, k, b;
    ಪ್ರತಿ(b = 0; b < batch; ++b){
        ಪ್ರತಿ(k = 0; k < c; ++k){
            ಪ್ರತಿ(j = 0; j < h*stride; ++j){
                ಪ್ರತಿ(i = 0; i < w*stride; ++i){
                    ಪೂರ್ಣಾಙ್ಕ in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    ಪೂರ್ಣಾಙ್ಕ out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    ಯದಿ(forward) out[out_index] = scale*in[in_index];
                    ಅನ್ಯಥಾ in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


