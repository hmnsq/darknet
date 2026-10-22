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
    l.ٻاهريون_w = w*stride;
    l.ٻاهريون_h = h*stride;
    l.ٻاهريون_c = c;
    إذا(stride < 0){
        stride = -stride;
        l.ابتو=1;
        l.ٻاهريون_w = w/stride;
        l.ٻاهريون_h = h/stride;
    }
    l.stride = stride;
    l.outputs = l.ٻاهريون_w*l.ٻاهريون_h*l.ٻاهريون_c;
    l.inputs = l.w*l.h*l.c;
    l.delta =  صفر_يادگيري_ڏيو(l.outputs*batch, حجم(عائم));
    l.output = صفر_يادگيري_ڏيو(l.outputs*batch, حجم(عائم));;

    l.forward = forward_upsample_layer;
    l.backward = backward_upsample_layer;
    #جيڪڏهن_وصف_ٿيل GPU
    l.forward_gpu = forward_upsample_layer_gpu;
    l.backward_gpu = backward_upsample_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #جيڪڏهن_پورو
    إذا(l.ابتو) fprintf(stderr, "downsample         %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.ٻاهريون_w, l.ٻاهريون_h, l.ٻاهريون_c);
    وإلا fprintf(stderr, "upsample           %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.ٻاهريون_w, l.ٻاهريون_h, l.ٻاهريون_c);
    أرجع l;
}

فراغ resize_upsample_layer(layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;
    l->ٻاهريون_w = w*l->stride;
    l->ٻاهريون_h = h*l->stride;
    إذا(l->ابتو){
        l->ٻاهريون_w = w/l->stride;
        l->ٻاهريون_h = h/l->stride;
    }
    l->outputs = l->ٻاهريون_w*l->ٻاهريون_h*l->ٻاهريون_c;
    l->inputs = l->h*l->w*l->c;
    l->delta =  realloc(l->delta, l->outputs*l->batch*حجم(عائم));
    l->output = realloc(l->output, l->outputs*l->batch*حجم(عائم));

#جيڪڏهن_وصف_ٿيل GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#جيڪڏهن_پورو
    
}

فراغ forward_upsample_layer(ثابت layer l, ڄار net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    إذا(l.ابتو){
        upsample_cpu(l.output, l.ٻاهريون_w, l.ٻاهريون_h, l.c, l.batch, l.stride, 0, l.scale, net.داخلا);
    }وإلا{
        upsample_cpu(net.داخلا, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output);
    }
}

فراغ backward_upsample_layer(ثابت layer l, ڄار net)
{
    إذا(l.ابتو){
        upsample_cpu(l.delta, l.ٻاهريون_w, l.ٻاهريون_h, l.c, l.batch, l.stride, 1, l.scale, net.delta);
    }وإلا{
        upsample_cpu(net.delta, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta);
    }
}

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_upsample_layer_gpu(ثابت layer l, ڄار net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);
    إذا(l.ابتو){
        upsample_gpu(l.output_gpu, l.ٻاهريون_w, l.ٻاهريون_h, l.c, l.batch, l.stride, 0, l.scale, net.داخلا_gpu);
    }وإلا{
        upsample_gpu(net.داخلا_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output_gpu);
    }
}

فراغ backward_upsample_layer_gpu(ثابت layer l, ڄار net)
{
    إذا(l.ابتو){
        upsample_gpu(l.delta_gpu, l.ٻاهريون_w, l.ٻاهريون_h, l.c, l.batch, l.stride, 1, l.scale, net.delta_gpu);
    }وإلا{
        upsample_gpu(net.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta_gpu);
    }
}
#جيڪڏهن_پورو
