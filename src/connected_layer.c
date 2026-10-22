#ಅನ್ತರ್ಭಾವ "connected_layer.h"
#ಅನ್ತರ್ಭಾವ "convolutional_layer.h"
#ಅನ್ತರ್ಭಾವ "batchnorm_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "gemm.h"

#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>

layer make_connected_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ inputs, ಪೂರ್ಣಾಙ್ಕ outputs, ACTIVATION activation, ಪೂರ್ಣಾಙ್ಕ batch_normalize, ಪೂರ್ಣಾಙ್ಕ adam)
{
    ಪೂರ್ಣಾಙ್ಕ i;
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

    l.output = calloc(batch*outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.delta = calloc(batch*outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));

    l.weight_updates = calloc(inputs*outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.bias_updates = calloc(outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));

    l.weights = calloc(outputs*inputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.biases = calloc(outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));

    l.forward = forward_connected_layer;
    l.backward = backward_connected_layer;
    l.update = update_connected_layer;

    //float scale = 1./sqrt(inputs);
    ಭಿನ್ನ scale = sqrt(2./inputs);
    ಪ್ರತಿ(i = 0; i < outputs*inputs; ++i){
        l.weights[i] = scale*rand_uniform(-1, 1);
    }

    ಪ್ರತಿ(i = 0; i < outputs; ++i){
        l.biases[i] = 0;
    }

    ಯದಿ(adam){
        l.m = calloc(l.inputs*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.v = calloc(l.inputs*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.bias_m = calloc(l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.scale_m = calloc(l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.bias_v = calloc(l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.scale_v = calloc(l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
    }
    ಯದಿ(batch_normalize){
        l.scales = calloc(outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.scale_updates = calloc(outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
        ಪ್ರತಿ(i = 0; i < outputs; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.mean_delta = calloc(outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.variance = calloc(outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.variance_delta = calloc(outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));

        l.rolling_mean = calloc(outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.rolling_variance = calloc(outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));

        l.x = calloc(batch*outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.x_norm = calloc(batch*outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
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
    ಯದಿ (adam) {
        l.m_gpu =       cuda_make_array(0, inputs*outputs);
        l.v_gpu =       cuda_make_array(0, inputs*outputs);
        l.bias_m_gpu =  cuda_make_array(0, outputs);
        l.bias_v_gpu =  cuda_make_array(0, outputs);
        l.scale_m_gpu = cuda_make_array(0, outputs);
        l.scale_v_gpu = cuda_make_array(0, outputs);
    }

    ಯದಿ(batch_normalize){
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
    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ update_connected_layer(layer l, update_args a)
{
    ಭಿನ್ನ learning_rate = a.learning_rate*l.learning_rate_scale;
    ಭಿನ್ನ momentum = a.momentum;
    ಭಿನ್ನ decay = a.decay;
    ಪೂರ್ಣಾಙ್ಕ batch = a.batch;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    ಯದಿ(l.batch_normalize){
        axpy_cpu(l.outputs, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.outputs, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.inputs*l.outputs, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.inputs*l.outputs, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.inputs*l.outputs, momentum, l.weight_updates, 1);
}

ಶೂನ್ಯ forward_connected_layer(layer l, network net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    ಪೂರ್ಣಾಙ್ಕ m = l.batch;
    ಪೂರ್ಣಾಙ್ಕ k = l.inputs;
    ಪೂರ್ಣಾಙ್ಕ n = l.outputs;
    ಭಿನ್ನ *a = net.ಒಳಹರಿವು;
    ಭಿನ್ನ *b = l.weights;
    ಭಿನ್ನ *c = l.output;
    gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);
    ಯದಿ(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } ಅನ್ಯಥಾ {
        add_bias(l.output, l.biases, l.batch, l.outputs, 1);
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ಶೂನ್ಯ backward_connected_layer(layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ಯದಿ(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } ಅನ್ಯಥಾ {
        backward_bias(l.bias_updates, l.delta, l.batch, l.outputs, 1);
    }

    ಪೂರ್ಣಾಙ್ಕ m = l.outputs;
    ಪೂರ್ಣಾಙ್ಕ k = l.batch;
    ಪೂರ್ಣಾಙ್ಕ n = l.inputs;
    ಭಿನ್ನ *a = l.delta;
    ಭಿನ್ನ *b = net.ಒಳಹರಿವು;
    ಭಿನ್ನ *c = l.weight_updates;
    gemm(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta;
    b = l.weights;
    c = net.delta;

    ಯದಿ(c) gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
}


ಶೂನ್ಯ denormalize_connected_layer(layer l)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪ್ರತಿ(i = 0; i < l.outputs; ++i){
        ಭಿನ್ನ scale = l.scales[i]/sqrt(l.rolling_variance[i] + .000001);
        ಪ್ರತಿ(j = 0; j < l.inputs; ++j){
            l.weights[i*l.inputs + j] *= scale;
        }
        l.biases[i] -= l.rolling_mean[i] * scale;
        l.scales[i] = 1;
        l.rolling_mean[i] = 0;
        l.rolling_variance[i] = 1;
    }
}


ಶೂನ್ಯ statistics_connected_layer(layer l)
{
    ಯದಿ(l.batch_normalize){
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

ಶೂನ್ಯ pull_connected_layer(layer l)
{
    cuda_pull_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
    cuda_pull_array(l.weight_updates_gpu, l.weight_updates, l.inputs*l.outputs);
    cuda_pull_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    ಯದಿ (l.batch_normalize){
        cuda_pull_array(l.scales_gpu, l.scales, l.outputs);
        cuda_pull_array(l.rolling_mean_gpu, l.rolling_mean, l.outputs);
        cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

ಶೂನ್ಯ ತಳ್ಳು_connected_layer(layer l)
{
    cuda_ತಳ್ಳು_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_ತಳ್ಳು_array(l.biases_gpu, l.biases, l.outputs);
    cuda_ತಳ್ಳು_array(l.weight_updates_gpu, l.weight_updates, l.inputs*l.outputs);
    cuda_ತಳ್ಳು_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    ಯದಿ (l.batch_normalize){
        cuda_ತಳ್ಳು_array(l.scales_gpu, l.scales, l.outputs);
        cuda_ತಳ್ಳು_array(l.rolling_mean_gpu, l.rolling_mean, l.outputs);
        cuda_ತಳ್ಳು_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

ಶೂನ್ಯ update_connected_layer_gpu(layer l, update_args a)
{
    ಭಿನ್ನ learning_rate = a.learning_rate*l.learning_rate_scale;
    ಭಿನ್ನ momentum = a.momentum;
    ಭಿನ್ನ decay = a.decay;
    ಪೂರ್ಣಾಙ್ಕ batch = a.batch;
    ಯದಿ(a.adam){
        adam_update_gpu(l.weights_gpu, l.weight_updates_gpu, l.m_gpu, l.v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.inputs*l.outputs, batch, a.t);
        adam_update_gpu(l.biases_gpu, l.bias_updates_gpu, l.bias_m_gpu, l.bias_v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.outputs, batch, a.t);
        ಯದಿ(l.scales_gpu){
            adam_update_gpu(l.scales_gpu, l.scale_updates_gpu, l.scale_m_gpu, l.scale_v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.outputs, batch, a.t);
        }
    }ಅನ್ಯಥಾ{
        axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
        scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

        ಯದಿ(l.batch_normalize){
            axpy_gpu(l.outputs, learning_rate/batch, l.scale_updates_gpu, 1, l.scales_gpu, 1);
            scal_gpu(l.outputs, momentum, l.scale_updates_gpu, 1);
        }

        axpy_gpu(l.inputs*l.outputs, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
        axpy_gpu(l.inputs*l.outputs, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
        scal_gpu(l.inputs*l.outputs, momentum, l.weight_updates_gpu, 1);
    }
}

ಶೂನ್ಯ forward_connected_layer_gpu(layer l, network net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);

    ಪೂರ್ಣಾಙ್ಕ m = l.batch;
    ಪೂರ್ಣಾಙ್ಕ k = l.inputs;
    ಪೂರ್ಣಾಙ್ಕ n = l.outputs;
    ಭಿನ್ನ * a = net.ಒಳಹರಿವು_gpu;
    ಭಿನ್ನ * b = l.weights_gpu;
    ಭಿನ್ನ * c = l.output_gpu;
    gemm_gpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

    ಯದಿ (l.batch_normalize) {
        forward_batchnorm_layer_gpu(l, net);
    } ಅನ್ಯಥಾ {
        add_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.outputs, 1);
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ಶೂನ್ಯ backward_connected_layer_gpu(layer l, network net)
{
    constrain_gpu(l.outputs*l.batch, 1, l.delta_gpu, 1);
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    ಯದಿ(l.batch_normalize){
        backward_batchnorm_layer_gpu(l, net);
    } ಅನ್ಯಥಾ {
        backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.outputs, 1);
    }

    ಪೂರ್ಣಾಙ್ಕ m = l.outputs;
    ಪೂರ್ಣಾಙ್ಕ k = l.batch;
    ಪೂರ್ಣಾಙ್ಕ n = l.inputs;
    ಭಿನ್ನ * a = l.delta_gpu;
    ಭಿನ್ನ * b = net.ಒಳಹರಿವು_gpu;
    ಭಿನ್ನ * c = l.weight_updates_gpu;
    gemm_gpu(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta_gpu;
    b = l.weights_gpu;
    c = net.delta_gpu;

    ಯದಿ(c) gemm_gpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
}
#endif
