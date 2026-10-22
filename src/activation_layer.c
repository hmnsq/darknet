#అన్తర్భావ "activation_layer.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "gemm.h"

#అన్తర్భావ <math.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>

layer make_activation_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, ACTIVATION activation)
{
    layer l = {0};
    l.type = చురుకైన;

    l.inputs = inputs;
    l.outputs = inputs;
    l.batch=batch;

    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*inputs, పరిమాణం(దశాంశం*));
    l.delta = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*inputs, పరిమాణం(దశాంశం*));

    l.forward = forward_activation_layer;
    l.backward = backward_activation_layer;
#నిర్వచితమైతే GPU
    l.forward_gpu = forward_activation_layer_gpu;
    l.backward_gpu = backward_activation_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch);
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch);
#అయితే_ముగింపు
    l.activation = activation;
    fprintf(stderr, "Activation Layer: %d inputs\n", inputs);
    తిరిగి l;
}

శూన్య forward_activation_layer(layer l, జాలం net)
{
    copy_cpu(l.outputs*l.batch, net.ఆదానం, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

శూన్య backward_activation_layer(layer l, జాలం net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#నిర్వచితమైతే GPU

శూన్య forward_activation_layer_gpu(layer l, జాలం net)
{
    copy_gpu(l.outputs*l.batch, net.ఆదానం_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

శూన్య backward_activation_layer_gpu(layer l, జాలం net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#అయితే_ముగింపు
