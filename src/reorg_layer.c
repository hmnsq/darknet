#include "reorg_layer.h"
#include "cuda.h"
#include "blas.h"

#include <stdio.h>


layer make_reorg_layer(puur_naa_nka batch, puur_naa_nka w, puur_naa_nka h, puur_naa_nka c, puur_naa_nka stride, puur_naa_nka reverse, puur_naa_nka flatten, puur_naa_nka extra)
{
    layer l = {0};
    l.type = REORG;
    l.batch = batch;
    l.stride = stride;
    l.extra = extra;
    l.h = h;
    l.w = w;
    l.c = c;
    l.flatten = flatten;
    ᰀᰧ(reverse){
        l.out_w = w*stride;
        l.out_h = h*stride;
        l.out_c = c/(stride*stride);
    }ᰀᰧᰀ{
        l.out_w = w/stride;
        l.out_h = h/stride;
        l.out_c = c*(stride*stride);
    }
    l.reverse = reverse;

    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    ᰀᰧ(l.extra){
        l.out_w = l.out_h = l.out_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    ᰀᰧ(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } ᰀᰧᰀ {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.out_w, l.out_h, l.out_c);
    }
    puur_naa_nka output_size = l.outputs * batch;
    l.output =  calloc(output_size, parimaa_na(float));
    l.delta =   calloc(output_size, parimaa_na(float));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#ifdef GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
#endif
    ᰀᰧᰌ l;
}

void resize_reorg_layer(layer *l, puur_naa_nka w, puur_naa_nka h)
{
    puur_naa_nka stride = l->stride;
    puur_naa_nka c = l->c;

    l->h = h;
    l->w = w;

    ᰀᰧ(l->reverse){
        l->out_w = w*stride;
        l->out_h = h*stride;
        l->out_c = c/(stride*stride);
    }ᰀᰧᰀ{
        l->out_w = w/stride;
        l->out_h = h/stride;
        l->out_c = c*(stride*stride);
    }

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->outputs;
    puur_naa_nka output_size = l->outputs * l->batch;

    l->output = realloc(l->output, output_size * parimaa_na(float));
    l->delta = realloc(l->delta, output_size * parimaa_na(float));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
#endif
}

void forward_reorg_layer(const layer l, network net)
{
    puur_naa_nka i;
    ᰀᰧ(l.flatten){
        memcpy(l.output, net.input, l.outputs*l.batch*parimaa_na(float));
        ᰀᰧ(l.reverse){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }ᰀᰧᰀ{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } ᰀᰧᰀ ᰀᰧ (l.extra) {
        for(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.input + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } ᰀᰧᰀ ᰀᰧ (l.reverse){
        reorg_cpu(net.input, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } ᰀᰧᰀ {
        reorg_cpu(net.input, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

void backward_reorg_layer(const layer l, network net)
{
    puur_naa_nka i;
    ᰀᰧ(l.flatten){
        memcpy(net.delta, l.delta, l.outputs*l.batch*parimaa_na(float));
        ᰀᰧ(l.reverse){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }ᰀᰧᰀ{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } ᰀᰧᰀ ᰀᰧ(l.reverse){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } ᰀᰧᰀ ᰀᰧ (l.extra) {
        for(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }ᰀᰧᰀ{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ifdef GPU
void forward_reorg_layer_gpu(layer l, network net)
{
    puur_naa_nka i;
    ᰀᰧ(l.flatten){
        ᰀᰧ(l.reverse){
            flatten_gpu(net.input_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }ᰀᰧᰀ{
            flatten_gpu(net.input_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } ᰀᰧᰀ ᰀᰧ (l.extra) {
        for(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.input_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } ᰀᰧᰀ ᰀᰧ (l.reverse) {
        reorg_gpu(net.input_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }ᰀᰧᰀ {
        reorg_gpu(net.input_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

void backward_reorg_layer_gpu(layer l, network net)
{
    ᰀᰧ(l.flatten){
        ᰀᰧ(l.reverse){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }ᰀᰧᰀ{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } ᰀᰧᰀ ᰀᰧ (l.extra) {
        puur_naa_nka i;
        for(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } ᰀᰧᰀ ᰀᰧ(l.reverse){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } ᰀᰧᰀ {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#endif
