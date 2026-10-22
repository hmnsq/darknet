#അന്തര്ഭാവ "reorg_layer.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "blas.h"

#അന്തര്ഭാവ <stdio.h>


layer make_reorg_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ reverse, പൂർണ്ണസംഖ്യ flatten, പൂർണ്ണസംഖ്യ extra)
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
    എങ്കിൽ(reverse){
        l.out_w = w*stride;
        l.out_h = h*stride;
        l.out_c = c/(stride*stride);
    }അല്ലെങ്കിൽ{
        l.out_w = w/stride;
        l.out_h = h/stride;
        l.out_c = c*(stride*stride);
    }
    l.reverse = reverse;

    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    എങ്കിൽ(l.extra){
        l.out_w = l.out_h = l.out_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    എങ്കിൽ(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } അല്ലെങ്കിൽ {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.out_w, l.out_h, l.out_c);
    }
    പൂർണ്ണസംഖ്യ output_size = l.outputs * batch;
    l.output =  calloc(output_size, വലുപ്പം(ദശാംശം));
    l.delta =   calloc(output_size, വലുപ്പം(ദശാംശം));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#ifdef GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
#endif
    തിരികെ l;
}

ശൂന്യം resize_reorg_layer(layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    പൂർണ്ണസംഖ്യ stride = l->stride;
    പൂർണ്ണസംഖ്യ c = l->c;

    l->h = h;
    l->w = w;

    എങ്കിൽ(l->reverse){
        l->out_w = w*stride;
        l->out_h = h*stride;
        l->out_c = c/(stride*stride);
    }അല്ലെങ്കിൽ{
        l->out_w = w/stride;
        l->out_h = h/stride;
        l->out_c = c*(stride*stride);
    }

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->outputs;
    പൂർണ്ണസംഖ്യ output_size = l->outputs * l->batch;

    l->output = realloc(l->output, output_size * വലുപ്പം(ദശാംശം));
    l->delta = realloc(l->delta, output_size * വലുപ്പം(ദശാംശം));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
#endif
}

ശൂന്യം forward_reorg_layer(സ്ഥിരം layer l, network net)
{
    പൂർണ്ണസംഖ്യ i;
    എങ്കിൽ(l.flatten){
        memcpy(l.output, net.നിവേശം, l.outputs*l.batch*വലുപ്പം(ദശാംശം));
        എങ്കിൽ(l.reverse){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }അല്ലെങ്കിൽ{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } അല്ലെങ്കിൽ എങ്കിൽ (l.extra) {
        വേണ്ടി(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.നിവേശം + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } അല്ലെങ്കിൽ എങ്കിൽ (l.reverse){
        reorg_cpu(net.നിവേശം, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } അല്ലെങ്കിൽ {
        reorg_cpu(net.നിവേശം, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

ശൂന്യം backward_reorg_layer(സ്ഥിരം layer l, network net)
{
    പൂർണ്ണസംഖ്യ i;
    എങ്കിൽ(l.flatten){
        memcpy(net.delta, l.delta, l.outputs*l.batch*വലുപ്പം(ദശാംശം));
        എങ്കിൽ(l.reverse){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }അല്ലെങ്കിൽ{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } അല്ലെങ്കിൽ എങ്കിൽ(l.reverse){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } അല്ലെങ്കിൽ എങ്കിൽ (l.extra) {
        വേണ്ടി(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }അല്ലെങ്കിൽ{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ifdef GPU
ശൂന്യം forward_reorg_layer_gpu(layer l, network net)
{
    പൂർണ്ണസംഖ്യ i;
    എങ്കിൽ(l.flatten){
        എങ്കിൽ(l.reverse){
            flatten_gpu(net.നിവേശം_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }അല്ലെങ്കിൽ{
            flatten_gpu(net.നിവേശം_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } അല്ലെങ്കിൽ എങ്കിൽ (l.extra) {
        വേണ്ടി(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.നിവേശം_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } അല്ലെങ്കിൽ എങ്കിൽ (l.reverse) {
        reorg_gpu(net.നിവേശം_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }അല്ലെങ്കിൽ {
        reorg_gpu(net.നിവേശം_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

ശൂന്യം backward_reorg_layer_gpu(layer l, network net)
{
    എങ്കിൽ(l.flatten){
        എങ്കിൽ(l.reverse){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }അല്ലെങ്കിൽ{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } അല്ലെങ്കിൽ എങ്കിൽ (l.extra) {
        പൂർണ്ണസംഖ്യ i;
        വേണ്ടി(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } അല്ലെങ്കിൽ എങ്കിൽ(l.reverse){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } അല്ലെങ്കിൽ {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#endif
