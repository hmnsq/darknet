#अन्तर्भाव "blas.h"

#अन्तर्भाव <math.h>
#अन्तर्भाव <assert.h>
#अन्तर्भाव <float.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
शून्य reorg_cpu(भिन्न *x, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c, पूर्णाङ्क batch, पूर्णाङ्क stride, पूर्णाङ्क forward, भिन्न *बहिर्गमनम्)
{
    पूर्णाङ्क b,i,j,k;
    पूर्णाङ्क बहिर्गमनम्_c = c/(stride*stride);

    प्रति(b = 0; b < batch; ++b){
        प्रति(k = 0; k < c; ++k){
            प्रति(j = 0; j < h; ++j){
                प्रति(i = 0; i < w; ++i){
                    पूर्णाङ्क अन्तर्_सूचकांक  = i + w*(j + h*(k + c*b));
                    पूर्णाङ्क c2 = k % बहिर्गमनम्_c;
                    पूर्णाङ्क विस्थापनम् = k / बहिर्गमनम्_c;
                    पूर्णाङ्क w2 = i*stride + विस्थापनम् % stride;
                    पूर्णाङ्क h2 = j*stride + विस्थापनम् / stride;
                    पूर्णाङ्क बहिर्गमनम्_सूचकांक = w2 + w*stride*(h2 + h*stride*(c2 + बहिर्गमनम्_c*b));
                    यदि(forward) बहिर्गमनम्[बहिर्गमनम्_सूचकांक] = x[अन्तर्_सूचकांक];
                    अन्यथा बहिर्गमनम्[अन्तर्_सूचकांक] = x[बहिर्गमनम्_सूचकांक];
                }
            }
        }
    }
}

शून्य flatten(भिन्न *x, पूर्णाङ्क आकार, पूर्णाङ्क layers, पूर्णाङ्क batch, पूर्णाङ्क forward)
{
    भिन्न *swap = शून्यस्मृतिं_देहि(आकार*layers*batch, परिमाण(भिन्न));
    पूर्णाङ्क i,c,b;
    प्रति(b = 0; b < batch; ++b){
        प्रति(c = 0; c < layers; ++c){
            प्रति(i = 0; i < आकार; ++i){
                पूर्णाङ्क i1 = b*layers*आकार + c*आकार + i;
                पूर्णाङ्क i2 = b*layers*आकार + i*layers + c;
                यदि (forward) swap[i2] = x[i1];
                अन्यथा swap[i1] = x[i2];
            }
        }
    }
    स्मृतिं_प्रतिलिख(x, swap, आकार*layers*batch*परिमाण(भिन्न));
    मुक्त(swap);
}

शून्य weighted_योग_cpu(भिन्न *a, भिन्न *b, भिन्न *s, पूर्णाङ्क n, भिन्न *c)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

शून्य weighted_delta_cpu(भिन्न *a, भिन्न *b, भिन्न *s, भिन्न *da, भिन्न *db, भिन्न *ds, पूर्णाङ्क n, भिन्न *dc)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        यदि(da) da[i] += dc[i] * s[i];
        यदि(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

शून्य shortcut_cpu(पूर्णाङ्क batch, पूर्णाङ्क w1, पूर्णाङ्क h1, पूर्णाङ्क c1, भिन्न *सङ्कलय, पूर्णाङ्क w2, पूर्णाङ्क h2, पूर्णाङ्क c2, भिन्न s1, भिन्न s2, भिन्न *बहिर्गमनम्)
{
    पूर्णाङ्क stride = w1/w2;
    पूर्णाङ्क sample = w2/w1;
    प्रतिज्ञा(stride == h1/h2);
    प्रतिज्ञा(sample == h2/h1);
    यदि(stride < 1) stride = 1;
    यदि(sample < 1) sample = 1;
    पूर्णाङ्क minw = (w1 < w2) ? w1 : w2;
    पूर्णाङ्क minh = (h1 < h2) ? h1 : h2;
    पूर्णाङ्क minc = (c1 < c2) ? c1 : c2;

    पूर्णाङ्क i,j,k,b;
    प्रति(b = 0; b < batch; ++b){
        प्रति(k = 0; k < minc; ++k){
            प्रति(j = 0; j < minh; ++j){
                प्रति(i = 0; i < minw; ++i){
                    पूर्णाङ्क बहिर्गमनम्_सूचकांक = i*sample + w2*(j*sample + h2*(k + c2*b));
                    पूर्णाङ्क सङ्कलय_सूचकांक = i*stride + w1*(j*stride + h1*(k + c1*b));
                    बहिर्गमनम्[बहिर्गमनम्_सूचकांक] = s1*बहिर्गमनम्[बहिर्गमनम्_सूचकांक] + s2*सङ्कलय[सङ्कलय_सूचकांक];
                }
            }
        }
    }
}

शून्य माध्यम्_cpu(भिन्न *x, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *माध्यम्)
{
    भिन्न scale = 1./(batch * spatial);
    पूर्णाङ्क i,j,k;
    प्रति(i = 0; i < filters; ++i){
        माध्यम्[i] = 0;
        प्रति(j = 0; j < batch; ++j){
            प्रति(k = 0; k < spatial; ++k){
                पूर्णाङ्क सूचकांक = j*filters*spatial + i*spatial + k;
                माध्यम्[i] += x[सूचकांक];
            }
        }
        माध्यम्[i] *= scale;
    }
}

शून्य variance_cpu(भिन्न *x, भिन्न *माध्यम्, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *variance)
{
    भिन्न scale = 1./(batch * spatial - 1);
    पूर्णाङ्क i,j,k;
    प्रति(i = 0; i < filters; ++i){
        variance[i] = 0;
        प्रति(j = 0; j < batch; ++j){
            प्रति(k = 0; k < spatial; ++k){
                पूर्णाङ्क सूचकांक = j*filters*spatial + i*spatial + k;
                variance[i] += घातः((x[सूचकांक] - माध्यम्[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

शून्य l2normalize_cpu(भिन्न *x, भिन्न *dx, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial)
{
    पूर्णाङ्क b,f,i;
    प्रति(b = 0; b < batch; ++b){
        प्रति(i = 0; i < spatial; ++i){
            भिन्न योग = 0;
            प्रति(f = 0; f < filters; ++f){
                पूर्णाङ्क सूचकांक = b*filters*spatial + f*spatial + i;
                योग += powf(x[सूचकांक], 2);
            }
            योग = sqrtf(योग);
            प्रति(f = 0; f < filters; ++f){
                पूर्णाङ्क सूचकांक = b*filters*spatial + f*spatial + i;
                x[सूचकांक] /= योग;
                dx[सूचकांक] = (1 - x[सूचकांक]) / योग;
            }
        }
    }
}


शून्य normalize_cpu(भिन्न *x, भिन्न *माध्यम्, भिन्न *variance, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial)
{
    पूर्णाङ्क b, f, i;
    प्रति(b = 0; b < batch; ++b){
        प्रति(f = 0; f < filters; ++f){
            प्रति(i = 0; i < spatial; ++i){
                पूर्णाङ्क सूचकांक = b*filters*spatial + f*spatial + i;
                x[सूचकांक] = (x[सूचकांक] - माध्यम्[f])/(वर्गमूलम्(variance[f]) + .000001f);
            }
        }
    }
}

शून्य const_cpu(पूर्णाङ्क N, भिन्न ALPHA, भिन्न *X, पूर्णाङ्क INCX)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

शून्य mul_cpu(पूर्णाङ्क N, भिन्न *X, पूर्णाङ्क INCX, भिन्न *Y, पूर्णाङ्क INCY)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

शून्य pow_cpu(पूर्णाङ्क N, भिन्न ALPHA, भिन्न *X, पूर्णाङ्क INCX, भिन्न *Y, पूर्णाङ्क INCY)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < N; ++i) Y[i*INCY] = घातः(X[i*INCX], ALPHA);
}

शून्य axpy_cpu(पूर्णाङ्क N, भिन्न ALPHA, भिन्न *X, पूर्णाङ्क INCX, भिन्न *Y, पूर्णाङ्क INCY)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

शून्य scal_cpu(पूर्णाङ्क N, भिन्न ALPHA, भिन्न *X, पूर्णाङ्क INCX)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

शून्य fill_cpu(पूर्णाङ्क N, भिन्न ALPHA, भिन्न *X, पूर्णाङ्क INCX)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

शून्य deinter_cpu(पूर्णाङ्क NX, भिन्न *X, पूर्णाङ्क NY, भिन्न *Y, पूर्णाङ्क B, भिन्न *बहिर्गमनम्_2)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क सूचकांक = 0;
    प्रति(j = 0; j < B; ++j) {
        प्रति(i = 0; i < NX; ++i){
            यदि(X) X[j*NX + i] += बहिर्गमनम्_2[सूचकांक];
            ++सूचकांक;
        }
        प्रति(i = 0; i < NY; ++i){
            यदि(Y) Y[j*NY + i] += बहिर्गमनम्_2[सूचकांक];
            ++सूचकांक;
        }
    }
}

शून्य inter_cpu(पूर्णाङ्क NX, भिन्न *X, पूर्णाङ्क NY, भिन्न *Y, पूर्णाङ्क B, भिन्न *बहिर्गमनम्_2)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क सूचकांक = 0;
    प्रति(j = 0; j < B; ++j) {
        प्रति(i = 0; i < NX; ++i){
            बहिर्गमनम्_2[सूचकांक++] = X[j*NX + i];
        }
        प्रति(i = 0; i < NY; ++i){
            बहिर्गमनम्_2[सूचकांक++] = Y[j*NY + i];
        }
    }
}

शून्य copy_cpu(पूर्णाङ्क N, भिन्न *X, पूर्णाङ्क INCX, भिन्न *Y, पूर्णाङ्क INCY)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

शून्य mult_सङ्कलय_into_cpu(पूर्णाङ्क N, भिन्न *X, भिन्न *Y, भिन्न *Z)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

शून्य smooth_l1_cpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        भिन्न diff = truth[i] - pred[i];
        भिन्न abs_val = fabs(diff);
        यदि(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        अन्यथा {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

शून्य l1_cpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        भिन्न diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

शून्य softmax_x_ent_cpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        भिन्न t = truth[i];
        भिन्न p = pred[i];
        error[i] = (t) ? -लघुगणकः(p) : 0;
        delta[i] = t-p;
    }
}

शून्य logistic_x_ent_cpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        भिन्न t = truth[i];
        भिन्न p = pred[i];
        error[i] = -t*लघुगणकः(p) - (1-t)*लघुगणकः(1-p);
        delta[i] = t-p;
    }
}

शून्य l2_cpu(पूर्णाङ्क n, भिन्न *pred, भिन्न *truth, भिन्न *delta, भिन्न *error)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        भिन्न diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

भिन्न dot_cpu(पूर्णाङ्क N, भिन्न *X, पूर्णाङ्क INCX, भिन्न *Y, पूर्णाङ्क INCY)
{
    पूर्णाङ्क i;
    भिन्न dot = 0;
    प्रति(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    प्रत्यागम dot;
}

शून्य softmax(भिन्न *निवेशः, पूर्णाङ्क n, भिन्न अस्थायी, पूर्णाङ्क stride, भिन्न *output)
{
    पूर्णाङ्क i;
    भिन्न योग = 0;
    भिन्न largest = -FLT_MAX;
    प्रति(i = 0; i < n; ++i){
        यदि(निवेशः[i*stride] > largest) largest = निवेशः[i*stride];
    }
    प्रति(i = 0; i < n; ++i){
        भिन्न e = घातांकः(निवेशः[i*stride]/अस्थायी - largest/अस्थायी);
        योग += e;
        output[i*stride] = e;
    }
    प्रति(i = 0; i < n; ++i){
        output[i*stride] /= योग;
    }
}


शून्य softmax_cpu(भिन्न *निवेशः, पूर्णाङ्क n, पूर्णाङ्क batch, पूर्णाङ्क batch_विस्थापनम्, पूर्णाङ्क groups, पूर्णाङ्क गणः_विस्थापनम्, पूर्णाङ्क stride, भिन्न अस्थायी, भिन्न *output)
{
    पूर्णाङ्क g, b;
    प्रति(b = 0; b < batch; ++b){
        प्रति(g = 0; g < groups; ++g){
            softmax(निवेशः + b*batch_विस्थापनम् + g*गणः_विस्थापनम्, n, अस्थायी, stride, output + b*batch_विस्थापनम् + g*गणः_विस्थापनम्);
        }
    }
}

शून्य upsample_cpu(भिन्न *अन्तर्, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c, पूर्णाङ्क batch, पूर्णाङ्क stride, पूर्णाङ्क forward, भिन्न scale, भिन्न *बहिर्गमनम्)
{
    पूर्णाङ्क i, j, k, b;
    प्रति(b = 0; b < batch; ++b){
        प्रति(k = 0; k < c; ++k){
            प्रति(j = 0; j < h*stride; ++j){
                प्रति(i = 0; i < w*stride; ++i){
                    पूर्णाङ्क अन्तर्_सूचकांक = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    पूर्णाङ्क बहिर्गमनम्_सूचकांक = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    यदि(forward) बहिर्गमनम्[बहिर्गमनम्_सूचकांक] = scale*अन्तर्[अन्तर्_सूचकांक];
                    अन्यथा अन्तर्[अन्तर्_सूचकांक] += scale*बहिर्गमनम्[बहिर्गमनम्_सूचकांक];
                }
            }
        }
    }
}


