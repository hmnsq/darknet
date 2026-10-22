#ਅਨ੍ਤਰ੍ਭਾਵ "upsample_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

layer make_upsample_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c, ਪੂਰਨ stride)
{
    layer l = {0};
    l.type = UPSAMPLE;
    l.batch = batch;
    l.w = w;
    l.h = h;
    l.c = c;
    l.ਬਾਹਰਲਾ_w = w*stride;
    l.ਬਾਹਰਲਾ_h = h*stride;
    l.ਬਾਹਰਲਾ_c = c;
    ਜੇ(stride < 0){
        stride = -stride;
        l.ਉਲਟ=1;
        l.ਬਾਹਰਲਾ_w = w/stride;
        l.ਬਾਹਰਲਾ_h = h/stride;
    }
    l.stride = stride;
    l.outputs = l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_c;
    l.inputs = l.w*l.h*l.c;
    l.delta =  ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.outputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.outputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));;

    l.forward = forward_upsample_layer;
    l.backward = backward_upsample_layer;
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_upsample_layer_gpu;
    l.backward_gpu = backward_upsample_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #ਜੇ_ਅੰਤ
    ਜੇ(l.ਉਲਟ) fprintf(stderr, "downsample         %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.ਬਾਹਰਲਾ_w, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_c);
    ਨਹੀਂਤਾਂ fprintf(stderr, "upsample           %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.ਬਾਹਰਲਾ_w, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_c);
    ਵਾਪਸ l;
}

ਖਾਲੀ resize_upsample_layer(layer *l, ਪੂਰਨ w, ਪੂਰਨ h)
{
    l->w = w;
    l->h = h;
    l->ਬਾਹਰਲਾ_w = w*l->stride;
    l->ਬਾਹਰਲਾ_h = h*l->stride;
    ਜੇ(l->ਉਲਟ){
        l->ਬਾਹਰਲਾ_w = w/l->stride;
        l->ਬਾਹਰਲਾ_h = h/l->stride;
    }
    l->outputs = l->ਬਾਹਰਲਾ_w*l->ਬਾਹਰਲਾ_h*l->ਬਾਹਰਲਾ_c;
    l->inputs = l->h*l->w*l->c;
    l->delta =  realloc(l->delta, l->outputs*l->batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l->output = realloc(l->output, l->outputs*l->batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#ਜੇ_ਅੰਤ
    
}

ਖਾਲੀ forward_upsample_layer(ਅਚਲ layer l, ਜਾਲ net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    ਜੇ(l.ਉਲਟ){
        upsample_cpu(l.output, l.ਬਾਹਰਲਾ_w, l.ਬਾਹਰਲਾ_h, l.c, l.batch, l.stride, 0, l.scale, net.ਨਿਵੇਸ਼);
    }ਨਹੀਂਤਾਂ{
        upsample_cpu(net.ਨਿਵੇਸ਼, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output);
    }
}

ਖਾਲੀ backward_upsample_layer(ਅਚਲ layer l, ਜਾਲ net)
{
    ਜੇ(l.ਉਲਟ){
        upsample_cpu(l.delta, l.ਬਾਹਰਲਾ_w, l.ਬਾਹਰਲਾ_h, l.c, l.batch, l.stride, 1, l.scale, net.delta);
    }ਨਹੀਂਤਾਂ{
        upsample_cpu(net.delta, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta);
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_upsample_layer_gpu(ਅਚਲ layer l, ਜਾਲ net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);
    ਜੇ(l.ਉਲਟ){
        upsample_gpu(l.output_gpu, l.ਬਾਹਰਲਾ_w, l.ਬਾਹਰਲਾ_h, l.c, l.batch, l.stride, 0, l.scale, net.ਨਿਵੇਸ਼_gpu);
    }ਨਹੀਂਤਾਂ{
        upsample_gpu(net.ਨਿਵੇਸ਼_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output_gpu);
    }
}

ਖਾਲੀ backward_upsample_layer_gpu(ਅਚਲ layer l, ਜਾਲ net)
{
    ਜੇ(l.ਉਲਟ){
        upsample_gpu(l.delta_gpu, l.ਬਾਹਰਲਾ_w, l.ਬਾਹਰਲਾ_h, l.c, l.batch, l.stride, 1, l.scale, net.delta_gpu);
    }ਨਹੀਂਤਾਂ{
        upsample_gpu(net.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta_gpu);
    }
}
#ਜੇ_ਅੰਤ
