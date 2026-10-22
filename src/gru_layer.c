#ਅਨ੍ਤਰ੍ਭਾਵ "gru_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "connected_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gemm.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

ਸਥਿਰ ਖਾਲੀ increment_layer(layer *l, ਪੂਰਨ ਕਦਮਾਂ)
{
    ਪੂਰਨ ਅੰਕ = l->outputs*l->batch*ਕਦਮਾਂ;
    l->output += ਅੰਕ;
    l->delta += ਅੰਕ;
    l->x += ਅੰਕ;
    l->x_norm += ਅੰਕ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l->output_gpu += ਅੰਕ;
    l->delta_gpu += ਅੰਕ;
    l->x_gpu += ਅੰਕ;
    l->x_norm_gpu += ਅੰਕ;
#ਜੇ_ਅੰਤ
}

layer make_gru_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਪੂਰਨ outputs, ਪੂਰਨ ਕਦਮਾਂ, ਪੂਰਨ batch_normalize, ਪੂਰਨ adam)
{
    fprintf(stderr, "GRU Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / ਕਦਮਾਂ;
    layer l = {0};
    l.batch = batch;
    l.type = GRU;
    l.ਕਦਮਾਂ = ਕਦਮਾਂ;
    l.inputs = inputs;

    l.uz = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.uz) = make_connected_layer(batch*ਕਦਮਾਂ, inputs, outputs, ਰੇਖਿਕ, batch_normalize, adam);
    l.uz->batch = batch;

    l.wz = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.wz) = make_connected_layer(batch*ਕਦਮਾਂ, outputs, outputs, ਰੇਖਿਕ, batch_normalize, adam);
    l.wz->batch = batch;

    l.ur = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.ur) = make_connected_layer(batch*ਕਦਮਾਂ, inputs, outputs, ਰੇਖਿਕ, batch_normalize, adam);
    l.ur->batch = batch;

    l.wr = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.wr) = make_connected_layer(batch*ਕਦਮਾਂ, outputs, outputs, ਰੇਖਿਕ, batch_normalize, adam);
    l.wr->batch = batch;



    l.uh = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.uh) = make_connected_layer(batch*ਕਦਮਾਂ, inputs, outputs, ਰੇਖਿਕ, batch_normalize, adam);
    l.uh->batch = batch;

    l.wh = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.wh) = make_connected_layer(batch*ਕਦਮਾਂ, outputs, outputs, ਰੇਖਿਕ, batch_normalize, adam);
    l.wh->batch = batch;

    l.batch_normalize = batch_normalize;


    l.outputs = outputs;
    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs*batch*ਕਦਮਾਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs*batch*ਕਦਮਾਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.state = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.prev_state = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.forgot_state = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.forgot_delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.r_cpu = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.z_cpu = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.h_cpu = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.forward = forward_gru_layer;
    l.backward = backward_gru_layer;
    l.update = update_gru_layer;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_gru_layer_gpu;
    l.backward_gpu = backward_gru_layer_gpu;
    l.update_gpu = update_gru_layer_gpu;

    l.forgot_state_gpu = cuda_make_array(0, batch*outputs);
    l.forgot_delta_gpu = cuda_make_array(0, batch*outputs);
    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = cuda_make_array(0, batch*outputs*ਕਦਮਾਂ);
    l.delta_gpu = cuda_make_array(0, batch*outputs*ਕਦਮਾਂ);
    l.r_gpu = cuda_make_array(0, batch*outputs);
    l.z_gpu = cuda_make_array(0, batch*outputs);
    l.h_gpu = cuda_make_array(0, batch*outputs);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.uz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.uz->ਬਾਹਰਲਾ_c, l.uz->ਬਾਹਰਲਾ_h, l.uz->ਬਾਹਰਲਾ_w); 
    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.uh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.uh->ਬਾਹਰਲਾ_c, l.uh->ਬਾਹਰਲਾ_h, l.uh->ਬਾਹਰਲਾ_w); 
    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.ur->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.ur->ਬਾਹਰਲਾ_c, l.ur->ਬਾਹਰਲਾ_h, l.ur->ਬਾਹਰਲਾ_w); 
    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.wz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.wz->ਬਾਹਰਲਾ_c, l.wz->ਬਾਹਰਲਾ_h, l.wz->ਬਾਹਰਲਾ_w); 
    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.wh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.wh->ਬਾਹਰਲਾ_c, l.wh->ਬਾਹਰਲਾ_h, l.wh->ਬਾਹਰਲਾ_w); 
    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.wr->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.wr->ਬਾਹਰਲਾ_c, l.wr->ਬਾਹਰਲਾ_h, l.wr->ਬਾਹਰਲਾ_w); 
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

    ਵਾਪਸ l;
}

ਖਾਲੀ update_gru_layer(layer l, update_ਦਲੀਲਾਂ a)
{
    update_connected_layer(*(l.ur), a);
    update_connected_layer(*(l.uz), a);
    update_connected_layer(*(l.uh), a);
    update_connected_layer(*(l.wr), a);
    update_connected_layer(*(l.wz), a);
    update_connected_layer(*(l.wh), a);
}

ਖਾਲੀ forward_gru_layer(layer l, ਜਾਲ net)
{
    ਜਾਲ s = net;
    s.train = net.train;
    ਪੂਰਨ i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, uz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, ur.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, uh.delta, 1);

    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, wz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, wr.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, wh.delta, 1);
    ਜੇ(net.train) {
        fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, l.delta, 1);
        copy_cpu(l.outputs*l.batch, l.state, 1, l.prev_state, 1);
    }

    ਲਈ (i = 0; i < l.ਕਦਮਾਂ; ++i) {
        s.ਨਿਵੇਸ਼ = l.state;
        forward_connected_layer(wz, s);
        forward_connected_layer(wr, s);

        s.ਨਿਵੇਸ਼ = net.ਨਿਵੇਸ਼;
        forward_connected_layer(uz, s);
        forward_connected_layer(ur, s);
        forward_connected_layer(uh, s);


        copy_cpu(l.outputs*l.batch, uz.output, 1, l.z_cpu, 1);
        axpy_cpu(l.outputs*l.batch, 1, wz.output, 1, l.z_cpu, 1);

        copy_cpu(l.outputs*l.batch, ur.output, 1, l.r_cpu, 1);
        axpy_cpu(l.outputs*l.batch, 1, wr.output, 1, l.r_cpu, 1);

        activate_array(l.z_cpu, l.outputs*l.batch, LOGISTIC);
        activate_array(l.r_cpu, l.outputs*l.batch, LOGISTIC);

        copy_cpu(l.outputs*l.batch, l.state, 1, l.forgot_state, 1);
        mul_cpu(l.outputs*l.batch, l.r_cpu, 1, l.forgot_state, 1);

        s.ਨਿਵੇਸ਼ = l.forgot_state;
        forward_connected_layer(wh, s);

        copy_cpu(l.outputs*l.batch, uh.output, 1, l.h_cpu, 1);
        axpy_cpu(l.outputs*l.batch, 1, wh.output, 1, l.h_cpu, 1);

        ਜੇ(l.tanh){
            activate_array(l.h_cpu, l.outputs*l.batch, TANH);
        } ਨਹੀਂਤਾਂ {
            activate_array(l.h_cpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_ਜੋੜ_cpu(l.state, l.h_cpu, l.z_cpu, l.outputs*l.batch, l.output);

        copy_cpu(l.outputs*l.batch, l.output, 1, l.state, 1);

        net.ਨਿਵੇਸ਼ += l.inputs*l.batch;
        l.output += l.outputs*l.batch;
        increment_layer(&uz, 1);
        increment_layer(&ur, 1);
        increment_layer(&uh, 1);

        increment_layer(&wz, 1);
        increment_layer(&wr, 1);
        increment_layer(&wh, 1);
    }
}

ਖਾਲੀ backward_gru_layer(layer l, ਜਾਲ net)
{
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ pull_gru_layer(layer l)
{
}

ਖਾਲੀ ਧੱਕੋ_gru_layer(layer l)
{
}

ਖਾਲੀ update_gru_layer_gpu(layer l, update_ਦਲੀਲਾਂ a)
{
    update_connected_layer_gpu(*(l.ur), a);
    update_connected_layer_gpu(*(l.uz), a);
    update_connected_layer_gpu(*(l.uh), a);
    update_connected_layer_gpu(*(l.wr), a);
    update_connected_layer_gpu(*(l.wz), a);
    update_connected_layer_gpu(*(l.wh), a);
}

ਖਾਲੀ forward_gru_layer_gpu(layer l, ਜਾਲ net)
{
    ਜਾਲ s = {0};
    s.train = net.train;
    ਪੂਰਨ i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, uz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, ur.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, uh.delta_gpu, 1);

    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, wz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, wr.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, wh.delta_gpu, 1);
    ਜੇ(net.train) {
        fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    ਲਈ (i = 0; i < l.ਕਦਮਾਂ; ++i) {
        s.ਨਿਵੇਸ਼_gpu = l.state_gpu;
        forward_connected_layer_gpu(wz, s);
        forward_connected_layer_gpu(wr, s);

        s.ਨਿਵੇਸ਼_gpu = net.ਨਿਵੇਸ਼_gpu;
        forward_connected_layer_gpu(uz, s);
        forward_connected_layer_gpu(ur, s);
        forward_connected_layer_gpu(uh, s);

        copy_gpu(l.outputs*l.batch, uz.output_gpu, 1, l.z_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wz.output_gpu, 1, l.z_gpu, 1);

        copy_gpu(l.outputs*l.batch, ur.output_gpu, 1, l.r_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wr.output_gpu, 1, l.r_gpu, 1);

        activate_array_gpu(l.z_gpu, l.outputs*l.batch, LOGISTIC);
        activate_array_gpu(l.r_gpu, l.outputs*l.batch, LOGISTIC);

        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.forgot_state_gpu, 1);
        mul_gpu(l.outputs*l.batch, l.r_gpu, 1, l.forgot_state_gpu, 1);

        s.ਨਿਵੇਸ਼_gpu = l.forgot_state_gpu;
        forward_connected_layer_gpu(wh, s);

        copy_gpu(l.outputs*l.batch, uh.output_gpu, 1, l.h_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wh.output_gpu, 1, l.h_gpu, 1);

        ਜੇ(l.tanh){
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);
        } ਨਹੀਂਤਾਂ {
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_ਜੋੜ_gpu(l.state_gpu, l.h_gpu, l.z_gpu, l.outputs*l.batch, l.output_gpu);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.state_gpu, 1);

        net.ਨਿਵੇਸ਼_gpu += l.inputs*l.batch;
        l.output_gpu += l.outputs*l.batch;
        increment_layer(&uz, 1);
        increment_layer(&ur, 1);
        increment_layer(&uh, 1);

        increment_layer(&wz, 1);
        increment_layer(&wr, 1);
        increment_layer(&wh, 1);
    }
}

ਖਾਲੀ backward_gru_layer_gpu(layer l, ਜਾਲ net)
{
    ਜਾਲ s = {0};
    s.train = net.train;
    ਪੂਰਨ i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    increment_layer(&uz, l.ਕਦਮਾਂ - 1);
    increment_layer(&ur, l.ਕਦਮਾਂ - 1);
    increment_layer(&uh, l.ਕਦਮਾਂ - 1);

    increment_layer(&wz, l.ਕਦਮਾਂ - 1);
    increment_layer(&wr, l.ਕਦਮਾਂ - 1);
    increment_layer(&wh, l.ਕਦਮਾਂ - 1);

    net.ਨਿਵੇਸ਼_gpu += l.inputs*l.batch*(l.ਕਦਮਾਂ-1);
    ਜੇ(net.delta_gpu) net.delta_gpu += l.inputs*l.batch*(l.ਕਦਮਾਂ-1);
    l.output_gpu += l.outputs*l.batch*(l.ਕਦਮਾਂ-1);
    l.delta_gpu += l.outputs*l.batch*(l.ਕਦਮਾਂ-1);
    ਦਸ਼ਮਲਵ *ਸਿਰਾ_state = l.output_gpu;
    ਲਈ (i = l.ਕਦਮਾਂ-1; i >= 0; --i) {
        ਜੇ(i != 0) copy_gpu(l.outputs*l.batch, l.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        ਨਹੀਂਤਾਂ copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        ਦਸ਼ਮਲਵ *prev_delta_gpu = (i == 0) ? 0 : l.delta_gpu - l.outputs*l.batch;

        copy_gpu(l.outputs*l.batch, uz.output_gpu, 1, l.z_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wz.output_gpu, 1, l.z_gpu, 1);

        copy_gpu(l.outputs*l.batch, ur.output_gpu, 1, l.r_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wr.output_gpu, 1, l.r_gpu, 1);

        activate_array_gpu(l.z_gpu, l.outputs*l.batch, LOGISTIC);
        activate_array_gpu(l.r_gpu, l.outputs*l.batch, LOGISTIC);

        copy_gpu(l.outputs*l.batch, uh.output_gpu, 1, l.h_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wh.output_gpu, 1, l.h_gpu, 1);

        ਜੇ(l.tanh){
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);
        } ਨਹੀਂਤਾਂ {
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_delta_gpu(l.state_gpu, l.h_gpu, l.z_gpu, prev_delta_gpu, uh.delta_gpu, uz.delta_gpu, l.outputs*l.batch, l.delta_gpu);

        ਜੇ(l.tanh){
            gradient_array_gpu(l.h_gpu, l.outputs*l.batch, TANH, uh.delta_gpu);
        } ਨਹੀਂਤਾਂ {
            gradient_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC, uh.delta_gpu);
        }

        copy_gpu(l.outputs*l.batch, uh.delta_gpu, 1, wh.delta_gpu, 1);

        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.forgot_state_gpu, 1);
        mul_gpu(l.outputs*l.batch, l.r_gpu, 1, l.forgot_state_gpu, 1);
        fill_gpu(l.outputs*l.batch, 0, l.forgot_delta_gpu, 1);

        s.ਨਿਵੇਸ਼_gpu = l.forgot_state_gpu;
        s.delta_gpu = l.forgot_delta_gpu;

        backward_connected_layer_gpu(wh, s);
        ਜੇ(prev_delta_gpu) mult_ਜੋੜੋ_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.r_gpu, prev_delta_gpu);
        mult_ਜੋੜੋ_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.state_gpu, ur.delta_gpu);

        gradient_array_gpu(l.r_gpu, l.outputs*l.batch, LOGISTIC, ur.delta_gpu);
        copy_gpu(l.outputs*l.batch, ur.delta_gpu, 1, wr.delta_gpu, 1);

        gradient_array_gpu(l.z_gpu, l.outputs*l.batch, LOGISTIC, uz.delta_gpu);
        copy_gpu(l.outputs*l.batch, uz.delta_gpu, 1, wz.delta_gpu, 1);

        s.ਨਿਵੇਸ਼_gpu = l.state_gpu;
        s.delta_gpu = prev_delta_gpu;

        backward_connected_layer_gpu(wr, s);
        backward_connected_layer_gpu(wz, s);

        s.ਨਿਵੇਸ਼_gpu = net.ਨਿਵੇਸ਼_gpu;
        s.delta_gpu = net.delta_gpu;

        backward_connected_layer_gpu(uh, s);
        backward_connected_layer_gpu(ur, s);
        backward_connected_layer_gpu(uz, s);


        net.ਨਿਵੇਸ਼_gpu -= l.inputs*l.batch;
        ਜੇ(net.delta_gpu) net.delta_gpu -= l.inputs*l.batch;
        l.output_gpu -= l.outputs*l.batch;
        l.delta_gpu -= l.outputs*l.batch;
        increment_layer(&uz, -1);
        increment_layer(&ur, -1);
        increment_layer(&uh, -1);

        increment_layer(&wz, -1);
        increment_layer(&wr, -1);
        increment_layer(&wh, -1);
    }
    copy_gpu(l.outputs*l.batch, ਸਿਰਾ_state, 1, l.state_gpu, 1);
}
#ਜੇ_ਅੰਤ
