#অন্তর্ভাৱ "logistic_layer.h"
#অন্তর্ভাৱ "activations.h"
#অন্তর্ভাৱ "blas.h"
#অন্তর্ভাৱ "cuda.h"

#অন্তর্ভাৱ <float.h>
#অন্তর্ভাৱ <math.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <assert.h>

layer make_logistic_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs)
{
    fprintf(stderr, "logistic x entropy                             %4d\n",  inputs);
    layer l = {0};
    l.type = LOGXENT;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = calloc(inputs*batch, পরিমাণ(ভিন্ন));
    l.output = calloc(inputs*batch, পরিমাণ(ভিন্ন));
    l.delta = calloc(inputs*batch, পরিমাণ(ভিন্ন));
    l.cost = calloc(1, পরিমাণ(ভিন্ন));

    l.forward = forward_logistic_layer;
    l.backward = backward_logistic_layer;
    #ifdef GPU
    l.forward_gpu = forward_logistic_layer_gpu;
    l.backward_gpu = backward_logistic_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    ঘূৰাই l;
}

শূন্য forward_logistic_layer(ধ্রুৱ layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.প্ৰবেশ, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, LOGISTIC);
    যদি(net.truth){
        logistic_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

শূন্য backward_logistic_layer(ধ্রুৱ layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

শূন্য forward_logistic_layer_gpu(ধ্রুৱ layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.প্ৰবেশ_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, LOGISTIC);
    যদি(net.truth){
        logistic_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

শূন্য backward_logistic_layer_gpu(ধ্রুৱ layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
