#অন্তর্ভাব "logistic_layer.h"
#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "cuda.h"

#অন্তর্ভাব <float.h>
#অন্তর্ভাব <math.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <assert.h>

layer make_logistic_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা inputs)
{
    fprintf(stderr, "logistic x entropy                             %4d\n",  inputs);
    layer l = {0};
    l.type = LOGXENT;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = calloc(inputs*batch, আকার(দশমিক));
    l.output = calloc(inputs*batch, আকার(দশমিক));
    l.delta = calloc(inputs*batch, আকার(দশমিক));
    l.cost = calloc(1, আকার(দশমিক));

    l.forward = forward_logistic_layer;
    l.backward = backward_logistic_layer;
    #ifdef GPU
    l.forward_gpu = forward_logistic_layer_gpu;
    l.backward_gpu = backward_logistic_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    ফেরত l;
}

শূন্য forward_logistic_layer(ধ্রুবক layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.প্রবেশ, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, LOGISTIC);
    যদি(net.truth){
        logistic_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = যোগ_array(l.loss, l.batch*l.inputs);
    }
}

শূন্য backward_logistic_layer(ধ্রুবক layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

শূন্য forward_logistic_layer_gpu(ধ্রুবক layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.প্রবেশ_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, LOGISTIC);
    যদি(net.truth){
        logistic_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = যোগ_array(l.loss, l.batch*l.inputs);
    }
}

শূন্য backward_logistic_layer_gpu(ধ্রুবক layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
