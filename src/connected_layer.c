#അന്തര്ഭാവ "connected_layer.h"
#അന്തര്ഭാവ "convolutional_layer.h"
#അന്തര്ഭാവ "batchnorm_layer.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "gemm.h"

#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>

layer make_connected_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, പൂർണ്ണസംഖ്യ outputs, ACTIVATION activation, പൂർണ്ണസംഖ്യ batch_normalize, പൂർണ്ണസംഖ്യ adam)
{
    പൂർണ്ണസംഖ്യ i;
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

    l.output = calloc(batch*outputs, വലുപ്പം(ദശാംശം));
    l.delta = calloc(batch*outputs, വലുപ്പം(ദശാംശം));

    l.weight_updates = calloc(inputs*outputs, വലുപ്പം(ദശാംശം));
    l.bias_updates = calloc(outputs, വലുപ്പം(ദശാംശം));

    l.weights = calloc(outputs*inputs, വലുപ്പം(ദശാംശം));
    l.biases = calloc(outputs, വലുപ്പം(ദശാംശം));

    l.forward = forward_connected_layer;
    l.backward = backward_connected_layer;
    l.update = update_connected_layer;

    //float scale = 1./sqrt(inputs);
    ദശാംശം scale = sqrt(2./inputs);
    വേണ്ടി(i = 0; i < outputs*inputs; ++i){
        l.weights[i] = scale*rand_uniform(-1, 1);
    }

    വേണ്ടി(i = 0; i < outputs; ++i){
        l.biases[i] = 0;
    }

    എങ്കിൽ(adam){
        l.m = calloc(l.inputs*l.outputs, വലുപ്പം(ദശാംശം));
        l.v = calloc(l.inputs*l.outputs, വലുപ്പം(ദശാംശം));
        l.bias_m = calloc(l.outputs, വലുപ്പം(ദശാംശം));
        l.scale_m = calloc(l.outputs, വലുപ്പം(ദശാംശം));
        l.bias_v = calloc(l.outputs, വലുപ്പം(ദശാംശം));
        l.scale_v = calloc(l.outputs, വലുപ്പം(ദശാംശം));
    }
    എങ്കിൽ(batch_normalize){
        l.scales = calloc(outputs, വലുപ്പം(ദശാംശം));
        l.scale_updates = calloc(outputs, വലുപ്പം(ദശാംശം));
        വേണ്ടി(i = 0; i < outputs; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(outputs, വലുപ്പം(ദശാംശം));
        l.mean_delta = calloc(outputs, വലുപ്പം(ദശാംശം));
        l.variance = calloc(outputs, വലുപ്പം(ദശാംശം));
        l.variance_delta = calloc(outputs, വലുപ്പം(ദശാംശം));

        l.rolling_mean = calloc(outputs, വലുപ്പം(ദശാംശം));
        l.rolling_variance = calloc(outputs, വലുപ്പം(ദശാംശം));

        l.x = calloc(batch*outputs, വലുപ്പം(ദശാംശം));
        l.x_norm = calloc(batch*outputs, വലുപ്പം(ദശാംശം));
    }

#ifdef GPU
    l.forward_gpu = forward_connected_layer_gpu;
    l.backward_gpu = backward_connected_layer_gpu;
    l.update_gpu = update_connected_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, outputs*inputs);
    l.biases_gpu = cuda_make_array(l.biases, outputs);

    l.weight_updates_gpu = cuda_make_array(l.weight_updates, outputs*inputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, outputs);

    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    l.delta_gpu = cuda_make_array(l.delta, outputs*batch);
    എങ്കിൽ (adam) {
        l.m_gpu =       cuda_make_array(0, inputs*outputs);
        l.v_gpu =       cuda_make_array(0, inputs*outputs);
        l.bias_m_gpu =  cuda_make_array(0, outputs);
        l.bias_v_gpu =  cuda_make_array(0, outputs);
        l.scale_m_gpu = cuda_make_array(0, outputs);
        l.scale_v_gpu = cuda_make_array(0, outputs);
    }

    എങ്കിൽ(batch_normalize){
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
#ifdef CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnSetTensor4dDescriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l.batch, l.out_c, l.out_h, l.out_w); 
        cudnnSetTensor4dDescriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l.out_c, 1, 1); 
#endif
    }
#endif
    l.activation = activation;
    fprintf(stderr, "connected                            %4d  ->  %4d\n", inputs, outputs);
    തിരികെ l;
}

ശൂന്യം update_connected_layer(layer l, update_args a)
{
    ദശാംശം learning_rate = a.learning_rate*l.learning_rate_scale;
    ദശാംശം momentum = a.momentum;
    ദശാംശം decay = a.decay;
    പൂർണ്ണസംഖ്യ batch = a.batch;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    എങ്കിൽ(l.batch_normalize){
        axpy_cpu(l.outputs, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.outputs, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.inputs*l.outputs, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.inputs*l.outputs, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.inputs*l.outputs, momentum, l.weight_updates, 1);
}

ശൂന്യം forward_connected_layer(layer l, network net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    പൂർണ്ണസംഖ്യ m = l.batch;
    പൂർണ്ണസംഖ്യ k = l.inputs;
    പൂർണ്ണസംഖ്യ n = l.outputs;
    ദശാംശം *a = net.നിവേശം;
    ദശാംശം *b = l.weights;
    ദശാംശം *c = l.output;
    gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);
    എങ്കിൽ(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } അല്ലെങ്കിൽ {
        add_bias(l.output, l.biases, l.batch, l.outputs, 1);
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ശൂന്യം backward_connected_layer(layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    എങ്കിൽ(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } അല്ലെങ്കിൽ {
        backward_bias(l.bias_updates, l.delta, l.batch, l.outputs, 1);
    }

    പൂർണ്ണസംഖ്യ m = l.outputs;
    പൂർണ്ണസംഖ്യ k = l.batch;
    പൂർണ്ണസംഖ്യ n = l.inputs;
    ദശാംശം *a = l.delta;
    ദശാംശം *b = net.നിവേശം;
    ദശാംശം *c = l.weight_updates;
    gemm(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta;
    b = l.weights;
    c = net.delta;

    എങ്കിൽ(c) gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
}


ശൂന്യം denormalize_connected_layer(layer l)
{
    പൂർണ്ണസംഖ്യ i, j;
    വേണ്ടി(i = 0; i < l.outputs; ++i){
        ദശാംശം scale = l.scales[i]/sqrt(l.rolling_variance[i] + .000001);
        വേണ്ടി(j = 0; j < l.inputs; ++j){
            l.weights[i*l.inputs + j] *= scale;
        }
        l.biases[i] -= l.rolling_mean[i] * scale;
        l.scales[i] = 1;
        l.rolling_mean[i] = 0;
        l.rolling_variance[i] = 1;
    }
}


ശൂന്യം statistics_connected_layer(layer l)
{
    എങ്കിൽ(l.batch_normalize){
        printf("Scales ");
        print_statistics(l.scales, l.outputs);
        /*
           printf("Rolling Mean ");
           print_statistics(l.rolling_mean, l.outputs);
           printf("Rolling Variance ");
           print_statistics(l.rolling_variance, l.outputs);
         */
    }
    printf("Biases ");
    print_statistics(l.biases, l.outputs);
    printf("Weights ");
    print_statistics(l.weights, l.outputs);
}

#ifdef GPU

ശൂന്യം pull_connected_layer(layer l)
{
    cuda_pull_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
    cuda_pull_array(l.weight_updates_gpu, l.weight_updates, l.inputs*l.outputs);
    cuda_pull_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    എങ്കിൽ (l.batch_normalize){
        cuda_pull_array(l.scales_gpu, l.scales, l.outputs);
        cuda_pull_array(l.rolling_mean_gpu, l.rolling_mean, l.outputs);
        cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

ശൂന്യം തള്ളുക_connected_layer(layer l)
{
    cuda_തള്ളുക_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_തള്ളുക_array(l.biases_gpu, l.biases, l.outputs);
    cuda_തള്ളുക_array(l.weight_updates_gpu, l.weight_updates, l.inputs*l.outputs);
    cuda_തള്ളുക_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    എങ്കിൽ (l.batch_normalize){
        cuda_തള്ളുക_array(l.scales_gpu, l.scales, l.outputs);
        cuda_തള്ളുക_array(l.rolling_mean_gpu, l.rolling_mean, l.outputs);
        cuda_തള്ളുക_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

ശൂന്യം update_connected_layer_gpu(layer l, update_args a)
{
    ദശാംശം learning_rate = a.learning_rate*l.learning_rate_scale;
    ദശാംശം momentum = a.momentum;
    ദശാംശം decay = a.decay;
    പൂർണ്ണസംഖ്യ batch = a.batch;
    എങ്കിൽ(a.adam){
        adam_update_gpu(l.weights_gpu, l.weight_updates_gpu, l.m_gpu, l.v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.inputs*l.outputs, batch, a.t);
        adam_update_gpu(l.biases_gpu, l.bias_updates_gpu, l.bias_m_gpu, l.bias_v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.outputs, batch, a.t);
        എങ്കിൽ(l.scales_gpu){
            adam_update_gpu(l.scales_gpu, l.scale_updates_gpu, l.scale_m_gpu, l.scale_v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.outputs, batch, a.t);
        }
    }അല്ലെങ്കിൽ{
        axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
        scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

        എങ്കിൽ(l.batch_normalize){
            axpy_gpu(l.outputs, learning_rate/batch, l.scale_updates_gpu, 1, l.scales_gpu, 1);
            scal_gpu(l.outputs, momentum, l.scale_updates_gpu, 1);
        }

        axpy_gpu(l.inputs*l.outputs, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
        axpy_gpu(l.inputs*l.outputs, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
        scal_gpu(l.inputs*l.outputs, momentum, l.weight_updates_gpu, 1);
    }
}

ശൂന്യം forward_connected_layer_gpu(layer l, network net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);

    പൂർണ്ണസംഖ്യ m = l.batch;
    പൂർണ്ണസംഖ്യ k = l.inputs;
    പൂർണ്ണസംഖ്യ n = l.outputs;
    ദശാംശം * a = net.നിവേശം_gpu;
    ദശാംശം * b = l.weights_gpu;
    ദശാംശം * c = l.output_gpu;
    gemm_gpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

    എങ്കിൽ (l.batch_normalize) {
        forward_batchnorm_layer_gpu(l, net);
    } അല്ലെങ്കിൽ {
        add_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.outputs, 1);
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ശൂന്യം backward_connected_layer_gpu(layer l, network net)
{
    constrain_gpu(l.outputs*l.batch, 1, l.delta_gpu, 1);
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    എങ്കിൽ(l.batch_normalize){
        backward_batchnorm_layer_gpu(l, net);
    } അല്ലെങ്കിൽ {
        backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.outputs, 1);
    }

    പൂർണ്ണസംഖ്യ m = l.outputs;
    പൂർണ്ണസംഖ്യ k = l.batch;
    പൂർണ്ണസംഖ്യ n = l.inputs;
    ദശാംശം * a = l.delta_gpu;
    ദശാംശം * b = net.നിവേശം_gpu;
    ദശാംശം * c = l.weight_updates_gpu;
    gemm_gpu(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta_gpu;
    b = l.weights_gpu;
    c = net.delta_gpu;

    എങ്കിൽ(c) gemm_gpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
}
#endif
