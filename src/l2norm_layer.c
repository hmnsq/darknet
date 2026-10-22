#அந்தர்பாவ "l2norm_layer.h"
#அந்தர்பாவ "activations.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ "cuda.h"

#அந்தர்பாவ <float.h>
#அந்தர்பாவ <math.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <assert.h>

layer make_l2norm_layer(முழு batch, முழு inputs)
{
    fprintf(stderr, "l2norm                                         %4d\n",  inputs);
    layer l = {0};
    l.type = L2NORM;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.output = calloc(inputs*batch, அளவு(தசம));
    l.scales = calloc(inputs*batch, அளவு(தசம));
    l.delta = calloc(inputs*batch, அளவு(தசம));

    l.forward = forward_l2norm_layer;
    l.backward = backward_l2norm_layer;
    #ifdef GPU
    l.forward_gpu = forward_l2norm_layer_gpu;
    l.backward_gpu = backward_l2norm_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.scales_gpu = cuda_make_array(l.output, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    திரும்பு l;
}

வெறுமை forward_l2norm_layer(மாறிலி layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.உள்ளீடு, 1, l.output, 1);
    l2normalize_cpu(l.output, l.scales, l.batch, l.out_c, l.out_w*l.out_h);
}

வெறுமை backward_l2norm_layer(மாறிலி layer l, network net)
{
    //axpy_cpu(l.inputs*l.batch, 1, l.scales, 1, l.delta, 1);
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

வெறுமை forward_l2norm_layer_gpu(மாறிலி layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.உள்ளீடு_gpu, 1, l.output_gpu, 1);
    l2normalize_gpu(l.output_gpu, l.scales_gpu, l.batch, l.out_c, l.out_w*l.out_h);
}

வெறுமை backward_l2norm_layer_gpu(மாறிலி layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.scales_gpu, 1, l.delta_gpu, 1);
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
