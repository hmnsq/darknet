#समावेश "activation_layer.h"
#समावेश "utils.h"
#समावेश "cuda.h"
#समावेश "blas.h"
#समावेश "gemm.h"

#समावेश <math.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <string.h>

layer make_activation_layer(पूर्णांक batch, पूर्णांक inputs, ACTIVATION activation)
{
    layer l = {0};
    l.type = ACTIVE;

    l.inputs = inputs;
    l.outputs = inputs;
    l.batch=batch;

    l.output = सुस्मृति(batch*inputs, माप(भग्न*));
    l.delta = सुस्मृति(batch*inputs, माप(भग्न*));

    l.forward = forward_activation_layer;
    l.backward = backward_activation_layer;
#अगर_घोषित GPU
    l.forward_gpu = forward_activation_layer_gpu;
    l.backward_gpu = backward_activation_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch);
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch);
#पूर्ण_अगर
    l.activation = activation;
    ख_लिखो(मानक_त्रुटि, "Activation Layer: %d inputs\n", inputs);
    वापस l;
}

व्योम forward_activation_layer(layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.निवेश, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

व्योम backward_activation_layer(layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#अगर_घोषित GPU

व्योम forward_activation_layer_gpu(layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.निवेश_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

व्योम backward_activation_layer_gpu(layer l, network net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#पूर्ण_अगर
