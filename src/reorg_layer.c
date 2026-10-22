#ضمن "reorg_layer.h"
#ضمن "cuda.h"
#ضمن "blas.h"

#ضمن <stdio.h>


layer make_reorg_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح stride, صحيح معکوس, صحيح flatten, صحيح extra)
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
    إذا(معکوس){
        l.بیرون_w = w*stride;
        l.بیرون_h = h*stride;
        l.بیرون_c = c/(stride*stride);
    }وإلا{
        l.بیرون_w = w/stride;
        l.بیرون_h = h/stride;
        l.بیرون_c = c*(stride*stride);
    }
    l.معکوس = معکوس;

    l.outputs = l.بیرون_h * l.بیرون_w * l.بیرون_c;
    l.inputs = h*w*c;
    إذا(l.extra){
        l.بیرون_w = l.بیرون_h = l.بیرون_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    إذا(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } وإلا {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.بیرون_w, l.بیرون_h, l.بیرون_c);
    }
    صحيح output_اندازه = l.outputs * batch;
    l.output =  حافظه_صفر_بده(output_اندازه, حجم(عائم));
    l.delta =   حافظه_صفر_بده(output_اندازه, حجم(عائم));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#اگر_تعریف_شده GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_اندازه);
    l.delta_gpu   = cuda_make_array(l.delta, output_اندازه);
#پایان_اگر
    أرجع l;
}

فراغ resize_reorg_layer(layer *l, صحيح w, صحيح h)
{
    صحيح stride = l->stride;
    صحيح c = l->c;

    l->h = h;
    l->w = w;

    إذا(l->معکوس){
        l->بیرون_w = w*stride;
        l->بیرون_h = h*stride;
        l->بیرون_c = c/(stride*stride);
    }وإلا{
        l->بیرون_w = w/stride;
        l->بیرون_h = h/stride;
        l->بیرون_c = c*(stride*stride);
    }

    l->outputs = l->بیرون_h * l->بیرون_w * l->بیرون_c;
    l->inputs = l->outputs;
    صحيح output_اندازه = l->outputs * l->batch;

    l->output = realloc(l->output, output_اندازه * حجم(عائم));
    l->delta = realloc(l->delta, output_اندازه * حجم(عائم));

#اگر_تعریف_شده GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_اندازه);
    l->delta_gpu   = cuda_make_array(l->delta,  output_اندازه);
#پایان_اگر
}

فراغ forward_reorg_layer(ثابت layer l, شبکه net)
{
    صحيح i;
    إذا(l.flatten){
        کپی_حافظه(l.output, net.درون_داد, l.outputs*l.batch*حجم(عائم));
        إذا(l.معکوس){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }وإلا{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } وإلا إذا (l.extra) {
        لكل(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.درون_داد + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } وإلا إذا (l.معکوس){
        reorg_cpu(net.درون_داد, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } وإلا {
        reorg_cpu(net.درون_داد, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

فراغ backward_reorg_layer(ثابت layer l, شبکه net)
{
    صحيح i;
    إذا(l.flatten){
        کپی_حافظه(net.delta, l.delta, l.outputs*l.batch*حجم(عائم));
        إذا(l.معکوس){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }وإلا{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } وإلا إذا(l.معکوس){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } وإلا إذا (l.extra) {
        لكل(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }وإلا{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#اگر_تعریف_شده GPU
فراغ forward_reorg_layer_gpu(layer l, شبکه net)
{
    صحيح i;
    إذا(l.flatten){
        إذا(l.معکوس){
            flatten_gpu(net.درون_داد_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }وإلا{
            flatten_gpu(net.درون_داد_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } وإلا إذا (l.extra) {
        لكل(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.درون_داد_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } وإلا إذا (l.معکوس) {
        reorg_gpu(net.درون_داد_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }وإلا {
        reorg_gpu(net.درون_داد_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

فراغ backward_reorg_layer_gpu(layer l, شبکه net)
{
    إذا(l.flatten){
        إذا(l.معکوس){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }وإلا{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } وإلا إذا (l.extra) {
        صحيح i;
        لكل(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } وإلا إذا(l.معکوس){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } وإلا {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#پایان_اگر
