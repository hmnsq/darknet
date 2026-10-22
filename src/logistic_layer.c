#समावेश "logistic_layer.h"
#समावेश "activations.h"
#समावेश "blas.h"
#समावेश "cuda.h"

#समावेश <float.h>
#समावेश <math.h>
#समावेश <stdlib.h>
#समावेश <stdio.h>
#समावेश <assert.h>

layer make_logistic_layer(पूर्णांक batch, पूर्णांक inputs)
{
    ख_लिखो(मानक_त्रुटि, "logistic x entropy                             %4d\n",  inputs);
    layer l = {0};
    l.type = LOGXENT;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = सुस्मृति(inputs*batch, माप(भग्न));
    l.output = सुस्मृति(inputs*batch, माप(भग्न));
    l.delta = सुस्मृति(inputs*batch, माप(भग्न));
    l.cost = सुस्मृति(1, माप(भग्न));

    l.forward = forward_logistic_layer;
    l.backward = backward_logistic_layer;
    #अगर_घोषित GPU
    l.forward_gpu = forward_logistic_layer_gpu;
    l.backward_gpu = backward_logistic_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #पूर्ण_अगर
    वापस l;
}

व्योम forward_logistic_layer(स्थिर layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.निवेश, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, LOGISTIC);
    अगर(net.truth){
        logistic_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = योग_array(l.loss, l.batch*l.inputs);
    }
}

व्योम backward_logistic_layer(स्थिर layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#अगर_घोषित GPU

व्योम forward_logistic_layer_gpu(स्थिर layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.निवेश_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, LOGISTIC);
    अगर(net.truth){
        logistic_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = योग_array(l.loss, l.batch*l.inputs);
    }
}

व्योम backward_logistic_layer_gpu(स्थिर layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#पूर्ण_अगर
