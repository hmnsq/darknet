#অন্তর্ভাব "connected_layer.h"
#অন্তর্ভাব "convolutional_layer.h"
#অন্তর্ভাব "batchnorm_layer.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "gemm.h"

#অন্তর্ভাব <math.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>

layer make_connected_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs, পূর্ণাঙ্ক outputs, ACTIVATION activation, পূর্ণাঙ্ক batch_normalize, পূর্ণাঙ্ক adam)
{
    পূর্ণাঙ্ক i;
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

    l.output = calloc(batch*outputs, পরিমাণ(ভিন্ন));
    l.delta = calloc(batch*outputs, পরিমাণ(ভিন্ন));

    l.weight_updates = calloc(inputs*outputs, পরিমাণ(ভিন্ন));
    l.bias_updates = calloc(outputs, পরিমাণ(ভিন্ন));

    l.weights = calloc(outputs*inputs, পরিমাণ(ভিন্ন));
    l.biases = calloc(outputs, পরিমাণ(ভিন্ন));

    l.forward = forward_connected_layer;
    l.backward = backward_connected_layer;
    l.update = update_connected_layer;

    //float scale = 1./sqrt(inputs);
    ভিন্ন scale = sqrt(2./inputs);
    প্রতি(i = 0; i < outputs*inputs; ++i){
        l.weights[i] = scale*rand_uniform(-1, 1);
    }

    প্রতি(i = 0; i < outputs; ++i){
        l.biases[i] = 0;
    }

    যদি(adam){
        l.m = calloc(l.inputs*l.outputs, পরিমাণ(ভিন্ন));
        l.v = calloc(l.inputs*l.outputs, পরিমাণ(ভিন্ন));
        l.bias_m = calloc(l.outputs, পরিমাণ(ভিন্ন));
        l.scale_m = calloc(l.outputs, পরিমাণ(ভিন্ন));
        l.bias_v = calloc(l.outputs, পরিমাণ(ভিন্ন));
        l.scale_v = calloc(l.outputs, পরিমাণ(ভিন্ন));
    }
    যদি(batch_normalize){
        l.scales = calloc(outputs, পরিমাণ(ভিন্ন));
        l.scale_updates = calloc(outputs, পরিমাণ(ভিন্ন));
        প্রতি(i = 0; i < outputs; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(outputs, পরিমাণ(ভিন্ন));
        l.mean_delta = calloc(outputs, পরিমাণ(ভিন্ন));
        l.variance = calloc(outputs, পরিমাণ(ভিন্ন));
        l.variance_delta = calloc(outputs, পরিমাণ(ভিন্ন));

        l.rolling_mean = calloc(outputs, পরিমাণ(ভিন্ন));
        l.rolling_variance = calloc(outputs, পরিমাণ(ভিন্ন));

        l.x = calloc(batch*outputs, পরিমাণ(ভিন্ন));
        l.x_norm = calloc(batch*outputs, পরিমাণ(ভিন্ন));
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
    যদি (adam) {
        l.m_gpu =       cuda_make_array(0, inputs*outputs);
        l.v_gpu =       cuda_make_array(0, inputs*outputs);
        l.bias_m_gpu =  cuda_make_array(0, outputs);
        l.bias_v_gpu =  cuda_make_array(0, outputs);
        l.scale_m_gpu = cuda_make_array(0, outputs);
        l.scale_v_gpu = cuda_make_array(0, outputs);
    }

    যদি(batch_normalize){
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
    প্রত্যাগম l;
}

শূন্য update_connected_layer(layer l, update_args a)
{
    ভিন্ন learning_rate = a.learning_rate*l.learning_rate_scale;
    ভিন্ন momentum = a.momentum;
    ভিন্ন decay = a.decay;
    পূর্ণাঙ্ক batch = a.batch;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    যদি(l.batch_normalize){
        axpy_cpu(l.outputs, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.outputs, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.inputs*l.outputs, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.inputs*l.outputs, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.inputs*l.outputs, momentum, l.weight_updates, 1);
}

শূন্য forward_connected_layer(layer l, network net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    পূর্ণাঙ্ক m = l.batch;
    পূর্ণাঙ্ক k = l.inputs;
    পূর্ণাঙ্ক n = l.outputs;
    ভিন্ন *a = net.প্রবেশ;
    ভিন্ন *b = l.weights;
    ভিন্ন *c = l.output;
    gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);
    যদি(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } অন্যথা {
        add_bias(l.output, l.biases, l.batch, l.outputs, 1);
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

শূন্য backward_connected_layer(layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    যদি(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } অন্যথা {
        backward_bias(l.bias_updates, l.delta, l.batch, l.outputs, 1);
    }

    পূর্ণাঙ্ক m = l.outputs;
    পূর্ণাঙ্ক k = l.batch;
    পূর্ণাঙ্ক n = l.inputs;
    ভিন্ন *a = l.delta;
    ভিন্ন *b = net.প্রবেশ;
    ভিন্ন *c = l.weight_updates;
    gemm(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta;
    b = l.weights;
    c = net.delta;

    যদি(c) gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
}


শূন্য denormalize_connected_layer(layer l)
{
    পূর্ণাঙ্ক i, j;
    প্রতি(i = 0; i < l.outputs; ++i){
        ভিন্ন scale = l.scales[i]/sqrt(l.rolling_variance[i] + .000001);
        প্রতি(j = 0; j < l.inputs; ++j){
            l.weights[i*l.inputs + j] *= scale;
        }
        l.biases[i] -= l.rolling_mean[i] * scale;
        l.scales[i] = 1;
        l.rolling_mean[i] = 0;
        l.rolling_variance[i] = 1;
    }
}


শূন্য statistics_connected_layer(layer l)
{
    যদি(l.batch_normalize){
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

শূন্য pull_connected_layer(layer l)
{
    cuda_pull_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
    cuda_pull_array(l.weight_updates_gpu, l.weight_updates, l.inputs*l.outputs);
    cuda_pull_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    যদি (l.batch_normalize){
        cuda_pull_array(l.scales_gpu, l.scales, l.outputs);
        cuda_pull_array(l.rolling_mean_gpu, l.rolling_mean, l.outputs);
        cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

শূন্য ঠেলো_connected_layer(layer l)
{
    cuda_ঠেলো_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_ঠেলো_array(l.biases_gpu, l.biases, l.outputs);
    cuda_ঠেলো_array(l.weight_updates_gpu, l.weight_updates, l.inputs*l.outputs);
    cuda_ঠেলো_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    যদি (l.batch_normalize){
        cuda_ঠেলো_array(l.scales_gpu, l.scales, l.outputs);
        cuda_ঠেলো_array(l.rolling_mean_gpu, l.rolling_mean, l.outputs);
        cuda_ঠেলো_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

শূন্য update_connected_layer_gpu(layer l, update_args a)
{
    ভিন্ন learning_rate = a.learning_rate*l.learning_rate_scale;
    ভিন্ন momentum = a.momentum;
    ভিন্ন decay = a.decay;
    পূর্ণাঙ্ক batch = a.batch;
    যদি(a.adam){
        adam_update_gpu(l.weights_gpu, l.weight_updates_gpu, l.m_gpu, l.v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.inputs*l.outputs, batch, a.t);
        adam_update_gpu(l.biases_gpu, l.bias_updates_gpu, l.bias_m_gpu, l.bias_v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.outputs, batch, a.t);
        যদি(l.scales_gpu){
            adam_update_gpu(l.scales_gpu, l.scale_updates_gpu, l.scale_m_gpu, l.scale_v_gpu, a.B1, a.B2, a.eps, decay, learning_rate, l.outputs, batch, a.t);
        }
    }অন্যথা{
        axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
        scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

        যদি(l.batch_normalize){
            axpy_gpu(l.outputs, learning_rate/batch, l.scale_updates_gpu, 1, l.scales_gpu, 1);
            scal_gpu(l.outputs, momentum, l.scale_updates_gpu, 1);
        }

        axpy_gpu(l.inputs*l.outputs, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
        axpy_gpu(l.inputs*l.outputs, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
        scal_gpu(l.inputs*l.outputs, momentum, l.weight_updates_gpu, 1);
    }
}

শূন্য forward_connected_layer_gpu(layer l, network net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);

    পূর্ণাঙ্ক m = l.batch;
    পূর্ণাঙ্ক k = l.inputs;
    পূর্ণাঙ্ক n = l.outputs;
    ভিন্ন * a = net.প্রবেশ_gpu;
    ভিন্ন * b = l.weights_gpu;
    ভিন্ন * c = l.output_gpu;
    gemm_gpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

    যদি (l.batch_normalize) {
        forward_batchnorm_layer_gpu(l, net);
    } অন্যথা {
        add_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.outputs, 1);
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

শূন্য backward_connected_layer_gpu(layer l, network net)
{
    constrain_gpu(l.outputs*l.batch, 1, l.delta_gpu, 1);
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    যদি(l.batch_normalize){
        backward_batchnorm_layer_gpu(l, net);
    } অন্যথা {
        backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.outputs, 1);
    }

    পূর্ণাঙ্ক m = l.outputs;
    পূর্ণাঙ্ক k = l.batch;
    পূর্ণাঙ্ক n = l.inputs;
    ভিন্ন * a = l.delta_gpu;
    ভিন্ন * b = net.প্রবেশ_gpu;
    ভিন্ন * c = l.weight_updates_gpu;
    gemm_gpu(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta_gpu;
    b = l.weights_gpu;
    c = net.delta_gpu;

    যদি(c) gemm_gpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
}
#endif
