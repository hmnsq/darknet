#અન્તર્ભાવ "upsample_layer.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "blas.h"

#અન્તર્ભાવ <stdio.h>

layer make_upsample_layer(પૂર્ણાંક batch, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c, પૂર્ણાંક stride)
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
    જો(stride < 0){
        stride = -stride;
        l.reverse=1;
        l.out_w = w/stride;
        l.out_h = h/stride;
    }
    l.stride = stride;
    l.outputs = l.out_w*l.out_h*l.out_c;
    l.inputs = l.w*l.h*l.c;
    l.delta =  calloc(l.outputs*batch, કદ(દશાંશ));
    l.output = calloc(l.outputs*batch, કદ(દશાંશ));;

    l.forward = forward_upsample_layer;
    l.backward = backward_upsample_layer;
    #ifdef GPU
    l.forward_gpu = forward_upsample_layer_gpu;
    l.backward_gpu = backward_upsample_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #endif
    જો(l.reverse) fprintf(stderr, "downsample         %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.out_w, l.out_h, l.out_c);
    નહીંતર fprintf(stderr, "upsample           %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.out_w, l.out_h, l.out_c);
    પાછા l;
}

શૂન્ય resize_upsample_layer(layer *l, પૂર્ણાંક w, પૂર્ણાંક h)
{
    l->w = w;
    l->h = h;
    l->out_w = w*l->stride;
    l->out_h = h*l->stride;
    જો(l->reverse){
        l->out_w = w/l->stride;
        l->out_h = h/l->stride;
    }
    l->outputs = l->out_w*l->out_h*l->out_c;
    l->inputs = l->h*l->w*l->c;
    l->delta =  realloc(l->delta, l->outputs*l->batch*કદ(દશાંશ));
    l->output = realloc(l->output, l->outputs*l->batch*કદ(દશાંશ));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

શૂન્ય forward_upsample_layer(અચલ layer l, network net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    જો(l.reverse){
        upsample_cpu(l.output, l.out_w, l.out_h, l.c, l.batch, l.stride, 0, l.scale, net.આગત);
    }નહીંતર{
        upsample_cpu(net.આગત, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output);
    }
}

શૂન્ય backward_upsample_layer(અચલ layer l, network net)
{
    જો(l.reverse){
        upsample_cpu(l.delta, l.out_w, l.out_h, l.c, l.batch, l.stride, 1, l.scale, net.delta);
    }નહીંતર{
        upsample_cpu(net.delta, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta);
    }
}

#ifdef GPU
શૂન્ય forward_upsample_layer_gpu(અચલ layer l, network net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);
    જો(l.reverse){
        upsample_gpu(l.output_gpu, l.out_w, l.out_h, l.c, l.batch, l.stride, 0, l.scale, net.આગત_gpu);
    }નહીંતર{
        upsample_gpu(net.આગત_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output_gpu);
    }
}

શૂન્ય backward_upsample_layer_gpu(અચલ layer l, network net)
{
    જો(l.reverse){
        upsample_gpu(l.delta_gpu, l.out_w, l.out_h, l.c, l.batch, l.stride, 1, l.scale, net.delta_gpu);
    }નહીંતર{
        upsample_gpu(net.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta_gpu);
    }
}
#endif
