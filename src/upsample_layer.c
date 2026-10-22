#ଅନ୍ତର୍ଭାଵ "upsample_layer.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"

#ଅନ୍ତର୍ଭାଵ <stdio.h>

layer make_upsample_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride)
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
    ଯଦି(stride < 0){
        stride = -stride;
        l.reverse=1;
        l.out_w = w/stride;
        l.out_h = h/stride;
    }
    l.stride = stride;
    l.outputs = l.out_w*l.out_h*l.out_c;
    l.inputs = l.w*l.h*l.c;
    l.delta =  calloc(l.outputs*batch, ଆକାର(ଦଶମିକ));
    l.output = calloc(l.outputs*batch, ଆକାର(ଦଶମିକ));;

    l.forward = forward_upsample_layer;
    l.backward = backward_upsample_layer;
    #ifdef GPU
    l.forward_gpu = forward_upsample_layer_gpu;
    l.backward_gpu = backward_upsample_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #endif
    ଯଦି(l.reverse) fprintf(stderr, "downsample         %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.out_w, l.out_h, l.out_c);
    ଅନ୍ୟଥା fprintf(stderr, "upsample           %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.out_w, l.out_h, l.out_c);
    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ resize_upsample_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    l->w = w;
    l->h = h;
    l->out_w = w*l->stride;
    l->out_h = h*l->stride;
    ଯଦି(l->reverse){
        l->out_w = w/l->stride;
        l->out_h = h/l->stride;
    }
    l->outputs = l->out_w*l->out_h*l->out_c;
    l->inputs = l->h*l->w*l->c;
    l->delta =  realloc(l->delta, l->outputs*l->batch*ଆକାର(ଦଶମିକ));
    l->output = realloc(l->output, l->outputs*l->batch*ଆକାର(ଦଶମିକ));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

ଶୂନ୍ୟ forward_upsample_layer(ଅଚଳ layer l, network net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    ଯଦି(l.reverse){
        upsample_cpu(l.output, l.out_w, l.out_h, l.c, l.batch, l.stride, 0, l.scale, net.ନିବେଶ);
    }ଅନ୍ୟଥା{
        upsample_cpu(net.ନିବେଶ, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output);
    }
}

ଶୂନ୍ୟ backward_upsample_layer(ଅଚଳ layer l, network net)
{
    ଯଦି(l.reverse){
        upsample_cpu(l.delta, l.out_w, l.out_h, l.c, l.batch, l.stride, 1, l.scale, net.delta);
    }ଅନ୍ୟଥା{
        upsample_cpu(net.delta, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta);
    }
}

#ifdef GPU
ଶୂନ୍ୟ forward_upsample_layer_gpu(ଅଚଳ layer l, network net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);
    ଯଦି(l.reverse){
        upsample_gpu(l.output_gpu, l.out_w, l.out_h, l.c, l.batch, l.stride, 0, l.scale, net.ନିବେଶ_gpu);
    }ଅନ୍ୟଥା{
        upsample_gpu(net.ନିବେଶ_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output_gpu);
    }
}

ଶୂନ୍ୟ backward_upsample_layer_gpu(ଅଚଳ layer l, network net)
{
    ଯଦି(l.reverse){
        upsample_gpu(l.delta_gpu, l.out_w, l.out_h, l.c, l.batch, l.stride, 1, l.scale, net.delta_gpu);
    }ଅନ୍ୟଥା{
        upsample_gpu(net.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta_gpu);
    }
}
#endif
