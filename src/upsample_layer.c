#अन्तर्भाव "upsample_layer.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "blas.h"

#अन्तर्भाव <stdio.h>

layer make_upsample_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c, पूर्णाङ्क stride)
{
    layer l = {0};
    l.type = UPSAMPLE;
    l.batch = batch;
    l.w = w;
    l.h = h;
    l.c = c;
    l.बहिर्गमनम्_w = w*stride;
    l.बहिर्गमनम्_h = h*stride;
    l.बहिर्गमनम्_c = c;
    यदि(stride < 0){
        stride = -stride;
        l.reverse=1;
        l.बहिर्गमनम्_w = w/stride;
        l.बहिर्गमनम्_h = h/stride;
    }
    l.stride = stride;
    l.outputs = l.बहिर्गमनम्_w*l.बहिर्गमनम्_h*l.बहिर्गमनम्_c;
    l.inputs = l.w*l.h*l.c;
    l.delta =  शून्यस्मृतिं_देहि(l.outputs*batch, परिमाण(भिन्न));
    l.output = शून्यस्मृतिं_देहि(l.outputs*batch, परिमाण(भिन्न));;

    l.forward = forward_upsample_layer;
    l.backward = backward_upsample_layer;
    #यदि_परिभाषितम् GPU
    l.forward_gpu = forward_upsample_layer_gpu;
    l.backward_gpu = backward_upsample_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #यदि_अन्तः
    यदि(l.reverse) fprintf(stderr, "downsample         %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.बहिर्गमनम्_w, l.बहिर्गमनम्_h, l.बहिर्गमनम्_c);
    अन्यथा fprintf(stderr, "upsample           %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.बहिर्गमनम्_w, l.बहिर्गमनम्_h, l.बहिर्गमनम्_c);
    प्रत्यागम l;
}

शून्य resize_upsample_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    l->w = w;
    l->h = h;
    l->बहिर्गमनम्_w = w*l->stride;
    l->बहिर्गमनम्_h = h*l->stride;
    यदि(l->reverse){
        l->बहिर्गमनम्_w = w/l->stride;
        l->बहिर्गमनम्_h = h/l->stride;
    }
    l->outputs = l->बहिर्गमनम्_w*l->बहिर्गमनम्_h*l->बहिर्गमनम्_c;
    l->inputs = l->h*l->w*l->c;
    l->delta =  realloc(l->delta, l->outputs*l->batch*परिमाण(भिन्न));
    l->output = realloc(l->output, l->outputs*l->batch*परिमाण(भिन्न));

#यदि_परिभाषितम् GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#यदि_अन्तः
    
}

शून्य forward_upsample_layer(ध्रुव layer l, जालम् net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    यदि(l.reverse){
        upsample_cpu(l.output, l.बहिर्गमनम्_w, l.बहिर्गमनम्_h, l.c, l.batch, l.stride, 0, l.scale, net.निवेशः);
    }अन्यथा{
        upsample_cpu(net.निवेशः, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output);
    }
}

शून्य backward_upsample_layer(ध्रुव layer l, जालम् net)
{
    यदि(l.reverse){
        upsample_cpu(l.delta, l.बहिर्गमनम्_w, l.बहिर्गमनम्_h, l.c, l.batch, l.stride, 1, l.scale, net.delta);
    }अन्यथा{
        upsample_cpu(net.delta, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta);
    }
}

#यदि_परिभाषितम् GPU
शून्य forward_upsample_layer_gpu(ध्रुव layer l, जालम् net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);
    यदि(l.reverse){
        upsample_gpu(l.output_gpu, l.बहिर्गमनम्_w, l.बहिर्गमनम्_h, l.c, l.batch, l.stride, 0, l.scale, net.निवेशः_gpu);
    }अन्यथा{
        upsample_gpu(net.निवेशः_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output_gpu);
    }
}

शून्य backward_upsample_layer_gpu(ध्रुव layer l, जालम् net)
{
    यदि(l.reverse){
        upsample_gpu(l.delta_gpu, l.बहिर्गमनम्_w, l.बहिर्गमनम्_h, l.c, l.batch, l.stride, 1, l.scale, net.delta_gpu);
    }अन्यथा{
        upsample_gpu(net.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta_gpu);
    }
}
#यदि_अन्तः
