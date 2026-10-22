#include "reorg_layer.h"
#include "cuda.h"
#include "blas.h"

#include <stdio.h>


layer make_reorg_layer(целое batch, целое w, целое h, целое c, целое stride, целое reverse, целое flatten, целое extra)
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
    если(reverse){
        l.out_w = w*stride;
        l.out_h = h*stride;
        l.out_c = c/(stride*stride);
    }иначе{
        l.out_w = w/stride;
        l.out_h = h/stride;
        l.out_c = c*(stride*stride);
    }
    l.reverse = reverse;

    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    если(l.extra){
        l.out_w = l.out_h = l.out_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    если(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } иначе {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.out_w, l.out_h, l.out_c);
    }
    целое output_size = l.outputs * batch;
    l.output =  calloc(output_size, sizeof(float));
    l.delta =   calloc(output_size, sizeof(float));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#ifdef GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
#endif
    вернуть l;
}

void resize_reorg_layer(layer *l, целое w, целое h)
{
    целое stride = l->stride;
    целое c = l->c;

    l->h = h;
    l->w = w;

    если(l->reverse){
        l->out_w = w*stride;
        l->out_h = h*stride;
        l->out_c = c/(stride*stride);
    }иначе{
        l->out_w = w/stride;
        l->out_h = h/stride;
        l->out_c = c*(stride*stride);
    }

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->outputs;
    целое output_size = l->outputs * l->batch;

    l->output = realloc(l->output, output_size * sizeof(float));
    l->delta = realloc(l->delta, output_size * sizeof(float));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
#endif
}

void forward_reorg_layer(const layer l, network net)
{
    целое i;
    если(l.flatten){
        memcpy(l.output, net.input, l.outputs*l.batch*sizeof(float));
        если(l.reverse){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }иначе{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } иначе если (l.extra) {
        для(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.input + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } иначе если (l.reverse){
        reorg_cpu(net.input, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } иначе {
        reorg_cpu(net.input, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

void backward_reorg_layer(const layer l, network net)
{
    целое i;
    если(l.flatten){
        memcpy(net.delta, l.delta, l.outputs*l.batch*sizeof(float));
        если(l.reverse){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }иначе{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } иначе если(l.reverse){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } иначе если (l.extra) {
        для(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }иначе{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ifdef GPU
void forward_reorg_layer_gpu(layer l, network net)
{
    целое i;
    если(l.flatten){
        если(l.reverse){
            flatten_gpu(net.input_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }иначе{
            flatten_gpu(net.input_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } иначе если (l.extra) {
        для(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.input_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } иначе если (l.reverse) {
        reorg_gpu(net.input_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }иначе {
        reorg_gpu(net.input_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

void backward_reorg_layer_gpu(layer l, network net)
{
    если(l.flatten){
        если(l.reverse){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }иначе{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } иначе если (l.extra) {
        целое i;
        для(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } иначе если(l.reverse){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } иначе {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#endif
