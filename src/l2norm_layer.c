#ਅਨ੍ਤਰ੍ਭਾਵ "l2norm_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <float.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

layer make_l2norm_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs)
{
    fprintf(stderr, "l2norm                                         %4d\n",  inputs);
    layer l = {0};
    l.type = L2NORM;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(inputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.scales = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(inputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(inputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.forward = forward_l2norm_layer;
    l.backward = backward_l2norm_layer;
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_l2norm_layer_gpu;
    l.backward_gpu = backward_l2norm_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.scales_gpu = cuda_make_array(l.output, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #ਜੇ_ਅੰਤ
    ਵਾਪਸ l;
}

ਖਾਲੀ forward_l2norm_layer(ਅਚਲ layer l, ਜਾਲ net)
{
    copy_cpu(l.outputs*l.batch, net.ਨਿਵੇਸ਼, 1, l.output, 1);
    l2normalize_cpu(l.output, l.scales, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h);
}

ਖਾਲੀ backward_l2norm_layer(ਅਚਲ layer l, ਜਾਲ net)
{
    //axpy_cpu(l.inputs*l.batch, 1, l.scales, 1, l.delta, 1);
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ forward_l2norm_layer_gpu(ਅਚਲ layer l, ਜਾਲ net)
{
    copy_gpu(l.outputs*l.batch, net.ਨਿਵੇਸ਼_gpu, 1, l.output_gpu, 1);
    l2normalize_gpu(l.output_gpu, l.scales_gpu, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h);
}

ਖਾਲੀ backward_l2norm_layer_gpu(ਅਚਲ layer l, ਜਾਲ net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.scales_gpu, 1, l.delta_gpu, 1);
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#ਜੇ_ਅੰਤ
