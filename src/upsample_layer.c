#समावेश "upsample_layer.h"
#समावेश "cuda.h"
#समावेश "blas.h"

#समावेश <stdio.h>

layer make_upsample_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक stride)
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
    अगर(stride < 0){
        stride = -stride;
        l.reverse=1;
        l.out_w = w/stride;
        l.out_h = h/stride;
    }
    l.stride = stride;
    l.outputs = l.out_w*l.out_h*l.out_c;
    l.inputs = l.w*l.h*l.c;
    l.delta =  सुस्मृति(l.outputs*batch, माप(भग्न));
    l.output = सुस्मृति(l.outputs*batch, माप(भग्न));;

    l.forward = forward_upsample_layer;
    l.backward = backward_upsample_layer;
    #अगर_घोषित GPU
    l.forward_gpu = forward_upsample_layer_gpu;
    l.backward_gpu = backward_upsample_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #पूर्ण_अगर
    अगर(l.reverse) ख_लिखो(मानक_त्रुटि, "downsample         %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.out_w, l.out_h, l.out_c);
    अन्यथा ख_लिखो(मानक_त्रुटि, "upsample           %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.out_w, l.out_h, l.out_c);
    वापस l;
}

व्योम resize_upsample_layer(layer *l, पूर्णांक w, पूर्णांक h)
{
    l->w = w;
    l->h = h;
    l->out_w = w*l->stride;
    l->out_h = h*l->stride;
    अगर(l->reverse){
        l->out_w = w/l->stride;
        l->out_h = h/l->stride;
    }
    l->outputs = l->out_w*l->out_h*l->out_c;
    l->inputs = l->h*l->w*l->c;
    l->delta =  पुनः_स्मृति(l->delta, l->outputs*l->batch*माप(भग्न));
    l->output = पुनः_स्मृति(l->output, l->outputs*l->batch*माप(भग्न));

#अगर_घोषित GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#पूर्ण_अगर
    
}

व्योम forward_upsample_layer(स्थिर layer l, network net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    अगर(l.reverse){
        upsample_cpu(l.output, l.out_w, l.out_h, l.c, l.batch, l.stride, 0, l.scale, net.निवेश);
    }अन्यथा{
        upsample_cpu(net.निवेश, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output);
    }
}

व्योम backward_upsample_layer(स्थिर layer l, network net)
{
    अगर(l.reverse){
        upsample_cpu(l.delta, l.out_w, l.out_h, l.c, l.batch, l.stride, 1, l.scale, net.delta);
    }अन्यथा{
        upsample_cpu(net.delta, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta);
    }
}

#अगर_घोषित GPU
व्योम forward_upsample_layer_gpu(स्थिर layer l, network net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);
    अगर(l.reverse){
        upsample_gpu(l.output_gpu, l.out_w, l.out_h, l.c, l.batch, l.stride, 0, l.scale, net.निवेश_gpu);
    }अन्यथा{
        upsample_gpu(net.निवेश_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output_gpu);
    }
}

व्योम backward_upsample_layer_gpu(स्थिर layer l, network net)
{
    अगर(l.reverse){
        upsample_gpu(l.delta_gpu, l.out_w, l.out_h, l.c, l.batch, l.stride, 1, l.scale, net.delta_gpu);
    }अन्यथा{
        upsample_gpu(net.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta_gpu);
    }
}
#पूर्ण_अगर
