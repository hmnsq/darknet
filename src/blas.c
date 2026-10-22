#अन्तर्भाव "blas.h"

#अन्तर्भाव <math.h>
#अन्तर्भाव <assert.h>
#अन्तर्भाव <float.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
शून्य reorg_cpu(दशांश *x, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक batch, पूर्णांक stride, पूर्णांक forward, दशांश *out)
{
    पूर्णांक b,i,j,k;
    पूर्णांक out_c = c/(stride*stride);

    साठी(b = 0; b < batch; ++b){
        साठी(k = 0; k < c; ++k){
            साठी(j = 0; j < h; ++j){
                साठी(i = 0; i < w; ++i){
                    पूर्णांक in_index  = i + w*(j + h*(k + c*b));
                    पूर्णांक c2 = k % out_c;
                    पूर्णांक offset = k / out_c;
                    पूर्णांक w2 = i*stride + offset % stride;
                    पूर्णांक h2 = j*stride + offset / stride;
                    पूर्णांक out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    जर(forward) out[out_index] = x[in_index];
                    अन्यथा out[in_index] = x[out_index];
                }
            }
        }
    }
}

शून्य flatten(दशांश *x, पूर्णांक size, पूर्णांक layers, पूर्णांक batch, पूर्णांक forward)
{
    दशांश *swap = सुस्मृति(size*layers*batch, आकार(दशांश));
    पूर्णांक i,c,b;
    साठी(b = 0; b < batch; ++b){
        साठी(c = 0; c < layers; ++c){
            साठी(i = 0; i < size; ++i){
                पूर्णांक i1 = b*layers*size + c*size + i;
                पूर्णांक i2 = b*layers*size + i*layers + c;
                जर (forward) swap[i2] = x[i1];
                अन्यथा swap[i1] = x[i2];
            }
        }
    }
    स_नकल(x, swap, size*layers*batch*आकार(दशांश));
    मुक्त(swap);
}

शून्य weighted_sum_cpu(दशांश *a, दशांश *b, दशांश *s, पूर्णांक n, दशांश *c)
{
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

शून्य weighted_delta_cpu(दशांश *a, दशांश *b, दशांश *s, दशांश *da, दशांश *db, दशांश *ds, पूर्णांक n, दशांश *dc)
{
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        जर(da) da[i] += dc[i] * s[i];
        जर(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

शून्य shortcut_cpu(पूर्णांक batch, पूर्णांक w1, पूर्णांक h1, पूर्णांक c1, दशांश *add, पूर्णांक w2, पूर्णांक h2, पूर्णांक c2, दशांश s1, दशांश s2, दशांश *out)
{
    पूर्णांक stride = w1/w2;
    पूर्णांक sample = w2/w1;
    प्रतिज्ञा(stride == h1/h2);
    प्रतिज्ञा(sample == h2/h1);
    जर(stride < 1) stride = 1;
    जर(sample < 1) sample = 1;
    पूर्णांक minw = (w1 < w2) ? w1 : w2;
    पूर्णांक minh = (h1 < h2) ? h1 : h2;
    पूर्णांक minc = (c1 < c2) ? c1 : c2;

    पूर्णांक i,j,k,b;
    साठी(b = 0; b < batch; ++b){
        साठी(k = 0; k < minc; ++k){
            साठी(j = 0; j < minh; ++j){
                साठी(i = 0; i < minw; ++i){
                    पूर्णांक out_index = i*sample + w2*(j*sample + h2*(k + c2*b));
                    पूर्णांक add_index = i*stride + w1*(j*stride + h1*(k + c1*b));
                    out[out_index] = s1*out[out_index] + s2*add[add_index];
                }
            }
        }
    }
}

शून्य mean_cpu(दशांश *x, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, दशांश *mean)
{
    दशांश scale = 1./(batch * spatial);
    पूर्णांक i,j,k;
    साठी(i = 0; i < filters; ++i){
        mean[i] = 0;
        साठी(j = 0; j < batch; ++j){
            साठी(k = 0; k < spatial; ++k){
                पूर्णांक index = j*filters*spatial + i*spatial + k;
                mean[i] += x[index];
            }
        }
        mean[i] *= scale;
    }
}

शून्य variance_cpu(दशांश *x, दशांश *mean, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial, दशांश *variance)
{
    दशांश scale = 1./(batch * spatial - 1);
    पूर्णांक i,j,k;
    साठी(i = 0; i < filters; ++i){
        variance[i] = 0;
        साठी(j = 0; j < batch; ++j){
            साठी(k = 0; k < spatial; ++k){
                पूर्णांक index = j*filters*spatial + i*spatial + k;
                variance[i] += घात((x[index] - mean[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

शून्य l2normalize_cpu(दशांश *x, दशांश *dx, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial)
{
    पूर्णांक b,f,i;
    साठी(b = 0; b < batch; ++b){
        साठी(i = 0; i < spatial; ++i){
            दशांश sum = 0;
            साठी(f = 0; f < filters; ++f){
                पूर्णांक index = b*filters*spatial + f*spatial + i;
                sum += powf(x[index], 2);
            }
            sum = sqrtf(sum);
            साठी(f = 0; f < filters; ++f){
                पूर्णांक index = b*filters*spatial + f*spatial + i;
                x[index] /= sum;
                dx[index] = (1 - x[index]) / sum;
            }
        }
    }
}


शून्य normalize_cpu(दशांश *x, दशांश *mean, दशांश *variance, पूर्णांक batch, पूर्णांक filters, पूर्णांक spatial)
{
    पूर्णांक b, f, i;
    साठी(b = 0; b < batch; ++b){
        साठी(f = 0; f < filters; ++f){
            साठी(i = 0; i < spatial; ++i){
                पूर्णांक index = b*filters*spatial + f*spatial + i;
                x[index] = (x[index] - mean[f])/(वर्ग_मूल(variance[f]) + .000001f);
            }
        }
    }
}

शून्य const_cpu(पूर्णांक N, दशांश ALPHA, दशांश *X, पूर्णांक INCX)
{
    पूर्णांक i;
    साठी(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

शून्य mul_cpu(पूर्णांक N, दशांश *X, पूर्णांक INCX, दशांश *Y, पूर्णांक INCY)
{
    पूर्णांक i;
    साठी(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

शून्य pow_cpu(पूर्णांक N, दशांश ALPHA, दशांश *X, पूर्णांक INCX, दशांश *Y, पूर्णांक INCY)
{
    पूर्णांक i;
    साठी(i = 0; i < N; ++i) Y[i*INCY] = घात(X[i*INCX], ALPHA);
}

शून्य axpy_cpu(पूर्णांक N, दशांश ALPHA, दशांश *X, पूर्णांक INCX, दशांश *Y, पूर्णांक INCY)
{
    पूर्णांक i;
    साठी(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

शून्य scal_cpu(पूर्णांक N, दशांश ALPHA, दशांश *X, पूर्णांक INCX)
{
    पूर्णांक i;
    साठी(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

शून्य fill_cpu(पूर्णांक N, दशांश ALPHA, दशांश *X, पूर्णांक INCX)
{
    पूर्णांक i;
    साठी(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

शून्य deinter_cpu(पूर्णांक NX, दशांश *X, पूर्णांक NY, दशांश *Y, पूर्णांक B, दशांश *OUT)
{
    पूर्णांक i, j;
    पूर्णांक index = 0;
    साठी(j = 0; j < B; ++j) {
        साठी(i = 0; i < NX; ++i){
            जर(X) X[j*NX + i] += OUT[index];
            ++index;
        }
        साठी(i = 0; i < NY; ++i){
            जर(Y) Y[j*NY + i] += OUT[index];
            ++index;
        }
    }
}

शून्य inter_cpu(पूर्णांक NX, दशांश *X, पूर्णांक NY, दशांश *Y, पूर्णांक B, दशांश *OUT)
{
    पूर्णांक i, j;
    पूर्णांक index = 0;
    साठी(j = 0; j < B; ++j) {
        साठी(i = 0; i < NX; ++i){
            OUT[index++] = X[j*NX + i];
        }
        साठी(i = 0; i < NY; ++i){
            OUT[index++] = Y[j*NY + i];
        }
    }
}

शून्य copy_cpu(पूर्णांक N, दशांश *X, पूर्णांक INCX, दशांश *Y, पूर्णांक INCY)
{
    पूर्णांक i;
    साठी(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

शून्य mult_add_into_cpu(पूर्णांक N, दशांश *X, दशांश *Y, दशांश *Z)
{
    पूर्णांक i;
    साठी(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

शून्य smooth_l1_cpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error)
{
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        दशांश diff = truth[i] - pred[i];
        दशांश abs_val = भ_असल(diff);
        जर(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        अन्यथा {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

शून्य l1_cpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error)
{
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        दशांश diff = truth[i] - pred[i];
        error[i] = भ_असल(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

शून्य softmax_x_ent_cpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error)
{
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        दशांश t = truth[i];
        दशांश p = pred[i];
        error[i] = (t) ? -log(p) : 0;
        delta[i] = t-p;
    }
}

शून्य logistic_x_ent_cpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error)
{
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        दशांश t = truth[i];
        दशांश p = pred[i];
        error[i] = -t*log(p) - (1-t)*log(1-p);
        delta[i] = t-p;
    }
}

शून्य l2_cpu(पूर्णांक n, दशांश *pred, दशांश *truth, दशांश *delta, दशांश *error)
{
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        दशांश diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

दशांश dot_cpu(पूर्णांक N, दशांश *X, पूर्णांक INCX, दशांश *Y, पूर्णांक INCY)
{
    पूर्णांक i;
    दशांश dot = 0;
    साठी(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    परत dot;
}

शून्य softmax(दशांश *आदान, पूर्णांक n, दशांश temp, पूर्णांक stride, दशांश *output)
{
    पूर्णांक i;
    दशांश sum = 0;
    दशांश largest = -भग्न_उच्च;
    साठी(i = 0; i < n; ++i){
        जर(आदान[i*stride] > largest) largest = आदान[i*stride];
    }
    साठी(i = 0; i < n; ++i){
        दशांश e = exp(आदान[i*stride]/temp - largest/temp);
        sum += e;
        output[i*stride] = e;
    }
    साठी(i = 0; i < n; ++i){
        output[i*stride] /= sum;
    }
}


शून्य softmax_cpu(दशांश *आदान, पूर्णांक n, पूर्णांक batch, पूर्णांक batch_offset, पूर्णांक groups, पूर्णांक group_offset, पूर्णांक stride, दशांश temp, दशांश *output)
{
    पूर्णांक g, b;
    साठी(b = 0; b < batch; ++b){
        साठी(g = 0; g < groups; ++g){
            softmax(आदान + b*batch_offset + g*group_offset, n, temp, stride, output + b*batch_offset + g*group_offset);
        }
    }
}

शून्य upsample_cpu(दशांश *in, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक batch, पूर्णांक stride, पूर्णांक forward, दशांश scale, दशांश *out)
{
    पूर्णांक i, j, k, b;
    साठी(b = 0; b < batch; ++b){
        साठी(k = 0; k < c; ++k){
            साठी(j = 0; j < h*stride; ++j){
                साठी(i = 0; i < w*stride; ++i){
                    पूर्णांक in_index = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    पूर्णांक out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    जर(forward) out[out_index] = scale*in[in_index];
                    अन्यथा in[in_index] += scale*out[out_index];
                }
            }
        }
    }
}


