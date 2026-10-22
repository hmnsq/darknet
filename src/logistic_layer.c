#अन्तर्भाव "logistic_layer.h"
#अन्तर्भाव "activations.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "cuda.h"

#अन्तर्भाव <float.h>
#अन्तर्भाव <math.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <assert.h>

layer make_logistic_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs)
{
    ख_लिखो(मानक_त्रुटि, "logistic x entropy                             %4d\n",  inputs);
    layer l = {0};
    l.type = LOGXENT;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = सुस्मृति(inputs*batch, परिमाण(भिन्न));
    l.output = सुस्मृति(inputs*batch, परिमाण(भिन्न));
    l.delta = सुस्मृति(inputs*batch, परिमाण(भिन्न));
    l.cost = सुस्मृति(1, परिमाण(भिन्न));

    l.forward = forward_logistic_layer;
    l.backward = backward_logistic_layer;
    #ifdef GPU
    l.forward_gpu = forward_logistic_layer_gpu;
    l.backward_gpu = backward_logistic_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    फर्काऊ l;
}

शून्य forward_logistic_layer(अचर layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.आगत, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, LOGISTIC);
    यदि(net.truth){
        logistic_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

शून्य backward_logistic_layer(अचर layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

शून्य forward_logistic_layer_gpu(अचर layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.आगत_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, LOGISTIC);
    यदि(net.truth){
        logistic_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

शून्य backward_logistic_layer_gpu(अचर layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
