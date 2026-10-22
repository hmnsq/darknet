#अन्तर्भाव "activation_layer.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "gemm.h"

#अन्तर्भाव <math.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

layer make_activation_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, ACTIVATION activation)
{
    layer l = {0};
    l.type = ACTIVE;

    l.inputs = inputs;
    l.outputs = inputs;
    l.batch=batch;

    l.output = सुस्मृति(batch*inputs, परिमाण(भिन्न*));
    l.delta = सुस्मृति(batch*inputs, परिमाण(भिन्न*));

    l.forward = forward_activation_layer;
    l.backward = backward_activation_layer;
#ifdef GPU
    l.forward_gpu = forward_activation_layer_gpu;
    l.backward_gpu = backward_activation_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch);
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch);
#endif
    l.activation = activation;
    ख_लिखो(मानक_त्रुटि, "Activation Layer: %d inputs\n", inputs);
    प्रत्यागम l;
}

शून्य forward_activation_layer(layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.निवेश, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

शून्य backward_activation_layer(layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#ifdef GPU

शून्य forward_activation_layer_gpu(layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.निवेश_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

शून्य backward_activation_layer_gpu(layer l, network net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif
