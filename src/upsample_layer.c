#شامل "upsample_layer.h"
#شامل "cuda.h"
#شامل "blas.h"

#شامل <stdio.h>

layer make_upsample_layer(صحیح batch, صحیح w, صحیح h, صحیح c, صحیح stride)
{
    layer l = {0};
    l.type = UPSAMPLE;
    l.batch = batch;
    l.w = w;
    l.h = h;
    l.c = c;
    l.برآمد_w = w*stride;
    l.برآمد_h = h*stride;
    l.برآمد_c = c;
    اگر(stride < 0){
        stride = -stride;
        l.الٹ=1;
        l.برآمد_w = w/stride;
        l.برآمد_h = h/stride;
    }
    l.stride = stride;
    l.outputs = l.برآمد_w*l.برآمد_h*l.برآمد_c;
    l.inputs = l.w*l.h*l.c;
    l.delta =  صفر_حافظہ_دو(l.outputs*batch, حجم(اعشاری));
    l.output = صفر_حافظہ_دو(l.outputs*batch, حجم(اعشاری));;

    l.forward = forward_upsample_layer;
    l.backward = backward_upsample_layer;
    #اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_upsample_layer_gpu;
    l.backward_gpu = backward_upsample_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #اگر_ختم
    اگر(l.الٹ) fprintf(stderr, "downsample         %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.برآمد_w, l.برآمد_h, l.برآمد_c);
    ورنہ fprintf(stderr, "upsample           %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.برآمد_w, l.برآمد_h, l.برآمد_c);
    واپس l;
}

خالی resize_upsample_layer(layer *l, صحیح w, صحیح h)
{
    l->w = w;
    l->h = h;
    l->برآمد_w = w*l->stride;
    l->برآمد_h = h*l->stride;
    اگر(l->الٹ){
        l->برآمد_w = w/l->stride;
        l->برآمد_h = h/l->stride;
    }
    l->outputs = l->برآمد_w*l->برآمد_h*l->برآمد_c;
    l->inputs = l->h*l->w*l->c;
    l->delta =  realloc(l->delta, l->outputs*l->batch*حجم(اعشاری));
    l->output = realloc(l->output, l->outputs*l->batch*حجم(اعشاری));

#اگر_تعریف_شدہ GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#اگر_ختم
    
}

خالی forward_upsample_layer(ثابت layer l, جال net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    اگر(l.الٹ){
        upsample_cpu(l.output, l.برآمد_w, l.برآمد_h, l.c, l.batch, l.stride, 0, l.scale, net.داخلہ);
    }ورنہ{
        upsample_cpu(net.داخلہ, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output);
    }
}

خالی backward_upsample_layer(ثابت layer l, جال net)
{
    اگر(l.الٹ){
        upsample_cpu(l.delta, l.برآمد_w, l.برآمد_h, l.c, l.batch, l.stride, 1, l.scale, net.delta);
    }ورنہ{
        upsample_cpu(net.delta, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta);
    }
}

#اگر_تعریف_شدہ GPU
خالی forward_upsample_layer_gpu(ثابت layer l, جال net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);
    اگر(l.الٹ){
        upsample_gpu(l.output_gpu, l.برآمد_w, l.برآمد_h, l.c, l.batch, l.stride, 0, l.scale, net.داخلہ_gpu);
    }ورنہ{
        upsample_gpu(net.داخلہ_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output_gpu);
    }
}

خالی backward_upsample_layer_gpu(ثابت layer l, جال net)
{
    اگر(l.الٹ){
        upsample_gpu(l.delta_gpu, l.برآمد_w, l.برآمد_h, l.c, l.batch, l.stride, 1, l.scale, net.delta_gpu);
    }ورنہ{
        upsample_gpu(net.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta_gpu);
    }
}
#اگر_ختم
