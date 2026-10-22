#ಅನ್ತರ್ಭಾವ "upsample_layer.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "blas.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>

layer make_upsample_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ stride)
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
    ಯದಿ(stride < 0){
        stride = -stride;
        l.reverse=1;
        l.out_w = w/stride;
        l.out_h = h/stride;
    }
    l.stride = stride;
    l.outputs = l.out_w*l.out_h*l.out_c;
    l.inputs = l.w*l.h*l.c;
    l.delta =  calloc(l.outputs*batch, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.output = calloc(l.outputs*batch, ಪರಿಮಾಣ(ಭಿನ್ನ));;

    l.forward = forward_upsample_layer;
    l.backward = backward_upsample_layer;
    #ifdef GPU
    l.forward_gpu = forward_upsample_layer_gpu;
    l.backward_gpu = backward_upsample_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #endif
    ಯದಿ(l.reverse) fprintf(stderr, "downsample         %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.out_w, l.out_h, l.out_c);
    ಅನ್ಯಥಾ fprintf(stderr, "upsample           %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.out_w, l.out_h, l.out_c);
    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ resize_upsample_layer(layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    l->w = w;
    l->h = h;
    l->out_w = w*l->stride;
    l->out_h = h*l->stride;
    ಯದಿ(l->reverse){
        l->out_w = w/l->stride;
        l->out_h = h/l->stride;
    }
    l->outputs = l->out_w*l->out_h*l->out_c;
    l->inputs = l->h*l->w*l->c;
    l->delta =  realloc(l->delta, l->outputs*l->batch*ಪರಿಮಾಣ(ಭಿನ್ನ));
    l->output = realloc(l->output, l->outputs*l->batch*ಪರಿಮಾಣ(ಭಿನ್ನ));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

ಶೂನ್ಯ forward_upsample_layer(ಧ್ರುವ layer l, network net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    ಯದಿ(l.reverse){
        upsample_cpu(l.output, l.out_w, l.out_h, l.c, l.batch, l.stride, 0, l.scale, net.ಒಳಹರಿವು);
    }ಅನ್ಯಥಾ{
        upsample_cpu(net.ಒಳಹರಿವು, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output);
    }
}

ಶೂನ್ಯ backward_upsample_layer(ಧ್ರುವ layer l, network net)
{
    ಯದಿ(l.reverse){
        upsample_cpu(l.delta, l.out_w, l.out_h, l.c, l.batch, l.stride, 1, l.scale, net.delta);
    }ಅನ್ಯಥಾ{
        upsample_cpu(net.delta, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta);
    }
}

#ifdef GPU
ಶೂನ್ಯ forward_upsample_layer_gpu(ಧ್ರುವ layer l, network net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);
    ಯದಿ(l.reverse){
        upsample_gpu(l.output_gpu, l.out_w, l.out_h, l.c, l.batch, l.stride, 0, l.scale, net.ಒಳಹರಿವು_gpu);
    }ಅನ್ಯಥಾ{
        upsample_gpu(net.ಒಳಹರಿವು_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output_gpu);
    }
}

ಶೂನ್ಯ backward_upsample_layer_gpu(ಧ್ರುವ layer l, network net)
{
    ಯದಿ(l.reverse){
        upsample_gpu(l.delta_gpu, l.out_w, l.out_h, l.c, l.batch, l.stride, 1, l.scale, net.delta_gpu);
    }ಅನ್ಯಥಾ{
        upsample_gpu(net.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta_gpu);
    }
}
#endif
