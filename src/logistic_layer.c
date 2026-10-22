#અન્તર્ભાવ "logistic_layer.h"
#અન્તર્ભાવ "activations.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ "cuda.h"

#અન્તર્ભાવ <float.h>
#અન્તર્ભાવ <math.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <assert.h>

layer make_logistic_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs)
{
    fprintf(stderr, "logistic x entropy                             %4d\n",  inputs);
    layer l = {0};
    l.type = LOGXENT;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = calloc(inputs*batch, કદ(દશાંશ));
    l.output = calloc(inputs*batch, કદ(દશાંશ));
    l.delta = calloc(inputs*batch, કદ(દશાંશ));
    l.cost = calloc(1, કદ(દશાંશ));

    l.forward = forward_logistic_layer;
    l.backward = backward_logistic_layer;
    #ifdef GPU
    l.forward_gpu = forward_logistic_layer_gpu;
    l.backward_gpu = backward_logistic_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    પાછા l;
}

શૂન્ય forward_logistic_layer(અચલ layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.આગત, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, LOGISTIC);
    જો(net.truth){
        logistic_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

શૂન્ય backward_logistic_layer(અચલ layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

શૂન્ય forward_logistic_layer_gpu(અચલ layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.આગત_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, LOGISTIC);
    જો(net.truth){
        logistic_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

શૂન્ય backward_logistic_layer_gpu(અચલ layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
