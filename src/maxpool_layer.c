#ಅನ್ತರ್ಭಾವ "maxpool_layer.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    ಪೂರ್ಣಾಂಕ h = l.ಹೊರಹೋಗು_h;
    ಪೂರ್ಣಾಂಕ w = l.ಹೊರಹೋಗು_w;
    ಪೂರ್ಣಾಂಕ c = l.c;
    ಹಿಂತಿರುಗಿ float_ಗಮ್ಯ_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    ಪೂರ್ಣಾಂಕ h = l.ಹೊರಹೋಗು_h;
    ಪೂರ್ಣಾಂಕ w = l.ಹೊರಹೋಗು_w;
    ಪೂರ್ಣಾಂಕ c = l.c;
    ಹಿಂತಿರುಗಿ float_ಗಮ್ಯ_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ stride, ಪೂರ್ಣಾಂಕ padding)
{
    maxpool_layer l = {0};
    l.type = MAXPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.pad = padding;
    l.ಹೊರಹೋಗು_w = (w + padding - ಗಾತ್ರ_2)/stride + 1;
    l.ಹೊರಹೋಗು_h = (h + padding - ಗಾತ್ರ_2)/stride + 1;
    l.ಹೊರಹೋಗು_c = c;
    l.outputs = l.ಹೊರಹೋಗು_h * l.ಹೊರಹೋಗು_w * l.ಹೊರಹೋಗು_c;
    l.inputs = h*w*c;
    l.ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
    l.stride = stride;
    ಪೂರ್ಣಾಂಕ output_ಗಾತ್ರ = l.ಹೊರಹೋಗು_h * l.ಹೊರಹೋಗು_w * l.ಹೊರಹೋಗು_c * batch;
    l.indexes = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(output_ಗಾತ್ರ, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
    l.output =  ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(output_ಗಾತ್ರ, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.delta =   ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(output_ಗಾತ್ರ, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.forward = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_maxpool_layer_gpu;
    l.backward_gpu = backward_maxpool_layer_gpu;
    l.indexes_gpu = cuda_make_int_array(0, output_ಗಾತ್ರ);
    l.output_gpu  = cuda_make_array(l.output, output_ಗಾತ್ರ);
    l.delta_gpu   = cuda_make_array(l.delta, output_ಗಾತ್ರ);
    #ಆದರೆ_ಅಂತ್ಯ
    fprintf(stderr, "max          %d x %d / %d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", ಗಾತ್ರ_2, ಗಾತ್ರ_2, stride, w, h, c, l.ಹೊರಹೋಗು_w, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_c);
    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ resize_maxpool_layer(maxpool_layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->ಹೊರಹೋಗು_w = (w + l->pad - l->ಗಾತ್ರ_2)/l->stride + 1;
    l->ಹೊರಹೋಗು_h = (h + l->pad - l->ಗಾತ್ರ_2)/l->stride + 1;
    l->outputs = l->ಹೊರಹೋಗು_w * l->ಹೊರಹೋಗು_h * l->c;
    ಪೂರ್ಣಾಂಕ output_ಗಾತ್ರ = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_ಗಾತ್ರ * ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
    l->output = realloc(l->output, output_ಗಾತ್ರ * ಗಾತ್ರ(ದಶಮಾಂಶ));
    l->delta = realloc(l->delta, output_ಗಾತ್ರ * ಗಾತ್ರ(ದಶಮಾಂಶ));

    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    cuda_free((ದಶಮಾಂಶ *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_ಗಾತ್ರ);
    l->output_gpu  = cuda_make_array(l->output, output_ಗಾತ್ರ);
    l->delta_gpu   = cuda_make_array(l->delta,  output_ಗಾತ್ರ);
    #ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ forward_maxpool_layer(ಸ್ಥಿರಾಂಕ maxpool_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ b,i,j,k,m,n;
    ಪೂರ್ಣಾಂಕ w_ಸ್ಥಾನಾಂತರ = -l.pad/2;
    ಪೂರ್ಣಾಂಕ h_ಸ್ಥಾನಾಂತರ = -l.pad/2;

    ಪೂರ್ಣಾಂಕ h = l.ಹೊರಹೋಗು_h;
    ಪೂರ್ಣಾಂಕ w = l.ಹೊರಹೋಗು_w;
    ಪೂರ್ಣಾಂಕ c = l.c;

    ಗಾಗಿ(b = 0; b < l.batch; ++b){
        ಗಾಗಿ(k = 0; k < c; ++k){
            ಗಾಗಿ(i = 0; i < h; ++i){
                ಗಾಗಿ(j = 0; j < w; ++j){
                    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ = j + w*(i + h*(k + c*b));
                    ದಶಮಾಂಶ max = -FLT_MAX;
                    ಪೂರ್ಣಾಂಕ max_i = -1;
                    ಗಾಗಿ(n = 0; n < l.ಗಾತ್ರ_2; ++n){
                        ಗಾಗಿ(m = 0; m < l.ಗಾತ್ರ_2; ++m){
                            ಪೂರ್ಣಾಂಕ cur_h = h_ಸ್ಥಾನಾಂತರ + i*l.stride + n;
                            ಪೂರ್ಣಾಂಕ cur_w = w_ಸ್ಥಾನಾಂತರ + j*l.stride + m;
                            ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            ಪೂರ್ಣಾಂಕ valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            ದಶಮಾಂಶ val = (valid != 0) ? net.ಒಳಹರಿವು[ಸೂಚ್ಯಂಕ] : -FLT_MAX;
                            max_i = (val > max) ? ಸೂಚ್ಯಂಕ : max_i;
                            max   = (val > max) ? val   : max;
                        }
                    }
                    l.output[ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ] = max;
                    l.indexes[ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ] = max_i;
                }
            }
        }
    }
}

ಶೂನ್ಯ backward_maxpool_layer(ಸ್ಥಿರಾಂಕ maxpool_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i;
    ಪೂರ್ಣಾಂಕ h = l.ಹೊರಹೋಗು_h;
    ಪೂರ್ಣಾಂಕ w = l.ಹೊರಹೋಗು_w;
    ಪೂರ್ಣಾಂಕ c = l.c;
    ಗಾಗಿ(i = 0; i < h*w*c*l.batch; ++i){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = l.indexes[i];
        net.delta[ಸೂಚ್ಯಂಕ] += l.delta[i];
    }
}

