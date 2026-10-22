#ਅਨ੍ਤਰ੍ਭਾਵ "reorg_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>


layer make_reorg_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c, ਪੂਰਨ stride, ਪੂਰਨ ਉਲਟ, ਪੂਰਨ flatten, ਪੂਰਨ extra)
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
    ਜੇ(ਉਲਟ){
        l.ਬਾਹਰਲਾ_w = w*stride;
        l.ਬਾਹਰਲਾ_h = h*stride;
        l.ਬਾਹਰਲਾ_c = c/(stride*stride);
    }ਨਹੀਂਤਾਂ{
        l.ਬਾਹਰਲਾ_w = w/stride;
        l.ਬਾਹਰਲਾ_h = h/stride;
        l.ਬਾਹਰਲਾ_c = c*(stride*stride);
    }
    l.ਉਲਟ = ਉਲਟ;

    l.outputs = l.ਬਾਹਰਲਾ_h * l.ਬਾਹਰਲਾ_w * l.ਬਾਹਰਲਾ_c;
    l.inputs = h*w*c;
    ਜੇ(l.extra){
        l.ਬਾਹਰਲਾ_w = l.ਬਾਹਰਲਾ_h = l.ਬਾਹਰਲਾ_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    ਜੇ(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } ਨਹੀਂਤਾਂ {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.ਬਾਹਰਲਾ_w, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_c);
    }
    ਪੂਰਨ output_ਮਾਪ = l.outputs * batch;
    l.output =  ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(output_ਮਾਪ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.delta =   ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(output_ਮਾਪ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_ਮਾਪ);
    l.delta_gpu   = cuda_make_array(l.delta, output_ਮਾਪ);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ l;
}

ਖਾਲੀ resize_reorg_layer(layer *l, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਪੂਰਨ stride = l->stride;
    ਪੂਰਨ c = l->c;

    l->h = h;
    l->w = w;

    ਜੇ(l->ਉਲਟ){
        l->ਬਾਹਰਲਾ_w = w*stride;
        l->ਬਾਹਰਲਾ_h = h*stride;
        l->ਬਾਹਰਲਾ_c = c/(stride*stride);
    }ਨਹੀਂਤਾਂ{
        l->ਬਾਹਰਲਾ_w = w/stride;
        l->ਬਾਹਰਲਾ_h = h/stride;
        l->ਬਾਹਰਲਾ_c = c*(stride*stride);
    }

    l->outputs = l->ਬਾਹਰਲਾ_h * l->ਬਾਹਰਲਾ_w * l->ਬਾਹਰਲਾ_c;
    l->inputs = l->outputs;
    ਪੂਰਨ output_ਮਾਪ = l->outputs * l->batch;

    l->output = realloc(l->output, output_ਮਾਪ * ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l->delta = realloc(l->delta, output_ਮਾਪ * ਆਕਾਰ(ਦਸ਼ਮਲਵ));

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_ਮਾਪ);
    l->delta_gpu   = cuda_make_array(l->delta,  output_ਮਾਪ);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ forward_reorg_layer(ਅਚਲ layer l, ਜਾਲ net)
{
    ਪੂਰਨ i;
    ਜੇ(l.flatten){
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(l.output, net.ਨਿਵੇਸ਼, l.outputs*l.batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        ਜੇ(l.ਉਲਟ){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }ਨਹੀਂਤਾਂ{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } ਨਹੀਂਤਾਂ ਜੇ (l.extra) {
        ਲਈ(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.ਨਿਵੇਸ਼ + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } ਨਹੀਂਤਾਂ ਜੇ (l.ਉਲਟ){
        reorg_cpu(net.ਨਿਵੇਸ਼, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } ਨਹੀਂਤਾਂ {
        reorg_cpu(net.ਨਿਵੇਸ਼, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

ਖਾਲੀ backward_reorg_layer(ਅਚਲ layer l, ਜਾਲ net)
{
    ਪੂਰਨ i;
    ਜੇ(l.flatten){
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(net.delta, l.delta, l.outputs*l.batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        ਜੇ(l.ਉਲਟ){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }ਨਹੀਂਤਾਂ{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } ਨਹੀਂਤਾਂ ਜੇ(l.ਉਲਟ){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } ਨਹੀਂਤਾਂ ਜੇ (l.extra) {
        ਲਈ(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }ਨਹੀਂਤਾਂ{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_reorg_layer_gpu(layer l, ਜਾਲ net)
{
    ਪੂਰਨ i;
    ਜੇ(l.flatten){
        ਜੇ(l.ਉਲਟ){
            flatten_gpu(net.ਨਿਵੇਸ਼_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }ਨਹੀਂਤਾਂ{
            flatten_gpu(net.ਨਿਵੇਸ਼_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } ਨਹੀਂਤਾਂ ਜੇ (l.extra) {
        ਲਈ(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.ਨਿਵੇਸ਼_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } ਨਹੀਂਤਾਂ ਜੇ (l.ਉਲਟ) {
        reorg_gpu(net.ਨਿਵੇਸ਼_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }ਨਹੀਂਤਾਂ {
        reorg_gpu(net.ਨਿਵੇਸ਼_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

ਖਾਲੀ backward_reorg_layer_gpu(layer l, ਜਾਲ net)
{
    ਜੇ(l.flatten){
        ਜੇ(l.ਉਲਟ){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }ਨਹੀਂਤਾਂ{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } ਨਹੀਂਤਾਂ ਜੇ (l.extra) {
        ਪੂਰਨ i;
        ਲਈ(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } ਨਹੀਂਤਾਂ ਜੇ(l.ਉਲਟ){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } ਨਹੀਂਤਾਂ {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#ਜੇ_ਅੰਤ
