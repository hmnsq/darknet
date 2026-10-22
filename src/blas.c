#அந்தர்பாவ "blas.h"

#அந்தர்பாவ <math.h>
#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <float.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>
வெறுமை reorg_cpu(தசம *x, முழு w, முழு h, முழு c, முழு batch, முழு stride, முழு forward, தசம *out)
{
    முழு b,i,j,k;
    முழு out_c = c/(stride*stride);

    க்கு(b = 0; b < batch; ++b){
        க்கு(k = 0; k < c; ++k){
            க்கு(j = 0; j < h; ++j){
                க்கு(i = 0; i < w; ++i){
                    முழு in_index  = i + w*(j + h*(k + c*b));
                    முழு c2 = k % out_c;
                    முழு offset = k / out_c;
                    முழு w2 = i*stride + offset % stride;
                    முழு h2 = j*stride + offset / stride;
                    முழு out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    எனில்(forward) out[out_index] = x[in_index];
                    இல்லையெனில் out[in_index] = x[out_index];
                }
            }
        }
    }
}

வெறுமை flatten(தசம *x, முழு size, முழு layers, முழு batch, முழு forward)
{
    தசம *swap = calloc(size*layers*batch, அளவு(தசம));
    முழு i,c,b;
    க்கு(b = 0; b < batch; ++b){
        க்கு(c = 0; c < layers; ++c){
            க்கு(i = 0; i < size; ++i){
                முழு i1 = b*layers*size + c*size + i;
                முழு i2 = b*layers*size + i*layers + c;
                எனில் (forward) swap[i2] = x[i1];
                இல்லையெனில் swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*அளவு(தசம));
    விடுவி(swap);
}

வெறுமை weighted_sum_cpu(தசம *a, தசம *b, தசம *s, முழு n, தசம *c)
{
    முழு i;
    க்கு(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

வெறுமை weighted_delta_cpu(தசம *a, தசம *b, தசம *s, தசம *da, தசம *db, தசம *ds, முழு n, தசம *dc)
{
    முழு i;
    க்கு(i = 0; i < n; ++i){
        எனில்(da) da[i] += dc[i] * s[i];
        எனில்(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

வெறுமை shortcut_cpu(முழு batch, முழு w1, முழு h1, முழு c1, தசம *add, முழு w2, முழு h2, முழு c2, தசம s1, தசம s2, தசம *out)
{
    முழு stride = w1/w2;
    முழு sample = w2/w1;
    ப்ரதிஜ்ஞா(stride == h1/h2);
    ப்ரதிஜ்ஞா(sample == h2/h1);
    எனில்(stride < 1) stride = 1;
    எனில்(sample < 1) sample = 1;
    முழு minw = (w1 < w2) ? w1 : w2;
    முழு minh = (h1 < h2) ? h1 : h2;
    முழு minc = (c1 < c2) ? c1 : c2;

    முழு i,j,k,b;
    க்கு(b = 0; b < batch; ++b){
        க்கு(k = 0; k < minc; ++k){
            க்கு(j = 0; j < minh; ++j){
                க்கு(i = 0; i < minw; ++i){
                    முழு out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    முழு add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

வெறுமை mean_cpu(தசம *x, முழு batch, முழு filters, முழு spatial, தசம *mean)
{
    தசம scale = 1./(batch * spatial);
    முழு i,j,k;
    க்கு(i = 0; i < filters; ++i){
        mean[i] = 0;
        க்கு(j = 0; j < batch; ++j){
            க்கு(k = 0; k < spatial; ++k){
                முழு index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

வெறுமை variance_cpu(தசம *x, தசம *mean, முழு batch, முழு filters, முழு spatial, தசம *variance)
{
    தசம scale = 1./(batch * spatial - 1);
    முழு i,j,k;
    க்கு(i = 0; i < filters; ++i){
        variance[i] = 0;
        க்கு(j = 0; j < batch; ++j){
            க்கு(k = 0; k < spatial; ++k){
                முழு index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

வெறுமை l2normalize_cpu(தசம *x, தசம *dx, முழு batch, முழு filters, முழு spatial)
{
    முழு b,f,i;
    க்கு(b = 0; b < batch; ++b){
        க்கு(i = 0; i < spatial; ++i){
            தசம sum = 0;
            க்கு(f = 0; f < filters; ++f){
                முழு index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            க்கு(f = 0; f < filters; ++f){
                முழு index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


வெறுமை normalize_cpu(தசம *x, தசம *mean, தசம *variance, முழு batch, முழு filters, முழு spatial)
{
    முழு b, f, i;
    க்கு(b = 0; b < batch; ++b){
        க்கு(f = 0; f < filters; ++f){
            க்கு(i = 0; i < spatial; ++i){
                முழு index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

வெறுமை const_cpu(முழு N, தசம ALPHA, தசம *X, முழு INCX)
{
    முழு i;
    க்கு(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

வெறுமை mul_cpu(முழு N, தசம *X, முழு INCX, தசம *Y, முழு INCY)
{
    முழு i;
    க்கு(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

வெறுமை pow_cpu(முழு N, தசம ALPHA, தசம *X, முழு INCX, தசம *Y, முழு INCY)
{
    முழு i;
    க்கு(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

வெறுமை axpy_cpu(முழு N, தசம ALPHA, தசம *X, முழு INCX, தசம *Y, முழு INCY)
{
    முழு i;
    க்கு(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

வெறுமை scal_cpu(முழு N, தசம ALPHA, தசம *X, முழு INCX)
{
    முழு i;
    க்கு(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

வெறுமை fill_cpu(முழு N, தசம ALPHA, தசம *X, முழு INCX)
{
    முழு i;
    க்கு(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

வெறுமை deinter_cpu(முழு NX, தசம *X, முழு NY, தசம *Y, முழு B, தசம *OUT)
{
    முழு i, j;
    முழு index = 0;
    க்கு(j = 0; j < B; ++j) {
        க்கு(i = 0; i < NX; ++i){
            எனில்(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        க்கு(i = 0; i < NY; ++i){
            எனில்(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

வெறுமை inter_cpu(முழு NX, தசம *X, முழு NY, தசம *Y, முழு B, தசம *OUT)
{
    முழு i, j;
    முழு index = 0;
    க்கு(j = 0; j < B; ++j) {
        க்கு(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        க்கு(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

வெறுமை copy_cpu(முழு N, தசம *X, முழு INCX, தசம *Y, முழு INCY)
{
    முழு i;
    க்கு(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

வெறுமை mult_add_into_cpu(முழு N, தசம *X, தசம *Y, தசம *Z)
{
    முழு i;
    க்கு(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

வெறுமை smooth_l1_cpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error)
{
    முழு i;
    க்கு(i = 0; i < n; ++i){
        தசம diff = truth[i] - pred[i];
        தசம abs_val = fabs(diff);
        எனில்(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        இல்லையெனில் {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

வெறுமை l1_cpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error)
{
    முழு i;
    க்கு(i = 0; i < n; ++i){
        தசம diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

வெறுமை softmax_x_ent_cpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error)
{
    முழு i;
    க்கு(i = 0; i < n; ++i){
        தசம t = truth[i];
        தசம p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

வெறுமை logistic_x_ent_cpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error)
{
    முழு i;
    க்கு(i = 0; i < n; ++i){
        தசம t = truth[i];
        தசம p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

வெறுமை l2_cpu(முழு n, தசம *pred, தசம *truth, தசம *delta, தசம *error)
{
    முழு i;
    க்கு(i = 0; i < n; ++i){
        தசம diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

தசம dot_cpu(முழு N, தசம *X, முழு INCX, தசம *Y, முழு INCY)
{
    முழு i;
    தசம dot = 0;
    க்கு(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    திரும்பு dot;
}

வெறுமை softmax(தசம *உள்ளீடு, முழு n, தசம temp, முழு stride, தசம *output)
{
    முழு i;
    தசம sum = 0;
    தசம largest = -FLT_MAX;
    க்கு(i = 0; i < n; ++i){
        எனில்(உள்ளீடு[i*stride] > largest) largest = உள்ளீடு[i*stride];
    }
    க்கு(i = 0; i < n; ++i){
        தசம e = exp(உள்ளீடு[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    க்கு(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


வெறுமை softmax_cpu(தசம *உள்ளீடு, முழு n, முழு batch, முழு batch_offset, முழு groups, முழு group_offset, முழு stride, தசம temp, தசம *output)
{
    முழு g, b;
    க்கு(b = 0; b < batch; ++b){
        க்கு(g = 0; g < groups; ++g){
            softmax(உள்ளீடு + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

வெறுமை upsample_cpu(தசம *in, முழு w, முழு h, முழு c, முழு batch, முழு stride, முழு forward, தசம scale, தசம *out)
{
    முழு i, j, k, b;
    க்கு(b = 0; b < batch; ++b){
        க்கு(k = 0; k < c; ++k){
            க்கு(j = 0; j < h*stride; ++j){
                க்கு(i = 0; i < w*stride; ++i){
                    முழு in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    முழு out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    எனில்(forward) out[out_index] = scale*in[in_index];
                    இல்லையெனில் in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


