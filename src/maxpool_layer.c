#include "maxpool_layer.h"
#include "cuda.h"
#include <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    số_nguyên h = l.out_h;
    số_nguyên w = l.out_w;
    số_nguyên c = l.c;
    trả_về float_to_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    số_nguyên h = l.out_h;
    số_nguyên w = l.out_w;
    số_nguyên c = l.c;
    trả_về float_to_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(số_nguyên batch, số_nguyên h, số_nguyên w, số_nguyên c, số_nguyên size, số_nguyên stride, số_nguyên padding)
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
    số_nguyên output_size = l.out_h * l.out_w * l.out_c * batch;
    l.indexes = calloc(output_size, sizeof(số_nguyên));
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
    trả_về l;
}

void resize_maxpool_layer(maxpool_layer *l, số_nguyên w, số_nguyên h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->out_w = (w + l->pad - l->size)/l->stride + 1;
    l->out_h = (h + l->pad - l->size)/l->stride + 1;
    l->outputs = l->out_w * l->out_h * l->c;
    số_nguyên output_size = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_size * sizeof(số_nguyên));
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
    số_nguyên b,i,j,k,m,n;
    số_nguyên w_offset = -l.pad/2;
    số_nguyên h_offset = -l.pad/2;

    số_nguyên h = l.out_h;
    số_nguyên w = l.out_w;
    số_nguyên c = l.c;

    cho(b = 0; b < l.batch; ++b){
        cho(k = 0; k < c; ++k){
            cho(i = 0; i < h; ++i){
                cho(j = 0; j < w; ++j){
                    số_nguyên out_index = j + w*(i + h*(k + c*b));
                    float max = -FLT_MAX;
                    số_nguyên max_i = -1;
                    cho(n = 0; n < l.size; ++n){
                        cho(m = 0; m < l.size; ++m){
                            số_nguyên cur_h = h_offset + i*l.stride + n;
                            số_nguyên cur_w = w_offset + j*l.stride + m;
                            số_nguyên index = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            số_nguyên valid = (cur_h >= 0 && cur_h < l.h &&
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
    số_nguyên i;
    số_nguyên h = l.out_h;
    số_nguyên w = l.out_w;
    số_nguyên c = l.c;
    cho(i = 0; i < h*w*c*l.batch; ++i){
        số_nguyên index = l.indexes[i];
        net.delta[index] += l.delta[i];
    }
}

