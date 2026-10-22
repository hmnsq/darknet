#समावेश "connected_layer.h"
#समावेश "convolutional_layer.h"
#समावेश "batchnorm_layer.h"
#समावेश "utils.h"
#समावेश "cuda.h"
#समावेश "blas.h"
#समावेश "gemm.h"

#समावेश <math.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <string.h>

layer make_connected_layer(पूर्णांक batch, पूर्णांक inputs, पूर्णांक outputs, ACTIVATION activation, पूर्णांक batch_normalize, पूर्णांक adam)
{
    पूर्णांक i;
    layer l = {0};
    l.learning_rate_scale = 1;
    l.type = CONNECTED;

    l.inputs = inputs;
    l.outputs = outputs;
    l.batch=batch;
    l.batch_normalize = batch_normalize;
    l.h = 1;
    l.w = 1;
    l.c = inputs;
    l.out_h = 1;
    l.out_w = 1;
    l.out_c = outputs;

    l.output = सुस्मृति(batch*outputs, माप(भग्न));
    l.delta = सुस्मृति(batch*outputs, माप(भग्न));

    l.weight_updates = सुस्मृति(inputs*outputs, माप(भग्न));
    l.bias_updates = सुस्मृति(outputs, माप(भग्न));

    l.weights = सुस्मृति(outputs*inputs, माप(भग्न));
    l.biases = सुस्मृति(outputs, माप(भग्न));

    l.forward = forward_connected_layer;
    l.backward = backward_connected_layer;
    l.update = update_connected_layer;

    //float scale = 1./sqrt(inputs);
    भग्न scale = वर्ग_मूल(2./inputs);
    क्रम(i = 0; i < outputs*inputs; ++i){
        l.weights[i] = scale*rand_uniform(-1, 1);
    }

    क्रम(i = 0; i < outputs; ++i){
        l.biases[i] = 0;
    }

    अगर(adam){
        l.m = सुस्मृति(l.inputs*l.outputs, माप(भग्न));
        l.v = सुस्मृति(l.inputs*l.outputs, माप(भग्न));
        l.bias_m = सुस्मृति(l.outputs, माप(भग्न));
        l.scale_m = सुस्मृति(l.outputs, माप(भग्न));
        l.bias_v = सुस्मृति(l.outputs, माप(भग्न));
        l.scale_v = सुस्मृति(l.outputs, माप(भग्न));
    }
    अगर(batch_normalize){
        l.scales = सुस्मृति(outputs, माप(भग्न));
        l.scale_updates = सुस्मृति(outputs, माप(भग्न));
        क्रम(i = 0; i < outputs; ++i){
            l.scales[i] = 1;
        }

        l.mean = सुस्मृति(outputs, माप(भग्न));
        l.mean_delta = सुस्मृति(outputs, माप(भग्न));
        l.variance = सुस्मृति(outputs, माप(भग्न));
        l.variance_delta = सुस्मृति(outputs, माप(भग्न));

        l.rolling_mean = सुस्मृति(outputs, माप(भग्न));
        l.rolling_variance = सुस्मृति(outputs, माप(भग्न));

        l.x = सुस्मृति(batch*outputs, माप(भग्न));
        l.x_norm = सुस्मृति(batch*outputs, माप(भग्न));
    }

#अगर_घोषित GPU
    l.forward_gpu = forward_connected_layer_gpu;
    l.backward_gpu = backward_connected_layer_gpu;
    l.update_gpu = update_connected_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, outputs*inputs);
    l.biases_gpu = cuda_make_array(l.biases, outputs);

    l.weight_updates_gpu = cuda_make_array(l.weight_updates, outputs*inputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, outputs);

    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    l.delta_gpu = cuda_make_array(l.delta, outputs*batch);
    अगर (adam) {
        l.m_gpu =       cuda_make_array(0, inputs*outputs);
        l.v_gpu =       cuda_make_array(0, inputs*outputs);
        l.bias_m_gpu =  cuda_make_array(0, outputs);
        l.bias_v_gpu =  cuda_make_array(0, outputs);
        l.scale_m_gpu = cuda_make_array(0, outputs);
        l.scale_v_gpu = cuda_make_array(0, outputs);
    }

    अगर(batch_normalize){
        l.mean_gpu = cuda_make_array(l.mean, outputs);
        l.variance_gpu = cuda_make_array(l.variance, outputs);

        l.rolling_mean_gpu = cuda_make_array(l.mean, outputs);
        l.rolling_variance_gpu = cuda_make_array(l.variance, outputs);

        l.mean_delta_gpu = cuda_make_array(l.mean, outputs);
        l.variance_delta_gpu = cuda_make_array(l.variance, outputs);

        l.scales_gpu = cuda_make_array(l.scales, outputs);
        l.scale_updates_gpu = cuda_make_array(l.scale_updates, outputs);

        l.x_gpu = cuda_make_array(l.output, l.batch*outputs);
        l.x_norm_gpu = cuda_make_array(l.output, l.batch*outputs);
#अगर_घोषित CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnSetTensor4dDescriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, l.batch, l.out_c, l.out_h, l.out_w); 
        cudnnSetTensor4dDescriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, 1, l.out_c, 1, 1); 
#पूर्ण_अगर
    }
#पूर्ण_अगर
    l.activation = activation;
    ख_लिखो(मानक_त्रुटि, "connected                            %4d  ->  %4d\n", inputs, outputs);
    वापस l;
}

व्योम update_connected_layer(layer l, update_args a)
{
    भग्न learning_rate = a.learning_rate*l.learning_rate_scale;
    भग्न momentum = a.momentum;
    भग्न decay = a.decay;
    पूर्णांक batch = a.batch;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    अगर(l.batch_normalize){
        axpy_cpu(l.outputs, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.outputs, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.inputs*l.outputs, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.inputs*l.outputs, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.inputs*l.outputs, momentum, l.weight_updates, 1);
}

व्योम forward_connected_layer(layer l, network net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    पूर्णांक m = l.batch;
    पूर्णांक k = l.inputs;
    पूर्णांक n = l.outputs;
    भग्न *a = net.निवेश;
    भग्न *b = l.weights;
    भग्न *c = l.output;
    gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);
    अगर(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } अन्यथा {
        add_bias(l.output, l.biases, l.batch, l.outputs, 1);
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

व्योम backward_connected_layer(layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    अगर(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } अन्यथा {
        backward_bias(l.bias_updates, l.delta, l.batch, l.outputs, 1);
    }

    पूर्णांक m = l.outputs;
    पूर्णांक k = l.batch;
    पूर्णांक n = l.inputs;
    भग्न *a = l.delta;
    भग्न *b = net.निवेश;
    भग्न *c = l.weight_updates;
    gemm(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta;
    b = l.weights;
    c = net.delta;

    अगर(c) gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
}


व्योम denormalize_connected_layer(layer l)
{
    पूर्णांक i, j;
    क्रम(i = 0; i < l.outputs; ++i){
        भग्न scale = l.scales[i]/वर्ग_मूल(l.rolling_variance[i] + .000001);
        क्रम(j = 0; j < l.inputs; ++j){
            l.weights[i*l.inputs + j] *= scale;
        }
        l.biases[i] -= l.rolling_mean[i] * scale;
        l.scales[i] = 1;
        l.rolling_mean[i] = 0;
        l.rolling_variance[i] = 1;
    }
}


व्योम statistics_connected_layer(layer l)
{
    अगर(l.batch_normalize){
        म_लिखो("Scales ");
        print_statistics(l.scales, l.outputs);
        /*
           printf("Rolling Mean ");
           print_statistics(l.rolling_mean, l.outputs);
           printf("Rolling Variance ");
           print_statistics(l.rolling_variance, l.outputs);
         */
    }
    म_लिखो("Biases ");
    print_statistics(l.biases, l.outputs);
    म_लिखो("Weights ");
    print_statistics(l.weights, l.outputs);
}

#अगर_घोषित GPU

व्योम pull_connected_layer(layer l)
{
    cuda_pull_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
    cuda_pull_array(l.weight_updates_gpu, l.weight_updates, l.inputs*l.outputs);
    cuda_pull_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    अगर (l.batch_normalize){
        cuda_pull_array(l.scales_gpu, l.scales, l.outputs);
        cuda_pull_array(l.rolling_mean_gpu, l.rolling_mean, l.outputs);
        cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

व्योम धकेलो_connected_layer(layer l)
{
    cuda_धकेलो_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_धकेलो_array(l.biases_gpu, l.biases, l.outputs);
    cuda_धकेलो_array(l.weight_updates_gpu, l.weight_updates, l.inputs*l.outputs);
    cuda_धकेलो_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    अगर (l.batch_normalize){
        cuda_धकेलो_array(l.scales_gpu, l.scales, l.outputs);
        cuda_धकेलो_array(l.rolling_mean_gpu, l.rolling_mean, l.outputs);
        cuda_धकेलो_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

व्योम update_connected_layer_gpu(layer l, update_args a)
{
    भग्न learning_rate = a.learning_rate*l.learning_rate_scale;
    भग्न momentum = a.momentum;
    भग्न decay = a.decay;
    पूर्णांक batch = a.batch;
    अगर(a.adam){
        adam_update_gpu(l.weights_gpu, l.weight_updates_gpu, l.m_gpu, l.v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.inputs*l.outputs, batch, a.t);
        adam_update_gpu(l.biases_gpu, l.bias_updates_gpu, l.bias_m_gpu, l.bias_v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.outputs, batch, a.t);
        अगर(l.scales_gpu){
            adam_update_gpu(l.scales_gpu, l.scale_updates_gpu, l.scale_m_gpu, l.scale_v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.outputs, batch, a.t);
        }
    }अन्यथा{
        axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
        scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

        अगर(l.batch_normalize){
            axpy_gpu(l.outputs, learning_rate/batch, l.scale_updates_gpu, 1, l.scales_gpu, 1);
            scal_gpu(l.outputs, momentum, l.scale_updates_gpu, 1);
        }

        axpy_gpu(l.inputs*l.outputs, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
        axpy_gpu(l.inputs*l.outputs, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
        scal_gpu(l.inputs*l.outputs, momentum, l.weight_updates_gpu, 1);
    }
}

व्योम forward_connected_layer_gpu(layer l, network net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);

    पूर्णांक m = l.batch;
    पूर्णांक k = l.inputs;
    पूर्णांक n = l.outputs;
    भग्न * a = net.निवेश_gpu;
    भग्न * b = l.weights_gpu;
    भग्न * c = l.output_gpu;
    gemm_gpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

    अगर (l.batch_normalize) {
        forward_batchnorm_layer_gpu(l, net);
    } अन्यथा {
        add_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.outputs, 1);
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

व्योम backward_connected_layer_gpu(layer l, network net)
{
    constrain_gpu(l.outputs*l.batch, 1, l.delta_gpu, 1);
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    अगर(l.batch_normalize){
        backward_batchnorm_layer_gpu(l, net);
    } अन्यथा {
        backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.outputs, 1);
    }

    पूर्णांक m = l.outputs;
    पूर्णांक k = l.batch;
    पूर्णांक n = l.inputs;
    भग्न * a = l.delta_gpu;
    भग्न * b = net.निवेश_gpu;
    भग्न * c = l.weight_updates_gpu;
    gemm_gpu(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta_gpu;
    b = l.weights_gpu;
    c = net.delta_gpu;

    अगर(c) gemm_gpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
}
#पूर्ण_अगर
