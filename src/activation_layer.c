#அந்தர்பாவ "activation_layer.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ "gemm.h"

#அந்தர்பாவ <math.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>

layer make_activation_layer(முழு batch, முழு inputs, ACTIVATION activation)
{
    layer l = {0};
    l.type = ACTIVE;

    l.inputs = inputs;
    l.outputs = inputs;
    l.batch=batch;

    l.output = calloc(batch*inputs, அளவு(தசம*));
    l.delta = calloc(batch*inputs, அளவு(தசம*));

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
    திரும்பு l;
}

வெறுமை forward_activation_layer(layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.உள்ளீடு, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

வெறுமை backward_activation_layer(layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#ifdef GPU

வெறுமை forward_activation_layer_gpu(layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.உள்ளீடு_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

வெறுமை backward_activation_layer_gpu(layer l, network net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif
