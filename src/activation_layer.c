#ਅਨ੍ਤਰ੍ਭਾਵ "activation_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gemm.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

layer make_activation_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ACTIVATION activation)
{
    layer l = {0};
    l.type = ਸਰਗਰਮ;

    l.inputs = inputs;
    l.outputs = inputs;
    l.batch=batch;

    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*inputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    l.delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*inputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));

    l.forward = forward_activation_layer;
    l.backward = backward_activation_layer;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_activation_layer_gpu;
    l.backward_gpu = backward_activation_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch);
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch);
#ਜੇ_ਅੰਤ
    l.activation = activation;
    fprintf(stderr, "Activation Layer: %d inputs\n", inputs);
    ਵਾਪਸ l;
}

ਖਾਲੀ forward_activation_layer(layer l, ਜਾਲ net)
{
    copy_cpu(l.outputs*l.batch, net.ਨਿਵੇਸ਼, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ਖਾਲੀ backward_activation_layer(layer l, ਜਾਲ net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ forward_activation_layer_gpu(layer l, ਜਾਲ net)
{
    copy_gpu(l.outputs*l.batch, net.ਨਿਵੇਸ਼_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ਖਾਲੀ backward_activation_layer_gpu(layer l, ਜਾਲ net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#ਜੇ_ਅੰਤ
