#ضمن "reorg_layer.h"
#ضمن "cuda.h"
#ضمن "blas.h"

#ضمن <stdio.h>


layer make_reorg_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح stride, صحيح سرچپه, صحيح flatten, صحيح extra)
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
    إذا(سرچپه){
        l.بهر_w = w*stride;
        l.بهر_h = h*stride;
        l.بهر_c = c/(stride*stride);
    }وإلا{
        l.بهر_w = w/stride;
        l.بهر_h = h/stride;
        l.بهر_c = c*(stride*stride);
    }
    l.سرچپه = سرچپه;

    l.outputs = l.بهر_h * l.بهر_w * l.بهر_c;
    l.inputs = h*w*c;
    إذا(l.extra){
        l.بهر_w = l.بهر_h = l.بهر_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    إذا(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } وإلا {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.بهر_w, l.بهر_h, l.بهر_c);
    }
    صحيح output_اندازه = l.outputs * batch;
    l.output =  حافظه_صفر_ورکړه(output_اندازه, حجم(عائم));
    l.delta =   حافظه_صفر_ورکړه(output_اندازه, حجم(عائم));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#که_تعریف_شوی GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_اندازه);
    l.delta_gpu   = cuda_make_array(l.delta, output_اندازه);
#د_که_پای
    أرجع l;
}

فراغ resize_reorg_layer(layer *l, صحيح w, صحيح h)
{
    صحيح stride = l->stride;
    صحيح c = l->c;

    l->h = h;
    l->w = w;

    إذا(l->سرچپه){
        l->بهر_w = w*stride;
        l->بهر_h = h*stride;
        l->بهر_c = c/(stride*stride);
    }وإلا{
        l->بهر_w = w/stride;
        l->بهر_h = h/stride;
        l->بهر_c = c*(stride*stride);
    }

    l->outputs = l->بهر_h * l->بهر_w * l->بهر_c;
    l->inputs = l->outputs;
    صحيح output_اندازه = l->outputs * l->batch;

    l->output = realloc(l->output, output_اندازه * حجم(عائم));
    l->delta = realloc(l->delta, output_اندازه * حجم(عائم));

#که_تعریف_شوی GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_اندازه);
    l->delta_gpu   = cuda_make_array(l->delta,  output_اندازه);
#د_که_پای
}

فراغ forward_reorg_layer(ثابت layer l, شبکه net)
{
    صحيح i;
    إذا(l.flatten){
        حافظه_نقل(l.output, net.ننوت, l.outputs*l.batch*حجم(عائم));
        إذا(l.سرچپه){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }وإلا{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } وإلا إذا (l.extra) {
        لكل(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.ننوت + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } وإلا إذا (l.سرچپه){
        reorg_cpu(net.ننوت, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } وإلا {
        reorg_cpu(net.ننوت, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

فراغ backward_reorg_layer(ثابت layer l, شبکه net)
{
    صحيح i;
    إذا(l.flatten){
        حافظه_نقل(net.delta, l.delta, l.outputs*l.batch*حجم(عائم));
        إذا(l.سرچپه){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }وإلا{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } وإلا إذا(l.سرچپه){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } وإلا إذا (l.extra) {
        لكل(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }وإلا{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#که_تعریف_شوی GPU
فراغ forward_reorg_layer_gpu(layer l, شبکه net)
{
    صحيح i;
    إذا(l.flatten){
        إذا(l.سرچپه){
            flatten_gpu(net.ننوت_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }وإلا{
            flatten_gpu(net.ننوت_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } وإلا إذا (l.extra) {
        لكل(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.ننوت_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } وإلا إذا (l.سرچپه) {
        reorg_gpu(net.ننوت_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }وإلا {
        reorg_gpu(net.ننوت_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

فراغ backward_reorg_layer_gpu(layer l, شبکه net)
{
    إذا(l.flatten){
        إذا(l.سرچپه){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }وإلا{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } وإلا إذا (l.extra) {
        صحيح i;
        لكل(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } وإلا إذا(l.سرچپه){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } وإلا {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#د_که_پای
