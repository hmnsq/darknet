#ଅନ୍ତର୍ଭାଵ "connected_layer.h"
#ଅନ୍ତର୍ଭାଵ "convolutional_layer.h"
#ଅନ୍ତର୍ଭାଵ "batchnorm_layer.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ "gemm.h"

#ଅନ୍ତର୍ଭାଵ <math.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

layer make_connected_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା outputs, ACTIVATION activation, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା adam)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
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

    l.output = calloc(batch*outputs, ଆକାର(ଦଶମିକ));
    l.delta = calloc(batch*outputs, ଆକାର(ଦଶମିକ));

    l.weight_updates = calloc(inputs*outputs, ଆକାର(ଦଶମିକ));
    l.bias_updates = calloc(outputs, ଆକାର(ଦଶମିକ));

    l.weights = calloc(outputs*inputs, ଆକାର(ଦଶମିକ));
    l.biases = calloc(outputs, ଆକାର(ଦଶମିକ));

    l.forward = forward_connected_layer;
    l.backward = backward_connected_layer;
    l.update = update_connected_layer;

    //float scale = 1./sqrt(inputs);
    ଦଶମିକ scale = sqrt(2./inputs);
    ପାଇଁ(i = 0; i < outputs*inputs; ++i){
        l.weights[i] = scale*rand_uniform(-1, 1);
    }

    ପାଇଁ(i = 0; i < outputs; ++i){
        l.biases[i] = 0;
    }

    ଯଦି(adam){
        l.m = calloc(l.inputs*l.outputs, ଆକାର(ଦଶମିକ));
        l.v = calloc(l.inputs*l.outputs, ଆକାର(ଦଶମିକ));
        l.bias_m = calloc(l.outputs, ଆକାର(ଦଶମିକ));
        l.scale_m = calloc(l.outputs, ଆକାର(ଦଶମିକ));
        l.bias_v = calloc(l.outputs, ଆକାର(ଦଶମିକ));
        l.scale_v = calloc(l.outputs, ଆକାର(ଦଶମିକ));
    }
    ଯଦି(batch_normalize){
        l.scales = calloc(outputs, ଆକାର(ଦଶମିକ));
        l.scale_updates = calloc(outputs, ଆକାର(ଦଶମିକ));
        ପାଇଁ(i = 0; i < outputs; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(outputs, ଆକାର(ଦଶମିକ));
        l.mean_delta = calloc(outputs, ଆକାର(ଦଶମିକ));
        l.variance = calloc(outputs, ଆକାର(ଦଶମିକ));
        l.variance_delta = calloc(outputs, ଆକାର(ଦଶମିକ));

        l.rolling_mean = calloc(outputs, ଆକାର(ଦଶମିକ));
        l.rolling_variance = calloc(outputs, ଆକାର(ଦଶମିକ));

        l.x = calloc(batch*outputs, ଆକାର(ଦଶମିକ));
        l.x_norm = calloc(batch*outputs, ଆକାର(ଦଶମିକ));
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
    ଯଦି (adam) {
        l.m_gpu =       cuda_make_array(0, inputs*outputs);
        l.v_gpu =       cuda_make_array(0, inputs*outputs);
        l.bias_m_gpu =  cuda_make_array(0, outputs);
        l.bias_v_gpu =  cuda_make_array(0, outputs);
        l.scale_m_gpu = cuda_make_array(0, outputs);
        l.scale_v_gpu = cuda_make_array(0, outputs);
    }

    ଯଦି(batch_normalize){
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
    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ update_connected_layer(layer l, update_args a)
{
    ଦଶମିକ learning_rate = a.learning_rate*l.learning_rate_scale;
    ଦଶମିକ momentum = a.momentum;
    ଦଶମିକ decay = a.decay;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch = a.batch;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    ଯଦି(l.batch_normalize){
        axpy_cpu(l.outputs, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.outputs, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.inputs*l.outputs, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.inputs*l.outputs, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.inputs*l.outputs, momentum, l.weight_updates, 1);
}

ଶୂନ୍ୟ forward_connected_layer(layer l, network net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = l.batch;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = l.inputs;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = l.outputs;
    ଦଶମିକ *a = net.ନିବେଶ;
    ଦଶମିକ *b = l.weights;
    ଦଶମିକ *c = l.output;
    gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);
    ଯଦି(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } ଅନ୍ୟଥା {
        add_bias(l.output, l.biases, l.batch, l.outputs, 1);
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ଶୂନ୍ୟ backward_connected_layer(layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ଯଦି(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } ଅନ୍ୟଥା {
        backward_bias(l.bias_updates, l.delta, l.batch, l.outputs, 1);
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = l.outputs;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = l.batch;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = l.inputs;
    ଦଶମିକ *a = l.delta;
    ଦଶମିକ *b = net.ନିବେଶ;
    ଦଶମିକ *c = l.weight_updates;
    gemm(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta;
    b = l.weights;
    c = net.delta;

    ଯଦି(c) gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
}


ଶୂନ୍ୟ denormalize_connected_layer(layer l)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପାଇଁ(i = 0; i < l.outputs; ++i){
        ଦଶମିକ scale = l.scales[i]/sqrt(l.rolling_variance[i] + .000001);
        ପାଇଁ(j = 0; j < l.inputs; ++j){
            l.weights[i*l.inputs + j] *= scale;
        }
        l.biases[i] -= l.rolling_mean[i] * scale;
        l.scales[i] = 1;
        l.rolling_mean[i] = 0;
        l.rolling_variance[i] = 1;
    }
}


ଶୂନ୍ୟ statistics_connected_layer(layer l)
{
    ଯଦି(l.batch_normalize){
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

ଶୂନ୍ୟ pull_connected_layer(layer l)
{
    cuda_pull_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
    cuda_pull_array(l.weight_updates_gpu, l.weight_updates, l.inputs*l.outputs);
    cuda_pull_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    ଯଦି (l.batch_normalize){
        cuda_pull_array(l.scales_gpu, l.scales, l.outputs);
        cuda_pull_array(l.rolling_mean_gpu, l.rolling_mean, l.outputs);
        cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

ଶୂନ୍ୟ ଠେଲ_connected_layer(layer l)
{
    cuda_ଠେଲ_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_ଠେଲ_array(l.biases_gpu, l.biases, l.outputs);
    cuda_ଠେଲ_array(l.weight_updates_gpu, l.weight_updates, l.inputs*l.outputs);
    cuda_ଠେଲ_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    ଯଦି (l.batch_normalize){
        cuda_ଠେଲ_array(l.scales_gpu, l.scales, l.outputs);
        cuda_ଠେଲ_array(l.rolling_mean_gpu, l.rolling_mean, l.outputs);
        cuda_ଠେଲ_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

ଶୂନ୍ୟ update_connected_layer_gpu(layer l, update_args a)
{
    ଦଶମିକ learning_rate = a.learning_rate*l.learning_rate_scale;
    ଦଶମିକ momentum = a.momentum;
    ଦଶମିକ decay = a.decay;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch = a.batch;
    ଯଦି(a.adam){
        adam_update_gpu(l.weights_gpu, l.weight_updates_gpu, l.m_gpu, l.v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.inputs*l.outputs, batch, a.t);
        adam_update_gpu(l.biases_gpu, l.bias_updates_gpu, l.bias_m_gpu, l.bias_v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.outputs, batch, a.t);
        ଯଦି(l.scales_gpu){
            adam_update_gpu(l.scales_gpu, l.scale_updates_gpu, l.scale_m_gpu, l.scale_v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.outputs, batch, a.t);
        }
    }ଅନ୍ୟଥା{
        axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
        scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

        ଯଦି(l.batch_normalize){
            axpy_gpu(l.outputs, learning_rate/batch, l.scale_updates_gpu, 1, l.scales_gpu, 1);
            scal_gpu(l.outputs, momentum, l.scale_updates_gpu, 1);
        }

        axpy_gpu(l.inputs*l.outputs, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
        axpy_gpu(l.inputs*l.outputs, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
        scal_gpu(l.inputs*l.outputs, momentum, l.weight_updates_gpu, 1);
    }
}

ଶୂନ୍ୟ forward_connected_layer_gpu(layer l, network net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = l.batch;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = l.inputs;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = l.outputs;
    ଦଶମିକ * a = net.ନିବେଶ_gpu;
    ଦଶମିକ * b = l.weights_gpu;
    ଦଶମିକ * c = l.output_gpu;
    gemm_gpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

    ଯଦି (l.batch_normalize) {
        forward_batchnorm_layer_gpu(l, net);
    } ଅନ୍ୟଥା {
        add_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.outputs, 1);
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ଶୂନ୍ୟ backward_connected_layer_gpu(layer l, network net)
{
    constrain_gpu(l.outputs*l.batch, 1, l.delta_gpu, 1);
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    ଯଦି(l.batch_normalize){
        backward_batchnorm_layer_gpu(l, net);
    } ଅନ୍ୟଥା {
        backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.outputs, 1);
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = l.outputs;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = l.batch;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = l.inputs;
    ଦଶମିକ * a = l.delta_gpu;
    ଦଶମିକ * b = net.ନିବେଶ_gpu;
    ଦଶମିକ * c = l.weight_updates_gpu;
    gemm_gpu(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta_gpu;
    b = l.weights_gpu;
    c = net.delta_gpu;

    ଯଦି(c) gemm_gpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
}
#endif
