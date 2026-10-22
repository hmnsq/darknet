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

layer make_connected_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ಪೂರ್ಣಾಂಕ outputs, ACTIVATION activation, ಪೂರ್ಣಾಂಕ batch_normalize, ಪೂರ್ಣಾಂಕ adam)
{
    ಪೂರ್ಣಾಂಕ i;
    layer l = {0};
    l.learning_ದರ_scale = 1;
    l.type = CONNECTED;

    l.inputs = inputs;
    l.outputs = outputs;
    l.batch=batch;
    l.batch_normalize = batch_normalize;
    l.h = 1;
    l.w = 1;
    l.c = inputs;
    l.ಹೊರಹೋಗು_h = 1;
    l.ಹೊರಹೋಗು_w = 1;
    l.ಹೊರಹೋಗು_c = outputs;

    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.delta = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.ತೂಕ_updates = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(inputs*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.bias_updates = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.weights = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs*inputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.biases = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.forward = forward_connected_layer;
    l.backward = backward_connected_layer;
    l.update = update_connected_layer;

    //float scale = 1./sqrt(inputs);
    ದಶಮಾಂಶ scale = ವರ್ಗಮೂಲ(2./inputs);
    ಗಾಗಿ(i = 0; i < outputs*inputs; ++i){
        l.weights[i] = scale*rand_ಏಕರೂಪ(-1, 1);
    }

    ಗಾಗಿ(i = 0; i < outputs; ++i){
        l.biases[i] = 0;
    }

    ಆದರೆ(adam){
        l.m = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.inputs*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.v = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.inputs*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.bias_m = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.scale_m = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.bias_v = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.scale_v = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    }
    ಆದರೆ(batch_normalize){
        l.scales = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.scale_updates = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
        ಗಾಗಿ(i = 0; i < outputs; ++i){
            l.scales[i] = 1;
        }

        l.ಸರಾಸರಿ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.ಸರಾಸರಿ_delta = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.variance = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.variance_delta = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));

        l.rolling_ಸರಾಸರಿ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.rolling_variance = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));

        l.x = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.x_norm = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_connected_layer_gpu;
    l.backward_gpu = backward_connected_layer_gpu;
    l.update_gpu = update_connected_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, outputs*inputs);
    l.biases_gpu = cuda_make_array(l.biases, outputs);

    l.ತೂಕ_updates_gpu = cuda_make_array(l.ತೂಕ_updates, outputs*inputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, outputs);

    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    l.delta_gpu = cuda_make_array(l.delta, outputs*batch);
    ಆದರೆ (adam) {
        l.m_gpu =       cuda_make_array(0, inputs*outputs);
        l.v_gpu =       cuda_make_array(0, inputs*outputs);
        l.bias_m_gpu =  cuda_make_array(0, outputs);
        l.bias_v_gpu =  cuda_make_array(0, outputs);
        l.scale_m_gpu = cuda_make_array(0, outputs);
        l.scale_v_gpu = cuda_make_array(0, outputs);
    }

    ಆದರೆ(batch_normalize){
        l.ಸರಾಸರಿ_gpu = cuda_make_array(l.ಸರಾಸರಿ, outputs);
        l.variance_gpu = cuda_make_array(l.variance, outputs);

        l.rolling_ಸರಾಸರಿ_gpu = cuda_make_array(l.ಸರಾಸರಿ, outputs);
        l.rolling_variance_gpu = cuda_make_array(l.variance, outputs);

        l.ಸರಾಸರಿ_delta_gpu = cuda_make_array(l.ಸರಾಸರಿ, outputs);
        l.variance_delta_gpu = cuda_make_array(l.variance, outputs);

        l.scales_gpu = cuda_make_array(l.scales, outputs);
        l.scale_updates_gpu = cuda_make_array(l.scale_updates, outputs);

        l.x_gpu = cuda_make_array(l.output, l.batch*outputs);
        l.x_norm_gpu = cuda_make_array(l.output, l.batch*outputs);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnn_ಗಣ_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_w); 
        cudnn_ಗಣ_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, 1, l.ಹೊರಹೋಗು_c, 1, 1); 
#ಆದರೆ_ಅಂತ್ಯ
    }
#ಆದರೆ_ಅಂತ್ಯ
    l.activation = activation;
    fprintf(stderr, "connected                            %4d  ->  %4d\n", inputs, outputs);
    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ update_connected_layer(layer l, update_ವಾದಗಳು a)
{
    ದಶಮಾಂಶ learning_ದರ = a.learning_ದರ*l.learning_ದರ_scale;
    ದಶಮಾಂಶ momentum = a.momentum;
    ದಶಮಾಂಶ decay = a.decay;
    ಪೂರ್ಣಾಂಕ batch = a.batch;
    axpy_cpu(l.outputs, learning_ದರ/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    ಆದರೆ(l.batch_normalize){
        axpy_cpu(l.outputs, learning_ದರ/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.outputs, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.inputs*l.outputs, -decay*batch, l.weights, 1, l.ತೂಕ_updates, 1);
    axpy_cpu(l.inputs*l.outputs, learning_ದರ/batch, l.ತೂಕ_updates, 1, l.weights, 1);
    scal_cpu(l.inputs*l.outputs, momentum, l.ತೂಕ_updates, 1);
}

ಶೂನ್ಯ forward_connected_layer(layer l, ಜಾಲ net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    ಪೂರ್ಣಾಂಕ m = l.batch;
    ಪೂರ್ಣಾಂಕ k = l.inputs;
    ಪೂರ್ಣಾಂಕ n = l.outputs;
    ದಶಮಾಂಶ *a = net.ಒಳಹರಿವು;
    ದಶಮಾಂಶ *b = l.weights;
    ದಶಮಾಂಶ *c = l.output;
    gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);
    ಆದರೆ(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಕೂಡಿಸು_bias(l.output, l.biases, l.batch, l.outputs, 1);
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ಶೂನ್ಯ backward_connected_layer(layer l, ಜಾಲ net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ಆದರೆ(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } ಇಲ್ಲದಿದ್ದರೆ {
        backward_bias(l.bias_updates, l.delta, l.batch, l.outputs, 1);
    }

    ಪೂರ್ಣಾಂಕ m = l.outputs;
    ಪೂರ್ಣಾಂಕ k = l.batch;
    ಪೂರ್ಣಾಂಕ n = l.inputs;
    ದಶಮಾಂಶ *a = l.delta;
    ದಶಮಾಂಶ *b = net.ಒಳಹರಿವು;
    ದಶಮಾಂಶ *c = l.ತೂಕ_updates;
    gemm(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta;
    b = l.weights;
    c = net.delta;

    ಆದರೆ(c) gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
}


ಶೂನ್ಯ denormalize_connected_layer(layer l)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಗಾಗಿ(i = 0; i < l.outputs; ++i){
        ದಶಮಾಂಶ scale = l.scales[i]/ವರ್ಗಮೂಲ(l.rolling_variance[i] + .000001);
        ಗಾಗಿ(j = 0; j < l.inputs; ++j){
            l.weights[i*l.inputs + j] *= scale;
        }
        l.biases[i] -= l.rolling_ಸರಾಸರಿ[i] * scale;
        l.scales[i] = 1;
        l.rolling_ಸರಾಸರಿ[i] = 0;
        l.rolling_variance[i] = 1;
    }
}


ಶೂನ್ಯ statistics_connected_layer(layer l)
{
    ಆದರೆ(l.batch_normalize){
        ಬರೆ("Scales ");
        print_statistics(l.scales, l.outputs);
        /*
           printf("Rolling Mean ");
           print_statistics(l.rolling_mean, l.outputs);
           printf("Rolling Variance ");
           print_statistics(l.rolling_variance, l.outputs);
         */
    }
    ಬರೆ("Biases ");
    print_statistics(l.biases, l.outputs);
    ಬರೆ("Weights ");
    print_statistics(l.weights, l.outputs);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

ಶೂನ್ಯ pull_connected_layer(layer l)
{
    cuda_pull_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
    cuda_pull_array(l.ತೂಕ_updates_gpu, l.ತೂಕ_updates, l.inputs*l.outputs);
    cuda_pull_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    ಆದರೆ (l.batch_normalize){
        cuda_pull_array(l.scales_gpu, l.scales, l.outputs);
        cuda_pull_array(l.rolling_ಸರಾಸರಿ_gpu, l.rolling_ಸರಾಸರಿ, l.outputs);
        cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

ಶೂನ್ಯ ತಳ್ಳು_connected_layer(layer l)
{
    cuda_ತಳ್ಳು_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_ತಳ್ಳು_array(l.biases_gpu, l.biases, l.outputs);
    cuda_ತಳ್ಳು_array(l.ತೂಕ_updates_gpu, l.ತೂಕ_updates, l.inputs*l.outputs);
    cuda_ತಳ್ಳು_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    ಆದರೆ (l.batch_normalize){
        cuda_ತಳ್ಳು_array(l.scales_gpu, l.scales, l.outputs);
        cuda_ತಳ್ಳು_array(l.rolling_ಸರಾಸರಿ_gpu, l.rolling_ಸರಾಸರಿ, l.outputs);
        cuda_ತಳ್ಳು_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

ಶೂನ್ಯ update_connected_layer_gpu(layer l, update_ವಾದಗಳು a)
{
    ದಶಮಾಂಶ learning_ದರ = a.learning_ದರ*l.learning_ದರ_scale;
    ದಶಮಾಂಶ momentum = a.momentum;
    ದಶಮಾಂಶ decay = a.decay;
    ಪೂರ್ಣಾಂಕ batch = a.batch;
    ಆದರೆ(a.adam){
        adam_update_gpu(l.weights_gpu, l.ತೂಕ_updates_gpu, l.m_gpu, l.v_gpu, a.B1, a.B2, a.eps, decay, learning_ದರ, l.inputs*l.outputs, batch, a.t);
        adam_update_gpu(l.biases_gpu, l.bias_updates_gpu, l.bias_m_gpu, l.bias_v_gpu, a.B1, a.B2, a.eps, decay, learning_ದರ, l.outputs, batch, a.t);
        ಆದರೆ(l.scales_gpu){
            adam_update_gpu(l.scales_gpu, l.scale_updates_gpu, l.scale_m_gpu, l.scale_v_gpu, a.B1, a.B2, a.eps, decay, learning_ದರ, l.outputs, batch, a.t);
        }
    }ಇಲ್ಲದಿದ್ದರೆ{
        axpy_gpu(l.outputs, learning_ದರ/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
        scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

        ಆದರೆ(l.batch_normalize){
            axpy_gpu(l.outputs, learning_ದರ/batch, l.scale_updates_gpu, 1, l.scales_gpu, 1);
            scal_gpu(l.outputs, momentum, l.scale_updates_gpu, 1);
        }

        axpy_gpu(l.inputs*l.outputs, -decay*batch, l.weights_gpu, 1, l.ತೂಕ_updates_gpu, 1);
        axpy_gpu(l.inputs*l.outputs, learning_ದರ/batch, l.ತೂಕ_updates_gpu, 1, l.weights_gpu, 1);
        scal_gpu(l.inputs*l.outputs, momentum, l.ತೂಕ_updates_gpu, 1);
    }
}

ಶೂನ್ಯ forward_connected_layer_gpu(layer l, ಜಾಲ net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);

    ಪೂರ್ಣಾಂಕ m = l.batch;
    ಪೂರ್ಣಾಂಕ k = l.inputs;
    ಪೂರ್ಣಾಂಕ n = l.outputs;
    ದಶಮಾಂಶ * a = net.ಒಳಹರಿವು_gpu;
    ದಶಮಾಂಶ * b = l.weights_gpu;
    ದಶಮಾಂಶ * c = l.output_gpu;
    gemm_gpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

    ಆದರೆ (l.batch_normalize) {
        forward_batchnorm_layer_gpu(l, net);
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಕೂಡಿಸು_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.outputs, 1);
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ಶೂನ್ಯ backward_connected_layer_gpu(layer l, ಜಾಲ net)
{
    constrain_gpu(l.outputs*l.batch, 1, l.delta_gpu, 1);
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    ಆದರೆ(l.batch_normalize){
        backward_batchnorm_layer_gpu(l, net);
    } ಇಲ್ಲದಿದ್ದರೆ {
        backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.outputs, 1);
    }

    ಪೂರ್ಣಾಂಕ m = l.outputs;
    ಪೂರ್ಣಾಂಕ k = l.batch;
    ಪೂರ್ಣಾಂಕ n = l.inputs;
    ದಶಮಾಂಶ * a = l.delta_gpu;
    ದಶಮಾಂಶ * b = net.ಒಳಹರಿವು_gpu;
    ದಶಮಾಂಶ * c = l.ತೂಕ_updates_gpu;
    gemm_gpu(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta_gpu;
    b = l.weights_gpu;
    c = net.delta_gpu;

    ಆದರೆ(c) gemm_gpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
}
#ಆದರೆ_ಅಂತ್ಯ
