#ਅਨ੍ਤਰ੍ਭਾਵ "connected_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "convolutional_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "batchnorm_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gemm.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

layer make_connected_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਪੂਰਨ outputs, ACTIVATION activation, ਪੂਰਨ batch_normalize, ਪੂਰਨ adam)
{
    ਪੂਰਨ i;
    layer l = {0};
    l.learning_ਦਰ_scale = 1;
    l.type = CONNECTED;

    l.inputs = inputs;
    l.outputs = outputs;
    l.batch=batch;
    l.batch_normalize = batch_normalize;
    l.h = 1;
    l.w = 1;
    l.c = inputs;
    l.ਬਾਹਰਲਾ_h = 1;
    l.ਬਾਹਰਲਾ_w = 1;
    l.ਬਾਹਰਲਾ_c = outputs;

    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.ਭਾਰ_updates = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(inputs*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.bias_updates = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.weights = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs*inputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.biases = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.forward = forward_connected_layer;
    l.backward = backward_connected_layer;
    l.update = update_connected_layer;

    //float scale = 1./sqrt(inputs);
    ਦਸ਼ਮਲਵ scale = ਵਰਗਮੂਲ(2./inputs);
    ਲਈ(i = 0; i < outputs*inputs; ++i){
        l.weights[i] = scale*rand_ਇਕਸਾਰ(-1, 1);
    }

    ਲਈ(i = 0; i < outputs; ++i){
        l.biases[i] = 0;
    }

    ਜੇ(adam){
        l.m = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.inputs*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.v = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.inputs*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.bias_m = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.scale_m = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.bias_v = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.scale_v = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    }
    ਜੇ(batch_normalize){
        l.scales = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.scale_updates = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        ਲਈ(i = 0; i < outputs; ++i){
            l.scales[i] = 1;
        }

        l.ਔਸਤ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.ਔਸਤ_delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.variance = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.variance_delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

        l.rolling_ਔਸਤ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.rolling_variance = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

        l.x = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.x_norm = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_connected_layer_gpu;
    l.backward_gpu = backward_connected_layer_gpu;
    l.update_gpu = update_connected_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, outputs*inputs);
    l.biases_gpu = cuda_make_array(l.biases, outputs);

    l.ਭਾਰ_updates_gpu = cuda_make_array(l.ਭਾਰ_updates, outputs*inputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, outputs);

    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    l.delta_gpu = cuda_make_array(l.delta, outputs*batch);
    ਜੇ (adam) {
        l.m_gpu =       cuda_make_array(0, inputs*outputs);
        l.v_gpu =       cuda_make_array(0, inputs*outputs);
        l.bias_m_gpu =  cuda_make_array(0, outputs);
        l.bias_v_gpu =  cuda_make_array(0, outputs);
        l.scale_m_gpu = cuda_make_array(0, outputs);
        l.scale_v_gpu = cuda_make_array(0, outputs);
    }

    ਜੇ(batch_normalize){
        l.ਔਸਤ_gpu = cuda_make_array(l.ਔਸਤ, outputs);
        l.variance_gpu = cuda_make_array(l.variance, outputs);

        l.rolling_ਔਸਤ_gpu = cuda_make_array(l.ਔਸਤ, outputs);
        l.rolling_variance_gpu = cuda_make_array(l.variance, outputs);

        l.ਔਸਤ_delta_gpu = cuda_make_array(l.ਔਸਤ, outputs);
        l.variance_delta_gpu = cuda_make_array(l.variance, outputs);

        l.scales_gpu = cuda_make_array(l.scales, outputs);
        l.scale_updates_gpu = cuda_make_array(l.scale_updates, outputs);

        l.x_gpu = cuda_make_array(l.output, l.batch*outputs);
        l.x_norm_gpu = cuda_make_array(l.output, l.batch*outputs);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_w); 
        cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, 1, l.ਬਾਹਰਲਾ_c, 1, 1); 
#ਜੇ_ਅੰਤ
    }
#ਜੇ_ਅੰਤ
    l.activation = activation;
    fprintf(stderr, "connected                            %4d  ->  %4d\n", inputs, outputs);
    ਵਾਪਸ l;
}

ਖਾਲੀ update_connected_layer(layer l, update_ਦਲੀਲਾਂ a)
{
    ਦਸ਼ਮਲਵ learning_ਦਰ = a.learning_ਦਰ*l.learning_ਦਰ_scale;
    ਦਸ਼ਮਲਵ momentum = a.momentum;
    ਦਸ਼ਮਲਵ decay = a.decay;
    ਪੂਰਨ batch = a.batch;
    axpy_cpu(l.outputs, learning_ਦਰ/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    ਜੇ(l.batch_normalize){
        axpy_cpu(l.outputs, learning_ਦਰ/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.outputs, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.inputs*l.outputs, -decay*batch, l.weights, 1, l.ਭਾਰ_updates, 1);
    axpy_cpu(l.inputs*l.outputs, learning_ਦਰ/batch, l.ਭਾਰ_updates, 1, l.weights, 1);
    scal_cpu(l.inputs*l.outputs, momentum, l.ਭਾਰ_updates, 1);
}

ਖਾਲੀ forward_connected_layer(layer l, ਜਾਲ net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    ਪੂਰਨ m = l.batch;
    ਪੂਰਨ k = l.inputs;
    ਪੂਰਨ n = l.outputs;
    ਦਸ਼ਮਲਵ *a = net.ਨਿਵੇਸ਼;
    ਦਸ਼ਮਲਵ *b = l.weights;
    ਦਸ਼ਮਲਵ *c = l.output;
    gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);
    ਜੇ(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } ਨਹੀਂਤਾਂ {
        ਜੋੜੋ_bias(l.output, l.biases, l.batch, l.outputs, 1);
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ਖਾਲੀ backward_connected_layer(layer l, ਜਾਲ net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ਜੇ(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } ਨਹੀਂਤਾਂ {
        backward_bias(l.bias_updates, l.delta, l.batch, l.outputs, 1);
    }

    ਪੂਰਨ m = l.outputs;
    ਪੂਰਨ k = l.batch;
    ਪੂਰਨ n = l.inputs;
    ਦਸ਼ਮਲਵ *a = l.delta;
    ਦਸ਼ਮਲਵ *b = net.ਨਿਵੇਸ਼;
    ਦਸ਼ਮਲਵ *c = l.ਭਾਰ_updates;
    gemm(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta;
    b = l.weights;
    c = net.delta;

    ਜੇ(c) gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
}


ਖਾਲੀ denormalize_connected_layer(layer l)
{
    ਪੂਰਨ i, j;
    ਲਈ(i = 0; i < l.outputs; ++i){
        ਦਸ਼ਮਲਵ scale = l.scales[i]/ਵਰਗਮੂਲ(l.rolling_variance[i] + .000001);
        ਲਈ(j = 0; j < l.inputs; ++j){
            l.weights[i*l.inputs + j] *= scale;
        }
        l.biases[i] -= l.rolling_ਔਸਤ[i] * scale;
        l.scales[i] = 1;
        l.rolling_ਔਸਤ[i] = 0;
        l.rolling_variance[i] = 1;
    }
}


ਖਾਲੀ statistics_connected_layer(layer l)
{
    ਜੇ(l.batch_normalize){
        ਲਿਖੋ("Scales ");
        print_statistics(l.scales, l.outputs);
        /*
           printf("Rolling Mean ");
           print_statistics(l.rolling_mean, l.outputs);
           printf("Rolling Variance ");
           print_statistics(l.rolling_variance, l.outputs);
         */
    }
    ਲਿਖੋ("Biases ");
    print_statistics(l.biases, l.outputs);
    ਲਿਖੋ("Weights ");
    print_statistics(l.weights, l.outputs);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ pull_connected_layer(layer l)
{
    cuda_pull_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
    cuda_pull_array(l.ਭਾਰ_updates_gpu, l.ਭਾਰ_updates, l.inputs*l.outputs);
    cuda_pull_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    ਜੇ (l.batch_normalize){
        cuda_pull_array(l.scales_gpu, l.scales, l.outputs);
        cuda_pull_array(l.rolling_ਔਸਤ_gpu, l.rolling_ਔਸਤ, l.outputs);
        cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

ਖਾਲੀ ਧੱਕੋ_connected_layer(layer l)
{
    cuda_ਧੱਕੋ_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_ਧੱਕੋ_array(l.biases_gpu, l.biases, l.outputs);
    cuda_ਧੱਕੋ_array(l.ਭਾਰ_updates_gpu, l.ਭਾਰ_updates, l.inputs*l.outputs);
    cuda_ਧੱਕੋ_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    ਜੇ (l.batch_normalize){
        cuda_ਧੱਕੋ_array(l.scales_gpu, l.scales, l.outputs);
        cuda_ਧੱਕੋ_array(l.rolling_ਔਸਤ_gpu, l.rolling_ਔਸਤ, l.outputs);
        cuda_ਧੱਕੋ_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

ਖਾਲੀ update_connected_layer_gpu(layer l, update_ਦਲੀਲਾਂ a)
{
    ਦਸ਼ਮਲਵ learning_ਦਰ = a.learning_ਦਰ*l.learning_ਦਰ_scale;
    ਦਸ਼ਮਲਵ momentum = a.momentum;
    ਦਸ਼ਮਲਵ decay = a.decay;
    ਪੂਰਨ batch = a.batch;
    ਜੇ(a.adam){
        adam_update_gpu(l.weights_gpu, l.ਭਾਰ_updates_gpu, l.m_gpu, l.v_gpu, a.B1, a.B2, a.eps, decay, learning_ਦਰ, l.inputs*l.outputs, batch, a.t);
        adam_update_gpu(l.biases_gpu, l.bias_updates_gpu, l.bias_m_gpu, l.bias_v_gpu, a.B1, a.B2, a.eps, decay, learning_ਦਰ, l.outputs, batch, a.t);
        ਜੇ(l.scales_gpu){
            adam_update_gpu(l.scales_gpu, l.scale_updates_gpu, l.scale_m_gpu, l.scale_v_gpu, a.B1, a.B2, a.eps, decay, learning_ਦਰ, l.outputs, batch, a.t);
        }
    }ਨਹੀਂਤਾਂ{
        axpy_gpu(l.outputs, learning_ਦਰ/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
        scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

        ਜੇ(l.batch_normalize){
            axpy_gpu(l.outputs, learning_ਦਰ/batch, l.scale_updates_gpu, 1, l.scales_gpu, 1);
            scal_gpu(l.outputs, momentum, l.scale_updates_gpu, 1);
        }

        axpy_gpu(l.inputs*l.outputs, -decay*batch, l.weights_gpu, 1, l.ਭਾਰ_updates_gpu, 1);
        axpy_gpu(l.inputs*l.outputs, learning_ਦਰ/batch, l.ਭਾਰ_updates_gpu, 1, l.weights_gpu, 1);
        scal_gpu(l.inputs*l.outputs, momentum, l.ਭਾਰ_updates_gpu, 1);
    }
}

ਖਾਲੀ forward_connected_layer_gpu(layer l, ਜਾਲ net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);

    ਪੂਰਨ m = l.batch;
    ਪੂਰਨ k = l.inputs;
    ਪੂਰਨ n = l.outputs;
    ਦਸ਼ਮਲਵ * a = net.ਨਿਵੇਸ਼_gpu;
    ਦਸ਼ਮਲਵ * b = l.weights_gpu;
    ਦਸ਼ਮਲਵ * c = l.output_gpu;
    gemm_gpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

    ਜੇ (l.batch_normalize) {
        forward_batchnorm_layer_gpu(l, net);
    } ਨਹੀਂਤਾਂ {
        ਜੋੜੋ_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.outputs, 1);
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ਖਾਲੀ backward_connected_layer_gpu(layer l, ਜਾਲ net)
{
    constrain_gpu(l.outputs*l.batch, 1, l.delta_gpu, 1);
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    ਜੇ(l.batch_normalize){
        backward_batchnorm_layer_gpu(l, net);
    } ਨਹੀਂਤਾਂ {
        backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.outputs, 1);
    }

    ਪੂਰਨ m = l.outputs;
    ਪੂਰਨ k = l.batch;
    ਪੂਰਨ n = l.inputs;
    ਦਸ਼ਮਲਵ * a = l.delta_gpu;
    ਦਸ਼ਮਲਵ * b = net.ਨਿਵੇਸ਼_gpu;
    ਦਸ਼ਮਲਵ * c = l.ਭਾਰ_updates_gpu;
    gemm_gpu(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta_gpu;
    b = l.weights_gpu;
    c = net.delta_gpu;

    ਜੇ(c) gemm_gpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
}
#ਜੇ_ਅੰਤ
