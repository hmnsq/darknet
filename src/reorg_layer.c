#include "reorg_layer.h"
#include "cuda.h"
#include "blas.h"

#include <stdio.h>


layer make_reorg_layer(ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ stride, ຈຳນວນເຕັມ reverse, ຈຳນວນເຕັມ flatten, ຈຳນວນເຕັມ extra)
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
    ຖ້າ(reverse){
        l.out_w = w*stride;
        l.out_h = h*stride;
        l.out_c = c/(stride*stride);
    }ບໍ່ດັ່ງນັ້ນ{
        l.out_w = w/stride;
        l.out_h = h/stride;
        l.out_c = c*(stride*stride);
    }
    l.reverse = reverse;

    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    ຖ້າ(l.extra){
        l.out_w = l.out_h = l.out_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    ຖ້າ(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } ບໍ່ດັ່ງນັ້ນ {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.out_w, l.out_h, l.out_c);
    }
    ຈຳນວນເຕັມ output_size = l.outputs * batch;
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
    ສົ່ງຄືນ l;
}

void resize_reorg_layer(layer *l, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h)
{
    ຈຳນວນເຕັມ stride = l->stride;
    ຈຳນວນເຕັມ c = l->c;

    l->h = h;
    l->w = w;

    ຖ້າ(l->reverse){
        l->out_w = w*stride;
        l->out_h = h*stride;
        l->out_c = c/(stride*stride);
    }ບໍ່ດັ່ງນັ້ນ{
        l->out_w = w/stride;
        l->out_h = h/stride;
        l->out_c = c*(stride*stride);
    }

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->outputs;
    ຈຳນວນເຕັມ output_size = l->outputs * l->batch;

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
    ຈຳນວນເຕັມ i;
    ຖ້າ(l.flatten){
        memcpy(l.output, net.input, l.outputs*l.batch*parimaa_na(float));
        ຖ້າ(l.reverse){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }ບໍ່ດັ່ງນັ້ນ{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (l.extra) {
        ສຳລັບ(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.input + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (l.reverse){
        reorg_cpu(net.input, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } ບໍ່ດັ່ງນັ້ນ {
        reorg_cpu(net.input, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

void backward_reorg_layer(const layer l, network net)
{
    ຈຳນວນເຕັມ i;
    ຖ້າ(l.flatten){
        memcpy(net.delta, l.delta, l.outputs*l.batch*parimaa_na(float));
        ຖ້າ(l.reverse){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }ບໍ່ດັ່ງນັ້ນ{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } ບໍ່ດັ່ງນັ້ນ ຖ້າ(l.reverse){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (l.extra) {
        ສຳລັບ(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }ບໍ່ດັ່ງນັ້ນ{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ifdef GPU
void forward_reorg_layer_gpu(layer l, network net)
{
    ຈຳນວນເຕັມ i;
    ຖ້າ(l.flatten){
        ຖ້າ(l.reverse){
            flatten_gpu(net.input_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }ບໍ່ດັ່ງນັ້ນ{
            flatten_gpu(net.input_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (l.extra) {
        ສຳລັບ(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.input_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (l.reverse) {
        reorg_gpu(net.input_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }ບໍ່ດັ່ງນັ້ນ {
        reorg_gpu(net.input_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

void backward_reorg_layer_gpu(layer l, network net)
{
    ຖ້າ(l.flatten){
        ຖ້າ(l.reverse){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }ບໍ່ດັ່ງນັ້ນ{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (l.extra) {
        ຈຳນວນເຕັມ i;
        ສຳລັບ(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } ບໍ່ດັ່ງນັ້ນ ຖ້າ(l.reverse){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } ບໍ່ດັ່ງນັ້ນ {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#endif
