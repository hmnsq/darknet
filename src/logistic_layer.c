#அந்தர்பாவ "logistic_layer.h"
#அந்தர்பாவ "activations.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ "cuda.h"

#அந்தர்பாவ <float.h>
#அந்தர்பாவ <math.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <assert.h>

layer make_logistic_layer(முழு batch, முழு inputs)
{
    fprintf(stderr, "logistic x entropy                             %4d\n",  inputs);
    layer l = {0};
    l.type = LOGXENT;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = calloc(inputs*batch, அளவு(தசம));
    l.output = calloc(inputs*batch, அளவு(தசம));
    l.delta = calloc(inputs*batch, அளவு(தசம));
    l.cost = calloc(1, அளவு(தசம));

    l.forward = forward_logistic_layer;
    l.backward = backward_logistic_layer;
    #ifdef GPU
    l.forward_gpu = forward_logistic_layer_gpu;
    l.backward_gpu = backward_logistic_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    திரும்பு l;
}

வெறுமை forward_logistic_layer(மாறிலி layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.உள்ளீடு, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, LOGISTIC);
    எனில்(net.truth){
        logistic_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

வெறுமை backward_logistic_layer(மாறிலி layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

வெறுமை forward_logistic_layer_gpu(மாறிலி layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.உள்ளீடு_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, LOGISTIC);
    எனில்(net.truth){
        logistic_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

வெறுமை backward_logistic_layer_gpu(மாறிலி layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
