#شامل "connected_layer.h"
#شامل "convolutional_layer.h"
#شامل "batchnorm_layer.h"
#شامل "utils.h"
#شامل "cuda.h"
#شامل "blas.h"
#شامل "gemm.h"

#شامل <math.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>

layer make_connected_layer(صحیح batch, صحیح inputs, صحیح outputs, ACTIVATION activation, صحیح batch_normalize, صحیح adam)
{
    صحیح i;
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

    l.output = calloc(batch*outputs, اندازه(اعشاری));
    l.delta = calloc(batch*outputs, اندازه(اعشاری));

    l.weight_updates = calloc(inputs*outputs, اندازه(اعشاری));
    l.bias_updates = calloc(outputs, اندازه(اعشاری));

    l.weights = calloc(outputs*inputs, اندازه(اعشاری));
    l.biases = calloc(outputs, اندازه(اعشاری));

    l.forward = forward_connected_layer;
    l.backward = backward_connected_layer;
    l.update = update_connected_layer;

    //float scale = 1./sqrt(inputs);
    اعشاری scale = sqrt(2./inputs);
    برای(i = 0; i < outputs*inputs; ++i){
        l.weights[i] = scale*rand_uniform(-1, 1);
    }

    برای(i = 0; i < outputs; ++i){
        l.biases[i] = 0;
    }

    اگر(adam){
        l.m = calloc(l.inputs*l.outputs, اندازه(اعشاری));
        l.v = calloc(l.inputs*l.outputs, اندازه(اعشاری));
        l.bias_m = calloc(l.outputs, اندازه(اعشاری));
        l.scale_m = calloc(l.outputs, اندازه(اعشاری));
        l.bias_v = calloc(l.outputs, اندازه(اعشاری));
        l.scale_v = calloc(l.outputs, اندازه(اعشاری));
    }
    اگر(batch_normalize){
        l.scales = calloc(outputs, اندازه(اعشاری));
        l.scale_updates = calloc(outputs, اندازه(اعشاری));
        برای(i = 0; i < outputs; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(outputs, اندازه(اعشاری));
        l.mean_delta = calloc(outputs, اندازه(اعشاری));
        l.variance = calloc(outputs, اندازه(اعشاری));
        l.variance_delta = calloc(outputs, اندازه(اعشاری));

        l.rolling_mean = calloc(outputs, اندازه(اعشاری));
        l.rolling_variance = calloc(outputs, اندازه(اعشاری));

        l.x = calloc(batch*outputs, اندازه(اعشاری));
        l.x_norm = calloc(batch*outputs, اندازه(اعشاری));
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
    اگر (adam) {
        l.m_gpu =       cuda_make_array(0, inputs*outputs);
        l.v_gpu =       cuda_make_array(0, inputs*outputs);
        l.bias_m_gpu =  cuda_make_array(0, outputs);
        l.bias_v_gpu =  cuda_make_array(0, outputs);
        l.scale_m_gpu = cuda_make_array(0, outputs);
        l.scale_v_gpu = cuda_make_array(0, outputs);
    }

    اگر(batch_normalize){
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
    بازگشت l;
}

تهی update_connected_layer(layer l, update_args a)
{
    اعشاری learning_rate = a.learning_rate*l.learning_rate_scale;
    اعشاری momentum = a.momentum;
    اعشاری decay = a.decay;
    صحیح batch = a.batch;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    اگر(l.batch_normalize){
        axpy_cpu(l.outputs, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.outputs, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.inputs*l.outputs, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.inputs*l.outputs, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.inputs*l.outputs, momentum, l.weight_updates, 1);
}

تهی forward_connected_layer(layer l, network net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    صحیح m = l.batch;
    صحیح k = l.inputs;
    صحیح n = l.outputs;
    اعشاری *a = net.درون_داد;
    اعشاری *b = l.weights;
    اعشاری *c = l.output;
    gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);
    اگر(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } وگرنه {
        add_bias(l.output, l.biases, l.batch, l.outputs, 1);
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

تهی backward_connected_layer(layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    اگر(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } وگرنه {
        backward_bias(l.bias_updates, l.delta, l.batch, l.outputs, 1);
    }

    صحیح m = l.outputs;
    صحیح k = l.batch;
    صحیح n = l.inputs;
    اعشاری *a = l.delta;
    اعشاری *b = net.درون_داد;
    اعشاری *c = l.weight_updates;
    gemm(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta;
    b = l.weights;
    c = net.delta;

    اگر(c) gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
}


تهی denormalize_connected_layer(layer l)
{
    صحیح i, j;
    برای(i = 0; i < l.outputs; ++i){
        اعشاری scale = l.scales[i]/sqrt(l.rolling_variance[i] + .000001);
        برای(j = 0; j < l.inputs; ++j){
            l.weights[i*l.inputs + j] *= scale;
        }
        l.biases[i] -= l.rolling_mean[i] * scale;
        l.scales[i] = 1;
        l.rolling_mean[i] = 0;
        l.rolling_variance[i] = 1;
    }
}


تهی statistics_connected_layer(layer l)
{
    اگر(l.batch_normalize){
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

تهی pull_connected_layer(layer l)
{
    cuda_pull_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
    cuda_pull_array(l.weight_updates_gpu, l.weight_updates, l.inputs*l.outputs);
    cuda_pull_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    اگر (l.batch_normalize){
        cuda_pull_array(l.scales_gpu, l.scales, l.outputs);
        cuda_pull_array(l.rolling_mean_gpu, l.rolling_mean, l.outputs);
        cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

تهی افزودن_connected_layer(layer l)
{
    cuda_افزودن_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_افزودن_array(l.biases_gpu, l.biases, l.outputs);
    cuda_افزودن_array(l.weight_updates_gpu, l.weight_updates, l.inputs*l.outputs);
    cuda_افزودن_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    اگر (l.batch_normalize){
        cuda_افزودن_array(l.scales_gpu, l.scales, l.outputs);
        cuda_افزودن_array(l.rolling_mean_gpu, l.rolling_mean, l.outputs);
        cuda_افزودن_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

تهی update_connected_layer_gpu(layer l, update_args a)
{
    اعشاری learning_rate = a.learning_rate*l.learning_rate_scale;
    اعشاری momentum = a.momentum;
    اعشاری decay = a.decay;
    صحیح batch = a.batch;
    اگر(a.adam){
        adam_update_gpu(l.weights_gpu, l.weight_updates_gpu, l.m_gpu, l.v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.inputs*l.outputs, batch, a.t);
        adam_update_gpu(l.biases_gpu, l.bias_updates_gpu, l.bias_m_gpu, l.bias_v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.outputs, batch, a.t);
        اگر(l.scales_gpu){
            adam_update_gpu(l.scales_gpu, l.scale_updates_gpu, l.scale_m_gpu, l.scale_v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.outputs, batch, a.t);
        }
    }وگرنه{
        axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
        scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

        اگر(l.batch_normalize){
            axpy_gpu(l.outputs, learning_rate/batch, l.scale_updates_gpu, 1, l.scales_gpu, 1);
            scal_gpu(l.outputs, momentum, l.scale_updates_gpu, 1);
        }

        axpy_gpu(l.inputs*l.outputs, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
        axpy_gpu(l.inputs*l.outputs, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
        scal_gpu(l.inputs*l.outputs, momentum, l.weight_updates_gpu, 1);
    }
}

تهی forward_connected_layer_gpu(layer l, network net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);

    صحیح m = l.batch;
    صحیح k = l.inputs;
    صحیح n = l.outputs;
    اعشاری * a = net.درون_داد_gpu;
    اعشاری * b = l.weights_gpu;
    اعشاری * c = l.output_gpu;
    gemm_gpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

    اگر (l.batch_normalize) {
        forward_batchnorm_layer_gpu(l, net);
    } وگرنه {
        add_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.outputs, 1);
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

تهی backward_connected_layer_gpu(layer l, network net)
{
    constrain_gpu(l.outputs*l.batch, 1, l.delta_gpu, 1);
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    اگر(l.batch_normalize){
        backward_batchnorm_layer_gpu(l, net);
    } وگرنه {
        backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.outputs, 1);
    }

    صحیح m = l.outputs;
    صحیح k = l.batch;
    صحیح n = l.inputs;
    اعشاری * a = l.delta_gpu;
    اعشاری * b = net.درون_داد_gpu;
    اعشاری * c = l.weight_updates_gpu;
    gemm_gpu(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta_gpu;
    b = l.weights_gpu;
    c = net.delta_gpu;

    اگر(c) gemm_gpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
}
#endif
