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
    l.باہرلا_w = w*stride;
    l.باہرلا_h = h*stride;
    l.باہرلا_c = c;
    إذا(stride < 0){
        stride = -stride;
        l.الٹ=1;
        l.باہرلا_w = w/stride;
        l.باہرلا_h = h/stride;
    }
    l.stride = stride;
    l.outputs = l.باہرلا_w*l.باہرلا_h*l.باہرلا_c;
    l.inputs = l.w*l.h*l.c;
    l.delta =  صفر_حافظہ_دیو(l.outputs*batch, حجم(عائم));
    l.output = صفر_حافظہ_دیو(l.outputs*batch, حجم(عائم));;

    l.forward = forward_upsample_layer;
    l.backward = backward_upsample_layer;
    #جے_تعریف_شدہ GPU
    l.forward_gpu = forward_upsample_layer_gpu;
    l.backward_gpu = backward_upsample_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #جے_انت
    إذا(l.الٹ) fprintf(stderr, "downsample         %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.باہرلا_w, l.باہرلا_h, l.باہرلا_c);
    وإلا fprintf(stderr, "upsample           %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.باہرلا_w, l.باہرلا_h, l.باہرلا_c);
    أرجع l;
}

فراغ resize_upsample_layer(layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;
    l->باہرلا_w = w*l->stride;
    l->باہرلا_h = h*l->stride;
    إذا(l->الٹ){
        l->باہرلا_w = w/l->stride;
        l->باہرلا_h = h/l->stride;
    }
    l->outputs = l->باہرلا_w*l->باہرلا_h*l->باہرلا_c;
    l->inputs = l->h*l->w*l->c;
    l->delta =  realloc(l->delta, l->outputs*l->batch*حجم(عائم));
    l->output = realloc(l->output, l->outputs*l->batch*حجم(عائم));

#جے_تعریف_شدہ GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#جے_انت
    
}

فراغ forward_upsample_layer(ثابت layer l, جال net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    إذا(l.الٹ){
        upsample_cpu(l.output, l.باہرلا_w, l.باہرلا_h, l.c, l.batch, l.stride, 0, l.scale, net.داخلہ);
    }وإلا{
        upsample_cpu(net.داخلہ, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output);
    }
}

فراغ backward_upsample_layer(ثابت layer l, جال net)
{
    إذا(l.الٹ){
        upsample_cpu(l.delta, l.باہرلا_w, l.باہرلا_h, l.c, l.batch, l.stride, 1, l.scale, net.delta);
    }وإلا{
        upsample_cpu(net.delta, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta);
    }
}

#جے_تعریف_شدہ GPU
فراغ forward_upsample_layer_gpu(ثابت layer l, جال net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);
    إذا(l.الٹ){
        upsample_gpu(l.output_gpu, l.باہرلا_w, l.باہرلا_h, l.c, l.batch, l.stride, 0, l.scale, net.داخلہ_gpu);
    }وإلا{
        upsample_gpu(net.داخلہ_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output_gpu);
    }
}

فراغ backward_upsample_layer_gpu(ثابت layer l, جال net)
{
    إذا(l.الٹ){
        upsample_gpu(l.delta_gpu, l.باہرلا_w, l.باہرلا_h, l.c, l.batch, l.stride, 1, l.scale, net.delta_gpu);
    }وإلا{
        upsample_gpu(net.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta_gpu);
    }
}
#جے_انت
