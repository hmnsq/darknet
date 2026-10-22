#شامل "gru_layer.h"
#شامل "connected_layer.h"
#شامل "utils.h"
#شامل "cuda.h"
#شامل "blas.h"
#شامل "gemm.h"

#شامل <math.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>

ساکن خالی increment_layer(layer *l, صحیح اقدام)
{
    صحیح عدد = l->outputs*l->batch*اقدام;
    l->output += عدد;
    l->delta += عدد;
    l->x += عدد;
    l->x_norm += عدد;

#اگر_تعریف_شدہ GPU
    l->output_gpu += عدد;
    l->delta_gpu += عدد;
    l->x_gpu += عدد;
    l->x_norm_gpu += عدد;
#اگر_ختم
}

layer make_gru_layer(صحیح batch, صحیح inputs, صحیح outputs, صحیح اقدام, صحیح batch_normalize, صحیح adam)
{
    fprintf(stderr, "GRU Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / اقدام;
    layer l = {0};
    l.batch = batch;
    l.type = GRU;
    l.اقدام = اقدام;
    l.inputs = inputs;

    l.uz = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.uz) = make_connected_layer(batch*اقدام, inputs, outputs, خطی, batch_normalize, adam);
    l.uz->batch = batch;

    l.wz = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wz) = make_connected_layer(batch*اقدام, outputs, outputs, خطی, batch_normalize, adam);
    l.wz->batch = batch;

    l.ur = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.ur) = make_connected_layer(batch*اقدام, inputs, outputs, خطی, batch_normalize, adam);
    l.ur->batch = batch;

    l.wr = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wr) = make_connected_layer(batch*اقدام, outputs, outputs, خطی, batch_normalize, adam);
    l.wr->batch = batch;



    l.uh = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.uh) = make_connected_layer(batch*اقدام, inputs, outputs, خطی, batch_normalize, adam);
    l.uh->batch = batch;

    l.wh = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wh) = make_connected_layer(batch*اقدام, outputs, outputs, خطی, batch_normalize, adam);
    l.wh->batch = batch;

    l.batch_normalize = batch_normalize;


    l.outputs = outputs;
    l.output = صفر_حافظہ_دو(outputs*batch*اقدام, حجم(اعشاری));
    l.delta = صفر_حافظہ_دو(outputs*batch*اقدام, حجم(اعشاری));
    l.state = صفر_حافظہ_دو(outputs*batch, حجم(اعشاری));
    l.پچھلا_state = صفر_حافظہ_دو(outputs*batch, حجم(اعشاری));
    l.forgot_state = صفر_حافظہ_دو(outputs*batch, حجم(اعشاری));
    l.forgot_delta = صفر_حافظہ_دو(outputs*batch, حجم(اعشاری));

    l.r_cpu = صفر_حافظہ_دو(outputs*batch, حجم(اعشاری));
    l.z_cpu = صفر_حافظہ_دو(outputs*batch, حجم(اعشاری));
    l.h_cpu = صفر_حافظہ_دو(outputs*batch, حجم(اعشاری));

    l.forward = forward_gru_layer;
    l.backward = backward_gru_layer;
    l.update = update_gru_layer;

#اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_gru_layer_gpu;
    l.backward_gpu = backward_gru_layer_gpu;
    l.update_gpu = update_gru_layer_gpu;

    l.forgot_state_gpu = cuda_make_array(0, batch*outputs);
    l.forgot_delta_gpu = cuda_make_array(0, batch*outputs);
    l.پچھلا_state_gpu = cuda_make_array(0, batch*outputs);
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = cuda_make_array(0, batch*outputs*اقدام);
    l.delta_gpu = cuda_make_array(0, batch*outputs*اقدام);
    l.r_gpu = cuda_make_array(0, batch*outputs);
    l.z_gpu = cuda_make_array(0, batch*outputs);
    l.h_gpu = cuda_make_array(0, batch*outputs);

#اگر_تعریف_شدہ CUDNN
    cudnn_سیٹ_Tensor4d_Descriptor(l.uz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.uz->برآمد_c, l.uz->برآمد_h, l.uz->برآمد_w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l.uh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.uh->برآمد_c, l.uh->برآمد_h, l.uh->برآمد_w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l.ur->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.ur->برآمد_c, l.ur->برآمد_h, l.ur->برآمد_w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l.wz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.wz->برآمد_c, l.wz->برآمد_h, l.wz->برآمد_w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l.wh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.wh->برآمد_c, l.wh->برآمد_h, l.wh->برآمد_w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l.wr->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.wr->برآمد_c, l.wr->برآمد_h, l.wr->برآمد_w); 
#اگر_ختم
#اگر_ختم

    واپس l;
}

خالی update_gru_layer(layer l, update_دلائل a)
{
    update_connected_layer(*(l.ur), a);
    update_connected_layer(*(l.uz), a);
    update_connected_layer(*(l.uh), a);
    update_connected_layer(*(l.wr), a);
    update_connected_layer(*(l.wz), a);
    update_connected_layer(*(l.wh), a);
}

خالی forward_gru_layer(layer l, جال net)
{
    جال s = net;
    s.train = net.train;
    صحیح i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_cpu(l.outputs * l.batch * l.اقدام, 0, uz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.اقدام, 0, ur.delta, 1);
    fill_cpu(l.outputs * l.batch * l.اقدام, 0, uh.delta, 1);

    fill_cpu(l.outputs * l.batch * l.اقدام, 0, wz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.اقدام, 0, wr.delta, 1);
    fill_cpu(l.outputs * l.batch * l.اقدام, 0, wh.delta, 1);
    اگر(net.train) {
        fill_cpu(l.outputs * l.batch * l.اقدام, 0, l.delta, 1);
        copy_cpu(l.outputs*l.batch, l.state, 1, l.پچھلا_state, 1);
    }

    برائے (i = 0; i < l.اقدام; ++i) {
        s.داخلہ = l.state;
        forward_connected_layer(wz, s);
        forward_connected_layer(wr, s);

        s.داخلہ = net.داخلہ;
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

        s.داخلہ = l.forgot_state;
        forward_connected_layer(wh, s);

        copy_cpu(l.outputs*l.batch, uh.output, 1, l.h_cpu, 1);
        axpy_cpu(l.outputs*l.batch, 1, wh.output, 1, l.h_cpu, 1);

        اگر(l.tanh){
            activate_array(l.h_cpu, l.outputs*l.batch, TANH);
        } ورنہ {
            activate_array(l.h_cpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_مجموع_cpu(l.state, l.h_cpu, l.z_cpu, l.outputs*l.batch, l.output);

        copy_cpu(l.outputs*l.batch, l.output, 1, l.state, 1);

        net.داخلہ += l.inputs*l.batch;
        l.output += l.outputs*l.batch;
        increment_layer(&uz, 1);
        increment_layer(&ur, 1);
        increment_layer(&uh, 1);

        increment_layer(&wz, 1);
        increment_layer(&wr, 1);
        increment_layer(&wh, 1);
    }
}

خالی backward_gru_layer(layer l, جال net)
{
}

#اگر_تعریف_شدہ GPU

خالی pull_gru_layer(layer l)
{
}

خالی ڈالو_gru_layer(layer l)
{
}

خالی update_gru_layer_gpu(layer l, update_دلائل a)
{
    update_connected_layer_gpu(*(l.ur), a);
    update_connected_layer_gpu(*(l.uz), a);
    update_connected_layer_gpu(*(l.uh), a);
    update_connected_layer_gpu(*(l.wr), a);
    update_connected_layer_gpu(*(l.wz), a);
    update_connected_layer_gpu(*(l.wh), a);
}

خالی forward_gru_layer_gpu(layer l, جال net)
{
    جال s = {0};
    s.train = net.train;
    صحیح i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_gpu(l.outputs * l.batch * l.اقدام, 0, uz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.اقدام, 0, ur.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.اقدام, 0, uh.delta_gpu, 1);

    fill_gpu(l.outputs * l.batch * l.اقدام, 0, wz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.اقدام, 0, wr.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.اقدام, 0, wh.delta_gpu, 1);
    اگر(net.train) {
        fill_gpu(l.outputs * l.batch * l.اقدام, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.پچھلا_state_gpu, 1);
    }

    برائے (i = 0; i < l.اقدام; ++i) {
        s.داخلہ_gpu = l.state_gpu;
        forward_connected_layer_gpu(wz, s);
        forward_connected_layer_gpu(wr, s);

        s.داخلہ_gpu = net.داخلہ_gpu;
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

        s.داخلہ_gpu = l.forgot_state_gpu;
        forward_connected_layer_gpu(wh, s);

        copy_gpu(l.outputs*l.batch, uh.output_gpu, 1, l.h_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wh.output_gpu, 1, l.h_gpu, 1);

        اگر(l.tanh){
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);
        } ورنہ {
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_مجموع_gpu(l.state_gpu, l.h_gpu, l.z_gpu, l.outputs*l.batch, l.output_gpu);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.state_gpu, 1);

        net.داخلہ_gpu += l.inputs*l.batch;
        l.output_gpu += l.outputs*l.batch;
        increment_layer(&uz, 1);
        increment_layer(&ur, 1);
        increment_layer(&uh, 1);

        increment_layer(&wz, 1);
        increment_layer(&wr, 1);
        increment_layer(&wh, 1);
    }
}

خالی backward_gru_layer_gpu(layer l, جال net)
{
    جال s = {0};
    s.train = net.train;
    صحیح i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    increment_layer(&uz, l.اقدام - 1);
    increment_layer(&ur, l.اقدام - 1);
    increment_layer(&uh, l.اقدام - 1);

    increment_layer(&wz, l.اقدام - 1);
    increment_layer(&wr, l.اقدام - 1);
    increment_layer(&wh, l.اقدام - 1);

    net.داخلہ_gpu += l.inputs*l.batch*(l.اقدام-1);
    اگر(net.delta_gpu) net.delta_gpu += l.inputs*l.batch*(l.اقدام-1);
    l.output_gpu += l.outputs*l.batch*(l.اقدام-1);
    l.delta_gpu += l.outputs*l.batch*(l.اقدام-1);
    اعشاری *انتہا_state = l.output_gpu;
    برائے (i = l.اقدام-1; i >= 0; --i) {
        اگر(i != 0) copy_gpu(l.outputs*l.batch, l.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        ورنہ copy_gpu(l.outputs*l.batch, l.پچھلا_state_gpu, 1, l.state_gpu, 1);
        اعشاری *پچھلا_delta_gpu = (i == 0) ? 0 : l.delta_gpu - l.outputs*l.batch;

        copy_gpu(l.outputs*l.batch, uz.output_gpu, 1, l.z_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wz.output_gpu, 1, l.z_gpu, 1);

        copy_gpu(l.outputs*l.batch, ur.output_gpu, 1, l.r_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wr.output_gpu, 1, l.r_gpu, 1);

        activate_array_gpu(l.z_gpu, l.outputs*l.batch, LOGISTIC);
        activate_array_gpu(l.r_gpu, l.outputs*l.batch, LOGISTIC);

        copy_gpu(l.outputs*l.batch, uh.output_gpu, 1, l.h_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wh.output_gpu, 1, l.h_gpu, 1);

        اگر(l.tanh){
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);
        } ورنہ {
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_delta_gpu(l.state_gpu, l.h_gpu, l.z_gpu, پچھلا_delta_gpu, uh.delta_gpu, uz.delta_gpu, l.outputs*l.batch, l.delta_gpu);

        اگر(l.tanh){
            gradient_array_gpu(l.h_gpu, l.outputs*l.batch, TANH, uh.delta_gpu);
        } ورنہ {
            gradient_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC, uh.delta_gpu);
        }

        copy_gpu(l.outputs*l.batch, uh.delta_gpu, 1, wh.delta_gpu, 1);

        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.forgot_state_gpu, 1);
        mul_gpu(l.outputs*l.batch, l.r_gpu, 1, l.forgot_state_gpu, 1);
        fill_gpu(l.outputs*l.batch, 0, l.forgot_delta_gpu, 1);

        s.داخلہ_gpu = l.forgot_state_gpu;
        s.delta_gpu = l.forgot_delta_gpu;

        backward_connected_layer_gpu(wh, s);
        اگر(پچھلا_delta_gpu) mult_جوڑو_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.r_gpu, پچھلا_delta_gpu);
        mult_جوڑو_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.state_gpu, ur.delta_gpu);

        gradient_array_gpu(l.r_gpu, l.outputs*l.batch, LOGISTIC, ur.delta_gpu);
        copy_gpu(l.outputs*l.batch, ur.delta_gpu, 1, wr.delta_gpu, 1);

        gradient_array_gpu(l.z_gpu, l.outputs*l.batch, LOGISTIC, uz.delta_gpu);
        copy_gpu(l.outputs*l.batch, uz.delta_gpu, 1, wz.delta_gpu, 1);

        s.داخلہ_gpu = l.state_gpu;
        s.delta_gpu = پچھلا_delta_gpu;

        backward_connected_layer_gpu(wr, s);
        backward_connected_layer_gpu(wz, s);

        s.داخلہ_gpu = net.داخلہ_gpu;
        s.delta_gpu = net.delta_gpu;

        backward_connected_layer_gpu(uh, s);
        backward_connected_layer_gpu(ur, s);
        backward_connected_layer_gpu(uz, s);


        net.داخلہ_gpu -= l.inputs*l.batch;
        اگر(net.delta_gpu) net.delta_gpu -= l.inputs*l.batch;
        l.output_gpu -= l.outputs*l.batch;
        l.delta_gpu -= l.outputs*l.batch;
        increment_layer(&uz, -1);
        increment_layer(&ur, -1);
        increment_layer(&uh, -1);

        increment_layer(&wz, -1);
        increment_layer(&wr, -1);
        increment_layer(&wh, -1);
    }
    copy_gpu(l.outputs*l.batch, انتہا_state, 1, l.state_gpu, 1);
}
#اگر_ختم
