#شامل "reorg_layer.h"
#شامل "cuda.h"
#شامل "blas.h"

#شامل <stdio.h>


layer make_reorg_layer(صحیح batch, صحیح w, صحیح h, صحیح c, صحیح stride, صحیح الٹ, صحیح flatten, صحیح extra)
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
    اگر(الٹ){
        l.برآمد_w = w*stride;
        l.برآمد_h = h*stride;
        l.برآمد_c = c/(stride*stride);
    }ورنہ{
        l.برآمد_w = w/stride;
        l.برآمد_h = h/stride;
        l.برآمد_c = c*(stride*stride);
    }
    l.الٹ = الٹ;

    l.outputs = l.برآمد_h * l.برآمد_w * l.برآمد_c;
    l.inputs = h*w*c;
    اگر(l.extra){
        l.برآمد_w = l.برآمد_h = l.برآمد_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    اگر(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } ورنہ {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.برآمد_w, l.برآمد_h, l.برآمد_c);
    }
    صحیح output_سائز = l.outputs * batch;
    l.output =  صفر_حافظہ_دو(output_سائز, حجم(اعشاری));
    l.delta =   صفر_حافظہ_دو(output_سائز, حجم(اعشاری));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_سائز);
    l.delta_gpu   = cuda_make_array(l.delta, output_سائز);
#اگر_ختم
    واپس l;
}

خالی resize_reorg_layer(layer *l, صحیح w, صحیح h)
{
    صحیح stride = l->stride;
    صحیح c = l->c;

    l->h = h;
    l->w = w;

    اگر(l->الٹ){
        l->برآمد_w = w*stride;
        l->برآمد_h = h*stride;
        l->برآمد_c = c/(stride*stride);
    }ورنہ{
        l->برآمد_w = w/stride;
        l->برآمد_h = h/stride;
        l->برآمد_c = c*(stride*stride);
    }

    l->outputs = l->برآمد_h * l->برآمد_w * l->برآمد_c;
    l->inputs = l->outputs;
    صحیح output_سائز = l->outputs * l->batch;

    l->output = realloc(l->output, output_سائز * حجم(اعشاری));
    l->delta = realloc(l->delta, output_سائز * حجم(اعشاری));

#اگر_تعریف_شدہ GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_سائز);
    l->delta_gpu   = cuda_make_array(l->delta,  output_سائز);
#اگر_ختم
}

خالی forward_reorg_layer(ثابت layer l, جال net)
{
    صحیح i;
    اگر(l.flatten){
        حافظہ_نقل(l.output, net.داخلہ, l.outputs*l.batch*حجم(اعشاری));
        اگر(l.الٹ){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }ورنہ{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } ورنہ اگر (l.extra) {
        برائے(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.داخلہ + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } ورنہ اگر (l.الٹ){
        reorg_cpu(net.داخلہ, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } ورنہ {
        reorg_cpu(net.داخلہ, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

خالی backward_reorg_layer(ثابت layer l, جال net)
{
    صحیح i;
    اگر(l.flatten){
        حافظہ_نقل(net.delta, l.delta, l.outputs*l.batch*حجم(اعشاری));
        اگر(l.الٹ){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }ورنہ{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } ورنہ اگر(l.الٹ){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } ورنہ اگر (l.extra) {
        برائے(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }ورنہ{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#اگر_تعریف_شدہ GPU
خالی forward_reorg_layer_gpu(layer l, جال net)
{
    صحیح i;
    اگر(l.flatten){
        اگر(l.الٹ){
            flatten_gpu(net.داخلہ_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }ورنہ{
            flatten_gpu(net.داخلہ_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } ورنہ اگر (l.extra) {
        برائے(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.داخلہ_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } ورنہ اگر (l.الٹ) {
        reorg_gpu(net.داخلہ_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }ورنہ {
        reorg_gpu(net.داخلہ_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

خالی backward_reorg_layer_gpu(layer l, جال net)
{
    اگر(l.flatten){
        اگر(l.الٹ){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }ورنہ{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } ورنہ اگر (l.extra) {
        صحیح i;
        برائے(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } ورنہ اگر(l.الٹ){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } ورنہ {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#اگر_ختم
