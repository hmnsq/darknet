#અન્તર્ભાવ "reorg_layer.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "blas.h"

#અન્તર્ભાવ <stdio.h>


layer make_reorg_layer(પૂર્ણાંક batch, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c, પૂર્ણાંક stride, પૂર્ણાંક reverse, પૂર્ણાંક flatten, પૂર્ણાંક extra)
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
    જો(reverse){
        l.out_w = w*stride;
        l.out_h = h*stride;
        l.out_c = c/(stride*stride);
    }નહીંતર{
        l.out_w = w/stride;
        l.out_h = h/stride;
        l.out_c = c*(stride*stride);
    }
    l.reverse = reverse;

    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    જો(l.extra){
        l.out_w = l.out_h = l.out_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    જો(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } નહીંતર {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.out_w, l.out_h, l.out_c);
    }
    પૂર્ણાંક output_size = l.outputs * batch;
    l.output =  calloc(output_size, કદ(દશાંશ));
    l.delta =   calloc(output_size, કદ(દશાંશ));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#ifdef GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
#endif
    પાછા l;
}

શૂન્ય resize_reorg_layer(layer *l, પૂર્ણાંક w, પૂર્ણાંક h)
{
    પૂર્ણાંક stride = l->stride;
    પૂર્ણાંક c = l->c;

    l->h = h;
    l->w = w;

    જો(l->reverse){
        l->out_w = w*stride;
        l->out_h = h*stride;
        l->out_c = c/(stride*stride);
    }નહીંતર{
        l->out_w = w/stride;
        l->out_h = h/stride;
        l->out_c = c*(stride*stride);
    }

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->outputs;
    પૂર્ણાંક output_size = l->outputs * l->batch;

    l->output = realloc(l->output, output_size * કદ(દશાંશ));
    l->delta = realloc(l->delta, output_size * કદ(દશાંશ));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
#endif
}

શૂન્ય forward_reorg_layer(અચલ layer l, network net)
{
    પૂર્ણાંક i;
    જો(l.flatten){
        memcpy(l.output, net.આગત, l.outputs*l.batch*કદ(દશાંશ));
        જો(l.reverse){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }નહીંતર{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } નહીંતર જો (l.extra) {
        માટે(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.આગત + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } નહીંતર જો (l.reverse){
        reorg_cpu(net.આગત, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } નહીંતર {
        reorg_cpu(net.આગત, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

શૂન્ય backward_reorg_layer(અચલ layer l, network net)
{
    પૂર્ણાંક i;
    જો(l.flatten){
        memcpy(net.delta, l.delta, l.outputs*l.batch*કદ(દશાંશ));
        જો(l.reverse){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }નહીંતર{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } નહીંતર જો(l.reverse){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } નહીંતર જો (l.extra) {
        માટે(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }નહીંતર{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ifdef GPU
શૂન્ય forward_reorg_layer_gpu(layer l, network net)
{
    પૂર્ણાંક i;
    જો(l.flatten){
        જો(l.reverse){
            flatten_gpu(net.આગત_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }નહીંતર{
            flatten_gpu(net.આગત_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } નહીંતર જો (l.extra) {
        માટે(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.આગત_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } નહીંતર જો (l.reverse) {
        reorg_gpu(net.આગત_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }નહીંતર {
        reorg_gpu(net.આગત_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

શૂન્ય backward_reorg_layer_gpu(layer l, network net)
{
    જો(l.flatten){
        જો(l.reverse){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }નહીંતર{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } નહીંતર જો (l.extra) {
        પૂર્ણાંક i;
        માટે(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } નહીંતર જો(l.reverse){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } નહીંતર {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#endif
