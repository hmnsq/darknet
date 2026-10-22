#ಅನ್ತರ್ಭಾವ "maxpool_layer.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    ಪೂರ್ಣಾಙ್ಕ h = l.out_h;
    ಪೂರ್ಣಾಙ್ಕ w = l.out_w;
    ಪೂರ್ಣಾಙ್ಕ c = l.c;
    ಪ್ರತ್ಯಾಗಮ float_to_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    ಪೂರ್ಣಾಙ್ಕ h = l.out_h;
    ಪೂರ್ಣಾಙ್ಕ w = l.out_w;
    ಪೂರ್ಣಾಙ್ಕ c = l.c;
    ಪ್ರತ್ಯಾಗಮ float_to_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ size, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ padding)
{
    maxpool_layer l = {0};
    l.type = MAXPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.pad = padding;
    l.out_w = (w + padding - size)/stride + 1;
    l.out_h = (h + padding - size)/stride + 1;
    l.out_c = c;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    l.size = size;
    l.stride = stride;
    ಪೂರ್ಣಾಙ್ಕ output_size = l.out_h * l.out_w * l.out_c * batch;
    l.indexes = calloc(output_size, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
    l.output =  calloc(output_size, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.delta =   calloc(output_size, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.forward = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_maxpool_layer_gpu;
    l.backward_gpu = backward_maxpool_layer_gpu;
    l.indexes_gpu = cuda_make_int_array(0, output_size);
    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
    #endif
    fprintf(stderr, "max          %d x %d / %d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", size, size, stride, w, h, c, l.out_w, l.out_h, l.out_c);
    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ resize_maxpool_layer(maxpool_layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->out_w = (w + l->pad - l->size)/l->stride + 1;
    l->out_h = (h + l->pad - l->size)/l->stride + 1;
    l->outputs = l->out_w * l->out_h * l->c;
    ಪೂರ್ಣಾಙ್ಕ output_size = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_size * ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
    l->output = realloc(l->output, output_size * ಪರಿಮಾಣ(ಭಿನ್ನ));
    l->delta = realloc(l->delta, output_size * ಪರಿಮಾಣ(ಭಿನ್ನ));

    #ifdef GPU
    cuda_free((ಭಿನ್ನ *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_size);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
    #endif
}

ಶೂನ್ಯ forward_maxpool_layer(ಧ್ರುವ maxpool_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ b,i,j,k,m,n;
    ಪೂರ್ಣಾಙ್ಕ w_offset = -l.pad/2;
    ಪೂರ್ಣಾಙ್ಕ h_offset = -l.pad/2;

    ಪೂರ್ಣಾಙ್ಕ h = l.out_h;
    ಪೂರ್ಣಾಙ್ಕ w = l.out_w;
    ಪೂರ್ಣಾಙ್ಕ c = l.c;

    ಪ್ರತಿ(b = 0; b < l.batch; ++b){
        ಪ್ರತಿ(k = 0; k < c; ++k){
            ಪ್ರತಿ(i = 0; i < h; ++i){
                ಪ್ರತಿ(j = 0; j < w; ++j){
                    ಪೂರ್ಣಾಙ್ಕ out_index = j + w*(i + h*(k + c*b));
                    ಭಿನ್ನ max = -FLT_MAX;
                    ಪೂರ್ಣಾಙ್ಕ max_i = -1;
                    ಪ್ರತಿ(n = 0; n < l.size; ++n){
                        ಪ್ರತಿ(m = 0; m < l.size; ++m){
                            ಪೂರ್ಣಾಙ್ಕ cur_h = h_offset + i*l.stride + n;
                            ಪೂರ್ಣಾಙ್ಕ cur_w = w_offset + j*l.stride + m;
                            ಪೂರ್ಣಾಙ್ಕ index = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            ಪೂರ್ಣಾಙ್ಕ valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            ಭಿನ್ನ val = (valid != 0) ? net.ಒಳಹರಿವು[index] : -FLT_MAX;
                            max_i = (val > max) ? index : max_i;
                            max   = (val > max) ? val   : max;
                        }
                    }
                    l.output[out_index] = max;
                    l.indexes[out_index] = max_i;
                }
            }
        }
    }
}

ಶೂನ್ಯ backward_maxpool_layer(ಧ್ರುವ maxpool_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ h = l.out_h;
    ಪೂರ್ಣಾಙ್ಕ w = l.out_w;
    ಪೂರ್ಣಾಙ್ಕ c = l.c;
    ಪ್ರತಿ(i = 0; i < h*w*c*l.batch; ++i){
        ಪೂರ್ಣಾಙ್ಕ index = l.indexes[i];
        net.delta[index] += l.delta[i];
    }
}

