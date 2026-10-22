#അന്തര്ഭാവ "blas.h"

#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <float.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>
ശൂന്യം reorg_cpu(ദശാംശം *x, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ forward, ദശാംശം *out)
{
    പൂർണ്ണസംഖ്യ b,i,j,k;
    പൂർണ്ണസംഖ്യ out_c = c/(stride*stride);

    വേണ്ടി(b = 0; b < batch; ++b){
        വേണ്ടി(k = 0; k < c; ++k){
            വേണ്ടി(j = 0; j < h; ++j){
                വേണ്ടി(i = 0; i < w; ++i){
                    പൂർണ്ണസംഖ്യ in_index  = i + w*(j + h*(k + c*b));
                    പൂർണ്ണസംഖ്യ c2 = k % out_c;
                    പൂർണ്ണസംഖ്യ offset = k / out_c;
                    പൂർണ്ണസംഖ്യ w2 = i*stride + offset % stride;
                    പൂർണ്ണസംഖ്യ h2 = j*stride + offset / stride;
                    പൂർണ്ണസംഖ്യ out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    എങ്കിൽ(forward) out[out_index] = x[in_index];
                    അല്ലെങ്കിൽ out[in_index] = x[out_index];
                }
            }
        }
    }
}

ശൂന്യം flatten(ദശാംശം *x, പൂർണ്ണസംഖ്യ size, പൂർണ്ണസംഖ്യ layers, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ forward)
{
    ദശാംശം *swap = calloc(size*layers*batch, വലുപ്പം(ദശാംശം));
    പൂർണ്ണസംഖ്യ i,c,b;
    വേണ്ടി(b = 0; b < batch; ++b){
        വേണ്ടി(c = 0; c < layers; ++c){
            വേണ്ടി(i = 0; i < size; ++i){
                പൂർണ്ണസംഖ്യ i1 = b*layers*size + c*size + i;
                പൂർണ്ണസംഖ്യ i2 = b*layers*size + i*layers + c;
                എങ്കിൽ (forward) swap[i2] = x[i1];
                അല്ലെങ്കിൽ swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*വലുപ്പം(ദശാംശം));
    സ്വതന്ത്രം(swap);
}

ശൂന്യം weighted_sum_cpu(ദശാംശം *a, ദശാംശം *b, ദശാംശം *s, പൂർണ്ണസംഖ്യ n, ദശാംശം *c)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

ശൂന്യം weighted_delta_cpu(ദശാംശം *a, ദശാംശം *b, ദശാംശം *s, ദശാംശം *da, ദശാംശം *db, ദശാംശം *ds, പൂർണ്ണസംഖ്യ n, ദശാംശം *dc)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        എങ്കിൽ(da) da[i] += dc[i] * s[i];
        എങ്കിൽ(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

ശൂന്യം shortcut_cpu(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w1, പൂർണ്ണസംഖ്യ h1, പൂർണ്ണസംഖ്യ c1, ദശാംശം *add, പൂർണ്ണസംഖ്യ w2, പൂർണ്ണസംഖ്യ h2, പൂർണ്ണസംഖ്യ c2, ദശാംശം s1, ദശാംശം s2, ദശാംശം *out)
{
    പൂർണ്ണസംഖ്യ stride = w1/w2;
    പൂർണ്ണസംഖ്യ sample = w2/w1;
    പ്രതിജ്ഞാ(stride == h1/h2);
    പ്രതിജ്ഞാ(sample == h2/h1);
    എങ്കിൽ(stride < 1) stride = 1;
    എങ്കിൽ(sample < 1) sample = 1;
    പൂർണ്ണസംഖ്യ minw = (w1 < w2) ? w1 : w2;
    പൂർണ്ണസംഖ്യ minh = (h1 < h2) ? h1 : h2;
    പൂർണ്ണസംഖ്യ minc = (c1 < c2) ? c1 : c2;

    പൂർണ്ണസംഖ്യ i,j,k,b;
    വേണ്ടി(b = 0; b < batch; ++b){
        വേണ്ടി(k = 0; k < minc; ++k){
            വേണ്ടി(j = 0; j < minh; ++j){
                വേണ്ടി(i = 0; i < minw; ++i){
                    പൂർണ്ണസംഖ്യ out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    പൂർണ്ണസംഖ്യ add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

ശൂന്യം mean_cpu(ദശാംശം *x, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial, ദശാംശം *mean)
{
    ദശാംശം scale = 1./(batch * spatial);
    പൂർണ്ണസംഖ്യ i,j,k;
    വേണ്ടി(i = 0; i < filters; ++i){
        mean[i] = 0;
        വേണ്ടി(j = 0; j < batch; ++j){
            വേണ്ടി(k = 0; k < spatial; ++k){
                പൂർണ്ണസംഖ്യ index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

ശൂന്യം variance_cpu(ദശാംശം *x, ദശാംശം *mean, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial, ദശാംശം *variance)
{
    ദശാംശം scale = 1./(batch * spatial - 1);
    പൂർണ്ണസംഖ്യ i,j,k;
    വേണ്ടി(i = 0; i < filters; ++i){
        variance[i] = 0;
        വേണ്ടി(j = 0; j < batch; ++j){
            വേണ്ടി(k = 0; k < spatial; ++k){
                പൂർണ്ണസംഖ്യ index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

ശൂന്യം l2normalize_cpu(ദശാംശം *x, ദശാംശം *dx, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial)
{
    പൂർണ്ണസംഖ്യ b,f,i;
    വേണ്ടി(b = 0; b < batch; ++b){
        വേണ്ടി(i = 0; i < spatial; ++i){
            ദശാംശം sum = 0;
            വേണ്ടി(f = 0; f < filters; ++f){
                പൂർണ്ണസംഖ്യ index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            വേണ്ടി(f = 0; f < filters; ++f){
                പൂർണ്ണസംഖ്യ index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


ശൂന്യം normalize_cpu(ദശാംശം *x, ദശാംശം *mean, ദശാംശം *variance, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ filters, പൂർണ്ണസംഖ്യ spatial)
{
    പൂർണ്ണസംഖ്യ b, f, i;
    വേണ്ടി(b = 0; b < batch; ++b){
        വേണ്ടി(f = 0; f < filters; ++f){
            വേണ്ടി(i = 0; i < spatial; ++i){
                പൂർണ്ണസംഖ്യ index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

ശൂന്യം const_cpu(പൂർണ്ണസംഖ്യ N, ദശാംശം ALPHA, ദശാംശം *X, പൂർണ്ണസംഖ്യ INCX)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

ശൂന്യം mul_cpu(പൂർണ്ണസംഖ്യ N, ദശാംശം *X, പൂർണ്ണസംഖ്യ INCX, ദശാംശം *Y, പൂർണ്ണസംഖ്യ INCY)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

ശൂന്യം pow_cpu(പൂർണ്ണസംഖ്യ N, ദശാംശം ALPHA, ദശാംശം *X, പൂർണ്ണസംഖ്യ INCX, ദശാംശം *Y, പൂർണ്ണസംഖ്യ INCY)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

ശൂന്യം axpy_cpu(പൂർണ്ണസംഖ്യ N, ദശാംശം ALPHA, ദശാംശം *X, പൂർണ്ണസംഖ്യ INCX, ദശാംശം *Y, പൂർണ്ണസംഖ്യ INCY)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

ശൂന്യം scal_cpu(പൂർണ്ണസംഖ്യ N, ദശാംശം ALPHA, ദശാംശം *X, പൂർണ്ണസംഖ്യ INCX)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

ശൂന്യം fill_cpu(പൂർണ്ണസംഖ്യ N, ദശാംശം ALPHA, ദശാംശം *X, പൂർണ്ണസംഖ്യ INCX)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

ശൂന്യം deinter_cpu(പൂർണ്ണസംഖ്യ NX, ദശാംശം *X, പൂർണ്ണസംഖ്യ NY, ദശാംശം *Y, പൂർണ്ണസംഖ്യ B, ദശാംശം *OUT)
{
    പൂർണ്ണസംഖ്യ i, j;
    പൂർണ്ണസംഖ്യ index = 0;
    വേണ്ടി(j = 0; j < B; ++j) {
        വേണ്ടി(i = 0; i < NX; ++i){
            എങ്കിൽ(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        വേണ്ടി(i = 0; i < NY; ++i){
            എങ്കിൽ(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

ശൂന്യം inter_cpu(പൂർണ്ണസംഖ്യ NX, ദശാംശം *X, പൂർണ്ണസംഖ്യ NY, ദശാംശം *Y, പൂർണ്ണസംഖ്യ B, ദശാംശം *OUT)
{
    പൂർണ്ണസംഖ്യ i, j;
    പൂർണ്ണസംഖ്യ index = 0;
    വേണ്ടി(j = 0; j < B; ++j) {
        വേണ്ടി(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        വേണ്ടി(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

ശൂന്യം copy_cpu(പൂർണ്ണസംഖ്യ N, ദശാംശം *X, പൂർണ്ണസംഖ്യ INCX, ദശാംശം *Y, പൂർണ്ണസംഖ്യ INCY)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

ശൂന്യം mult_add_into_cpu(പൂർണ്ണസംഖ്യ N, ദശാംശം *X, ദശാംശം *Y, ദശാംശം *Z)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

ശൂന്യം smooth_l1_cpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        ദശാംശം diff = truth[i] - pred[i];
        ദശാംശം abs_val = fabs(diff);
        എങ്കിൽ(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        അല്ലെങ്കിൽ {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

ശൂന്യം l1_cpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        ദശാംശം diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

ശൂന്യം softmax_x_ent_cpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        ദശാംശം t = truth[i];
        ദശാംശം p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

ശൂന്യം logistic_x_ent_cpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        ദശാംശം t = truth[i];
        ദശാംശം p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

ശൂന്യം l2_cpu(പൂർണ്ണസംഖ്യ n, ദശാംശം *pred, ദശാംശം *truth, ദശാംശം *delta, ദശാംശം *error)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        ദശാംശം diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

ദശാംശം dot_cpu(പൂർണ്ണസംഖ്യ N, ദശാംശം *X, പൂർണ്ണസംഖ്യ INCX, ദശാംശം *Y, പൂർണ്ണസംഖ്യ INCY)
{
    പൂർണ്ണസംഖ്യ i;
    ദശാംശം dot = 0;
    വേണ്ടി(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    തിരികെ dot;
}

ശൂന്യം softmax(ദശാംശം *നിവേശം, പൂർണ്ണസംഖ്യ n, ദശാംശം temp, പൂർണ്ണസംഖ്യ stride, ദശാംശം *output)
{
    പൂർണ്ണസംഖ്യ i;
    ദശാംശം sum = 0;
    ദശാംശം largest = -FLT_MAX;
    വേണ്ടി(i = 0; i < n; ++i){
        എങ്കിൽ(നിവേശം[i*stride] > largest) largest = നിവേശം[i*stride];
    }
    വേണ്ടി(i = 0; i < n; ++i){
        ദശാംശം e = exp(നിവേശം[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    വേണ്ടി(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


ശൂന്യം softmax_cpu(ദശാംശം *നിവേശം, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ batch_offset, പൂർണ്ണസംഖ്യ groups, പൂർണ്ണസംഖ്യ group_offset, പൂർണ്ണസംഖ്യ stride, ദശാംശം temp, ദശാംശം *output)
{
    പൂർണ്ണസംഖ്യ g, b;
    വേണ്ടി(b = 0; b < batch; ++b){
        വേണ്ടി(g = 0; g < groups; ++g){
            softmax(നിവേശം + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

ശൂന്യം upsample_cpu(ദശാംശം *in, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ forward, ദശാംശം scale, ദശാംശം *out)
{
    പൂർണ്ണസംഖ്യ i, j, k, b;
    വേണ്ടി(b = 0; b < batch; ++b){
        വേണ്ടി(k = 0; k < c; ++k){
            വേണ്ടി(j = 0; j < h*stride; ++j){
                വേണ്ടി(i = 0; i < w*stride; ++i){
                    പൂർണ്ണസംഖ്യ in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    പൂർണ്ണസംഖ്യ out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    എങ്കിൽ(forward) out[out_index] = scale*in[in_index];
                    അല്ലെങ്കിൽ in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


