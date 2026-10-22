#അന്തര്ഭാവ "l2norm_layer.h"
#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "cuda.h"

#അന്തര്ഭാവ <float.h>
#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <assert.h>

layer make_l2norm_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs)
{
    fprintf(stderr, "l2norm                                         %4d\n",  inputs);
    layer l = {0};
    l.type = L2NORM;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.output = calloc(inputs*batch, വലുപ്പം(ദശാംശം));
    l.scales = calloc(inputs*batch, വലുപ്പം(ദശാംശം));
    l.delta = calloc(inputs*batch, വലുപ്പം(ദശാംശം));

    l.forward = forward_l2norm_layer;
    l.backward = backward_l2norm_layer;
    #ifdef GPU
    l.forward_gpu = forward_l2norm_layer_gpu;
    l.backward_gpu = backward_l2norm_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.scales_gpu = cuda_make_array(l.output, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    തിരികെ l;
}

ശൂന്യം forward_l2norm_layer(സ്ഥിരം layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.നിവേശം, 1, l.output, 1);
    l2normalize_cpu(l.output, l.scales, l.batch, l.out_c, l.out_w*l.out_h);
}

ശൂന്യം backward_l2norm_layer(സ്ഥിരം layer l, network net)
{
    //axpy_cpu(l.inputs*l.batch, 1, l.scales, 1, l.delta, 1);
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

ശൂന്യം forward_l2norm_layer_gpu(സ്ഥിരം layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.നിവേശം_gpu, 1, l.output_gpu, 1);
    l2normalize_gpu(l.output_gpu, l.scales_gpu, l.batch, l.out_c, l.out_w*l.out_h);
}

ശൂന്യം backward_l2norm_layer_gpu(സ്ഥിരം layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.scales_gpu, 1, l.delta_gpu, 1);
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
