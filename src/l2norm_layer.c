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
    l.output = صفر_حافظہ_دو(inputs*batch, حجم(اعشاری));
    l.scales = صفر_حافظہ_دو(inputs*batch, حجم(اعشاری));
    l.delta = صفر_حافظہ_دو(inputs*batch, حجم(اعشاری));

    l.forward = forward_l2norm_layer;
    l.backward = backward_l2norm_layer;
    #اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_l2norm_layer_gpu;
    l.backward_gpu = backward_l2norm_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.scales_gpu = cuda_make_array(l.output, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #اگر_ختم
    واپس l;
}

خالی forward_l2norm_layer(ثابت layer l, جال net)
{
    copy_cpu(l.outputs*l.batch, net.داخلہ, 1, l.output, 1);
    l2normalize_cpu(l.output, l.scales, l.batch, l.برآمد_c, l.برآمد_w*l.برآمد_h);
}

خالی backward_l2norm_layer(ثابت layer l, جال net)
{
    //axpy_cpu(l.inputs*l.batch, 1, l.scales, 1, l.delta, 1);
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#اگر_تعریف_شدہ GPU

خالی forward_l2norm_layer_gpu(ثابت layer l, جال net)
{
    copy_gpu(l.outputs*l.batch, net.داخلہ_gpu, 1, l.output_gpu, 1);
    l2normalize_gpu(l.output_gpu, l.scales_gpu, l.batch, l.برآمد_c, l.برآمد_w*l.برآمد_h);
}

خالی backward_l2norm_layer_gpu(ثابت layer l, جال net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.scales_gpu, 1, l.delta_gpu, 1);
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#اگر_ختم
