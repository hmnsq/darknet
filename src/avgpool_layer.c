#include "avgpool_layer.h"
#include "cuda.h"
#include <stdio.h>

avgpool_layer make_avgpool_layer(puur_naa_nka batch, puur_naa_nka w, puur_naa_nka h, puur_naa_nka c)
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
    puur_naa_nka output_size = l.outputs * batch;
    l.output =  calloc(output_size, parimaa_na(float));
    l.delta =   calloc(output_size, parimaa_na(float));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
    #endif
    ꦮꦭꦶ l;
}

void resize_avgpool_layer(avgpool_layer *l, puur_naa_nka w, puur_naa_nka h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

void forward_avgpool_layer(const avgpool_layer l, network net)
{
    puur_naa_nka b,i,k;

    ꦏꦁꦒꦺꦴ(b = 0; b < l.batch; ++b){
        ꦏꦁꦒꦺꦴ(k = 0; k < l.c; ++k){
            puur_naa_nka out_index = k + b*l.c;
            l.output[out_index] = 0;
            ꦏꦁꦒꦺꦴ(i = 0; i < l.h*l.w; ++i){
                puur_naa_nka in_index = i + l.h*l.w*(k + b*l.c);
                l.output[out_index] += net.input[in_index];
            }
            l.output[out_index] /= l.h*l.w;
        }
    }
}

void backward_avgpool_layer(const avgpool_layer l, network net)
{
    puur_naa_nka b,i,k;

    ꦏꦁꦒꦺꦴ(b = 0; b < l.batch; ++b){
        ꦏꦁꦒꦺꦴ(k = 0; k < l.c; ++k){
            puur_naa_nka out_index = k + b*l.c;
            ꦏꦁꦒꦺꦴ(i = 0; i < l.h*l.w; ++i){
                puur_naa_nka in_index = i + l.h*l.w*(k + b*l.c);
                net.delta[in_index] += l.delta[out_index] / (l.h*l.w);
            }
        }
    }
}

