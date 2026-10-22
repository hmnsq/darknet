#ضمن "connected_layer.h"
#ضمن "convolutional_layer.h"
#ضمن "batchnorm_layer.h"
#ضمن "utils.h"
#ضمن "cuda.h"
#ضمن "blas.h"
#ضمن "gemm.h"

#ضمن <math.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>

layer make_connected_layer(صحيح batch, صحيح inputs, صحيح outputs, ACTIVATION activation, صحيح batch_normalize, صحيح adam)
{
    صحيح i;
    layer l = {0};
    l.learning_شرح_scale = 1;
    l.type = CONNECTED;

    l.inputs = inputs;
    l.outputs = outputs;
    l.batch=batch;
    l.batch_normalize = batch_normalize;
    l.h = 1;
    l.w = 1;
    l.c = inputs;
    l.ٻاهريون_h = 1;
    l.ٻاهريون_w = 1;
    l.ٻاهريون_c = outputs;

    l.output = صفر_يادگيري_ڏيو(batch*outputs, حجم(عائم));
    l.delta = صفر_يادگيري_ڏيو(batch*outputs, حجم(عائم));

    l.وزن_updates = صفر_يادگيري_ڏيو(inputs*outputs, حجم(عائم));
    l.bias_updates = صفر_يادگيري_ڏيو(outputs, حجم(عائم));

    l.weights = صفر_يادگيري_ڏيو(outputs*inputs, حجم(عائم));
    l.biases = صفر_يادگيري_ڏيو(outputs, حجم(عائم));

    l.forward = forward_connected_layer;
    l.backward = backward_connected_layer;
    l.update = update_connected_layer;

    //float scale = 1./sqrt(inputs);
    عائم scale = جذر(2./inputs);
    لكل(i = 0; i < outputs*inputs; ++i){
        l.weights[i] = scale*rand_هڪجهڙو(-1, 1);
    }

    لكل(i = 0; i < outputs; ++i){
        l.biases[i] = 0;
    }

    إذا(adam){
        l.m = صفر_يادگيري_ڏيو(l.inputs*l.outputs, حجم(عائم));
        l.v = صفر_يادگيري_ڏيو(l.inputs*l.outputs, حجم(عائم));
        l.bias_m = صفر_يادگيري_ڏيو(l.outputs, حجم(عائم));
        l.scale_m = صفر_يادگيري_ڏيو(l.outputs, حجم(عائم));
        l.bias_v = صفر_يادگيري_ڏيو(l.outputs, حجم(عائم));
        l.scale_v = صفر_يادگيري_ڏيو(l.outputs, حجم(عائم));
    }
    إذا(batch_normalize){
        l.scales = صفر_يادگيري_ڏيو(outputs, حجم(عائم));
        l.scale_updates = صفر_يادگيري_ڏيو(outputs, حجم(عائم));
        لكل(i = 0; i < outputs; ++i){
            l.scales[i] = 1;
        }

        l.سراسري = صفر_يادگيري_ڏيو(outputs, حجم(عائم));
        l.سراسري_delta = صفر_يادگيري_ڏيو(outputs, حجم(عائم));
        l.variance = صفر_يادگيري_ڏيو(outputs, حجم(عائم));
        l.variance_delta = صفر_يادگيري_ڏيو(outputs, حجم(عائم));

        l.rolling_سراسري = صفر_يادگيري_ڏيو(outputs, حجم(عائم));
        l.rolling_variance = صفر_يادگيري_ڏيو(outputs, حجم(عائم));

        l.x = صفر_يادگيري_ڏيو(batch*outputs, حجم(عائم));
        l.x_norm = صفر_يادگيري_ڏيو(batch*outputs, حجم(عائم));
    }

#جيڪڏهن_وصف_ٿيل GPU
    l.forward_gpu = forward_connected_layer_gpu;
    l.backward_gpu = backward_connected_layer_gpu;
    l.update_gpu = update_connected_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, outputs*inputs);
    l.biases_gpu = cuda_make_array(l.biases, outputs);

    l.وزن_updates_gpu = cuda_make_array(l.وزن_updates, outputs*inputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, outputs);

    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    l.delta_gpu = cuda_make_array(l.delta, outputs*batch);
    إذا (adam) {
        l.m_gpu =       cuda_make_array(0, inputs*outputs);
        l.v_gpu =       cuda_make_array(0, inputs*outputs);
        l.bias_m_gpu =  cuda_make_array(0, outputs);
        l.bias_v_gpu =  cuda_make_array(0, outputs);
        l.scale_m_gpu = cuda_make_array(0, outputs);
        l.scale_v_gpu = cuda_make_array(0, outputs);
    }

    إذا(batch_normalize){
        l.سراسري_gpu = cuda_make_array(l.سراسري, outputs);
        l.variance_gpu = cuda_make_array(l.variance, outputs);

        l.rolling_سراسري_gpu = cuda_make_array(l.سراسري, outputs);
        l.rolling_variance_gpu = cuda_make_array(l.variance, outputs);

        l.سراسري_delta_gpu = cuda_make_array(l.سراسري, outputs);
        l.variance_delta_gpu = cuda_make_array(l.variance, outputs);

        l.scales_gpu = cuda_make_array(l.scales, outputs);
        l.scale_updates_gpu = cuda_make_array(l.scale_updates, outputs);

        l.x_gpu = cuda_make_array(l.output, l.batch*outputs);
        l.x_norm_gpu = cuda_make_array(l.output, l.batch*outputs);
#جيڪڏهن_وصف_ٿيل CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnn_سيٽ_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, l.batch, l.ٻاهريون_c, l.ٻاهريون_h, l.ٻاهريون_w); 
        cudnn_سيٽ_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, 1, l.ٻاهريون_c, 1, 1); 
#جيڪڏهن_پورو
    }
#جيڪڏهن_پورو
    l.activation = activation;
    fprintf(stderr, "connected                            %4d  ->  %4d\n", inputs, outputs);
    أرجع l;
}

فراغ update_connected_layer(layer l, update_دليلون a)
{
    عائم learning_شرح = a.learning_شرح*l.learning_شرح_scale;
    عائم momentum = a.momentum;
    عائم decay = a.decay;
    صحيح batch = a.batch;
    axpy_cpu(l.outputs, learning_شرح/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    إذا(l.batch_normalize){
        axpy_cpu(l.outputs, learning_شرح/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.outputs, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.inputs*l.outputs, -decay*batch, l.weights, 1, l.وزن_updates, 1);
    axpy_cpu(l.inputs*l.outputs, learning_شرح/batch, l.وزن_updates, 1, l.weights, 1);
    scal_cpu(l.inputs*l.outputs, momentum, l.وزن_updates, 1);
}

فراغ forward_connected_layer(layer l, ڄار net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    صحيح m = l.batch;
    صحيح k = l.inputs;
    صحيح n = l.outputs;
    عائم *a = net.داخلا;
    عائم *b = l.weights;
    عائم *c = l.output;
    gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);
    إذا(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } وإلا {
        جوڙيو_bias(l.output, l.biases, l.batch, l.outputs, 1);
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

فراغ backward_connected_layer(layer l, ڄار net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    إذا(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } وإلا {
        backward_bias(l.bias_updates, l.delta, l.batch, l.outputs, 1);
    }

    صحيح m = l.outputs;
    صحيح k = l.batch;
    صحيح n = l.inputs;
    عائم *a = l.delta;
    عائم *b = net.داخلا;
    عائم *c = l.وزن_updates;
    gemm(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta;
    b = l.weights;
    c = net.delta;

    إذا(c) gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
}


فراغ denormalize_connected_layer(layer l)
{
    صحيح i, j;
    لكل(i = 0; i < l.outputs; ++i){
        عائم scale = l.scales[i]/جذر(l.rolling_variance[i] + .000001);
        لكل(j = 0; j < l.inputs; ++j){
            l.weights[i*l.inputs + j] *= scale;
        }
        l.biases[i] -= l.rolling_سراسري[i] * scale;
        l.scales[i] = 1;
        l.rolling_سراسري[i] = 0;
        l.rolling_variance[i] = 1;
    }
}


فراغ statistics_connected_layer(layer l)
{
    إذا(l.batch_normalize){
        لکو("Scales ");
        print_statistics(l.scales, l.outputs);
        /*
           printf("Rolling Mean ");
           print_statistics(l.rolling_mean, l.outputs);
           printf("Rolling Variance ");
           print_statistics(l.rolling_variance, l.outputs);
         */
    }
    لکو("Biases ");
    print_statistics(l.biases, l.outputs);
    لکو("Weights ");
    print_statistics(l.weights, l.outputs);
}

#جيڪڏهن_وصف_ٿيل GPU

فراغ pull_connected_layer(layer l)
{
    cuda_pull_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
    cuda_pull_array(l.وزن_updates_gpu, l.وزن_updates, l.inputs*l.outputs);
    cuda_pull_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    إذا (l.batch_normalize){
        cuda_pull_array(l.scales_gpu, l.scales, l.outputs);
        cuda_pull_array(l.rolling_سراسري_gpu, l.rolling_سراسري, l.outputs);
        cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

فراغ ڌڪو_connected_layer(layer l)
{
    cuda_ڌڪو_array(l.weights_gpu, l.weights, l.inputs*l.outputs);
    cuda_ڌڪو_array(l.biases_gpu, l.biases, l.outputs);
    cuda_ڌڪو_array(l.وزن_updates_gpu, l.وزن_updates, l.inputs*l.outputs);
    cuda_ڌڪو_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
    إذا (l.batch_normalize){
        cuda_ڌڪو_array(l.scales_gpu, l.scales, l.outputs);
        cuda_ڌڪو_array(l.rolling_سراسري_gpu, l.rolling_سراسري, l.outputs);
        cuda_ڌڪو_array(l.rolling_variance_gpu, l.rolling_variance, l.outputs);
    }
}

فراغ update_connected_layer_gpu(layer l, update_دليلون a)
{
    عائم learning_شرح = a.learning_شرح*l.learning_شرح_scale;
    عائم momentum = a.momentum;
    عائم decay = a.decay;
    صحيح batch = a.batch;
    إذا(a.adam){
        adam_update_gpu(l.weights_gpu, l.وزن_updates_gpu, l.m_gpu, l.v_gpu, a.B1, a.B2, a.eps, decay, learning_شرح, l.inputs*l.outputs, batch, a.t);
        adam_update_gpu(l.biases_gpu, l.bias_updates_gpu, l.bias_m_gpu, l.bias_v_gpu, a.B1, a.B2, a.eps, decay, learning_شرح, l.outputs, batch, a.t);
        إذا(l.scales_gpu){
            adam_update_gpu(l.scales_gpu, l.scale_updates_gpu, l.scale_m_gpu, l.scale_v_gpu, a.B1, a.B2, a.eps, decay, learning_شرح, l.outputs, batch, a.t);
        }
    }وإلا{
        axpy_gpu(l.outputs, learning_شرح/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
        scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

        إذا(l.batch_normalize){
            axpy_gpu(l.outputs, learning_شرح/batch, l.scale_updates_gpu, 1, l.scales_gpu, 1);
            scal_gpu(l.outputs, momentum, l.scale_updates_gpu, 1);
        }

        axpy_gpu(l.inputs*l.outputs, -decay*batch, l.weights_gpu, 1, l.وزن_updates_gpu, 1);
        axpy_gpu(l.inputs*l.outputs, learning_شرح/batch, l.وزن_updates_gpu, 1, l.weights_gpu, 1);
        scal_gpu(l.inputs*l.outputs, momentum, l.وزن_updates_gpu, 1);
    }
}

فراغ forward_connected_layer_gpu(layer l, ڄار net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);

    صحيح m = l.batch;
    صحيح k = l.inputs;
    صحيح n = l.outputs;
    عائم * a = net.داخلا_gpu;
    عائم * b = l.weights_gpu;
    عائم * c = l.output_gpu;
    gemm_gpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

    إذا (l.batch_normalize) {
        forward_batchnorm_layer_gpu(l, net);
    } وإلا {
        جوڙيو_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.outputs, 1);
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

فراغ backward_connected_layer_gpu(layer l, ڄار net)
{
    constrain_gpu(l.outputs*l.batch, 1, l.delta_gpu, 1);
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    إذا(l.batch_normalize){
        backward_batchnorm_layer_gpu(l, net);
    } وإلا {
        backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.outputs, 1);
    }

    صحيح m = l.outputs;
    صحيح k = l.batch;
    صحيح n = l.inputs;
    عائم * a = l.delta_gpu;
    عائم * b = net.داخلا_gpu;
    عائم * c = l.وزن_updates_gpu;
    gemm_gpu(1,0,m,n,k,1,a,m,b,n,1,c,n);

    m = l.batch;
    k = l.outputs;
    n = l.inputs;

    a = l.delta_gpu;
    b = l.weights_gpu;
    c = net.delta_gpu;

    إذا(c) gemm_gpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
}
#جيڪڏهن_پورو
