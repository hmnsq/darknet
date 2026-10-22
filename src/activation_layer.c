#અન્તર્ભાવ "activation_layer.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ "gemm.h"

#અન્તર્ભાવ <math.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>

layer make_activation_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs, ACTIVATION activation)
{
    layer l = {0};
    l.type = ACTIVE;

    l.inputs = inputs;
    l.outputs = inputs;
    l.batch=batch;

    l.output = calloc(batch*inputs, કદ(દશાંશ*));
    l.delta = calloc(batch*inputs, કદ(દશાંશ*));

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
    પાછા l;
}

શૂન્ય forward_activation_layer(layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.આગત, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

શૂન્ય backward_activation_layer(layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#ifdef GPU

શૂન્ય forward_activation_layer_gpu(layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.આગત_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

શૂન્ય backward_activation_layer_gpu(layer l, network net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif
