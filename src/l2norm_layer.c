#شامل "l2norm_layer.h"
#شامل "activations.h"
#شامل "blas.h"
#شامل "cuda.h"

#شامل <float.h>
#شامل <math.h>
#شامل <stdlib.h>
#شامل <stdio.h>
#شامل <assert.h>

layer make_l2norm_layer(صحیح batch, صحیح inputs)
{
    fprintf(stderr, "l2norm                                         %4d\n",  inputs);
    layer l = {0};
    l.type = L2NORM;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.output = calloc(inputs*batch, اندازه(اعشاری));
    l.scales = calloc(inputs*batch, اندازه(اعشاری));
    l.delta = calloc(inputs*batch, اندازه(اعشاری));

    l.forward = forward_l2norm_layer;
    l.backward = backward_l2norm_layer;
    #ifdef GPU
    l.forward_gpu = forward_l2norm_layer_gpu;
    l.backward_gpu = backward_l2norm_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.scales_gpu = cuda_make_array(l.output, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    بازگشت l;
}

تهی forward_l2norm_layer(ثابت layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.درون_داد, 1, l.output, 1);
    l2normalize_cpu(l.output, l.scales, l.batch, l.out_c, l.out_w*l.out_h);
}

تهی backward_l2norm_layer(ثابت layer l, network net)
{
    //axpy_cpu(l.inputs*l.batch, 1, l.scales, 1, l.delta, 1);
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

تهی forward_l2norm_layer_gpu(ثابت layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.درون_داد_gpu, 1, l.output_gpu, 1);
    l2normalize_gpu(l.output_gpu, l.scales_gpu, l.batch, l.out_c, l.out_w*l.out_h);
}

تهی backward_l2norm_layer_gpu(ثابت layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.scales_gpu, 1, l.delta_gpu, 1);
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
