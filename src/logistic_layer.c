#అన్తర్భావ "logistic_layer.h"
#అన్తర్భావ "activations.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "cuda.h"

#అన్తర్భావ <float.h>
#అన్తర్భావ <math.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <assert.h>

layer make_logistic_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs)
{
    fprintf(stderr, "logistic x entropy                             %4d\n",  inputs);
    layer l = {0};
    l.type = LOGXENT;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = సున్నా_జ్ఞాపకం_ఇవ్వు(inputs*batch, పరిమాణం(దశాంశం));
    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(inputs*batch, పరిమాణం(దశాంశం));
    l.delta = సున్నా_జ్ఞాపకం_ఇవ్వు(inputs*batch, పరిమాణం(దశాంశం));
    l.ఖర్చు_2 = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(దశాంశం));

    l.forward = forward_logistic_layer;
    l.backward = backward_logistic_layer;
    #నిర్వచితమైతే GPU
    l.forward_gpu = forward_logistic_layer_gpu;
    l.backward_gpu = backward_logistic_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #అయితే_ముగింపు
    తిరిగి l;
}

శూన్య forward_logistic_layer(స్థిరరాశి layer l, జాలం net)
{
    copy_cpu(l.outputs*l.batch, net.ఆదానం, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, LOGISTIC);
    ఒకవేళ(net.truth){
        logistic_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.ఖర్చు_2[0] = కూడిక_array(l.loss, l.batch*l.inputs);
    }
}

శూన్య backward_logistic_layer(స్థిరరాశి layer l, జాలం net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#నిర్వచితమైతే GPU

శూన్య forward_logistic_layer_gpu(స్థిరరాశి layer l, జాలం net)
{
    copy_gpu(l.outputs*l.batch, net.ఆదానం_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, LOGISTIC);
    ఒకవేళ(net.truth){
        logistic_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.ఖర్చు_2[0] = కూడిక_array(l.loss, l.batch*l.inputs);
    }
}

శూన్య backward_logistic_layer_gpu(స్థిరరాశి layer l, జాలం net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#అయితే_ముగింపు
