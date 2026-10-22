#include "avgpool_layer.h"
#include "cuda.h"
#include <stdio.h>

avgpool_layer make_avgpool_layer(số_nguyên batch, số_nguyên w, số_nguyên h, số_nguyên c)
{
    fprintf(stderr, "avg                     %4d x%4d x%4d   ->  %4d\n",  w, h, c, c);
    avgpool_layer l = {0};
    l.type = AVGPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.out_w = 1;
    l.out_h = 1;
    l.out_c = c;
    l.outputs = l.out_c;
    l.inputs = h*w*c;
    số_nguyên output_size = l.outputs * batch;
    l.output =  calloc(output_size, sizeof(float));
    l.delta =   calloc(output_size, sizeof(float));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
    #endif
    trả_về l;
}

void resize_avgpool_layer(avgpool_layer *l, số_nguyên w, số_nguyên h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

void forward_avgpool_layer(const avgpool_layer l, network net)
{
    số_nguyên b,i,k;

    cho(b = 0; b < l.batch; ++b){
        cho(k = 0; k < l.c; ++k){
            số_nguyên out_index = k + b*l.c;
            l.output[out_index] = 0;
            cho(i = 0; i < l.h*l.w; ++i){
                số_nguyên in_index = i + l.h*l.w*(k + b*l.c);
                l.output[out_index] += net.input[in_index];
            }
            l.output[out_index] /= l.h*l.w;
        }
    }
}

void backward_avgpool_layer(const avgpool_layer l, network net)
{
    số_nguyên b,i,k;

    cho(b = 0; b < l.batch; ++b){
        cho(k = 0; k < l.c; ++k){
            số_nguyên out_index = k + b*l.c;
            cho(i = 0; i < l.h*l.w; ++i){
                số_nguyên in_index = i + l.h*l.w*(k + b*l.c);
                net.delta[in_index] += l.delta[out_index] / (l.h*l.w);
            }
        }
    }
}

