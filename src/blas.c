#ଅନ୍ତର୍ଭାଵ "blas.h"

#ଅନ୍ତର୍ଭାଵ <math.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <float.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
ଶୂନ୍ୟ reorg_cpu(ଦଶମିକ *x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା forward, ଦଶମିକ *out)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b,i,j,k;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_c = c/(stride*stride);

    ପାଇଁ(b = 0; b < batch; ++b){
        ପାଇଁ(k = 0; k < c; ++k){
            ପାଇଁ(j = 0; j < h; ++j){
                ପାଇଁ(i = 0; i < w; ++i){
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_index  = i + w*(j + h*(k + c*b));
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c2 = k % out_c;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = k / out_c;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w2 = i*stride + offset % stride;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h2 = j*stride + offset / stride;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    ଯଦି(forward) out[out_index] = x[in_index];
                    ଅନ୍ୟଥା out[in_index] = x[out_index];
                }
            }
        }
    }
}

ଶୂନ୍ୟ flatten(ଦଶମିକ *x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା layers, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା forward)
{
    ଦଶମିକ *swap = calloc(size*layers*batch, ଆକାର(ଦଶମିକ));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,c,b;
    ପାଇଁ(b = 0; b < batch; ++b){
        ପାଇଁ(c = 0; c < layers; ++c){
            ପାଇଁ(i = 0; i < size; ++i){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା i1 = b*layers*size + c*size + i;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା i2 = b*layers*size + i*layers + c;
                ଯଦି (forward) swap[i2] = x[i1];
                ଅନ୍ୟଥା swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*ଆକାର(ଦଶମିକ));
    ମୁକ୍ତ(swap);
}

ଶୂନ୍ୟ weighted_sum_cpu(ଦଶମିକ *a, ଦଶମିକ *b, ଦଶମିକ *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *c)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

ଶୂନ୍ୟ weighted_delta_cpu(ଦଶମିକ *a, ଦଶମିକ *b, ଦଶମିକ *s, ଦଶମିକ *da, ଦଶମିକ *db, ଦଶମିକ *ds, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *dc)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        ଯଦି(da) da[i] += dc[i] * s[i];
        ଯଦି(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

ଶୂନ୍ୟ shortcut_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c1, ଦଶମିକ *add, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c2, ଦଶମିକ s1, ଦଶମିକ s2, ଦଶମିକ *out)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride = w1/w2;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sample = w2/w1;
    ପ୍ରତିଜ୍ଞା(stride == h1/h2);
    ପ୍ରତିଜ୍ଞା(sample == h2/h1);
    ଯଦି(stride < 1) stride = 1;
    ଯଦି(sample < 1) sample = 1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା minw = (w1 < w2) ? w1 : w2;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା minh = (h1 < h2) ? h1 : h2;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା minc = (c1 < c2) ? c1 : c2;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,k,b;
    ପାଇଁ(b = 0; b < batch; ++b){
        ପାଇଁ(k = 0; k < minc; ++k){
            ପାଇଁ(j = 0; j < minh; ++j){
                ପାଇଁ(i = 0; i < minw; ++i){
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

ଶୂନ୍ୟ mean_cpu(ଦଶମିକ *x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *mean)
{
    ଦଶମିକ scale = 1./(batch * spatial);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,k;
    ପାଇଁ(i = 0; i < filters; ++i){
        mean[i] = 0;
        ପାଇଁ(j = 0; j < batch; ++j){
            ପାଇଁ(k = 0; k < spatial; ++k){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

ଶୂନ୍ୟ variance_cpu(ଦଶମିକ *x, ଦଶମିକ *mean, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *variance)
{
    ଦଶମିକ scale = 1./(batch * spatial - 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,k;
    ପାଇଁ(i = 0; i < filters; ++i){
        variance[i] = 0;
        ପାଇଁ(j = 0; j < batch; ++j){
            ପାଇଁ(k = 0; k < spatial; ++k){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

ଶୂନ୍ୟ l2normalize_cpu(ଦଶମିକ *x, ଦଶମିକ *dx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b,f,i;
    ପାଇଁ(b = 0; b < batch; ++b){
        ପାଇଁ(i = 0; i < spatial; ++i){
            ଦଶମିକ sum = 0;
            ପାଇଁ(f = 0; f < filters; ++f){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            ପାଇଁ(f = 0; f < filters; ++f){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


ଶୂନ୍ୟ normalize_cpu(ଦଶମିକ *x, ଦଶମିକ *mean, ଦଶମିକ *variance, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b, f, i;
    ପାଇଁ(b = 0; b < batch; ++b){
        ପାଇଁ(f = 0; f < filters; ++f){
            ପାଇଁ(i = 0; i < spatial; ++i){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

ଶୂନ୍ୟ const_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ ALPHA, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

ଶୂନ୍ୟ mul_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX, ଦଶମିକ *Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCY)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

ଶୂନ୍ୟ pow_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ ALPHA, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX, ଦଶମିକ *Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCY)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

ଶୂନ୍ୟ axpy_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ ALPHA, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX, ଦଶମିକ *Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCY)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

ଶୂନ୍ୟ scal_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ ALPHA, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

ଶୂନ୍ୟ fill_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ ALPHA, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

ଶୂନ୍ୟ deinter_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା NX, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା NY, ଦଶମିକ *Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା B, ଦଶମିକ *OUT)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = 0;
    ପାଇଁ(j = 0; j < B; ++j) {
        ପାଇଁ(i = 0; i < NX; ++i){
            ଯଦି(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        ପାଇଁ(i = 0; i < NY; ++i){
            ଯଦି(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

ଶୂନ୍ୟ inter_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା NX, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା NY, ଦଶମିକ *Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା B, ଦଶମିକ *OUT)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = 0;
    ପାଇଁ(j = 0; j < B; ++j) {
        ପାଇଁ(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        ପାଇଁ(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

ଶୂନ୍ୟ copy_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX, ଦଶମିକ *Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCY)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

ଶୂନ୍ୟ mult_add_into_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ *X, ଦଶମିକ *Y, ଦଶମିକ *Z)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

ଶୂନ୍ୟ smooth_l1_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        ଦଶମିକ diff = truth[i] - pred[i];
        ଦଶମିକ abs_val = fabs(diff);
        ଯଦି(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        ଅନ୍ୟଥା {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

ଶୂନ୍ୟ l1_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        ଦଶମିକ diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

ଶୂନ୍ୟ softmax_x_ent_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        ଦଶମିକ t = truth[i];
        ଦଶମିକ p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

ଶୂନ୍ୟ logistic_x_ent_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        ଦଶମିକ t = truth[i];
        ଦଶମିକ p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

ଶୂନ୍ୟ l2_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *pred, ଦଶମିକ *truth, ଦଶମିକ *delta, ଦଶମିକ *error)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        ଦଶମିକ diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

ଦଶମିକ dot_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ଦଶମିକ *X, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCX, ଦଶମିକ *Y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା INCY)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଦଶମିକ dot = 0;
    ପାଇଁ(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    ଫେରନ୍ତୁ dot;
}

ଶୂନ୍ୟ softmax(ଦଶମିକ *ନିବେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ temp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ଦଶମିକ *output)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଦଶମିକ sum = 0;
    ଦଶମିକ largest = -FLT_MAX;
    ପାଇଁ(i = 0; i < n; ++i){
        ଯଦି(ନିବେଶ[i*stride] > largest) largest = ନିବେଶ[i*stride];
    }
    ପାଇଁ(i = 0; i < n; ++i){
        ଦଶମିକ e = exp(ନିବେଶ[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    ପାଇଁ(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


ଶୂନ୍ୟ softmax_cpu(ଦଶମିକ *ନିବେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_offset, ପୂର୍ଣ୍ଣସଂଖ୍ୟା groups, ପୂର୍ଣ୍ଣସଂଖ୍ୟା group_offset, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ଦଶମିକ temp, ଦଶମିକ *output)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା g, b;
    ପାଇଁ(b = 0; b < batch; ++b){
        ପାଇଁ(g = 0; g < groups; ++g){
            softmax(ନିବେଶ + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

ଶୂନ୍ୟ upsample_cpu(ଦଶମିକ *in, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା forward, ଦଶମିକ scale, ଦଶମିକ *out)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k, b;
    ପାଇଁ(b = 0; b < batch; ++b){
        ପାଇଁ(k = 0; k < c; ++k){
            ପାଇଁ(j = 0; j < h*stride; ++j){
                ପାଇଁ(i = 0; i < w*stride; ++i){
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    ଯଦି(forward) out[out_index] = scale*in[in_index];
                    ଅନ୍ୟଥା in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


