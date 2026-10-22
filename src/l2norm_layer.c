#అన్తర్భావ "l2norm_layer.h"
#అన్తర్భావ "activations.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "cuda.h"

#అన్తర్భావ <float.h>
#అన్తర్భావ <math.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <assert.h>

layer make_l2norm_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs)
{
    fprintf(stderr, "l2norm                                         %4d\n",  inputs);
    layer l = {0};
    l.type = L2NORM;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(inputs*batch, పరిమాణం(దశాంశం));
    l.scales = సున్నా_జ్ఞాపకం_ఇవ్వు(inputs*batch, పరిమాణం(దశాంశం));
    l.delta = సున్నా_జ్ఞాపకం_ఇవ్వు(inputs*batch, పరిమాణం(దశాంశం));

    l.forward = forward_l2norm_layer;
    l.backward = backward_l2norm_layer;
    #నిర్వచితమైతే GPU
    l.forward_gpu = forward_l2norm_layer_gpu;
    l.backward_gpu = backward_l2norm_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.scales_gpu = cuda_make_array(l.output, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #అయితే_ముగింపు
    తిరిగి l;
}

శూన్య forward_l2norm_layer(స్థిరరాశి layer l, జాలం net)
{
    copy_cpu(l.outputs*l.batch, net.ఆదానం, 1, l.output, 1);
    l2normalize_cpu(l.output, l.scales, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_w*l.బహిర్గతం_h);
}

శూన్య backward_l2norm_layer(స్థిరరాశి layer l, జాలం net)
{
    //axpy_cpu(l.inputs*l.batch, 1, l.scales, 1, l.delta, 1);
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#నిర్వచితమైతే GPU

శూన్య forward_l2norm_layer_gpu(స్థిరరాశి layer l, జాలం net)
{
    copy_gpu(l.outputs*l.batch, net.ఆదానం_gpu, 1, l.output_gpu, 1);
    l2normalize_gpu(l.output_gpu, l.scales_gpu, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_w*l.బహిర్గతం_h);
}

శూన్య backward_l2norm_layer_gpu(స్థిరరాశి layer l, జాలం net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.scales_gpu, 1, l.delta_gpu, 1);
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#అయితే_ముగింపు
