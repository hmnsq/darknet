#ଅନ୍ତର୍ଭାଵ "reorg_layer.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"

#ଅନ୍ତର୍ଭାଵ <stdio.h>


layer make_reorg_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା reverse, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flatten, ପୂର୍ଣ୍ଣସଂଖ୍ୟା extra)
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
    ଯଦି(reverse){
        l.out_w = w*stride;
        l.out_h = h*stride;
        l.out_c = c/(stride*stride);
    }ଅନ୍ୟଥା{
        l.out_w = w/stride;
        l.out_h = h/stride;
        l.out_c = c*(stride*stride);
    }
    l.reverse = reverse;

    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    ଯଦି(l.extra){
        l.out_w = l.out_h = l.out_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    ଯଦି(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } ଅନ୍ୟଥା {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.out_w, l.out_h, l.out_c);
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_size = l.outputs * batch;
    l.output =  calloc(output_size, ଆକାର(ଦଶମିକ));
    l.delta =   calloc(output_size, ଆକାର(ଦଶମିକ));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#ifdef GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
#endif
    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ resize_reorg_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride = l->stride;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = l->c;

    l->h = h;
    l->w = w;

    ଯଦି(l->reverse){
        l->out_w = w*stride;
        l->out_h = h*stride;
        l->out_c = c/(stride*stride);
    }ଅନ୍ୟଥା{
        l->out_w = w/stride;
        l->out_h = h/stride;
        l->out_c = c*(stride*stride);
    }

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->outputs;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_size = l->outputs * l->batch;

    l->output = realloc(l->output, output_size * ଆକାର(ଦଶମିକ));
    l->delta = realloc(l->delta, output_size * ଆକାର(ଦଶମିକ));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
#endif
}

ଶୂନ୍ୟ forward_reorg_layer(ଅଚଳ layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଯଦି(l.flatten){
        memcpy(l.output, net.ନିବେଶ, l.outputs*l.batch*ଆକାର(ଦଶମିକ));
        ଯଦି(l.reverse){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }ଅନ୍ୟଥା{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } ଅନ୍ୟଥା ଯଦି (l.extra) {
        ପାଇଁ(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.ନିବେଶ + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } ଅନ୍ୟଥା ଯଦି (l.reverse){
        reorg_cpu(net.ନିବେଶ, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } ଅନ୍ୟଥା {
        reorg_cpu(net.ନିବେଶ, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

ଶୂନ୍ୟ backward_reorg_layer(ଅଚଳ layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଯଦି(l.flatten){
        memcpy(net.delta, l.delta, l.outputs*l.batch*ଆକାର(ଦଶମିକ));
        ଯଦି(l.reverse){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }ଅନ୍ୟଥା{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } ଅନ୍ୟଥା ଯଦି(l.reverse){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } ଅନ୍ୟଥା ଯଦି (l.extra) {
        ପାଇଁ(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }ଅନ୍ୟଥା{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ifdef GPU
ଶୂନ୍ୟ forward_reorg_layer_gpu(layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଯଦି(l.flatten){
        ଯଦି(l.reverse){
            flatten_gpu(net.ନିବେଶ_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }ଅନ୍ୟଥା{
            flatten_gpu(net.ନିବେଶ_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } ଅନ୍ୟଥା ଯଦି (l.extra) {
        ପାଇଁ(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.ନିବେଶ_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } ଅନ୍ୟଥା ଯଦି (l.reverse) {
        reorg_gpu(net.ନିବେଶ_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }ଅନ୍ୟଥା {
        reorg_gpu(net.ନିବେଶ_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

ଶୂନ୍ୟ backward_reorg_layer_gpu(layer l, network net)
{
    ଯଦି(l.flatten){
        ଯଦି(l.reverse){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }ଅନ୍ୟଥା{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } ଅନ୍ୟଥା ଯଦି (l.extra) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
        ପାଇଁ(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } ଅନ୍ୟଥା ଯଦି(l.reverse){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } ଅନ୍ୟଥା {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#endif
