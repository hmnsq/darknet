#ಅನ್ತರ್ಭಾವ "blas.h"

#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <float.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
ಶೂನ್ಯ reorg_cpu(ದಶಮಾಂಶ *x, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ stride, ಪೂರ್ಣಾಂಕ forward, ದಶಮಾಂಶ *ಹೊರಹೋಗು)
{
    ಪೂರ್ಣಾಂಕ b,i,j,k;
    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_c = c/(stride*stride);

    ಗಾಗಿ(b = 0; b < batch; ++b){
        ಗಾಗಿ(k = 0; k < c; ++k){
            ಗಾಗಿ(j = 0; j < h; ++j){
                ಗಾಗಿ(i = 0; i < w; ++i){
                    ಪೂರ್ಣಾಂಕ ಒಳ_ಸೂಚ್ಯಂಕ  = i + w*(j + h*(k + c*b));
                    ಪೂರ್ಣಾಂಕ c2 = k % ಹೊರಹೋಗು_c;
                    ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = k / ಹೊರಹೋಗು_c;
                    ಪೂರ್ಣಾಂಕ w2 = i*stride + ಸ್ಥಾನಾಂತರ % stride;
                    ಪೂರ್ಣಾಂಕ h2 = j*stride + ಸ್ಥಾನಾಂತರ / stride;
                    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ = w2 + w*stride*(h2 + h*stride*(c2 + ಹೊರಹೋಗು_c*b));
                    ಆದರೆ(forward) ಹೊರಹೋಗು[ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ] = x[ಒಳ_ಸೂಚ್ಯಂಕ];
                    ಇಲ್ಲದಿದ್ದರೆ ಹೊರಹೋಗು[ಒಳ_ಸೂಚ್ಯಂಕ] = x[ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ];
                }
            }
        }
    }
}

ಶೂನ್ಯ flatten(ದಶಮಾಂಶ *x, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ layers, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ forward)
{
    ದಶಮಾಂಶ *ಅದಲುಬದಲು = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ_2*layers*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಪೂರ್ಣಾಂಕ i,c,b;
    ಗಾಗಿ(b = 0; b < batch; ++b){
        ಗಾಗಿ(c = 0; c < layers; ++c){
            ಗಾಗಿ(i = 0; i < ಗಾತ್ರ_2; ++i){
                ಪೂರ್ಣಾಂಕ i1 = b*layers*ಗಾತ್ರ_2 + c*ಗಾತ್ರ_2 + i;
                ಪೂರ್ಣಾಂಕ i2 = b*layers*ಗಾತ್ರ_2 + i*layers + c;
                ಆದರೆ (forward) ಅದಲುಬದಲು[i2] = x[i1];
                ಇಲ್ಲದಿದ್ದರೆ ಅದಲುಬದಲು[i1] = x[i2];
            }
        }
    }
    ಸ್ಮರಣೆ_ನಕಲು(x, ಅದಲುಬದಲು, ಗಾತ್ರ_2*layers*batch*ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಬಿಡುಗಡೆ(ಅದಲುಬದಲು);
}

ಶೂನ್ಯ weighted_ಮೊತ್ತ_cpu(ದಶಮಾಂಶ *a, ದಶಮಾಂಶ *b, ದಶಮಾಂಶ *s, ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ *c)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        c[i] = s[i]*a[i] + (1-s[i])*(b ? b[i] : 0);
    }
}

ಶೂನ್ಯ weighted_delta_cpu(ದಶಮಾಂಶ *a, ದಶಮಾಂಶ *b, ದಶಮಾಂಶ *s, ದಶಮಾಂಶ *da, ದಶಮಾಂಶ *db, ದಶಮಾಂಶ *ds, ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ *dc)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ಆದರೆ(da) da[i] += dc[i] * s[i];
        ಆದರೆ(db) db[i] += dc[i] * (1-s[i]);
        ds[i] += dc[i] * (a[i] - b[i]);
    }
}

ಶೂನ್ಯ shortcut_cpu(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ w1, ಪೂರ್ಣಾಂಕ h1, ಪೂರ್ಣಾಂಕ c1, ದಶಮಾಂಶ *ಕೂಡಿಸು, ಪೂರ್ಣಾಂಕ w2, ಪೂರ್ಣಾಂಕ h2, ಪೂರ್ಣಾಂಕ c2, ದಶಮಾಂಶ s1, ದಶಮಾಂಶ s2, ದಶಮಾಂಶ *ಹೊರಹೋಗು)
{
    ಪೂರ್ಣಾಂಕ stride = w1/w2;
    ಪೂರ್ಣಾಂಕ sample = w2/w1;
    ಪ್ರತಿಜ್ಞಾ(stride == h1/h2);
    ಪ್ರತಿಜ್ಞಾ(sample == h2/h1);
    ಆದರೆ(stride < 1) stride = 1;
    ಆದರೆ(sample < 1) sample = 1;
    ಪೂರ್ಣಾಂಕ minw = (w1 < w2) ? w1 : w2;
    ಪೂರ್ಣಾಂಕ minh = (h1 < h2) ? h1 : h2;
    ಪೂರ್ಣಾಂಕ minc = (c1 < c2) ? c1 : c2;

    ಪೂರ್ಣಾಂಕ i,j,k,b;
    ಗಾಗಿ(b = 0; b < batch; ++b){
        ಗಾಗಿ(k = 0; k < minc; ++k){
            ಗಾಗಿ(j = 0; j < minh; ++j){
                ಗಾಗಿ(i = 0; i < minw; ++i){
                    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ = i*sample + w2*(j*sample + h2*(k + c2*b));
                    ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_ಸೂಚ್ಯಂಕ = i*stride + w1*(j*stride + h1*(k + c1*b));
                    ಹೊರಹೋಗು[ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ] = s1*ಹೊರಹೋಗು[ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ] + s2*ಕೂಡಿಸು[ಕೂಡಿಸು_ಸೂಚ್ಯಂಕ];
                }
            }
        }
    }
}

ಶೂನ್ಯ ಸರಾಸರಿ_cpu(ದಶಮಾಂಶ *x, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ filters, ಪೂರ್ಣಾಂಕ spatial, ದಶಮಾಂಶ *ಸರಾಸರಿ)
{
    ದಶಮಾಂಶ scale = 1./(batch * spatial);
    ಪೂರ್ಣಾಂಕ i,j,k;
    ಗಾಗಿ(i = 0; i < filters; ++i){
        ಸರಾಸರಿ[i] = 0;
        ಗಾಗಿ(j = 0; j < batch; ++j){
            ಗಾಗಿ(k = 0; k < spatial; ++k){
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = j*filters*spatial + i*spatial + k;
                ಸರಾಸರಿ[i] += x[ಸೂಚ್ಯಂಕ];
            }
        }
        ಸರಾಸರಿ[i] *= scale;
    }
}

ಶೂನ್ಯ variance_cpu(ದಶಮಾಂಶ *x, ದಶಮಾಂಶ *ಸರಾಸರಿ, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ filters, ಪೂರ್ಣಾಂಕ spatial, ದಶಮಾಂಶ *variance)
{
    ದಶಮಾಂಶ scale = 1./(batch * spatial - 1);
    ಪೂರ್ಣಾಂಕ i,j,k;
    ಗಾಗಿ(i = 0; i < filters; ++i){
        variance[i] = 0;
        ಗಾಗಿ(j = 0; j < batch; ++j){
            ಗಾಗಿ(k = 0; k < spatial; ++k){
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = j*filters*spatial + i*spatial + k;
                variance[i] += ಘಾತ((x[ಸೂಚ್ಯಂಕ] - ಸರಾಸರಿ[i]), 2);
            }
        }
        variance[i] *= scale;
    }
}

ಶೂನ್ಯ l2normalize_cpu(ದಶಮಾಂಶ *x, ದಶಮಾಂಶ *dx, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ filters, ಪೂರ್ಣಾಂಕ spatial)
{
    ಪೂರ್ಣಾಂಕ b,f,i;
    ಗಾಗಿ(b = 0; b < batch; ++b){
        ಗಾಗಿ(i = 0; i < spatial; ++i){
            ದಶಮಾಂಶ ಮೊತ್ತ = 0;
            ಗಾಗಿ(f = 0; f < filters; ++f){
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = b*filters*spatial + f*spatial + i;
                ಮೊತ್ತ += powf(x[ಸೂಚ್ಯಂಕ], 2);
            }
            ಮೊತ್ತ = sqrtf(ಮೊತ್ತ);
            ಗಾಗಿ(f = 0; f < filters; ++f){
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = b*filters*spatial + f*spatial + i;
                x[ಸೂಚ್ಯಂಕ] /= ಮೊತ್ತ;
                dx[ಸೂಚ್ಯಂಕ] = (1 - x[ಸೂಚ್ಯಂಕ]) / ಮೊತ್ತ;
            }
        }
    }
}


ಶೂನ್ಯ normalize_cpu(ದಶಮಾಂಶ *x, ದಶಮಾಂಶ *ಸರಾಸರಿ, ದಶಮಾಂಶ *variance, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ filters, ಪೂರ್ಣಾಂಕ spatial)
{
    ಪೂರ್ಣಾಂಕ b, f, i;
    ಗಾಗಿ(b = 0; b < batch; ++b){
        ಗಾಗಿ(f = 0; f < filters; ++f){
            ಗಾಗಿ(i = 0; i < spatial; ++i){
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = b*filters*spatial + f*spatial + i;
                x[ಸೂಚ್ಯಂಕ] = (x[ಸೂಚ್ಯಂಕ] - ಸರಾಸರಿ[f])/(ವರ್ಗಮೂಲ(variance[f]) + .000001f);
            }
        }
    }
}

ಶೂನ್ಯ const_cpu(ಪೂರ್ಣಾಂಕ N, ದಶಮಾಂಶ ALPHA, ದಶಮಾಂಶ *X, ಪೂರ್ಣಾಂಕ INCX)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

ಶೂನ್ಯ mul_cpu(ಪೂರ್ಣಾಂಕ N, ದಶಮಾಂಶ *X, ಪೂರ್ಣಾಂಕ INCX, ದಶಮಾಂಶ *Y, ಪೂರ್ಣಾಂಕ INCY)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < N; ++i) Y[i*INCY] *= X[i*INCX];
}

ಶೂನ್ಯ pow_cpu(ಪೂರ್ಣಾಂಕ N, ದಶಮಾಂಶ ALPHA, ದಶಮಾಂಶ *X, ಪೂರ್ಣಾಂಕ INCX, ದಶಮಾಂಶ *Y, ಪೂರ್ಣಾಂಕ INCY)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < N; ++i) Y[i*INCY] = ಘಾತ(X[i*INCX], ALPHA);
}

ಶೂನ್ಯ axpy_cpu(ಪೂರ್ಣಾಂಕ N, ದಶಮಾಂಶ ALPHA, ದಶಮಾಂಶ *X, ಪೂರ್ಣಾಂಕ INCX, ದಶಮಾಂಶ *Y, ಪೂರ್ಣಾಂಕ INCY)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < N; ++i) Y[i*INCY] += ALPHA*X[i*INCX];
}

ಶೂನ್ಯ scal_cpu(ಪೂರ್ಣಾಂಕ N, ದಶಮಾಂಶ ALPHA, ದಶಮಾಂಶ *X, ಪೂರ್ಣಾಂಕ INCX)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < N; ++i) X[i*INCX] *= ALPHA;
}

ಶೂನ್ಯ fill_cpu(ಪೂರ್ಣಾಂಕ N, ದಶಮಾಂಶ ALPHA, ದಶಮಾಂಶ *X, ಪೂರ್ಣಾಂಕ INCX)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < N; ++i) X[i*INCX] = ALPHA;
}

ಶೂನ್ಯ deinter_cpu(ಪೂರ್ಣಾಂಕ NX, ದಶಮಾಂಶ *X, ಪೂರ್ಣಾಂಕ NY, ದಶಮಾಂಶ *Y, ಪೂರ್ಣಾಂಕ B, ದಶಮಾಂಶ *ಹೊರಹೋಗು_2)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = 0;
    ಗಾಗಿ(j = 0; j < B; ++j) {
        ಗಾಗಿ(i = 0; i < NX; ++i){
            ಆದರೆ(X) X[j*NX + i] += ಹೊರಹೋಗು_2[ಸೂಚ್ಯಂಕ];
            ++ಸೂಚ್ಯಂಕ;
        }
        ಗಾಗಿ(i = 0; i < NY; ++i){
            ಆದರೆ(Y) Y[j*NY + i] += ಹೊರಹೋಗು_2[ಸೂಚ್ಯಂಕ];
            ++ಸೂಚ್ಯಂಕ;
        }
    }
}

ಶೂನ್ಯ inter_cpu(ಪೂರ್ಣಾಂಕ NX, ದಶಮಾಂಶ *X, ಪೂರ್ಣಾಂಕ NY, ದಶಮಾಂಶ *Y, ಪೂರ್ಣಾಂಕ B, ದಶಮಾಂಶ *ಹೊರಹೋಗು_2)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = 0;
    ಗಾಗಿ(j = 0; j < B; ++j) {
        ಗಾಗಿ(i = 0; i < NX; ++i){
            ಹೊರಹೋಗು_2[ಸೂಚ್ಯಂಕ++] = X[j*NX + i];
        }
        ಗಾಗಿ(i = 0; i < NY; ++i){
            ಹೊರಹೋಗು_2[ಸೂಚ್ಯಂಕ++] = Y[j*NY + i];
        }
    }
}

ಶೂನ್ಯ copy_cpu(ಪೂರ್ಣಾಂಕ N, ದಶಮಾಂಶ *X, ಪೂರ್ಣಾಂಕ INCX, ದಶಮಾಂಶ *Y, ಪೂರ್ಣಾಂಕ INCY)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < N; ++i) Y[i*INCY] = X[i*INCX];
}

ಶೂನ್ಯ mult_ಕೂಡಿಸು_into_cpu(ಪೂರ್ಣಾಂಕ N, ದಶಮಾಂಶ *X, ದಶಮಾಂಶ *Y, ದಶಮಾಂಶ *Z)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < N; ++i) Z[i] += X[i]*Y[i];
}

ಶೂನ್ಯ smooth_l1_cpu(ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ *pred, ದಶಮಾಂಶ *truth, ದಶಮಾಂಶ *delta, ದಶಮಾಂಶ *error)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ದಶಮಾಂಶ diff = truth[i] - pred[i];
        ದಶಮಾಂಶ abs_val = fabs(diff);
        ಆದರೆ(abs_val < 1) {
            error[i] = diff * diff;
            delta[i] = diff;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            error[i] = 2*abs_val - 1;
            delta[i] = (diff < 0) ? 1 : -1;
        }
    }
}

ಶೂನ್ಯ l1_cpu(ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ *pred, ದಶಮಾಂಶ *truth, ದಶಮಾಂಶ *delta, ದಶಮಾಂಶ *error)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ದಶಮಾಂಶ diff = truth[i] - pred[i];
        error[i] = fabs(diff);
        delta[i] = diff > 0 ? 1 : -1;
    }
}

ಶೂನ್ಯ softmax_x_ent_cpu(ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ *pred, ದಶಮಾಂಶ *truth, ದಶಮಾಂಶ *delta, ದಶಮಾಂಶ *error)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ದಶಮಾಂಶ t = truth[i];
        ದಶಮಾಂಶ p = pred[i];
        error[i] = (t) ? -ಲಘುಗಣಕ(p) : 0;
        delta[i] = t-p;
    }
}

ಶೂನ್ಯ logistic_x_ent_cpu(ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ *pred, ದಶಮಾಂಶ *truth, ದಶಮಾಂಶ *delta, ದಶಮಾಂಶ *error)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ದಶಮಾಂಶ t = truth[i];
        ದಶಮಾಂಶ p = pred[i];
        error[i] = -t*ಲಘುಗಣಕ(p) - (1-t)*ಲಘುಗಣಕ(1-p);
        delta[i] = t-p;
    }
}

ಶೂನ್ಯ l2_cpu(ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ *pred, ದಶಮಾಂಶ *truth, ದಶಮಾಂಶ *delta, ದಶಮಾಂಶ *error)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ದಶಮಾಂಶ diff = truth[i] - pred[i];
        error[i] = diff * diff;
        delta[i] = diff;
    }
}

ದಶಮಾಂಶ dot_cpu(ಪೂರ್ಣಾಂಕ N, ದಶಮಾಂಶ *X, ಪೂರ್ಣಾಂಕ INCX, ದಶಮಾಂಶ *Y, ಪೂರ್ಣಾಂಕ INCY)
{
    ಪೂರ್ಣಾಂಕ i;
    ದಶಮಾಂಶ dot = 0;
    ಗಾಗಿ(i = 0; i < N; ++i) dot += X[i*INCX] * Y[i*INCY];
    ಹಿಂತಿರುಗಿ dot;
}

ಶೂನ್ಯ softmax(ದಶಮಾಂಶ *ಒಳಹರಿವು, ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ ತಾತ್ಕಾಲಿಕ, ಪೂರ್ಣಾಂಕ stride, ದಶಮಾಂಶ *output)
{
    ಪೂರ್ಣಾಂಕ i;
    ದಶಮಾಂಶ ಮೊತ್ತ = 0;
    ದಶಮಾಂಶ ಅತಿದೊಡ್ಡ = -FLT_MAX;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ಆದರೆ(ಒಳಹರಿವು[i*stride] > ಅತಿದೊಡ್ಡ) ಅತಿದೊಡ್ಡ = ಒಳಹರಿವು[i*stride];
    }
    ಗಾಗಿ(i = 0; i < n; ++i){
        ದಶಮಾಂಶ e = ಘಾತಾಂಕ(ಒಳಹರಿವು[i*stride]/ತಾತ್ಕಾಲಿಕ - ಅತಿದೊಡ್ಡ/ತಾತ್ಕಾಲಿಕ);
        ಮೊತ್ತ += e;
        output[i*stride] = e;
    }
    ಗಾಗಿ(i = 0; i < n; ++i){
        output[i*stride] /= ಮೊತ್ತ;
    }
}


ಶೂನ್ಯ softmax_cpu(ದಶಮಾಂಶ *ಒಳಹರಿವು, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ batch_ಸ್ಥಾನಾಂತರ, ಪೂರ್ಣಾಂಕ groups, ಪೂರ್ಣಾಂಕ ಗುಂಪು_ಸ್ಥಾನಾಂತರ, ಪೂರ್ಣಾಂಕ stride, ದಶಮಾಂಶ ತಾತ್ಕಾಲಿಕ, ದಶಮಾಂಶ *output)
{
    ಪೂರ್ಣಾಂಕ g, b;
    ಗಾಗಿ(b = 0; b < batch; ++b){
        ಗಾಗಿ(g = 0; g < groups; ++g){
            softmax(ಒಳಹರಿವು + b*batch_ಸ್ಥಾನಾಂತರ + g*ಗುಂಪು_ಸ್ಥಾನಾಂತರ, n, ತಾತ್ಕಾಲಿಕ, stride, output + b*batch_ಸ್ಥಾನಾಂತರ + g*ಗುಂಪು_ಸ್ಥಾನಾಂತರ);
        }
    }
}

ಶೂನ್ಯ upsample_cpu(ದಶಮಾಂಶ *ಒಳ, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ stride, ಪೂರ್ಣಾಂಕ forward, ದಶಮಾಂಶ scale, ದಶಮಾಂಶ *ಹೊರಹೋಗು)
{
    ಪೂರ್ಣಾಂಕ i, j, k, b;
    ಗಾಗಿ(b = 0; b < batch; ++b){
        ಗಾಗಿ(k = 0; k < c; ++k){
            ಗಾಗಿ(j = 0; j < h*stride; ++j){
                ಗಾಗಿ(i = 0; i < w*stride; ++i){
                    ಪೂರ್ಣಾಂಕ ಒಳ_ಸೂಚ್ಯಂಕ = b*w*h*c + k*w*h + (j/stride)*w + i/stride;
                    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
                    ಆದರೆ(forward) ಹೊರಹೋಗು[ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ] = scale*ಒಳ[ಒಳ_ಸೂಚ್ಯಂಕ];
                    ಇಲ್ಲದಿದ್ದರೆ ಒಳ[ಒಳ_ಸೂಚ್ಯಂಕ] += scale*ಹೊರಹೋಗು[ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ];
                }
            }
        }
    }
}


