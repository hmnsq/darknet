#അന്തര്ഭാവ "logistic_layer.h"
#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "cuda.h"

#അന്തര്ഭാവ <float.h>
#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <assert.h>

layer make_logistic_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs)
{
    fprintf(stderr, "logistic x entropy                             %4d\n",  inputs);
    layer l = {0};
    l.type = LOGXENT;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = calloc(inputs*batch, വലുപ്പം(ദശാംശം));
    l.output = calloc(inputs*batch, വലുപ്പം(ദശാംശം));
    l.delta = calloc(inputs*batch, വലുപ്പം(ദശാംശം));
    l.cost = calloc(1, വലുപ്പം(ദശാംശം));

    l.forward = forward_logistic_layer;
    l.backward = backward_logistic_layer;
    #ifdef GPU
    l.forward_gpu = forward_logistic_layer_gpu;
    l.backward_gpu = backward_logistic_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    തിരികെ l;
}

ശൂന്യം forward_logistic_layer(സ്ഥിരം layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.നിവേശം, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, LOGISTIC);
    എങ്കിൽ(net.truth){
        logistic_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

ശൂന്യം backward_logistic_layer(സ്ഥിരം layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

ശൂന്യം forward_logistic_layer_gpu(സ്ഥിരം layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.നിവേശം_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, LOGISTIC);
    എങ്കിൽ(net.truth){
        logistic_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

ശൂന്യം backward_logistic_layer_gpu(സ്ഥിരം layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
