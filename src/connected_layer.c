#అన్తర్భావ "connected_layer.h"
#అన్తర్భావ "convolutional_layer.h"
#అన్తర్భావ "batchnorm_layer.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "gemm.h"

#అన్తర్భావ <math.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>

layer make_connected_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, పూర్ణాంకం outputs, ACTIVATION activation, పూర్ణాంకం batch_normalize, పూర్ణాంకం adam)
{
    పూర్ణాంకం i;
    layer l = {0};
    l.learning_రేటు_scale = 1;
    l.type = CONNECTED;

    l.inputs = inputs;
    l.outputs = outputs;
    l.batch=batch;
    l.batch_normalize = batch_normalize;
    l.h = 1;
    l.w = 1;
    l.c = inputs;
    l.బహిర్గతం_h = 1;
    l.బహిర్గతం_w = 1;
    l.బహిర్గతం_c = outputs;

    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
    l.delta = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));

    l.బరువు_updates = సున్నా_జ్ఞాపకం_ఇవ్వు(inputs*outputs, పరిమాణం(దశాంశం));
    l.bias_updates = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs, పరిమాణం(దశాంశం));

    l.weights = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs*inputs, పరిమాణం(దశాంశం));
    l.biases = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs, పరిమాణం(దశాంశం));

    l.forward = forward_connected_layer;
    l.backward = backward_connected_layer;
    l.update = update_connected_layer;

    //float scale = 1./sqrt(inputs);
    దశాంశం scale = వర్గమూలం(2./inputs);
    కొరకు(i = 0; i < outputs*inputs; ++i){
        l.weights[i] = scale*rand_ఏకరీతి(-1, 1);
    }

    కొరకు(i = 0; i < outputs; ++i){
        l.biases[i] = 0;
    }

    ఒకవేళ(adam){
        l.m = సున్నా_జ్ఞాపకం_ఇవ్వు(l.inputs*l.outputs, పరిమాణం(దశాంశం));
        l.v = సున్నా_జ్ఞాపకం_ఇవ్వు(l.inputs*l.outputs, పరిమాణం(దశాంశం));
        l.bias_m = సున్నా_జ్ఞాపకం_ఇవ్వు(l.outputs, పరిమాణం(దశాంశం));
        l.scale_m = సున్నా_జ్ఞాపకం_ఇవ్వు(l.outputs, పరిమాణం(దశాంశం));
        l.bias_v = సున్నా_జ్ఞాపకం_ఇవ్వు(l.outputs, పరిమాణం(దశాంశం));
        l.scale_v = సున్నా_జ్ఞాపకం_ఇవ్వు(l.outputs, పరిమాణం(దశాంశం));
    }
    ఒకవేళ(batch_normalize){
        l.scales = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs, పరిమాణం(దశాంశం));
        l.scale_updates = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs, పరిమాణం(దశాంశం));
        కొరకు(i = 0; i < outputs; ++i){
            l.scales[i] = 1;
        }

        l.సగటు = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs, పరిమాణం(దశాంశం));
        l.సగటు_delta = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs, పరిమాణం(దశాంశం));
        l.variance = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs, పరిమాణం(దశాంశం));
        l.variance_delta = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs, పరిమాణం(దశాంశం));

        l.rolling_సగటు = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs, పరిమాణం(దశాంశం));
        l.rolling_variance = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs, పరిమాణం(దశాంశం));

        l.x = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
        l.x_norm = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
    }

#నిర్వచితమైతే GPU
    l.forward_gpu = forward_connected_layer_gpu;
    l.backward_gpu = backward_connected_layer_gpu;
    l.update_gpu = update_connected_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, outputs*inputs);
    l.biases_gpu = cuda_make_array(l.biases, outputs);

    l.బరువు_updates_gpu = cuda_make_array(l.బరువు_updates, outputs*inputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, outputs);

    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    l.delta_gpu = cuda_make_array(l.delta, outputs*batch);
    ఒకవేళ (adam) {
        l.m_gpu =       cuda_make_array(0, inputs*outputs);
        l.v_gpu =       cuda_make_array(0, inputs*outputs);
        l.bias_m_gpu =  cuda_make_array(0, outputs);
        l.bias_v_gpu =  cuda_make_array(0, outputs);
        l.scale_m_gpu = cuda_make_array(0, outputs);
        l.scale_v_gpu = cuda_make_array(0, outputs);
    }

    ఒకవేళ(batch_normalize){
        l.సగటు_gpu = cuda_make_array(l.సగటు, outputs);
        l.variance_gpu = cuda_make_array(l.variance, outputs);

        l.rolling_సగటు_gpu = cuda_make_array(l.సగటు, outputs);
        l.rolling_variance_gpu = cuda_make_array(l.variance, outputs);

        l.సగటు_delta_gpu = cuda_make_array(l.సగటు, outputs);
        l.variance_delta_gpu = cuda_make_array(l.variance, outputs);

        l.scales_gpu = cuda_make_array(l.scales, outputs);
        l.scale_updates_gpu = cuda_make_array(l.scale_updates, outputs);

        l.x_gpu = cuda_make_array(l.output, l.batch*outputs);
        l.x_norm_gpu = cuda_make_array(l.output, l.batch*outputs);
#నిర్వచితమైతే CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnn_సమితి_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h, l.బహిర్గతం_w); 
        cudnn_సమితి_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, 1, l.బహిర్గతం_c, 1, 1); 
#అయితే_ముగింపు
    }
#అయితే_ముగింపు
    l.activation = activation;
    fprintf(stderr, "connected                            %4d  ->  %4d\n", inputs, outputs);
    తిరిగి l;
}

శూన్య update_connected_layer(layer l, update_వాదనలు a)
{
    దశాంశం learning_రేటు = a.learning_రేటు*l.learning_రేటు_scale;
    దశాంశం momentum = a.momentum;
    దశాంశం decay = a.decay;
    పూర్ణాంకం batch = a.batch;
    axpy_cpu(l.outputs, learning_రేటు/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    ఒకవేళ(l.batch_normalize){
        axpy_cpu(l.outputs, learning_రేటు/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.outputs, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.inputs*l.outputs, -decay*batch, l.weights, 1, l.బరువు_updates, 1);
    axpy_cpu(l.inputs*l.outputs, learning_రేటు/batch, l.బరువు_updates, 1, l.weights, 1);
    scal_cpu(l.inputs*l.outputs, momentum, l.బరువు_updates, 1);
}

శూన్య forward_connected_layer(layer l, జాలం net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    పూర్ణాంకం m = l.batch;
    పూర్ణాంకం k = l.inputs;
    పూర్ణాంకం n = l.outputs;
    దశాంశం *a = net.ఆదానం;
    దశాంశం *b = l.weights;
    దశాంశం *c = l.output;
    gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);
    ఒకవేళ(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } లేకపోతే {
        కూడు_bias(l.output, l.biases, l.batch, l.outputs, 1);
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

శూన్య backward_connected_layer(layer l, జాలం net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ఒకవేళ(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } లేకపోతే {
        backward_bias(l.bias_updates, l.delta, l.batch, l.outputs, 1);
    }

    పూర్ణాంకం m = l.outputs;
    పూర్ణాంకం k = l.batch;
    పూర్ణాంకం n = l.inputs;
    దశాంశం *a = l.delta;
    దశాంశం *b = net.ఆదానం;
    దశాంశం *c = l.బరువు_updates;
    gemm(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta;
    b = l.weights;
    c = net.delta;

    ఒకవేళ(c) gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
}


శూన్య denormalize_connected_layer(layer l)
{
    పూర్ణాంకం i, j;
    కొరకు(i = 0; i < l.outputs; ++i){
        దశాంశం scale = l.scales[i]/వర్గమూలం(l.rolling_variance[i] + .000001);
        కొరకు(j = 0; j < l.inputs; ++j){
            l.weights[i*l.inputs + j] *= scale;
        }
        l.biases[i] -= l.rolling_సగటు[i] * scale;
        l.scales[i] = 1;
        l.rolling_సగటు[i] = 0;
        l.rolling_variance[i] = 1;
    }
}


శూన్య statistics_connected_layer(layer l)
{
    ఒకవేళ(l.batch_normalize){
        రాయి("Scales ");
        print_statistics(l.scales, l.outputs);
        /*
           printf("Rolling Mean ");
           print_statistics(l.rolling_mean, l.outputs);
           printf("Rolling Variance ");
           print_statistics(l.rolling_variance, l.outputs);
         */
    }
    రాయి("Biases ");
    print_statistics(l.biases, l.outputs);
    రాయి("Weights ");
    print_statistics(l.weights, l.outputs);
}

#నిర్వచితమైతే GPU

శూన్య pull_connected_layer(layer l)
{
    cuda_pull_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
    cuda_pull_array(l.బరువు_updates_gpu, l.బరువు_updates, l.inputs*l.outputs);
    cuda_pull_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    ఒకవేళ (l.batch_normalize){
        cuda_pull_array(l.scales_gpu, l.scales, l.outputs);
        cuda_pull_array(l.rolling_సగటు_gpu, l.rolling_సగటు, l.outputs);
        cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

శూన్య నెట్టు_connected_layer(layer l)
{
    cuda_నెట్టు_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_నెట్టు_array(l.biases_gpu, l.biases, l.outputs);
    cuda_నెట్టు_array(l.బరువు_updates_gpu, l.బరువు_updates, l.inputs*l.outputs);
    cuda_నెట్టు_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    ఒకవేళ (l.batch_normalize){
        cuda_నెట్టు_array(l.scales_gpu, l.scales, l.outputs);
        cuda_నెట్టు_array(l.rolling_సగటు_gpu, l.rolling_సగటు, l.outputs);
        cuda_నెట్టు_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

శూన్య update_connected_layer_gpu(layer l, update_వాదనలు a)
{
    దశాంశం learning_రేటు = a.learning_రేటు*l.learning_రేటు_scale;
    దశాంశం momentum = a.momentum;
    దశాంశం decay = a.decay;
    పూర్ణాంకం batch = a.batch;
    ఒకవేళ(a.adam){
        adam_update_gpu(l.weights_gpu, l.బరువు_updates_gpu, l.m_gpu, l.v_gpu, a.B1, a.B2, a.eps, decay, learning_రేటు, l.inputs*l.outputs, batch, a.t);
        adam_update_gpu(l.biases_gpu, l.bias_updates_gpu, l.bias_m_gpu, l.bias_v_gpu, a.B1, a.B2, a.eps, decay, learning_రేటు, l.outputs, batch, a.t);
        ఒకవేళ(l.scales_gpu){
            adam_update_gpu(l.scales_gpu, l.scale_updates_gpu, l.scale_m_gpu, l.scale_v_gpu, a.B1, a.B2, a.eps, decay, learning_రేటు, l.outputs, batch, a.t);
        }
    }లేకపోతే{
        axpy_gpu(l.outputs, learning_రేటు/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
        scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

        ఒకవేళ(l.batch_normalize){
            axpy_gpu(l.outputs, learning_రేటు/batch, l.scale_updates_gpu, 1, l.scales_gpu, 1);
            scal_gpu(l.outputs, momentum, l.scale_updates_gpu, 1);
        }

        axpy_gpu(l.inputs*l.outputs, -decay*batch, l.weights_gpu, 1, l.బరువు_updates_gpu, 1);
        axpy_gpu(l.inputs*l.outputs, learning_రేటు/batch, l.బరువు_updates_gpu, 1, l.weights_gpu, 1);
        scal_gpu(l.inputs*l.outputs, momentum, l.బరువు_updates_gpu, 1);
    }
}

శూన్య forward_connected_layer_gpu(layer l, జాలం net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);

    పూర్ణాంకం m = l.batch;
    పూర్ణాంకం k = l.inputs;
    పూర్ణాంకం n = l.outputs;
    దశాంశం * a = net.ఆదానం_gpu;
    దశాంశం * b = l.weights_gpu;
    దశాంశం * c = l.output_gpu;
    gemm_gpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

    ఒకవేళ (l.batch_normalize) {
        forward_batchnorm_layer_gpu(l, net);
    } లేకపోతే {
        కూడు_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.outputs, 1);
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

శూన్య backward_connected_layer_gpu(layer l, జాలం net)
{
    constrain_gpu(l.outputs*l.batch, 1, l.delta_gpu, 1);
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    ఒకవేళ(l.batch_normalize){
        backward_batchnorm_layer_gpu(l, net);
    } లేకపోతే {
        backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.outputs, 1);
    }

    పూర్ణాంకం m = l.outputs;
    పూర్ణాంకం k = l.batch;
    పూర్ణాంకం n = l.inputs;
    దశాంశం * a = l.delta_gpu;
    దశాంశం * b = net.ఆదానం_gpu;
    దశాంశం * c = l.బరువు_updates_gpu;
    gemm_gpu(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta_gpu;
    b = l.weights_gpu;
    c = net.delta_gpu;

    ఒకవేళ(c) gemm_gpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
}
#అయితే_ముగింపు
