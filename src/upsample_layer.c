#অন্তর্ভাব "upsample_layer.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "blas.h"

#অন্তর্ভাব <stdio.h>

layer make_upsample_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c, পূর্ণাঙ্ক stride)
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
    যদি(stride < 0){
        stride = -stride;
        l.reverse=1;
        l.out_w = w/stride;
        l.out_h = h/stride;
    }
    l.stride = stride;
    l.outputs = l.out_w*l.out_h*l.out_c;
    l.inputs = l.w*l.h*l.c;
    l.delta =  calloc(l.outputs*batch, পরিমাণ(ভিন্ন));
    l.output = calloc(l.outputs*batch, পরিমাণ(ভিন্ন));;

    l.forward = forward_upsample_layer;
    l.backward = backward_upsample_layer;
    #ifdef GPU
    l.forward_gpu = forward_upsample_layer_gpu;
    l.backward_gpu = backward_upsample_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #endif
    যদি(l.reverse) fprintf(stderr, "downsample         %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.out_w, l.out_h, l.out_c);
    অন্যথা fprintf(stderr, "upsample           %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.out_w, l.out_h, l.out_c);
    প্রত্যাগম l;
}

শূন্য resize_upsample_layer(layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    l->w = w;
    l->h = h;
    l->out_w = w*l->stride;
    l->out_h = h*l->stride;
    যদি(l->reverse){
        l->out_w = w/l->stride;
        l->out_h = h/l->stride;
    }
    l->outputs = l->out_w*l->out_h*l->out_c;
    l->inputs = l->h*l->w*l->c;
    l->delta =  realloc(l->delta, l->outputs*l->batch*পরিমাণ(ভিন্ন));
    l->output = realloc(l->output, l->outputs*l->batch*পরিমাণ(ভিন্ন));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

শূন্য forward_upsample_layer(ধ্রুব layer l, network net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    যদি(l.reverse){
        upsample_cpu(l.output, l.out_w, l.out_h, l.c, l.batch, l.stride, 0, l.scale, net.প্রবেশ);
    }অন্যথা{
        upsample_cpu(net.প্রবেশ, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output);
    }
}

শূন্য backward_upsample_layer(ধ্রুব layer l, network net)
{
    যদি(l.reverse){
        upsample_cpu(l.delta, l.out_w, l.out_h, l.c, l.batch, l.stride, 1, l.scale, net.delta);
    }অন্যথা{
        upsample_cpu(net.delta, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta);
    }
}

#ifdef GPU
শূন্য forward_upsample_layer_gpu(ধ্রুব layer l, network net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);
    যদি(l.reverse){
        upsample_gpu(l.output_gpu, l.out_w, l.out_h, l.c, l.batch, l.stride, 0, l.scale, net.প্রবেশ_gpu);
    }অন্যথা{
        upsample_gpu(net.প্রবেশ_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output_gpu);
    }
}

শূন্য backward_upsample_layer_gpu(ধ্রুব layer l, network net)
{
    যদি(l.reverse){
        upsample_gpu(l.delta_gpu, l.out_w, l.out_h, l.c, l.batch, l.stride, 1, l.scale, net.delta_gpu);
    }অন্যথা{
        upsample_gpu(net.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta_gpu);
    }
}
#endif
