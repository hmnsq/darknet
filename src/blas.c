#অন্তর্ভাৱ "blas.h"

#অন্তর্ভাৱ <math.h>
#অন্তর্ভাৱ <assert.h>
#অন্তর্ভাৱ <float.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <string.h>
শূন্য reorg_cpu(ভিন্ন *x, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক forward, ভিন্ন *out)
{
    পূর্ণাঙ্ক b,i,j,k;
    পূর্ণাঙ্ক out_c = c/(stride*stride);

    বাবে(b = 0; b < batch; ++b){
        বাবে(k = 0; k < c; ++k){
            বাবে(j = 0; j < h; ++j){
                বাবে(i = 0; i < w; ++i){
                    পূর্ণাঙ্ক in_index  = i + w*(j + h*(k + c*b));
                    পূর্ণাঙ্ক c2 = k % out_c;
                    পূর্ণাঙ্ক offset = k / out_c;
                    পূর্ণাঙ্ক w2 = i*stride + offset % stride;
                    পূর্ণাঙ্ক h2 = j*stride + offset / stride;
                    পূর্ণাঙ্ক out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    যদি(forward) out[out_index] = x[in_index];
                    অন্যথা out[in_index] = x[out_index];
                }
            }
        }
    }
}

শূন্য flatten(ভিন্ন *x, পূর্ণাঙ্ক size, পূর্ণাঙ্ক layers, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক forward)
{
    ভিন্ন *swap = calloc(size*layers*batch, পরিমাণ(ভিন্ন));
    পূর্ণাঙ্ক i,c,b;
    বাবে(b = 0; b < batch; ++b){
        বাবে(c = 0; c < layers; ++c){
            বাবে(i = 0; i < size; ++i){
                পূর্ণাঙ্ক i1 = b*layers*size + c*size + i;
                পূর্ণাঙ্ক i2 = b*layers*size + i*layers + c;
                যদি (forward) swap[i2] = x[i1];
                অন্যথা swap[i1] = x[i2];
            }
        }
    }
    memcpy(x, swap, size*layers*batch*পরিমাণ(ভিন্ন));
    মুক্ত(swap);
}

শূন্য weighted_sum_cpu(ভিন্ন *a, ভিন্ন *b, ভিন্ন *s, পূর্ণাঙ্ক n, ভিন্ন *c)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

শূন্য weighted_delta_cpu(ভিন্ন *a, ভিন্ন *b, ভিন্ন *s, ভিন্ন *da, ভিন্ন *db, ভিন্ন *ds, পূর্ণাঙ্ক n, ভিন্ন *dc)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        যদি(da) da[i] += dc[i] * s[i];
        যদি(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

শূন্য shortcut_cpu(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w1, পূর্ণাঙ্ক h1, পূর্ণাঙ্ক c1, ভিন্ন *add, পূর্ণাঙ্ক w2, পূর্ণাঙ্ক h2, পূর্ণাঙ্ক c2, ভিন্ন s1, ভিন্ন s2, ভিন্ন *out)
{
    পূর্ণাঙ্ক stride = w1/w2;
    পূর্ণাঙ্ক sample = w2/w1;
    প্রতিজ্ঞা(stride == h1/h2);
    প্রতিজ্ঞা(sample == h2/h1);
    যদি(stride < 1) stride = 1;
    যদি(sample < 1) sample = 1;
    পূর্ণাঙ্ক minw = (w1 < w2) ? w1 : w2;
    পূর্ণাঙ্ক minh = (h1 < h2) ? h1 : h2;
    পূর্ণাঙ্ক minc = (c1 < c2) ? c1 : c2;

    পূর্ণাঙ্ক i,j,k,b;
    বাবে(b = 0; b < batch; ++b){
        বাবে(k = 0; k < minc; ++k){
            বাবে(j = 0; j < minh; ++j){
                বাবে(i = 0; i < minw; ++i){
                    পূর্ণাঙ্ক out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    পূর্ণাঙ্ক add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

শূন্য mean_cpu(ভিন্ন *x, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial, ভিন্ন *mean)
{
    ভিন্ন scale = 1./(batch * spatial);
    পূর্ণাঙ্ক i,j,k;
    বাবে(i = 0; i < filters; ++i){
        mean[i] = 0;
        বাবে(j = 0; j < batch; ++j){
            বাবে(k = 0; k < spatial; ++k){
                পূর্ণাঙ্ক index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

শূন্য variance_cpu(ভিন্ন *x, ভিন্ন *mean, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial, ভিন্ন *variance)
{
    ভিন্ন scale = 1./(batch * spatial - 1);
    পূর্ণাঙ্ক i,j,k;
    বাবে(i = 0; i < filters; ++i){
        variance[i] = 0;
        বাবে(j = 0; j < batch; ++j){
            বাবে(k = 0; k < spatial; ++k){
                পূর্ণাঙ্ক index = j*filters*spatial + i*spatial + k;
                variance[i] += pow((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

শূন্য l2normalize_cpu(ভিন্ন *x, ভিন্ন *dx, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial)
{
    পূর্ণাঙ্ক b,f,i;
    বাবে(b = 0; b < batch; ++b){
        বাবে(i = 0; i < spatial; ++i){
            ভিন্ন sum = 0;
            বাবে(f = 0; f < filters; ++f){
                পূর্ণাঙ্ক index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            বাবে(f = 0; f < filters; ++f){
                পূর্ণাঙ্ক index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


শূন্য normalize_cpu(ভিন্ন *x, ভিন্ন *mean, ভিন্ন *variance, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক filters, পূর্ণাঙ্ক spatial)
{
    পূর্ণাঙ্ক b, f, i;
    বাবে(b = 0; b < batch; ++b){
        বাবে(f = 0; f < filters; ++f){
            বাবে(i = 0; i < spatial; ++i){
                পূর্ণাঙ্ক index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(sqrt(variance[f]) + .000001f);
            }
        }
    }
}

শূন্য const_cpu(পূর্ণাঙ্ক N, ভিন্ন ALPHA, ভিন্ন *X, পূর্ণাঙ্ক INCX)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

শূন্য mul_cpu(পূর্ণাঙ্ক N, ভিন্ন *X, পূর্ণাঙ্ক INCX, ভিন্ন *Y, পূর্ণাঙ্ক INCY)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

শূন্য pow_cpu(পূর্ণাঙ্ক N, ভিন্ন ALPHA, ভিন্ন *X, পূর্ণাঙ্ক INCX, ভিন্ন *Y, পূর্ণাঙ্ক INCY)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < N; ++i) Y[i*INCY] = pow(X[i*INCX], ALPHA);
}

শূন্য axpy_cpu(পূর্ণাঙ্ক N, ভিন্ন ALPHA, ভিন্ন *X, পূর্ণাঙ্ক INCX, ভিন্ন *Y, পূর্ণাঙ্ক INCY)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

শূন্য scal_cpu(পূর্ণাঙ্ক N, ভিন্ন ALPHA, ভিন্ন *X, পূর্ণাঙ্ক INCX)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

শূন্য fill_cpu(পূর্ণাঙ্ক N, ভিন্ন ALPHA, ভিন্ন *X, পূর্ণাঙ্ক INCX)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

শূন্য deinter_cpu(পূর্ণাঙ্ক NX, ভিন্ন *X, পূর্ণাঙ্ক NY, ভিন্ন *Y, পূর্ণাঙ্ক B, ভিন্ন *OUT)
{
    পূর্ণাঙ্ক i, j;
    পূর্ণাঙ্ক index = 0;
    বাবে(j = 0; j < B; ++j) {
        বাবে(i = 0; i < NX; ++i){
            যদি(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        বাবে(i = 0; i < NY; ++i){
            যদি(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

শূন্য inter_cpu(পূর্ণাঙ্ক NX, ভিন্ন *X, পূর্ণাঙ্ক NY, ভিন্ন *Y, পূর্ণাঙ্ক B, ভিন্ন *OUT)
{
    পূর্ণাঙ্ক i, j;
    পূর্ণাঙ্ক index = 0;
    বাবে(j = 0; j < B; ++j) {
        বাবে(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        বাবে(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

শূন্য copy_cpu(পূর্ণাঙ্ক N, ভিন্ন *X, পূর্ণাঙ্ক INCX, ভিন্ন *Y, পূর্ণাঙ্ক INCY)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

শূন্য mult_add_into_cpu(পূর্ণাঙ্ক N, ভিন্ন *X, ভিন্ন *Y, ভিন্ন *Z)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

শূন্য smooth_l1_cpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        ভিন্ন diff = truth[i] - pred[i];
        ভিন্ন abs_val = fabs(diff);
        যদি(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        অন্যথা {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

শূন্য l1_cpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        ভিন্ন diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

শূন্য softmax_x_ent_cpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        ভিন্ন t = truth[i];
        ভিন্ন p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

শূন্য logistic_x_ent_cpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        ভিন্ন t = truth[i];
        ভিন্ন p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

শূন্য l2_cpu(পূর্ণাঙ্ক n, ভিন্ন *pred, ভিন্ন *truth, ভিন্ন *delta, ভিন্ন *error)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        ভিন্ন diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

ভিন্ন dot_cpu(পূর্ণাঙ্ক N, ভিন্ন *X, পূর্ণাঙ্ক INCX, ভিন্ন *Y, পূর্ণাঙ্ক INCY)
{
    পূর্ণাঙ্ক i;
    ভিন্ন dot = 0;
    বাবে(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    ঘূৰাই dot;
}

শূন্য softmax(ভিন্ন *প্ৰবেশ, পূর্ণাঙ্ক n, ভিন্ন temp, পূর্ণাঙ্ক stride, ভিন্ন *output)
{
    পূর্ণাঙ্ক i;
    ভিন্ন sum = 0;
    ভিন্ন largest = -FLT_MAX;
    বাবে(i = 0; i < n; ++i){
        যদি(প্ৰবেশ[i*stride] > largest) largest = প্ৰবেশ[i*stride];
    }
    বাবে(i = 0; i < n; ++i){
        ভিন্ন e = exp(প্ৰবেশ[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    বাবে(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


শূন্য softmax_cpu(ভিন্ন *প্ৰবেশ, পূর্ণাঙ্ক n, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক batch_offset, পূর্ণাঙ্ক groups, পূর্ণাঙ্ক group_offset, পূর্ণাঙ্ক stride, ভিন্ন temp, ভিন্ন *output)
{
    পূর্ণাঙ্ক g, b;
    বাবে(b = 0; b < batch; ++b){
        বাবে(g = 0; g < groups; ++g){
            softmax(প্ৰবেশ + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

শূন্য upsample_cpu(ভিন্ন *in, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক forward, ভিন্ন scale, ভিন্ন *out)
{
    পূর্ণাঙ্ক i, j, k, b;
    বাবে(b = 0; b < batch; ++b){
        বাবে(k = 0; k < c; ++k){
            বাবে(j = 0; j < h*stride; ++j){
                বাবে(i = 0; i < w*stride; ++i){
                    পূর্ণাঙ্ক in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    পূর্ণাঙ্ক out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    যদি(forward) out[out_index] = scale*in[in_index];
                    অন্যথা in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


