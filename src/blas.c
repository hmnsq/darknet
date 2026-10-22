#ضمن "blas.h"

#ضمن <math.h>
#ضمن <assert.h>
#ضمن <float.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>
فراغ reorg_cpu(عائم *x, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم *out)
{
    صحيح b,i,j,k;
    صحيح out_c = c/(stride*stride);

    لكل(b = 0; b < batch; ++b){
        لكل(k = 0; k < c; ++k){
            لكل(j = 0; j < h; ++j){
                لكل(i = 0; i < w; ++i){
                    صحيح in_index  = i + w*(j + h*(k + c*b));
                    صحيح c2 = k % out_c;
                    صحيح offset = k / out_c;
                    صحيح w2 = i*stride + offset % stride;
                    صحيح h2 = j*stride + offset / stride;
                    صحيح out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    𐡄𐡍(forward) out[out_index] = x[in_index];
                    وإلا out[in_index] = x[out_index];
                }
            }
        }
    }
}

فراغ flatten(عائم *x, صحيح size, صحيح layers, صحيح batch, صحيح forward)
{
    عائم *swap = calloc(size*layers*batch, حجم(عائم));
    صحيح i,c,b;
    لكل(b = 0; b < batch; ++b){
        لكل(c = 0; c < layers; ++c){
            لكل(i = 0; i < size; ++i){
                صحيح i1 = b*layers*size + c*size + i;
                صحيح i2 = b*layers*size + i*layers + c;
                𐡄𐡍 (forward) swap[i2] = x[i1];
                وإلا swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*حجم(عائم));
    حرر(swap);
}

فراغ weighted_sum_cpu(عائم *a, عائم *b, عائم *s, صحيح n, عائم *c)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

فراغ weighted_delta_cpu(عائم *a, عائم *b, عائم *s, عائم *da, عائم *db, عائم *ds, صحيح n, عائم *dc)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        𐡄𐡍(da) da[i] += dc[i] * s[i];
        𐡄𐡍(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

فراغ shortcut_cpu(صحيح batch, صحيح w1, صحيح h1, صحيح c1, عائم *add, صحيح w2, صحيح h2, صحيح c2, عائم s1, عائم s2, عائم *out)
{
    صحيح stride = w1/w2;
    صحيح sample = w2/w1;
    أكد(stride == h1/h2);
    أكد(sample == h2/h1);
    𐡄𐡍(stride < 1) stride = 1;
    𐡄𐡍(sample < 1) sample = 1;
    صحيح minw = (w1 < w2) ? w1 : w2;
    صحيح minh = (h1 < h2) ? h1 : h2;
    صحيح minc = (c1 < c2) ? c1 : c2;

    صحيح i,j,k,b;
    لكل(b = 0; b < batch; ++b){
        لكل(k = 0; k < minc; ++k){
            لكل(j = 0; j < minh; ++j){
                لكل(i = 0; i < minw; ++i){
                    صحيح out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    صحيح add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

فراغ mean_cpu(عائم *x, صحيح batch, صحيح filters, صحيح spatial, عائم *mean)
{
    عائم scale = 1./(batch * spatial);
    صحيح i,j,k;
    لكل(i = 0; i < filters; ++i){
        mean[i] = 0;
        لكل(j = 0; j < batch; ++j){
            لكل(k = 0; k < spatial; ++k){
                صحيح index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

فراغ variance_cpu(عائم *x, عائم *mean, صحيح batch, صحيح filters, صحيح spatial, عائم *variance)
{
    عائم scale = 1./(batch * spatial - 1);
    صحيح i,j,k;
    لكل(i = 0; i < filters; ++i){
        variance[i] = 0;
        لكل(j = 0; j < batch; ++j){
            لكل(k = 0; k < spatial; ++k){
                صحيح index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

فراغ l2normalize_cpu(عائم *x, عائم *dx, صحيح batch, صحيح filters, صحيح spatial)
{
    صحيح b,f,i;
    لكل(b = 0; b < batch; ++b){
        لكل(i = 0; i < spatial; ++i){
            عائم sum = 0;
            لكل(f = 0; f < filters; ++f){
                صحيح index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            لكل(f = 0; f < filters; ++f){
                صحيح index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


فراغ normalize_cpu(عائم *x, عائم *mean, عائم *variance, صحيح batch, صحيح filters, صحيح spatial)
{
    صحيح b, f, i;
    لكل(b = 0; b < batch; ++b){
        لكل(f = 0; f < filters; ++f){
            لكل(i = 0; i < spatial; ++i){
                صحيح index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

فراغ const_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

فراغ mul_cpu(صحيح N, عائم *X, صحيح INCX, عائم *Y, صحيح INCY)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

فراغ pow_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX, عائم *Y, صحيح INCY)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

فراغ axpy_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX, عائم *Y, صحيح INCY)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

فراغ scal_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

فراغ fill_cpu(صحيح N, عائم ALPHA, عائم *X, صحيح INCX)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

فراغ deinter_cpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *OUT)
{
    صحيح i, j;
    صحيح index = 0;
    لكل(j = 0; j < B; ++j) {
        لكل(i = 0; i < NX; ++i){
            𐡄𐡍(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        لكل(i = 0; i < NY; ++i){
            𐡄𐡍(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

فراغ inter_cpu(صحيح NX, عائم *X, صحيح NY, عائم *Y, صحيح B, عائم *OUT)
{
    صحيح i, j;
    صحيح index = 0;
    لكل(j = 0; j < B; ++j) {
        لكل(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        لكل(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

فراغ copy_cpu(صحيح N, عائم *X, صحيح INCX, عائم *Y, صحيح INCY)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

فراغ mult_add_into_cpu(صحيح N, عائم *X, عائم *Y, عائم *Z)
{
    صحيح i;
    لكل(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

فراغ smooth_l1_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        عائم diff = truth[i] - pred[i];
        عائم abs_val = fabs(diff);
        𐡄𐡍(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        وإلا {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

فراغ l1_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        عائم diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

فراغ softmax_x_ent_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        عائم t = truth[i];
        عائم p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

فراغ logistic_x_ent_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        عائم t = truth[i];
        عائم p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

فراغ l2_cpu(صحيح n, عائم *pred, عائم *truth, عائم *delta, عائم *error)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        عائم diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

عائم dot_cpu(صحيح N, عائم *X, صحيح INCX, عائم *Y, صحيح INCY)
{
    صحيح i;
    عائم dot = 0;
    لكل(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    أرجع dot;
}

فراغ softmax(عائم *input, صحيح n, عائم temp, صحيح stride, عائم *output)
{
    صحيح i;
    عائم sum = 0;
    عائم largest = -FLT_MAX;
    لكل(i = 0; i < n; ++i){
        𐡄𐡍(input[i*stride] > largest) largest = input[i*stride];
    }
    لكل(i = 0; i < n; ++i){
        عائم e = exp(input[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    لكل(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


فراغ softmax_cpu(عائم *input, صحيح n, صحيح batch, صحيح batch_offset, صحيح groups, صحيح group_offset, صحيح stride, عائم temp, عائم *output)
{
    صحيح g, b;
    لكل(b = 0; b < batch; ++b){
        لكل(g = 0; g < groups; ++g){
            softmax(input + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

فراغ upsample_cpu(عائم *in, صحيح w, صحيح h, صحيح c, صحيح batch, صحيح stride, صحيح forward, عائم scale, عائم *out)
{
    صحيح i, j, k, b;
    لكل(b = 0; b < batch; ++b){
        لكل(k = 0; k < c; ++k){
            لكل(j = 0; j < h*stride; ++j){
                لكل(i = 0; i < w*stride; ++i){
                    صحيح in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    صحيح out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    𐡄𐡍(forward) out[out_index] = scale*in[in_index];
                    وإلا in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


