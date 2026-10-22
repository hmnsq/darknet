#অন্তর্ভাব "l2norm_layer.h"
#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "cuda.h"

#অন্তর্ভাব <float.h>
#অন্তর্ভাব <math.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <assert.h>

layer make_l2norm_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs)
{
    fprintf(stderr, "l2norm                                         %4d\n",  inputs);
    layer l = {0};
    l.type = L2NORM;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.output = calloc(inputs*batch, পরিমাণ(ভিন্ন));
    l.scales = calloc(inputs*batch, পরিমাণ(ভিন্ন));
    l.delta = calloc(inputs*batch, পরিমাণ(ভিন্ন));

    l.forward = forward_l2norm_layer;
    l.backward = backward_l2norm_layer;
    #ifdef GPU
    l.forward_gpu = forward_l2norm_layer_gpu;
    l.backward_gpu = backward_l2norm_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.scales_gpu = cuda_make_array(l.output, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    প্রত্যাগম l;
}

শূন্য forward_l2norm_layer(ধ্রুব layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.প্রবেশ, 1, l.output, 1);
    l2normalize_cpu(l.output, l.scales, l.batch, l.out_c, l.out_w*l.out_h);
}

শূন্য backward_l2norm_layer(ধ্রুব layer l, network net)
{
    //axpy_cpu(l.inputs*l.batch, 1, l.scales, 1, l.delta, 1);
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

শূন্য forward_l2norm_layer_gpu(ধ্রুব layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.প্রবেশ_gpu, 1, l.output_gpu, 1);
    l2normalize_gpu(l.output_gpu, l.scales_gpu, l.batch, l.out_c, l.out_w*l.out_h);
}

শূন্য backward_l2norm_layer_gpu(ধ্রুব layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.scales_gpu, 1, l.delta_gpu, 1);
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
