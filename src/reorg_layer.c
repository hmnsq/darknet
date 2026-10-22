#অন্তর্ভাব "reorg_layer.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "blas.h"

#অন্তর্ভাব <stdio.h>


layer make_reorg_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা reverse, পূর্ণসংখ্যা flatten, পূর্ণসংখ্যা extra)
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
    যদি(reverse){
        l.out_w = w*stride;
        l.out_h = h*stride;
        l.out_c = c/(stride*stride);
    }অন্যথা{
        l.out_w = w/stride;
        l.out_h = h/stride;
        l.out_c = c*(stride*stride);
    }
    l.reverse = reverse;

    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    যদি(l.extra){
        l.out_w = l.out_h = l.out_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    যদি(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } অন্যথা {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.out_w, l.out_h, l.out_c);
    }
    পূর্ণসংখ্যা output_আকার = l.outputs * batch;
    l.output =  calloc(output_আকার, আকার(দশমিক));
    l.delta =   calloc(output_আকার, আকার(দশমিক));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#ifdef GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_আকার);
    l.delta_gpu   = cuda_make_array(l.delta, output_আকার);
#endif
    ফেরত l;
}

শূন্য resize_reorg_layer(layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    পূর্ণসংখ্যা stride = l->stride;
    পূর্ণসংখ্যা c = l->c;

    l->h = h;
    l->w = w;

    যদি(l->reverse){
        l->out_w = w*stride;
        l->out_h = h*stride;
        l->out_c = c/(stride*stride);
    }অন্যথা{
        l->out_w = w/stride;
        l->out_h = h/stride;
        l->out_c = c*(stride*stride);
    }

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->outputs;
    পূর্ণসংখ্যা output_আকার = l->outputs * l->batch;

    l->output = realloc(l->output, output_আকার * আকার(দশমিক));
    l->delta = realloc(l->delta, output_আকার * আকার(দশমিক));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_আকার);
    l->delta_gpu   = cuda_make_array(l->delta,  output_আকার);
#endif
}

শূন্য forward_reorg_layer(ধ্রুবক layer l, network net)
{
    পূর্ণসংখ্যা i;
    যদি(l.flatten){
        memcpy(l.output, net.প্রবেশ, l.outputs*l.batch*আকার(দশমিক));
        যদি(l.reverse){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }অন্যথা{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } অন্যথা যদি (l.extra) {
        জন্য(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.প্রবেশ + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } অন্যথা যদি (l.reverse){
        reorg_cpu(net.প্রবেশ, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } অন্যথা {
        reorg_cpu(net.প্রবেশ, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

শূন্য backward_reorg_layer(ধ্রুবক layer l, network net)
{
    পূর্ণসংখ্যা i;
    যদি(l.flatten){
        memcpy(net.delta, l.delta, l.outputs*l.batch*আকার(দশমিক));
        যদি(l.reverse){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }অন্যথা{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } অন্যথা যদি(l.reverse){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } অন্যথা যদি (l.extra) {
        জন্য(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }অন্যথা{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ifdef GPU
শূন্য forward_reorg_layer_gpu(layer l, network net)
{
    পূর্ণসংখ্যা i;
    যদি(l.flatten){
        যদি(l.reverse){
            flatten_gpu(net.প্রবেশ_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }অন্যথা{
            flatten_gpu(net.প্রবেশ_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } অন্যথা যদি (l.extra) {
        জন্য(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.প্রবেশ_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } অন্যথা যদি (l.reverse) {
        reorg_gpu(net.প্রবেশ_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }অন্যথা {
        reorg_gpu(net.প্রবেশ_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

শূন্য backward_reorg_layer_gpu(layer l, network net)
{
    যদি(l.flatten){
        যদি(l.reverse){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }অন্যথা{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } অন্যথা যদি (l.extra) {
        পূর্ণসংখ্যা i;
        জন্য(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } অন্যথা যদি(l.reverse){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } অন্যথা {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#endif
