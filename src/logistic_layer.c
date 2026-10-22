#شامل "logistic_layer.h"
#شامل "activations.h"
#شامل "blas.h"
#شامل "cuda.h"

#شامل <float.h>
#شامل <math.h>
#شامل <stdlib.h>
#شامل <stdio.h>
#شامل <assert.h>

layer make_logistic_layer(صحیح batch, صحیح inputs)
{
    fprintf(stderr, "logistic x entropy                             %4d\n",  inputs);
    layer l = {0};
    l.type = LOGXENT;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = صفر_حافظہ_دو(inputs*batch, حجم(اعشاری));
    l.output = صفر_حافظہ_دو(inputs*batch, حجم(اعشاری));
    l.delta = صفر_حافظہ_دو(inputs*batch, حجم(اعشاری));
    l.لاگت_2 = صفر_حافظہ_دو(1, حجم(اعشاری));

    l.forward = forward_logistic_layer;
    l.backward = backward_logistic_layer;
    #اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_logistic_layer_gpu;
    l.backward_gpu = backward_logistic_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #اگر_ختم
    واپس l;
}

خالی forward_logistic_layer(ثابت layer l, جال net)
{
    copy_cpu(l.outputs*l.batch, net.داخلہ, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, LOGISTIC);
    اگر(net.truth){
        logistic_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.لاگت_2[0] = مجموع_array(l.loss, l.batch*l.inputs);
    }
}

خالی backward_logistic_layer(ثابت layer l, جال net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#اگر_تعریف_شدہ GPU

خالی forward_logistic_layer_gpu(ثابت layer l, جال net)
{
    copy_gpu(l.outputs*l.batch, net.داخلہ_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, LOGISTIC);
    اگر(net.truth){
        logistic_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.لاگت_2[0] = مجموع_array(l.loss, l.batch*l.inputs);
    }
}

خالی backward_logistic_layer_gpu(ثابت layer l, جال net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#اگر_ختم
