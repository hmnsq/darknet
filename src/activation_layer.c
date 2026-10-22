#ضمن "activation_layer.h"
#ضمن "utils.h"
#ضمن "cuda.h"
#ضمن "blas.h"
#ضمن "gemm.h"

#ضمن <math.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>

layer make_activation_layer(صحيح batch, صحيح inputs, ACTIVATION activation)
{
    layer l = {0};
    l.type = فعال;

    l.inputs = inputs;
    l.outputs = inputs;
    l.batch=batch;

    l.output = حافظه_صفر_ورکړه(batch*inputs, حجم(عائم*));
    l.delta = حافظه_صفر_ورکړه(batch*inputs, حجم(عائم*));

    l.forward = forward_activation_layer;
    l.backward = backward_activation_layer;
#که_تعریف_شوی GPU
    l.forward_gpu = forward_activation_layer_gpu;
    l.backward_gpu = backward_activation_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch);
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch);
#د_که_پای
    l.activation = activation;
    fprintf(stderr, "Activation Layer: %d inputs\n", inputs);
    أرجع l;
}

فراغ forward_activation_layer(layer l, شبکه net)
{
    copy_cpu(l.outputs*l.batch, net.ننوت, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

فراغ backward_activation_layer(layer l, شبکه net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#که_تعریف_شوی GPU

فراغ forward_activation_layer_gpu(layer l, شبکه net)
{
    copy_gpu(l.outputs*l.batch, net.ننوت_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

فراغ backward_activation_layer_gpu(layer l, شبکه net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#د_که_پای
