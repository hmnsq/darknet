#ਅਨ੍ਤਰ੍ਭਾਵ "logistic_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <float.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

layer make_logistic_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs)
{
    fprintf(stderr, "logistic x entropy                             %4d\n",  inputs);
    layer l = {0};
    l.type = LOGXENT;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(inputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(inputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(inputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.ਲਾਗਤ_2 = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.forward = forward_logistic_layer;
    l.backward = backward_logistic_layer;
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_logistic_layer_gpu;
    l.backward_gpu = backward_logistic_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #ਜੇ_ਅੰਤ
    ਵਾਪਸ l;
}

ਖਾਲੀ forward_logistic_layer(ਅਚਲ layer l, ਜਾਲ net)
{
    copy_cpu(l.outputs*l.batch, net.ਨਿਵੇਸ਼, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, LOGISTIC);
    ਜੇ(net.truth){
        logistic_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.ਲਾਗਤ_2[0] = ਜੋੜ_array(l.loss, l.batch*l.inputs);
    }
}

ਖਾਲੀ backward_logistic_layer(ਅਚਲ layer l, ਜਾਲ net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ forward_logistic_layer_gpu(ਅਚਲ layer l, ਜਾਲ net)
{
    copy_gpu(l.outputs*l.batch, net.ਨਿਵੇਸ਼_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, LOGISTIC);
    ਜੇ(net.truth){
        logistic_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.ਲਾਗਤ_2[0] = ਜੋੜ_array(l.loss, l.batch*l.inputs);
    }
}

ਖਾਲੀ backward_logistic_layer_gpu(ਅਚਲ layer l, ਜਾਲ net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#ਜੇ_ਅੰਤ
