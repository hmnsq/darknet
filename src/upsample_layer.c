#அந்தர்பாவ "upsample_layer.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "blas.h"

#அந்தர்பாவ <stdio.h>

layer make_upsample_layer(முழு batch, முழு w, முழு h, முழு c, முழு stride)
{
    layer l = {0};
    l.type = UPSAMPLE;
    l.batch = batch;
    l.w = w;
    l.h = h;
    l.c = c;
    l.out_w = w*stride;
    l.out_h = h*stride;
    l.out_c = c;
    எனில்(stride < 0){
        stride = -stride;
        l.reverse=1;
        l.out_w = w/stride;
        l.out_h = h/stride;
    }
    l.stride = stride;
    l.outputs = l.out_w*l.out_h*l.out_c;
    l.inputs = l.w*l.h*l.c;
    l.delta =  calloc(l.outputs*batch, அளவு(தசம));
    l.output = calloc(l.outputs*batch, அளவு(தசம));;

    l.forward = forward_upsample_layer;
    l.backward = backward_upsample_layer;
    #ifdef GPU
    l.forward_gpu = forward_upsample_layer_gpu;
    l.backward_gpu = backward_upsample_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #endif
    எனில்(l.reverse) fprintf(stderr, "downsample         %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.out_w, l.out_h, l.out_c);
    இல்லையெனில் fprintf(stderr, "upsample           %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.out_w, l.out_h, l.out_c);
    திரும்பு l;
}

வெறுமை resize_upsample_layer(layer *l, முழு w, முழு h)
{
    l->w = w;
    l->h = h;
    l->out_w = w*l->stride;
    l->out_h = h*l->stride;
    எனில்(l->reverse){
        l->out_w = w/l->stride;
        l->out_h = h/l->stride;
    }
    l->outputs = l->out_w*l->out_h*l->out_c;
    l->inputs = l->h*l->w*l->c;
    l->delta =  realloc(l->delta, l->outputs*l->batch*அளவு(தசம));
    l->output = realloc(l->output, l->outputs*l->batch*அளவு(தசம));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

வெறுமை forward_upsample_layer(மாறிலி layer l, network net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    எனில்(l.reverse){
        upsample_cpu(l.output, l.out_w, l.out_h, l.c, l.batch, l.stride, 0, l.scale, net.உள்ளீடு);
    }இல்லையெனில்{
        upsample_cpu(net.உள்ளீடு, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output);
    }
}

வெறுமை backward_upsample_layer(மாறிலி layer l, network net)
{
    எனில்(l.reverse){
        upsample_cpu(l.delta, l.out_w, l.out_h, l.c, l.batch, l.stride, 1, l.scale, net.delta);
    }இல்லையெனில்{
        upsample_cpu(net.delta, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta);
    }
}

#ifdef GPU
வெறுமை forward_upsample_layer_gpu(மாறிலி layer l, network net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);
    எனில்(l.reverse){
        upsample_gpu(l.output_gpu, l.out_w, l.out_h, l.c, l.batch, l.stride, 0, l.scale, net.உள்ளீடு_gpu);
    }இல்லையெனில்{
        upsample_gpu(net.உள்ளீடு_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output_gpu);
    }
}

வெறுமை backward_upsample_layer_gpu(மாறிலி layer l, network net)
{
    எனில்(l.reverse){
        upsample_gpu(l.delta_gpu, l.out_w, l.out_h, l.c, l.batch, l.stride, 1, l.scale, net.delta_gpu);
    }இல்லையெனில்{
        upsample_gpu(net.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta_gpu);
    }
}
#endif
