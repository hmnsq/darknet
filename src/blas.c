#અન્તર્ભાવ "blas.h"

#અન્તર્ભાવ <math.h>
#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <float.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>
શૂન્ય reorg_cpu(દશાંશ *x, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c, પૂર્ણાંક batch, પૂર્ણાંક stride, પૂર્ણાંક forward, દશાંશ *out)
{
    પૂર્ણાંક b,i,j,k;
    પૂર્ણાંક out_c = c/(stride*stride);

    માટે(b = 0; b < batch; ++b){
        માટે(k = 0; k < c; ++k){
            માટે(j = 0; j < h; ++j){
                માટે(i = 0; i < w; ++i){
                    પૂર્ણાંક in_index  = i + w*(j + h*(k + c*b));
                    પૂર્ણાંક c2 = k % out_c;
                    પૂર્ણાંક offset = k / out_c;
                    પૂર્ણાંક w2 = i*stride + offset % stride;
                    પૂર્ણાંક h2 = j*stride + offset / stride;
                    પૂર્ણાંક out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    જો(forward) out[out_index] = x[in_index];
                    નહીંતર out[in_index] = x[out_index];
                }
            }
        }
    }
}

શૂન્ય flatten(દશાંશ *x, પૂર્ણાંક size, પૂર્ણાંક layers, પૂર્ણાંક batch, પૂર્ણાંક forward)
{
    દશાંશ *swap = calloc(size*layers*batch, કદ(દશાંશ));
    પૂર્ણાંક i,c,b;
    માટે(b = 0; b < batch; ++b){
        માટે(c = 0; c < layers; ++c){
            માટે(i = 0; i < size; ++i){
                પૂર્ણાંક i1 = b*layers*size + c*size + i;
                પૂર્ણાંક i2 = b*layers*size + i*layers + c;
                જો (forward) swap[i2] = x[i1];
                નહીંતર swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*કદ(દશાંશ));
    મુક્ત(swap);
}

શૂન્ય weighted_sum_cpu(દશાંશ *a, દશાંશ *b, દશાંશ *s, પૂર્ણાંક n, દશાંશ *c)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

શૂન્ય weighted_delta_cpu(દશાંશ *a, દશાંશ *b, દશાંશ *s, દશાંશ *da, દશાંશ *db, દશાંશ *ds, પૂર્ણાંક n, દશાંશ *dc)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        જો(da) da[i] += dc[i] * s[i];
        જો(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

શૂન્ય shortcut_cpu(પૂર્ણાંક batch, પૂર્ણાંક w1, પૂર્ણાંક h1, પૂર્ણાંક c1, દશાંશ *add, પૂર્ણાંક w2, પૂર્ણાંક h2, પૂર્ણાંક c2, દશાંશ s1, દશાંશ s2, દશાંશ *out)
{
    પૂર્ણાંક stride = w1/w2;
    પૂર્ણાંક sample = w2/w1;
    પ્રતિજ્ઞા(stride == h1/h2);
    પ્રતિજ્ઞા(sample == h2/h1);
    જો(stride < 1) stride = 1;
    જો(sample < 1) sample = 1;
    પૂર્ણાંક minw = (w1 < w2) ? w1 : w2;
    પૂર્ણાંક minh = (h1 < h2) ? h1 : h2;
    પૂર્ણાંક minc = (c1 < c2) ? c1 : c2;

    પૂર્ણાંક i,j,k,b;
    માટે(b = 0; b < batch; ++b){
        માટે(k = 0; k < minc; ++k){
            માટે(j = 0; j < minh; ++j){
                માટે(i = 0; i < minw; ++i){
                    પૂર્ણાંક out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    પૂર્ણાંક add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

શૂન્ય mean_cpu(દશાંશ *x, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *mean)
{
    દશાંશ scale = 1./(batch * spatial);
    પૂર્ણાંક i,j,k;
    માટે(i = 0; i < filters; ++i){
        mean[i] = 0;
        માટે(j = 0; j < batch; ++j){
            માટે(k = 0; k < spatial; ++k){
                પૂર્ણાંક index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

શૂન્ય variance_cpu(દશાંશ *x, દશાંશ *mean, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *variance)
{
    દશાંશ scale = 1./(batch * spatial - 1);
    પૂર્ણાંક i,j,k;
    માટે(i = 0; i < filters; ++i){
        variance[i] = 0;
        માટે(j = 0; j < batch; ++j){
            માટે(k = 0; k < spatial; ++k){
                પૂર્ણાંક index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

શૂન્ય l2normalize_cpu(દશાંશ *x, દશાંશ *dx, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial)
{
    પૂર્ણાંક b,f,i;
    માટે(b = 0; b < batch; ++b){
        માટે(i = 0; i < spatial; ++i){
            દશાંશ sum = 0;
            માટે(f = 0; f < filters; ++f){
                પૂર્ણાંક index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            માટે(f = 0; f < filters; ++f){
                પૂર્ણાંક index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


શૂન્ય normalize_cpu(દશાંશ *x, દશાંશ *mean, દશાંશ *variance, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial)
{
    પૂર્ણાંક b, f, i;
    માટે(b = 0; b < batch; ++b){
        માટે(f = 0; f < filters; ++f){
            માટે(i = 0; i < spatial; ++i){
                પૂર્ણાંક index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

શૂન્ય const_cpu(પૂર્ણાંક N, દશાંશ ALPHA, દશાંશ *X, પૂર્ણાંક INCX)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

શૂન્ય mul_cpu(પૂર્ણાંક N, દશાંશ *X, પૂર્ણાંક INCX, દશાંશ *Y, પૂર્ણાંક INCY)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

શૂન્ય pow_cpu(પૂર્ણાંક N, દશાંશ ALPHA, દશાંશ *X, પૂર્ણાંક INCX, દશાંશ *Y, પૂર્ણાંક INCY)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

શૂન્ય axpy_cpu(પૂર્ણાંક N, દશાંશ ALPHA, દશાંશ *X, પૂર્ણાંક INCX, દશાંશ *Y, પૂર્ણાંક INCY)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

શૂન્ય scal_cpu(પૂર્ણાંક N, દશાંશ ALPHA, દશાંશ *X, પૂર્ણાંક INCX)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

શૂન્ય fill_cpu(પૂર્ણાંક N, દશાંશ ALPHA, દશાંશ *X, પૂર્ણાંક INCX)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

શૂન્ય deinter_cpu(પૂર્ણાંક NX, દશાંશ *X, પૂર્ણાંક NY, દશાંશ *Y, પૂર્ણાંક B, દશાંશ *OUT)
{
    પૂર્ણાંક i, j;
    પૂર્ણાંક index = 0;
    માટે(j = 0; j < B; ++j) {
        માટે(i = 0; i < NX; ++i){
            જો(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        માટે(i = 0; i < NY; ++i){
            જો(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

શૂન્ય inter_cpu(પૂર્ણાંક NX, દશાંશ *X, પૂર્ણાંક NY, દશાંશ *Y, પૂર્ણાંક B, દશાંશ *OUT)
{
    પૂર્ણાંક i, j;
    પૂર્ણાંક index = 0;
    માટે(j = 0; j < B; ++j) {
        માટે(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        માટે(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

શૂન્ય copy_cpu(પૂર્ણાંક N, દશાંશ *X, પૂર્ણાંક INCX, દશાંશ *Y, પૂર્ણાંક INCY)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

શૂન્ય mult_add_into_cpu(પૂર્ણાંક N, દશાંશ *X, દશાંશ *Y, દશાંશ *Z)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

શૂન્ય smooth_l1_cpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        દશાંશ diff = truth[i] - pred[i];
        દશાંશ abs_val = fabs(diff);
        જો(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        નહીંતર {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

શૂન્ય l1_cpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        દશાંશ diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

શૂન્ય softmax_x_ent_cpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        દશાંશ t = truth[i];
        દશાંશ p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

શૂન્ય logistic_x_ent_cpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        દશાંશ t = truth[i];
        દશાંશ p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

શૂન્ય l2_cpu(પૂર્ણાંક n, દશાંશ *pred, દશાંશ *truth, દશાંશ *delta, દશાંશ *error)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        દશાંશ diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

દશાંશ dot_cpu(પૂર્ણાંક N, દશાંશ *X, પૂર્ણાંક INCX, દશાંશ *Y, પૂર્ણાંક INCY)
{
    પૂર્ણાંક i;
    દશાંશ dot = 0;
    માટે(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    પાછા dot;
}

શૂન્ય softmax(દશાંશ *આગત, પૂર્ણાંક n, દશાંશ temp, પૂર્ણાંક stride, દશાંશ *output)
{
    પૂર્ણાંક i;
    દશાંશ sum = 0;
    દશાંશ largest = -FLT_MAX;
    માટે(i = 0; i < n; ++i){
        જો(આગત[i*stride] > largest) largest = આગત[i*stride];
    }
    માટે(i = 0; i < n; ++i){
        દશાંશ e = exp(આગત[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    માટે(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


શૂન્ય softmax_cpu(દશાંશ *આગત, પૂર્ણાંક n, પૂર્ણાંક batch, પૂર્ણાંક batch_offset, પૂર્ણાંક groups, પૂર્ણાંક group_offset, પૂર્ણાંક stride, દશાંશ temp, દશાંશ *output)
{
    પૂર્ણાંક g, b;
    માટે(b = 0; b < batch; ++b){
        માટે(g = 0; g < groups; ++g){
            softmax(આગત + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

શૂન્ય upsample_cpu(દશાંશ *in, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c, પૂર્ણાંક batch, પૂર્ણાંક stride, પૂર્ણાંક forward, દશાંશ scale, દશાંશ *out)
{
    પૂર્ણાંક i, j, k, b;
    માટે(b = 0; b < batch; ++b){
        માટે(k = 0; k < c; ++k){
            માટે(j = 0; j < h*stride; ++j){
                માટે(i = 0; i < w*stride; ++i){
                    પૂર્ણાંક in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    પૂર્ણાંક out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    જો(forward) out[out_index] = scale*in[in_index];
                    નહીંતર in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


