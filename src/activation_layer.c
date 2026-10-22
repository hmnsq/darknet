#അന്തര്ഭാവ "activation_layer.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "gemm.h"

#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>

layer make_activation_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, ACTIVATION activation)
{
    layer l = {0};
    l.type = ACTIVE;

    l.inputs = inputs;
    l.outputs = inputs;
    l.batch=batch;

    l.output = calloc(batch*inputs, വലുപ്പം(ദശാംശം*));
    l.delta = calloc(batch*inputs, വലുപ്പം(ദശാംശം*));

    l.forward = forward_activation_layer;
    l.backward = backward_activation_layer;
#ifdef GPU
    l.forward_gpu = forward_activation_layer_gpu;
    l.backward_gpu = backward_activation_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch);
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch);
#endif
    l.activation = activation;
    fprintf(stderr, "Activation Layer: %d inputs\n", inputs);
    തിരികെ l;
}

ശൂന്യം forward_activation_layer(layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.നിവേശം, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ശൂന്യം backward_activation_layer(layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#ifdef GPU

ശൂന്യം forward_activation_layer_gpu(layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.നിവേശം_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ശൂന്യം backward_activation_layer_gpu(layer l, network net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif
