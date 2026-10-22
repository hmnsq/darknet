#include "reorg_layer.h"
#include "cuda.h"
#include "blas.h"

#include <stdio.h>


layer make_reorg_layer(ကိန်းပြည့် batch, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် c, ကိန်းပြည့် stride, ကိန်းပြည့် reverse, ကိန်းပြည့် flatten, ကိန်းပြည့် extra)
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
    အကယ်၍(reverse){
        l.out_w = w*stride;
        l.out_h = h*stride;
        l.out_c = c/(stride*stride);
    }မဟုတ်ပါက{
        l.out_w = w/stride;
        l.out_h = h/stride;
        l.out_c = c*(stride*stride);
    }
    l.reverse = reverse;

    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    အကယ်၍(l.extra){
        l.out_w = l.out_h = l.out_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    အကယ်၍(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } မဟုတ်ပါက {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.out_w, l.out_h, l.out_c);
    }
    ကိန်းပြည့် output_size = l.outputs * batch;
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
    ပြန်ပေး l;
}

void resize_reorg_layer(layer *l, ကိန်းပြည့် w, ကိန်းပြည့် h)
{
    ကိန်းပြည့် stride = l->stride;
    ကိန်းပြည့် c = l->c;

    l->h = h;
    l->w = w;

    အကယ်၍(l->reverse){
        l->out_w = w*stride;
        l->out_h = h*stride;
        l->out_c = c/(stride*stride);
    }မဟုတ်ပါက{
        l->out_w = w/stride;
        l->out_h = h/stride;
        l->out_c = c*(stride*stride);
    }

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->outputs;
    ကိန်းပြည့် output_size = l->outputs * l->batch;

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
    ကိန်းပြည့် i;
    အကယ်၍(l.flatten){
        memcpy(l.output, net.input, l.outputs*l.batch*parimaa_na(float));
        အကယ်၍(l.reverse){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }မဟုတ်ပါက{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } မဟုတ်ပါက အကယ်၍ (l.extra) {
        အတွက်(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.input + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } မဟုတ်ပါက အကယ်၍ (l.reverse){
        reorg_cpu(net.input, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } မဟုတ်ပါက {
        reorg_cpu(net.input, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

void backward_reorg_layer(const layer l, network net)
{
    ကိန်းပြည့် i;
    အကယ်၍(l.flatten){
        memcpy(net.delta, l.delta, l.outputs*l.batch*parimaa_na(float));
        အကယ်၍(l.reverse){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }မဟုတ်ပါက{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } မဟုတ်ပါက အကယ်၍(l.reverse){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } မဟုတ်ပါက အကယ်၍ (l.extra) {
        အတွက်(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }မဟုတ်ပါက{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ifdef GPU
void forward_reorg_layer_gpu(layer l, network net)
{
    ကိန်းပြည့် i;
    အကယ်၍(l.flatten){
        အကယ်၍(l.reverse){
            flatten_gpu(net.input_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }မဟုတ်ပါက{
            flatten_gpu(net.input_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } မဟုတ်ပါက အကယ်၍ (l.extra) {
        အတွက်(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.input_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } မဟုတ်ပါက အကယ်၍ (l.reverse) {
        reorg_gpu(net.input_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }မဟုတ်ပါက {
        reorg_gpu(net.input_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

void backward_reorg_layer_gpu(layer l, network net)
{
    အကယ်၍(l.flatten){
        အကယ်၍(l.reverse){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }မဟုတ်ပါက{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } မဟုတ်ပါက အကယ်၍ (l.extra) {
        ကိန်းပြည့် i;
        အတွက်(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } မဟုတ်ပါက အကယ်၍(l.reverse){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } မဟုတ်ပါက {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#endif
