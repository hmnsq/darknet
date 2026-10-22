#ضمن "l2norm_layer.h"
#ضمن "activations.h"
#ضمن "blas.h"
#ضمن "cuda.h"

#ضمن <float.h>
#ضمن <math.h>
#ضمن <stdlib.h>
#ضمن <stdio.h>
#ضمن <assert.h>

layer make_l2norm_layer(صحيح batch, صحيح inputs)
{
    fprintf(stderr, "l2norm                                         %4d\n",  inputs);
    layer l = {0};
    l.type = L2NORM;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.output = حافظه_صفر_بده(inputs*batch, حجم(عائم));
    l.scales = حافظه_صفر_بده(inputs*batch, حجم(عائم));
    l.delta = حافظه_صفر_بده(inputs*batch, حجم(عائم));

    l.forward = forward_l2norm_layer;
    l.backward = backward_l2norm_layer;
    #اگر_تعریف_شده GPU
    l.forward_gpu = forward_l2norm_layer_gpu;
    l.backward_gpu = backward_l2norm_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.scales_gpu = cuda_make_array(l.output, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #پایان_اگر
    أرجع l;
}

فراغ forward_l2norm_layer(ثابت layer l, شبکه net)
{
    copy_cpu(l.outputs*l.batch, net.درون_داد, 1, l.output, 1);
    l2normalize_cpu(l.output, l.scales, l.batch, l.بیرون_c, l.بیرون_w*l.بیرون_h);
}

فراغ backward_l2norm_layer(ثابت layer l, شبکه net)
{
    //axpy_cpu(l.inputs*l.batch, 1, l.scales, 1, l.delta, 1);
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#اگر_تعریف_شده GPU

فراغ forward_l2norm_layer_gpu(ثابت layer l, شبکه net)
{
    copy_gpu(l.outputs*l.batch, net.درون_داد_gpu, 1, l.output_gpu, 1);
    l2normalize_gpu(l.output_gpu, l.scales_gpu, l.batch, l.بیرون_c, l.بیرون_w*l.بیرون_h);
}

فراغ backward_l2norm_layer_gpu(ثابت layer l, شبکه net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.scales_gpu, 1, l.delta_gpu, 1);
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#پایان_اگر
