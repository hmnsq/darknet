#ضمن "gru_layer.h"
#ضمن "connected_layer.h"
#ضمن "utils.h"
#ضمن "cuda.h"
#ضمن "blas.h"
#ضمن "gemm.h"

#ضمن <math.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>

ساكن فراغ increment_layer(layer *l, صحيح قدماں)
{
    صحيح عدد = l->outputs*l->batch*قدماں;
    l->output += عدد;
    l->delta += عدد;
    l->x += عدد;
    l->x_norm += عدد;

#جے_تعریف_شدہ GPU
    l->output_gpu += عدد;
    l->delta_gpu += عدد;
    l->x_gpu += عدد;
    l->x_norm_gpu += عدد;
#جے_انت
}

layer make_gru_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح قدماں, صحيح batch_normalize, صحيح adam)
{
    fprintf(stderr, "GRU Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / قدماں;
    layer l = {0};
    l.batch = batch;
    l.type = GRU;
    l.قدماں = قدماں;
    l.inputs = inputs;

    l.uz = حافظہ_دیو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.uz) = make_connected_layer(batch*قدماں, inputs, outputs, خطی, batch_normalize, adam);
    l.uz->batch = batch;

    l.wz = حافظہ_دیو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wz) = make_connected_layer(batch*قدماں, outputs, outputs, خطی, batch_normalize, adam);
    l.wz->batch = batch;

    l.ur = حافظہ_دیو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.ur) = make_connected_layer(batch*قدماں, inputs, outputs, خطی, batch_normalize, adam);
    l.ur->batch = batch;

    l.wr = حافظہ_دیو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wr) = make_connected_layer(batch*قدماں, outputs, outputs, خطی, batch_normalize, adam);
    l.wr->batch = batch;



    l.uh = حافظہ_دیو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.uh) = make_connected_layer(batch*قدماں, inputs, outputs, خطی, batch_normalize, adam);
    l.uh->batch = batch;

    l.wh = حافظہ_دیو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wh) = make_connected_layer(batch*قدماں, outputs, outputs, خطی, batch_normalize, adam);
    l.wh->batch = batch;

    l.batch_normalize = batch_normalize;


    l.outputs = outputs;
    l.output = صفر_حافظہ_دیو(outputs*batch*قدماں, حجم(عائم));
    l.delta = صفر_حافظہ_دیو(outputs*batch*قدماں, حجم(عائم));
    l.state = صفر_حافظہ_دیو(outputs*batch, حجم(عائم));
    l.prev_state = صفر_حافظہ_دیو(outputs*batch, حجم(عائم));
    l.forgot_state = صفر_حافظہ_دیو(outputs*batch, حجم(عائم));
    l.forgot_delta = صفر_حافظہ_دیو(outputs*batch, حجم(عائم));

    l.r_cpu = صفر_حافظہ_دیو(outputs*batch, حجم(عائم));
    l.z_cpu = صفر_حافظہ_دیو(outputs*batch, حجم(عائم));
    l.h_cpu = صفر_حافظہ_دیو(outputs*batch, حجم(عائم));

    l.forward = forward_gru_layer;
    l.backward = backward_gru_layer;
    l.update = update_gru_layer;

#جے_تعریف_شدہ GPU
    l.forward_gpu = forward_gru_layer_gpu;
    l.backward_gpu = backward_gru_layer_gpu;
    l.update_gpu = update_gru_layer_gpu;

    l.forgot_state_gpu = cuda_make_array(0, batch*outputs);
    l.forgot_delta_gpu = cuda_make_array(0, batch*outputs);
    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = cuda_make_array(0, batch*outputs*قدماں);
    l.delta_gpu = cuda_make_array(0, batch*outputs*قدماں);
    l.r_gpu = cuda_make_array(0, batch*outputs);
    l.z_gpu = cuda_make_array(0, batch*outputs);
    l.h_gpu = cuda_make_array(0, batch*outputs);

#جے_تعریف_شدہ CUDNN
    cudnn_سیٹ_Tensor4d_Descriptor(l.uz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_انکڑے_FLOAT, batch, l.uz->باہرلا_c, l.uz->باہرلا_h, l.uz->باہرلا_w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l.uh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_انکڑے_FLOAT, batch, l.uh->باہرلا_c, l.uh->باہرلا_h, l.uh->باہرلا_w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l.ur->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_انکڑے_FLOAT, batch, l.ur->باہرلا_c, l.ur->باہرلا_h, l.ur->باہرلا_w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l.wz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_انکڑے_FLOAT, batch, l.wz->باہرلا_c, l.wz->باہرلا_h, l.wz->باہرلا_w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l.wh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_انکڑے_FLOAT, batch, l.wh->باہرلا_c, l.wh->باہرلا_h, l.wh->باہرلا_w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l.wr->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_انکڑے_FLOAT, batch, l.wr->باہرلا_c, l.wr->باہرلا_h, l.wr->باہرلا_w); 
#جے_انت
#جے_انت

    أرجع l;
}

فراغ update_gru_layer(layer l, update_دلیلاں a)
{
    update_connected_layer(*(l.ur), a);
    update_connected_layer(*(l.uz), a);
    update_connected_layer(*(l.uh), a);
    update_connected_layer(*(l.wr), a);
    update_connected_layer(*(l.wz), a);
    update_connected_layer(*(l.wh), a);
}

فراغ forward_gru_layer(layer l, جال net)
{
    جال s = net;
    s.train = net.train;
    صحيح i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_cpu(l.outputs * l.batch * l.قدماں, 0, uz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.قدماں, 0, ur.delta, 1);
    fill_cpu(l.outputs * l.batch * l.قدماں, 0, uh.delta, 1);

    fill_cpu(l.outputs * l.batch * l.قدماں, 0, wz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.قدماں, 0, wr.delta, 1);
    fill_cpu(l.outputs * l.batch * l.قدماں, 0, wh.delta, 1);
    إذا(net.train) {
        fill_cpu(l.outputs * l.batch * l.قدماں, 0, l.delta, 1);
        copy_cpu(l.outputs*l.batch, l.state, 1, l.prev_state, 1);
    }

    لكل (i = 0; i < l.قدماں; ++i) {
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

        إذا(l.tanh){
            activate_array(l.h_cpu, l.outputs*l.batch, TANH);
        } وإلا {
            activate_array(l.h_cpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_جوڑ_cpu(l.state, l.h_cpu, l.z_cpu, l.outputs*l.batch, l.output);

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

فراغ backward_gru_layer(layer l, جال net)
{
}

#جے_تعریف_شدہ GPU

فراغ pull_gru_layer(layer l)
{
}

فراغ ڈالو_gru_layer(layer l)
{
}

فراغ update_gru_layer_gpu(layer l, update_دلیلاں a)
{
    update_connected_layer_gpu(*(l.ur), a);
    update_connected_layer_gpu(*(l.uz), a);
    update_connected_layer_gpu(*(l.uh), a);
    update_connected_layer_gpu(*(l.wr), a);
    update_connected_layer_gpu(*(l.wz), a);
    update_connected_layer_gpu(*(l.wh), a);
}

فراغ forward_gru_layer_gpu(layer l, جال net)
{
    جال s = {0};
    s.train = net.train;
    صحيح i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_gpu(l.outputs * l.batch * l.قدماں, 0, uz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.قدماں, 0, ur.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.قدماں, 0, uh.delta_gpu, 1);

    fill_gpu(l.outputs * l.batch * l.قدماں, 0, wz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.قدماں, 0, wr.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.قدماں, 0, wh.delta_gpu, 1);
    إذا(net.train) {
        fill_gpu(l.outputs * l.batch * l.قدماں, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    لكل (i = 0; i < l.قدماں; ++i) {
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

        إذا(l.tanh){
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);
        } وإلا {
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_جوڑ_gpu(l.state_gpu, l.h_gpu, l.z_gpu, l.outputs*l.batch, l.output_gpu);
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

فراغ backward_gru_layer_gpu(layer l, جال net)
{
    جال s = {0};
    s.train = net.train;
    صحيح i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    increment_layer(&uz, l.قدماں - 1);
    increment_layer(&ur, l.قدماں - 1);
    increment_layer(&uh, l.قدماں - 1);

    increment_layer(&wz, l.قدماں - 1);
    increment_layer(&wr, l.قدماں - 1);
    increment_layer(&wh, l.قدماں - 1);

    net.داخلہ_gpu += l.inputs*l.batch*(l.قدماں-1);
    إذا(net.delta_gpu) net.delta_gpu += l.inputs*l.batch*(l.قدماں-1);
    l.output_gpu += l.outputs*l.batch*(l.قدماں-1);
    l.delta_gpu += l.outputs*l.batch*(l.قدماں-1);
    عائم *سرا_state = l.output_gpu;
    لكل (i = l.قدماں-1; i >= 0; --i) {
        إذا(i != 0) copy_gpu(l.outputs*l.batch, l.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        وإلا copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        عائم *prev_delta_gpu = (i == 0) ? 0 : l.delta_gpu - l.outputs*l.batch;

        copy_gpu(l.outputs*l.batch, uz.output_gpu, 1, l.z_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wz.output_gpu, 1, l.z_gpu, 1);

        copy_gpu(l.outputs*l.batch, ur.output_gpu, 1, l.r_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wr.output_gpu, 1, l.r_gpu, 1);

        activate_array_gpu(l.z_gpu, l.outputs*l.batch, LOGISTIC);
        activate_array_gpu(l.r_gpu, l.outputs*l.batch, LOGISTIC);

        copy_gpu(l.outputs*l.batch, uh.output_gpu, 1, l.h_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wh.output_gpu, 1, l.h_gpu, 1);

        إذا(l.tanh){
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);
        } وإلا {
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_delta_gpu(l.state_gpu, l.h_gpu, l.z_gpu, prev_delta_gpu, uh.delta_gpu, uz.delta_gpu, l.outputs*l.batch, l.delta_gpu);

        إذا(l.tanh){
            gradient_array_gpu(l.h_gpu, l.outputs*l.batch, TANH, uh.delta_gpu);
        } وإلا {
            gradient_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC, uh.delta_gpu);
        }

        copy_gpu(l.outputs*l.batch, uh.delta_gpu, 1, wh.delta_gpu, 1);

        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.forgot_state_gpu, 1);
        mul_gpu(l.outputs*l.batch, l.r_gpu, 1, l.forgot_state_gpu, 1);
        fill_gpu(l.outputs*l.batch, 0, l.forgot_delta_gpu, 1);

        s.داخلہ_gpu = l.forgot_state_gpu;
        s.delta_gpu = l.forgot_delta_gpu;

        backward_connected_layer_gpu(wh, s);
        إذا(prev_delta_gpu) mult_جوڑو_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.r_gpu, prev_delta_gpu);
        mult_جوڑو_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.state_gpu, ur.delta_gpu);

        gradient_array_gpu(l.r_gpu, l.outputs*l.batch, LOGISTIC, ur.delta_gpu);
        copy_gpu(l.outputs*l.batch, ur.delta_gpu, 1, wr.delta_gpu, 1);

        gradient_array_gpu(l.z_gpu, l.outputs*l.batch, LOGISTIC, uz.delta_gpu);
        copy_gpu(l.outputs*l.batch, uz.delta_gpu, 1, wz.delta_gpu, 1);

        s.داخلہ_gpu = l.state_gpu;
        s.delta_gpu = prev_delta_gpu;

        backward_connected_layer_gpu(wr, s);
        backward_connected_layer_gpu(wz, s);

        s.داخلہ_gpu = net.داخلہ_gpu;
        s.delta_gpu = net.delta_gpu;

        backward_connected_layer_gpu(uh, s);
        backward_connected_layer_gpu(ur, s);
        backward_connected_layer_gpu(uz, s);


        net.داخلہ_gpu -= l.inputs*l.batch;
        إذا(net.delta_gpu) net.delta_gpu -= l.inputs*l.batch;
        l.output_gpu -= l.outputs*l.batch;
        l.delta_gpu -= l.outputs*l.batch;
        increment_layer(&uz, -1);
        increment_layer(&ur, -1);
        increment_layer(&uh, -1);

        increment_layer(&wz, -1);
        increment_layer(&wr, -1);
        increment_layer(&wh, -1);
    }
    copy_gpu(l.outputs*l.batch, سرا_state, 1, l.state_gpu, 1);
}
#جے_انت
