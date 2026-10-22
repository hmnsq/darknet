#شامل "activation_layer.h"
#شامل "utils.h"
#شامل "cuda.h"
#شامل "blas.h"
#شامل "gemm.h"

#شامل <math.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>

layer make_activation_layer(صحیح batch, صحیح inputs, ACTIVATION activation)
{
    layer l = {0};
    l.type = فعال;

    l.inputs = inputs;
    l.outputs = inputs;
    l.batch=batch;

    l.output = صفر_حافظہ_دو(batch*inputs, حجم(اعشاری*));
    l.delta = صفر_حافظہ_دو(batch*inputs, حجم(اعشاری*));

    l.forward = forward_activation_layer;
    l.backward = backward_activation_layer;
#اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_activation_layer_gpu;
    l.backward_gpu = backward_activation_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch);
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch);
#اگر_ختم
    l.activation = activation;
    fprintf(stderr, "Activation Layer: %d inputs\n", inputs);
    واپس l;
}

خالی forward_activation_layer(layer l, جال net)
{
    copy_cpu(l.outputs*l.batch, net.داخلہ, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

خالی backward_activation_layer(layer l, جال net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#اگر_تعریف_شدہ GPU

خالی forward_activation_layer_gpu(layer l, جال net)
{
    copy_gpu(l.outputs*l.batch, net.داخلہ_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

خالی backward_activation_layer_gpu(layer l, جال net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#اگر_ختم
