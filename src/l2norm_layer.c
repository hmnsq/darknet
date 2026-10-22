#ଅନ୍ତର୍ଭାଵ "l2norm_layer.h"
#ଅନ୍ତର୍ଭାଵ "activations.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"

#ଅନ୍ତର୍ଭାଵ <float.h>
#ଅନ୍ତର୍ଭାଵ <math.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>

layer make_l2norm_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs)
{
    fprintf(stderr, "l2norm                                         %4d\n",  inputs);
    layer l = {0};
    l.type = L2NORM;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.output = calloc(inputs*batch, ଆକାର(ଦଶମିକ));
    l.scales = calloc(inputs*batch, ଆକାର(ଦଶମିକ));
    l.delta = calloc(inputs*batch, ଆକାର(ଦଶମିକ));

    l.forward = forward_l2norm_layer;
    l.backward = backward_l2norm_layer;
    #ifdef GPU
    l.forward_gpu = forward_l2norm_layer_gpu;
    l.backward_gpu = backward_l2norm_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.scales_gpu = cuda_make_array(l.output, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ forward_l2norm_layer(ଅଚଳ layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.ନିବେଶ, 1, l.output, 1);
    l2normalize_cpu(l.output, l.scales, l.batch, l.out_c, l.out_w*l.out_h);
}

ଶୂନ୍ୟ backward_l2norm_layer(ଅଚଳ layer l, network net)
{
    //axpy_cpu(l.inputs*l.batch, 1, l.scales, 1, l.delta, 1);
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

ଶୂନ୍ୟ forward_l2norm_layer_gpu(ଅଚଳ layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.ନିବେଶ_gpu, 1, l.output_gpu, 1);
    l2normalize_gpu(l.output_gpu, l.scales_gpu, l.batch, l.out_c, l.out_w*l.out_h);
}

ଶୂନ୍ୟ backward_l2norm_layer_gpu(ଅଚଳ layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.scales_gpu, 1, l.delta_gpu, 1);
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
