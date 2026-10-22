#شامل "reorg_layer.h"
#شامل "cuda.h"
#شامل "blas.h"

#شامل <stdio.h>


layer make_reorg_layer(صحیح batch, صحیح w, صحیح h, صحیح c, صحیح stride, صحیح reverse, صحیح flatten, صحیح extra)
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
    اگر(reverse){
        l.out_w = w*stride;
        l.out_h = h*stride;
        l.out_c = c/(stride*stride);
    }وگرنه{
        l.out_w = w/stride;
        l.out_h = h/stride;
        l.out_c = c*(stride*stride);
    }
    l.reverse = reverse;

    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    اگر(l.extra){
        l.out_w = l.out_h = l.out_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    اگر(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } وگرنه {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.out_w, l.out_h, l.out_c);
    }
    صحیح output_size = l.outputs * batch;
    l.output =  calloc(output_size, اندازه(اعشاری));
    l.delta =   calloc(output_size, اندازه(اعشاری));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#ifdef GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
#endif
    بازگشت l;
}

تهی resize_reorg_layer(layer *l, صحیح w, صحیح h)
{
    صحیح stride = l->stride;
    صحیح c = l->c;

    l->h = h;
    l->w = w;

    اگر(l->reverse){
        l->out_w = w*stride;
        l->out_h = h*stride;
        l->out_c = c/(stride*stride);
    }وگرنه{
        l->out_w = w/stride;
        l->out_h = h/stride;
        l->out_c = c*(stride*stride);
    }

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->outputs;
    صحیح output_size = l->outputs * l->batch;

    l->output = realloc(l->output, output_size * اندازه(اعشاری));
    l->delta = realloc(l->delta, output_size * اندازه(اعشاری));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
#endif
}

تهی forward_reorg_layer(ثابت layer l, network net)
{
    صحیح i;
    اگر(l.flatten){
        memcpy(l.output, net.درون_داد, l.outputs*l.batch*اندازه(اعشاری));
        اگر(l.reverse){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }وگرنه{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } وگرنه اگر (l.extra) {
        برای(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.درون_داد + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } وگرنه اگر (l.reverse){
        reorg_cpu(net.درون_داد, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } وگرنه {
        reorg_cpu(net.درون_داد, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

تهی backward_reorg_layer(ثابت layer l, network net)
{
    صحیح i;
    اگر(l.flatten){
        memcpy(net.delta, l.delta, l.outputs*l.batch*اندازه(اعشاری));
        اگر(l.reverse){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }وگرنه{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } وگرنه اگر(l.reverse){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } وگرنه اگر (l.extra) {
        برای(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }وگرنه{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ifdef GPU
تهی forward_reorg_layer_gpu(layer l, network net)
{
    صحیح i;
    اگر(l.flatten){
        اگر(l.reverse){
            flatten_gpu(net.درون_داد_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }وگرنه{
            flatten_gpu(net.درون_داد_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } وگرنه اگر (l.extra) {
        برای(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.درون_داد_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } وگرنه اگر (l.reverse) {
        reorg_gpu(net.درون_داد_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }وگرنه {
        reorg_gpu(net.درون_داد_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

تهی backward_reorg_layer_gpu(layer l, network net)
{
    اگر(l.flatten){
        اگر(l.reverse){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }وگرنه{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } وگرنه اگر (l.extra) {
        صحیح i;
        برای(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } وگرنه اگر(l.reverse){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } وگرنه {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#endif
