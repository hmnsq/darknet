#ضمن "logistic_layer.h"
#ضمن "activations.h"
#ضمن "blas.h"
#ضمن "cuda.h"

#ضمن <float.h>
#ضمن <math.h>
#ضمن <stdlib.h>
#ضمن <stdio.h>
#ضمن <assert.h>

layer make_logistic_layer(صحيح batch, صحيح inputs)
{
    fprintf(stderr, "logistic x entropy                             %4d\n",  inputs);
    layer l = {0};
    l.type = LOGXENT;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = حافظه_صفر_ورکړه(inputs*batch, حجم(عائم));
    l.output = حافظه_صفر_ورکړه(inputs*batch, حجم(عائم));
    l.delta = حافظه_صفر_ورکړه(inputs*batch, حجم(عائم));
    l.لګښت_2 = حافظه_صفر_ورکړه(1, حجم(عائم));

    l.forward = forward_logistic_layer;
    l.backward = backward_logistic_layer;
    #که_تعریف_شوی GPU
    l.forward_gpu = forward_logistic_layer_gpu;
    l.backward_gpu = backward_logistic_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #د_که_پای
    أرجع l;
}

فراغ forward_logistic_layer(ثابت layer l, شبکه net)
{
    copy_cpu(l.outputs*l.batch, net.ننوت, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, LOGISTIC);
    إذا(net.truth){
        logistic_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.لګښت_2[0] = مجموعه_array(l.loss, l.batch*l.inputs);
    }
}

فراغ backward_logistic_layer(ثابت layer l, شبکه net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#که_تعریف_شوی GPU

فراغ forward_logistic_layer_gpu(ثابت layer l, شبکه net)
{
    copy_gpu(l.outputs*l.batch, net.ننوت_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, LOGISTIC);
    إذا(net.truth){
        logistic_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.لګښت_2[0] = مجموعه_array(l.loss, l.batch*l.inputs);
    }
}

فراغ backward_logistic_layer_gpu(ثابت layer l, شبکه net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#د_که_پای
