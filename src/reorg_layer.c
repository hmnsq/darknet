#ضمن "reorg_layer.h"
#ضمن "cuda.h"
#ضمن "blas.h"

#ضمن <stdio.h>


layer make_reorg_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح stride, صحيح reverse, صحيح flatten, صحيح extra)
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
    𐡄𐡍(reverse){
        l.out_w = w*stride;
        l.out_h = h*stride;
        l.out_c = c/(stride*stride);
    }وإلا{
        l.out_w = w/stride;
        l.out_h = h/stride;
        l.out_c = c*(stride*stride);
    }
    l.reverse = reverse;

    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    𐡄𐡍(l.extra){
        l.out_w = l.out_h = l.out_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    𐡄𐡍(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } وإلا {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.out_w, l.out_h, l.out_c);
    }
    صحيح output_size = l.outputs * batch;
    l.output =  calloc(output_size, حجم(عائم));
    l.delta =   calloc(output_size, حجم(عائم));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#ifdef GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
#endif
    أرجع l;
}

فراغ resize_reorg_layer(layer *l, صحيح w, صحيح h)
{
    صحيح stride = l->stride;
    صحيح c = l->c;

    l->h = h;
    l->w = w;

    𐡄𐡍(l->reverse){
        l->out_w = w*stride;
        l->out_h = h*stride;
        l->out_c = c/(stride*stride);
    }وإلا{
        l->out_w = w/stride;
        l->out_h = h/stride;
        l->out_c = c*(stride*stride);
    }

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->outputs;
    صحيح output_size = l->outputs * l->batch;

    l->output = realloc(l->output, output_size * حجم(عائم));
    l->delta = realloc(l->delta, output_size * حجم(عائم));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
#endif
}

فراغ forward_reorg_layer(ثابت layer l, network net)
{
    صحيح i;
    𐡄𐡍(l.flatten){
        memcpy(l.output, net.input, l.outputs*l.batch*حجم(عائم));
        𐡄𐡍(l.reverse){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }وإلا{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } وإلا 𐡄𐡍 (l.extra) {
        لكل(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.input + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } وإلا 𐡄𐡍 (l.reverse){
        reorg_cpu(net.input, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } وإلا {
        reorg_cpu(net.input, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

فراغ backward_reorg_layer(ثابت layer l, network net)
{
    صحيح i;
    𐡄𐡍(l.flatten){
        memcpy(net.delta, l.delta, l.outputs*l.batch*حجم(عائم));
        𐡄𐡍(l.reverse){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }وإلا{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } وإلا 𐡄𐡍(l.reverse){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } وإلا 𐡄𐡍 (l.extra) {
        لكل(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }وإلا{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ifdef GPU
فراغ forward_reorg_layer_gpu(layer l, network net)
{
    صحيح i;
    𐡄𐡍(l.flatten){
        𐡄𐡍(l.reverse){
            flatten_gpu(net.input_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }وإلا{
            flatten_gpu(net.input_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } وإلا 𐡄𐡍 (l.extra) {
        لكل(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.input_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } وإلا 𐡄𐡍 (l.reverse) {
        reorg_gpu(net.input_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }وإلا {
        reorg_gpu(net.input_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

فراغ backward_reorg_layer_gpu(layer l, network net)
{
    𐡄𐡍(l.flatten){
        𐡄𐡍(l.reverse){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }وإلا{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } وإلا 𐡄𐡍 (l.extra) {
        صحيح i;
        لكل(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } وإلا 𐡄𐡍(l.reverse){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } وإلا {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#endif
