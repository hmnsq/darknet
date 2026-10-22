#அந்தர்பாவ "reorg_layer.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "blas.h"

#அந்தர்பாவ <stdio.h>


layer make_reorg_layer(முழு batch, முழு w, முழு h, முழு c, முழு stride, முழு reverse, முழு flatten, முழு extra)
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
    எனில்(reverse){
        l.out_w = w*stride;
        l.out_h = h*stride;
        l.out_c = c/(stride*stride);
    }இல்லையெனில்{
        l.out_w = w/stride;
        l.out_h = h/stride;
        l.out_c = c*(stride*stride);
    }
    l.reverse = reverse;

    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    எனில்(l.extra){
        l.out_w = l.out_h = l.out_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    எனில்(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } இல்லையெனில் {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.out_w, l.out_h, l.out_c);
    }
    முழு output_size = l.outputs * batch;
    l.output =  calloc(output_size, அளவு(தசம));
    l.delta =   calloc(output_size, அளவு(தசம));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#ifdef GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
#endif
    திரும்பு l;
}

வெறுமை resize_reorg_layer(layer *l, முழு w, முழு h)
{
    முழு stride = l->stride;
    முழு c = l->c;

    l->h = h;
    l->w = w;

    எனில்(l->reverse){
        l->out_w = w*stride;
        l->out_h = h*stride;
        l->out_c = c/(stride*stride);
    }இல்லையெனில்{
        l->out_w = w/stride;
        l->out_h = h/stride;
        l->out_c = c*(stride*stride);
    }

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->outputs;
    முழு output_size = l->outputs * l->batch;

    l->output = realloc(l->output, output_size * அளவு(தசம));
    l->delta = realloc(l->delta, output_size * அளவு(தசம));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
#endif
}

வெறுமை forward_reorg_layer(மாறிலி layer l, network net)
{
    முழு i;
    எனில்(l.flatten){
        memcpy(l.output, net.உள்ளீடு, l.outputs*l.batch*அளவு(தசம));
        எனில்(l.reverse){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }இல்லையெனில்{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } இல்லையெனில் எனில் (l.extra) {
        க்கு(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.உள்ளீடு + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } இல்லையெனில் எனில் (l.reverse){
        reorg_cpu(net.உள்ளீடு, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } இல்லையெனில் {
        reorg_cpu(net.உள்ளீடு, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

வெறுமை backward_reorg_layer(மாறிலி layer l, network net)
{
    முழு i;
    எனில்(l.flatten){
        memcpy(net.delta, l.delta, l.outputs*l.batch*அளவு(தசம));
        எனில்(l.reverse){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }இல்லையெனில்{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } இல்லையெனில் எனில்(l.reverse){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } இல்லையெனில் எனில் (l.extra) {
        க்கு(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }இல்லையெனில்{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ifdef GPU
வெறுமை forward_reorg_layer_gpu(layer l, network net)
{
    முழு i;
    எனில்(l.flatten){
        எனில்(l.reverse){
            flatten_gpu(net.உள்ளீடு_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }இல்லையெனில்{
            flatten_gpu(net.உள்ளீடு_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } இல்லையெனில் எனில் (l.extra) {
        க்கு(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.உள்ளீடு_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } இல்லையெனில் எனில் (l.reverse) {
        reorg_gpu(net.உள்ளீடு_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }இல்லையெனில் {
        reorg_gpu(net.உள்ளீடு_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

வெறுமை backward_reorg_layer_gpu(layer l, network net)
{
    எனில்(l.flatten){
        எனில்(l.reverse){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }இல்லையெனில்{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } இல்லையெனில் எனில் (l.extra) {
        முழு i;
        க்கு(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } இல்லையெனில் எனில்(l.reverse){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } இல்லையெனில் {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#endif
