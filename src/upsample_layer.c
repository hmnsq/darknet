#ضمن "upsample_layer.h"
#ضمن "cuda.h"
#ضمن "blas.h"

#ضمن <stdio.h>

layer make_upsample_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح stride)
{
    layer l = {0};
    l.type = UPSAMPLE;
    l.batch = batch;
    l.w = w;
    l.h = h;
    l.c = c;
    l.بیرون_w = w*stride;
    l.بیرون_h = h*stride;
    l.بیرون_c = c;
    إذا(stride < 0){
        stride = -stride;
        l.معکوس=1;
        l.بیرون_w = w/stride;
        l.بیرون_h = h/stride;
    }
    l.stride = stride;
    l.outputs = l.بیرون_w*l.بیرون_h*l.بیرون_c;
    l.inputs = l.w*l.h*l.c;
    l.delta =  حافظه_صفر_بده(l.outputs*batch, حجم(عائم));
    l.output = حافظه_صفر_بده(l.outputs*batch, حجم(عائم));;

    l.forward = forward_upsample_layer;
    l.backward = backward_upsample_layer;
    #اگر_تعریف_شده GPU
    l.forward_gpu = forward_upsample_layer_gpu;
    l.backward_gpu = backward_upsample_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #پایان_اگر
    إذا(l.معکوس) fprintf(stderr, "downsample         %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.بیرون_w, l.بیرون_h, l.بیرون_c);
    وإلا fprintf(stderr, "upsample           %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.بیرون_w, l.بیرون_h, l.بیرون_c);
    أرجع l;
}

فراغ resize_upsample_layer(layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;
    l->بیرون_w = w*l->stride;
    l->بیرون_h = h*l->stride;
    إذا(l->معکوس){
        l->بیرون_w = w/l->stride;
        l->بیرون_h = h/l->stride;
    }
    l->outputs = l->بیرون_w*l->بیرون_h*l->بیرون_c;
    l->inputs = l->h*l->w*l->c;
    l->delta =  realloc(l->delta, l->outputs*l->batch*حجم(عائم));
    l->output = realloc(l->output, l->outputs*l->batch*حجم(عائم));

#اگر_تعریف_شده GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#پایان_اگر
    
}

فراغ forward_upsample_layer(ثابت layer l, شبکه net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    إذا(l.معکوس){
        upsample_cpu(l.output, l.بیرون_w, l.بیرون_h, l.c, l.batch, l.stride, 0, l.scale, net.درون_داد);
    }وإلا{
        upsample_cpu(net.درون_داد, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output);
    }
}

فراغ backward_upsample_layer(ثابت layer l, شبکه net)
{
    إذا(l.معکوس){
        upsample_cpu(l.delta, l.بیرون_w, l.بیرون_h, l.c, l.batch, l.stride, 1, l.scale, net.delta);
    }وإلا{
        upsample_cpu(net.delta, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta);
    }
}

#اگر_تعریف_شده GPU
فراغ forward_upsample_layer_gpu(ثابت layer l, شبکه net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);
    إذا(l.معکوس){
        upsample_gpu(l.output_gpu, l.بیرون_w, l.بیرون_h, l.c, l.batch, l.stride, 0, l.scale, net.درون_داد_gpu);
    }وإلا{
        upsample_gpu(net.درون_داد_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output_gpu);
    }
}

فراغ backward_upsample_layer_gpu(ثابت layer l, شبکه net)
{
    إذا(l.معکوس){
        upsample_gpu(l.delta_gpu, l.بیرون_w, l.بیرون_h, l.c, l.batch, l.stride, 1, l.scale, net.delta_gpu);
    }وإلا{
        upsample_gpu(net.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta_gpu);
    }
}
#پایان_اگر
