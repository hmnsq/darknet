#include "maxpool_layer.h"
#include "cuda.h"
#include <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    整数 h = l.out_h;
    整数 w = l.out_w;
    整数 c = l.c;
    返回 float_to_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    整数 h = l.out_h;
    整数 w = l.out_w;
    整数 c = l.c;
    返回 float_to_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(整数 batch, 整数 h, 整数 w, 整数 c, 整数 size, 整数 stride, 整数 padding)
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
    整数 output_size = l.out_h * l.out_w * l.out_c * batch;
    l.indexes = calloc(output_size, sizeof(整数));
    l.output =  calloc(output_size, sizeof(float));
    l.delta =   calloc(output_size, sizeof(float));
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
    返回 l;
}

void resize_maxpool_layer(maxpool_layer *l, 整数 w, 整数 h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->out_w = (w + l->pad - l->size)/l->stride + 1;
    l->out_h = (h + l->pad - l->size)/l->stride + 1;
    l->outputs = l->out_w * l->out_h * l->c;
    整数 output_size = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_size * sizeof(整数));
    l->output = realloc(l->output, output_size * sizeof(float));
    l->delta = realloc(l->delta, output_size * sizeof(float));

    #ifdef GPU
    cuda_free((float *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_size);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
    #endif
}

void forward_maxpool_layer(const maxpool_layer l, network net)
{
    整数 b,i,j,k,m,n;
    整数 w_offset = -l.pad/2;
    整数 h_offset = -l.pad/2;

    整数 h = l.out_h;
    整数 w = l.out_w;
    整数 c = l.c;

    为(b = 0; b < l.batch; ++b){
        为(k = 0; k < c; ++k){
            为(i = 0; i < h; ++i){
                为(j = 0; j < w; ++j){
                    整数 out_index = j + w*(i + h*(k + c*b));
                    float max = -FLT_MAX;
                    整数 max_i = -1;
                    为(n = 0; n < l.size; ++n){
                        为(m = 0; m < l.size; ++m){
                            整数 cur_h = h_offset + i*l.stride + n;
                            整数 cur_w = w_offset + j*l.stride + m;
                            整数 index = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            整数 valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            float val = (valid != 0) ? net.input[index] : -FLT_MAX;
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

void backward_maxpool_layer(const maxpool_layer l, network net)
{
    整数 i;
    整数 h = l.out_h;
    整数 w = l.out_w;
    整数 c = l.c;
    为(i = 0; i < h*w*c*l.batch; ++i){
        整数 index = l.indexes[i];
        net.delta[index] += l.delta[i];
    }
}

