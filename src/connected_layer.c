#अन्तर्भाव "connected_layer.h"
#अन्तर्भाव "convolutional_layer.h"
#अन्तर्भाव "batchnorm_layer.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "gemm.h"

#अन्तर्भाव <math.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

layer make_connected_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, पूर्णाङ्क outputs, ACTIVATION activation, पूर्णाङ्क batch_normalize, पूर्णाङ्क adam)
{
    पूर्णाङ्क i;
    layer l = {0};
    l.learning_दरः_scale = 1;
    l.type = CONNECTED;

    l.inputs = inputs;
    l.outputs = outputs;
    l.batch=batch;
    l.batch_normalize = batch_normalize;
    l.h = 1;
    l.w = 1;
    l.c = inputs;
    l.बहिर्गमनम्_h = 1;
    l.बहिर्गमनम्_w = 1;
    l.बहिर्गमनम्_c = outputs;

    l.output = शून्यस्मृतिं_देहि(batch*outputs, परिमाण(भिन्न));
    l.delta = शून्यस्मृतिं_देहि(batch*outputs, परिमाण(भिन्न));

    l.weight_updates = शून्यस्मृतिं_देहि(inputs*outputs, परिमाण(भिन्न));
    l.bias_updates = शून्यस्मृतिं_देहि(outputs, परिमाण(भिन्न));

    l.weights = शून्यस्मृतिं_देहि(outputs*inputs, परिमाण(भिन्न));
    l.biases = शून्यस्मृतिं_देहि(outputs, परिमाण(भिन्न));

    l.forward = forward_connected_layer;
    l.backward = backward_connected_layer;
    l.update = update_connected_layer;

    //float scale = 1./sqrt(inputs);
    भिन्न scale = वर्गमूलम्(2./inputs);
    प्रति(i = 0; i < outputs*inputs; ++i){
        l.weights[i] = scale*rand_एकरूपम्(-1, 1);
    }

    प्रति(i = 0; i < outputs; ++i){
        l.biases[i] = 0;
    }

    यदि(adam){
        l.m = शून्यस्मृतिं_देहि(l.inputs*l.outputs, परिमाण(भिन्न));
        l.v = शून्यस्मृतिं_देहि(l.inputs*l.outputs, परिमाण(भिन्न));
        l.bias_m = शून्यस्मृतिं_देहि(l.outputs, परिमाण(भिन्न));
        l.scale_m = शून्यस्मृतिं_देहि(l.outputs, परिमाण(भिन्न));
        l.bias_v = शून्यस्मृतिं_देहि(l.outputs, परिमाण(भिन्न));
        l.scale_v = शून्यस्मृतिं_देहि(l.outputs, परिमाण(भिन्न));
    }
    यदि(batch_normalize){
        l.scales = शून्यस्मृतिं_देहि(outputs, परिमाण(भिन्न));
        l.scale_updates = शून्यस्मृतिं_देहि(outputs, परिमाण(भिन्न));
        प्रति(i = 0; i < outputs; ++i){
            l.scales[i] = 1;
        }

        l.माध्यम् = शून्यस्मृतिं_देहि(outputs, परिमाण(भिन्न));
        l.माध्यम्_delta = शून्यस्मृतिं_देहि(outputs, परिमाण(भिन्न));
        l.variance = शून्यस्मृतिं_देहि(outputs, परिमाण(भिन्न));
        l.variance_delta = शून्यस्मृतिं_देहि(outputs, परिमाण(भिन्न));

        l.rolling_माध्यम् = शून्यस्मृतिं_देहि(outputs, परिमाण(भिन्न));
        l.rolling_variance = शून्यस्मृतिं_देहि(outputs, परिमाण(भिन्न));

        l.x = शून्यस्मृतिं_देहि(batch*outputs, परिमाण(भिन्न));
        l.x_norm = शून्यस्मृतिं_देहि(batch*outputs, परिमाण(भिन्न));
    }

#यदि_परिभाषितम् GPU
    l.forward_gpu = forward_connected_layer_gpu;
    l.backward_gpu = backward_connected_layer_gpu;
    l.update_gpu = update_connected_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, outputs*inputs);
    l.biases_gpu = cuda_make_array(l.biases, outputs);

    l.weight_updates_gpu = cuda_make_array(l.weight_updates, outputs*inputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, outputs);

    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    l.delta_gpu = cuda_make_array(l.delta, outputs*batch);
    यदि (adam) {
        l.m_gpu =       cuda_make_array(0, inputs*outputs);
        l.v_gpu =       cuda_make_array(0, inputs*outputs);
        l.bias_m_gpu =  cuda_make_array(0, outputs);
        l.bias_v_gpu =  cuda_make_array(0, outputs);
        l.scale_m_gpu = cuda_make_array(0, outputs);
        l.scale_v_gpu = cuda_make_array(0, outputs);
    }

    यदि(batch_normalize){
        l.माध्यम्_gpu = cuda_make_array(l.माध्यम्, outputs);
        l.variance_gpu = cuda_make_array(l.variance, outputs);

        l.rolling_माध्यम्_gpu = cuda_make_array(l.माध्यम्, outputs);
        l.rolling_variance_gpu = cuda_make_array(l.variance, outputs);

        l.माध्यम्_delta_gpu = cuda_make_array(l.माध्यम्, outputs);
        l.variance_delta_gpu = cuda_make_array(l.variance, outputs);

        l.scales_gpu = cuda_make_array(l.scales, outputs);
        l.scale_updates_gpu = cuda_make_array(l.scale_updates, outputs);

        l.x_gpu = cuda_make_array(l.output, l.batch*outputs);
        l.x_norm_gpu = cuda_make_array(l.output, l.batch*outputs);
#यदि_परिभाषितम् CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnn_समुच्चयः_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_दत्त_FLOAT, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h, l.बहिर्गमनम्_w); 
        cudnn_समुच्चयः_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_दत्त_FLOAT, 1, l.बहिर्गमनम्_c, 1, 1); 
#यदि_अन्तः
    }
#यदि_अन्तः
    l.activation = activation;
    fprintf(stderr, "connected                            %4d  ->  %4d\n", inputs, outputs);
    प्रत्यागम l;
}

शून्य update_connected_layer(layer l, update_तर्काः a)
{
    भिन्न learning_दरः = a.learning_दरः*l.learning_दरः_scale;
    भिन्न momentum = a.momentum;
    भिन्न decay = a.decay;
    पूर्णाङ्क batch = a.batch;
    axpy_cpu(l.outputs, learning_दरः/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    यदि(l.batch_normalize){
        axpy_cpu(l.outputs, learning_दरः/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.outputs, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.inputs*l.outputs, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.inputs*l.outputs, learning_दरः/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.inputs*l.outputs, momentum, l.weight_updates, 1);
}

शून्य forward_connected_layer(layer l, जालम् net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    पूर्णाङ्क m = l.batch;
    पूर्णाङ्क k = l.inputs;
    पूर्णाङ्क n = l.outputs;
    भिन्न *a = net.निवेशः;
    भिन्न *b = l.weights;
    भिन्न *c = l.output;
    gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);
    यदि(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } अन्यथा {
        सङ्कलय_bias(l.output, l.biases, l.batch, l.outputs, 1);
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

शून्य backward_connected_layer(layer l, जालम् net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    यदि(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } अन्यथा {
        backward_bias(l.bias_updates, l.delta, l.batch, l.outputs, 1);
    }

    पूर्णाङ्क m = l.outputs;
    पूर्णाङ्क k = l.batch;
    पूर्णाङ्क n = l.inputs;
    भिन्न *a = l.delta;
    भिन्न *b = net.निवेशः;
    भिन्न *c = l.weight_updates;
    gemm(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta;
    b = l.weights;
    c = net.delta;

    यदि(c) gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
}


शून्य denormalize_connected_layer(layer l)
{
    पूर्णाङ्क i, j;
    प्रति(i = 0; i < l.outputs; ++i){
        भिन्न scale = l.scales[i]/वर्गमूलम्(l.rolling_variance[i] + .000001);
        प्रति(j = 0; j < l.inputs; ++j){
            l.weights[i*l.inputs + j] *= scale;
        }
        l.biases[i] -= l.rolling_माध्यम्[i] * scale;
        l.scales[i] = 1;
        l.rolling_माध्यम्[i] = 0;
        l.rolling_variance[i] = 1;
    }
}


शून्य statistics_connected_layer(layer l)
{
    यदि(l.batch_normalize){
        प्रारूप_लिख("Scales ");
        print_statistics(l.scales, l.outputs);
        /*
           printf("Rolling Mean ");
           print_statistics(l.rolling_mean, l.outputs);
           printf("Rolling Variance ");
           print_statistics(l.rolling_variance, l.outputs);
         */
    }
    प्रारूप_लिख("Biases ");
    print_statistics(l.biases, l.outputs);
    प्रारूप_लिख("Weights ");
    print_statistics(l.weights, l.outputs);
}

#यदि_परिभाषितम् GPU

शून्य pull_connected_layer(layer l)
{
    cuda_pull_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
    cuda_pull_array(l.weight_updates_gpu, l.weight_updates, l.inputs*l.outputs);
    cuda_pull_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    यदि (l.batch_normalize){
        cuda_pull_array(l.scales_gpu, l.scales, l.outputs);
        cuda_pull_array(l.rolling_माध्यम्_gpu, l.rolling_माध्यम्, l.outputs);
        cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

शून्य प्रक्षिप_connected_layer(layer l)
{
    cuda_प्रक्षिप_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_प्रक्षिप_array(l.biases_gpu, l.biases, l.outputs);
    cuda_प्रक्षिप_array(l.weight_updates_gpu, l.weight_updates, l.inputs*l.outputs);
    cuda_प्रक्षिप_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    यदि (l.batch_normalize){
        cuda_प्रक्षिप_array(l.scales_gpu, l.scales, l.outputs);
        cuda_प्रक्षिप_array(l.rolling_माध्यम्_gpu, l.rolling_माध्यम्, l.outputs);
        cuda_प्रक्षिप_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

शून्य update_connected_layer_gpu(layer l, update_तर्काः a)
{
    भिन्न learning_दरः = a.learning_दरः*l.learning_दरः_scale;
    भिन्न momentum = a.momentum;
    भिन्न decay = a.decay;
    पूर्णाङ्क batch = a.batch;
    यदि(a.adam){
        adam_update_gpu(l.weights_gpu, l.weight_updates_gpu, l.m_gpu, l.v_gpu, a.B1, a.B2, a.eps, decay, learning_दरः, l.inputs*l.outputs, batch, a.t);
        adam_update_gpu(l.biases_gpu, l.bias_updates_gpu, l.bias_m_gpu, l.bias_v_gpu, a.B1, a.B2, a.eps, decay, learning_दरः, l.outputs, batch, a.t);
        यदि(l.scales_gpu){
            adam_update_gpu(l.scales_gpu, l.scale_updates_gpu, l.scale_m_gpu, l.scale_v_gpu, a.B1, a.B2, a.eps, decay, learning_दरः, l.outputs, batch, a.t);
        }
    }अन्यथा{
        axpy_gpu(l.outputs, learning_दरः/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
        scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

        यदि(l.batch_normalize){
            axpy_gpu(l.outputs, learning_दरः/batch, l.scale_updates_gpu, 1, l.scales_gpu, 1);
            scal_gpu(l.outputs, momentum, l.scale_updates_gpu, 1);
        }

        axpy_gpu(l.inputs*l.outputs, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
        axpy_gpu(l.inputs*l.outputs, learning_दरः/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
        scal_gpu(l.inputs*l.outputs, momentum, l.weight_updates_gpu, 1);
    }
}

शून्य forward_connected_layer_gpu(layer l, जालम् net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);

    पूर्णाङ्क m = l.batch;
    पूर्णाङ्क k = l.inputs;
    पूर्णाङ्क n = l.outputs;
    भिन्न * a = net.निवेशः_gpu;
    भिन्न * b = l.weights_gpu;
    भिन्न * c = l.output_gpu;
    gemm_gpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

    यदि (l.batch_normalize) {
        forward_batchnorm_layer_gpu(l, net);
    } अन्यथा {
        सङ्कलय_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.outputs, 1);
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

शून्य backward_connected_layer_gpu(layer l, जालम् net)
{
    constrain_gpu(l.outputs*l.batch, 1, l.delta_gpu, 1);
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    यदि(l.batch_normalize){
        backward_batchnorm_layer_gpu(l, net);
    } अन्यथा {
        backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.outputs, 1);
    }

    पूर्णाङ्क m = l.outputs;
    पूर्णाङ्क k = l.batch;
    पूर्णाङ्क n = l.inputs;
    भिन्न * a = l.delta_gpu;
    भिन्न * b = net.निवेशः_gpu;
    भिन्न * c = l.weight_updates_gpu;
    gemm_gpu(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta_gpu;
    b = l.weights_gpu;
    c = net.delta_gpu;

    यदि(c) gemm_gpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
}
#यदि_अन्तः
