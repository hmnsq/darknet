#অন্তর্ভাৱ "activation_layer.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "cuda.h"
#অন্তর্ভাৱ "blas.h"
#অন্তর্ভাৱ "gemm.h"

#অন্তর্ভাৱ <math.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <string.h>

layer make_activation_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs, ACTIVATION activation)
{
    layer l = {0};
    l.type = ACTIVE;

    l.inputs = inputs;
    l.outputs = inputs;
    l.batch=batch;

    l.output = calloc(batch*inputs, পরিমাণ(ভিন্ন*));
    l.delta = calloc(batch*inputs, পরিমাণ(ভিন্ন*));

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
    ঘূৰাই l;
}

শূন্য forward_activation_layer(layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.প্ৰবেশ, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

শূন্য backward_activation_layer(layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#ifdef GPU

শূন্য forward_activation_layer_gpu(layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.প্ৰবেশ_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

শূন্য backward_activation_layer_gpu(layer l, network net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif
