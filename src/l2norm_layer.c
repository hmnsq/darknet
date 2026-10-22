#અન્તર્ભાવ "l2norm_layer.h"
#અન્તર્ભાવ "activations.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ "cuda.h"

#અન્તર્ભાવ <float.h>
#અન્તર્ભાવ <math.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <assert.h>

layer make_l2norm_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs)
{
    fprintf(stderr, "l2norm                                         %4d\n",  inputs);
    layer l = {0};
    l.type = L2NORM;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.output = calloc(inputs*batch, કદ(દશાંશ));
    l.scales = calloc(inputs*batch, કદ(દશાંશ));
    l.delta = calloc(inputs*batch, કદ(દશાંશ));

    l.forward = forward_l2norm_layer;
    l.backward = backward_l2norm_layer;
    #ifdef GPU
    l.forward_gpu = forward_l2norm_layer_gpu;
    l.backward_gpu = backward_l2norm_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.scales_gpu = cuda_make_array(l.output, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    પાછા l;
}

શૂન્ય forward_l2norm_layer(અચલ layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.આગત, 1, l.output, 1);
    l2normalize_cpu(l.output, l.scales, l.batch, l.out_c, l.out_w*l.out_h);
}

શૂન્ય backward_l2norm_layer(અચલ layer l, network net)
{
    //axpy_cpu(l.inputs*l.batch, 1, l.scales, 1, l.delta, 1);
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

શૂન્ય forward_l2norm_layer_gpu(અચલ layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.આગત_gpu, 1, l.output_gpu, 1);
    l2normalize_gpu(l.output_gpu, l.scales_gpu, l.batch, l.out_c, l.out_w*l.out_h);
}

શૂન્ય backward_l2norm_layer_gpu(અચલ layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.scales_gpu, 1, l.delta_gpu, 1);
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
