#include "reorg_layer.h"
#include "cuda.h"
#include "blas.h"

#include <stdio.h>


layer make_reorg_layer(ចំនួនគត់ batch, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ c, ចំនួនគត់ stride, ចំនួនគត់ reverse, ចំនួនគត់ flatten, ចំនួនគត់ extra)
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
    ប្រសិនបើ(reverse){
        l.out_w = w*stride;
        l.out_h = h*stride;
        l.out_c = c/(stride*stride);
    }បើមិនដូច្នេះ{
        l.out_w = w/stride;
        l.out_h = h/stride;
        l.out_c = c*(stride*stride);
    }
    l.reverse = reverse;

    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    ប្រសិនបើ(l.extra){
        l.out_w = l.out_h = l.out_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    ប្រសិនបើ(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } បើមិនដូច្នេះ {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.out_w, l.out_h, l.out_c);
    }
    ចំនួនគត់ output_size = l.outputs * batch;
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
    ត្រឡប់ l;
}

void resize_reorg_layer(layer *l, ចំនួនគត់ w, ចំនួនគត់ h)
{
    ចំនួនគត់ stride = l->stride;
    ចំនួនគត់ c = l->c;

    l->h = h;
    l->w = w;

    ប្រសិនបើ(l->reverse){
        l->out_w = w*stride;
        l->out_h = h*stride;
        l->out_c = c/(stride*stride);
    }បើមិនដូច្នេះ{
        l->out_w = w/stride;
        l->out_h = h/stride;
        l->out_c = c*(stride*stride);
    }

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->outputs;
    ចំនួនគត់ output_size = l->outputs * l->batch;

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
    ចំនួនគត់ i;
    ប្រសិនបើ(l.flatten){
        memcpy(l.output, net.input, l.outputs*l.batch*parimaa_na(float));
        ប្រសិនបើ(l.reverse){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }បើមិនដូច្នេះ{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } បើមិនដូច្នេះ ប្រសិនបើ (l.extra) {
        សម្រាប់(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.input + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } បើមិនដូច្នេះ ប្រសិនបើ (l.reverse){
        reorg_cpu(net.input, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } បើមិនដូច្នេះ {
        reorg_cpu(net.input, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

void backward_reorg_layer(const layer l, network net)
{
    ចំនួនគត់ i;
    ប្រសិនបើ(l.flatten){
        memcpy(net.delta, l.delta, l.outputs*l.batch*parimaa_na(float));
        ប្រសិនបើ(l.reverse){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }បើមិនដូច្នេះ{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } បើមិនដូច្នេះ ប្រសិនបើ(l.reverse){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } បើមិនដូច្នេះ ប្រសិនបើ (l.extra) {
        សម្រាប់(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }បើមិនដូច្នេះ{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ifdef GPU
void forward_reorg_layer_gpu(layer l, network net)
{
    ចំនួនគត់ i;
    ប្រសិនបើ(l.flatten){
        ប្រសិនបើ(l.reverse){
            flatten_gpu(net.input_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }បើមិនដូច្នេះ{
            flatten_gpu(net.input_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } បើមិនដូច្នេះ ប្រសិនបើ (l.extra) {
        សម្រាប់(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.input_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } បើមិនដូច្នេះ ប្រសិនបើ (l.reverse) {
        reorg_gpu(net.input_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }បើមិនដូច្នេះ {
        reorg_gpu(net.input_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

void backward_reorg_layer_gpu(layer l, network net)
{
    ប្រសិនបើ(l.flatten){
        ប្រសិនបើ(l.reverse){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }បើមិនដូច្នេះ{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } បើមិនដូច្នេះ ប្រសិនបើ (l.extra) {
        ចំនួនគត់ i;
        សម្រាប់(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } បើមិនដូច្នេះ ប្រសិនបើ(l.reverse){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } បើមិនដូច្នេះ {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#endif
