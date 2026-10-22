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

ساكن فراغ increment_layer(layer *l, صحيح ګامونه)
{
    صحيح عدد = l->outputs*l->batch*ګامونه;
    l->output += عدد;
    l->delta += عدد;
    l->x += عدد;
    l->x_norm += عدد;

#که_تعریف_شوی GPU
    l->output_gpu += عدد;
    l->delta_gpu += عدد;
    l->x_gpu += عدد;
    l->x_norm_gpu += عدد;
#د_که_پای
}

layer make_gru_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح ګامونه, صحيح batch_normalize, صحيح adam)
{
    fprintf(stderr, "GRU Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / ګامونه;
    layer l = {0};
    l.batch = batch;
    l.type = GRU;
    l.ګامونه = ګامونه;
    l.inputs = inputs;

    l.uz = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.uz) = make_connected_layer(batch*ګامونه, inputs, outputs, خطي, batch_normalize, adam);
    l.uz->batch = batch;

    l.wz = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wz) = make_connected_layer(batch*ګامونه, outputs, outputs, خطي, batch_normalize, adam);
    l.wz->batch = batch;

    l.ur = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.ur) = make_connected_layer(batch*ګامونه, inputs, outputs, خطي, batch_normalize, adam);
    l.ur->batch = batch;

    l.wr = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wr) = make_connected_layer(batch*ګامونه, outputs, outputs, خطي, batch_normalize, adam);
    l.wr->batch = batch;



    l.uh = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.uh) = make_connected_layer(batch*ګامونه, inputs, outputs, خطي, batch_normalize, adam);
    l.uh->batch = batch;

    l.wh = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wh) = make_connected_layer(batch*ګامونه, outputs, outputs, خطي, batch_normalize, adam);
    l.wh->batch = batch;

    l.batch_normalize = batch_normalize;


    l.outputs = outputs;
    l.output = حافظه_صفر_ورکړه(outputs*batch*ګامونه, حجم(عائم));
    l.delta = حافظه_صفر_ورکړه(outputs*batch*ګامونه, حجم(عائم));
    l.state = حافظه_صفر_ورکړه(outputs*batch, حجم(عائم));
    l.prev_state = حافظه_صفر_ورکړه(outputs*batch, حجم(عائم));
    l.forgot_state = حافظه_صفر_ورکړه(outputs*batch, حجم(عائم));
    l.forgot_delta = حافظه_صفر_ورکړه(outputs*batch, حجم(عائم));

    l.r_cpu = حافظه_صفر_ورکړه(outputs*batch, حجم(عائم));
    l.z_cpu = حافظه_صفر_ورکړه(outputs*batch, حجم(عائم));
    l.h_cpu = حافظه_صفر_ورکړه(outputs*batch, حجم(عائم));

    l.forward = forward_gru_layer;
    l.backward = backward_gru_layer;
    l.update = update_gru_layer;

#که_تعریف_شوی GPU
    l.forward_gpu = forward_gru_layer_gpu;
    l.backward_gpu = backward_gru_layer_gpu;
    l.update_gpu = update_gru_layer_gpu;

    l.forgot_state_gpu = cuda_make_array(0, batch*outputs);
    l.forgot_delta_gpu = cuda_make_array(0, batch*outputs);
    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = cuda_make_array(0, batch*outputs*ګامونه);
    l.delta_gpu = cuda_make_array(0, batch*outputs*ګامونه);
    l.r_gpu = cuda_make_array(0, batch*outputs);
    l.z_gpu = cuda_make_array(0, batch*outputs);
    l.h_gpu = cuda_make_array(0, batch*outputs);

#که_تعریف_شوی CUDNN
    cudnn_سېټ_Tensor4d_Descriptor(l.uz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.uz->بهر_c, l.uz->بهر_h, l.uz->بهر_w); 
    cudnn_سېټ_Tensor4d_Descriptor(l.uh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.uh->بهر_c, l.uh->بهر_h, l.uh->بهر_w); 
    cudnn_سېټ_Tensor4d_Descriptor(l.ur->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.ur->بهر_c, l.ur->بهر_h, l.ur->بهر_w); 
    cudnn_سېټ_Tensor4d_Descriptor(l.wz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.wz->بهر_c, l.wz->بهر_h, l.wz->بهر_w); 
    cudnn_سېټ_Tensor4d_Descriptor(l.wh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.wh->بهر_c, l.wh->بهر_h, l.wh->بهر_w); 
    cudnn_سېټ_Tensor4d_Descriptor(l.wr->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.wr->بهر_c, l.wr->بهر_h, l.wr->بهر_w); 
#د_که_پای
#د_که_پای

    أرجع l;
}

فراغ update_gru_layer(layer l, update_دلیلونه a)
{
    update_connected_layer(*(l.ur), a);
    update_connected_layer(*(l.uz), a);
    update_connected_layer(*(l.uh), a);
    update_connected_layer(*(l.wr), a);
    update_connected_layer(*(l.wz), a);
    update_connected_layer(*(l.wh), a);
}

فراغ forward_gru_layer(layer l, شبکه net)
{
    شبکه s = net;
    s.train = net.train;
    صحيح i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, uz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, ur.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, uh.delta, 1);

    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, wz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, wr.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, wh.delta, 1);
    إذا(net.train) {
        fill_cpu(l.outputs * l.batch * l.ګامونه, 0, l.delta, 1);
        copy_cpu(l.outputs*l.batch, l.state, 1, l.prev_state, 1);
    }

    لكل (i = 0; i < l.ګامونه; ++i) {
        s.ننوت = l.state;
        forward_connected_layer(wz, s);
        forward_connected_layer(wr, s);

        s.ننوت = net.ننوت;
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

        s.ننوت = l.forgot_state;
        forward_connected_layer(wh, s);

        copy_cpu(l.outputs*l.batch, uh.output, 1, l.h_cpu, 1);
        axpy_cpu(l.outputs*l.batch, 1, wh.output, 1, l.h_cpu, 1);

        إذا(l.tanh){
            activate_array(l.h_cpu, l.outputs*l.batch, TANH);
        } وإلا {
            activate_array(l.h_cpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_مجموعه_cpu(l.state, l.h_cpu, l.z_cpu, l.outputs*l.batch, l.output);

        copy_cpu(l.outputs*l.batch, l.output, 1, l.state, 1);

        net.ننوت += l.inputs*l.batch;
        l.output += l.outputs*l.batch;
        increment_layer(&uz, 1);
        increment_layer(&ur, 1);
        increment_layer(&uh, 1);

        increment_layer(&wz, 1);
        increment_layer(&wr, 1);
        increment_layer(&wh, 1);
    }
}

فراغ backward_gru_layer(layer l, شبکه net)
{
}

#که_تعریف_شوی GPU

فراغ pull_gru_layer(layer l)
{
}

فراغ ټېل_gru_layer(layer l)
{
}

فراغ update_gru_layer_gpu(layer l, update_دلیلونه a)
{
    update_connected_layer_gpu(*(l.ur), a);
    update_connected_layer_gpu(*(l.uz), a);
    update_connected_layer_gpu(*(l.uh), a);
    update_connected_layer_gpu(*(l.wr), a);
    update_connected_layer_gpu(*(l.wz), a);
    update_connected_layer_gpu(*(l.wh), a);
}

فراغ forward_gru_layer_gpu(layer l, شبکه net)
{
    شبکه s = {0};
    s.train = net.train;
    صحيح i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, uz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, ur.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, uh.delta_gpu, 1);

    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, wz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, wr.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, wh.delta_gpu, 1);
    إذا(net.train) {
        fill_gpu(l.outputs * l.batch * l.ګامونه, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    لكل (i = 0; i < l.ګامونه; ++i) {
        s.ننوت_gpu = l.state_gpu;
        forward_connected_layer_gpu(wz, s);
        forward_connected_layer_gpu(wr, s);

        s.ننوت_gpu = net.ننوت_gpu;
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

        s.ننوت_gpu = l.forgot_state_gpu;
        forward_connected_layer_gpu(wh, s);

        copy_gpu(l.outputs*l.batch, uh.output_gpu, 1, l.h_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wh.output_gpu, 1, l.h_gpu, 1);

        إذا(l.tanh){
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);
        } وإلا {
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_مجموعه_gpu(l.state_gpu, l.h_gpu, l.z_gpu, l.outputs*l.batch, l.output_gpu);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.state_gpu, 1);

        net.ننوت_gpu += l.inputs*l.batch;
        l.output_gpu += l.outputs*l.batch;
        increment_layer(&uz, 1);
        increment_layer(&ur, 1);
        increment_layer(&uh, 1);

        increment_layer(&wz, 1);
        increment_layer(&wr, 1);
        increment_layer(&wh, 1);
    }
}

فراغ backward_gru_layer_gpu(layer l, شبکه net)
{
    شبکه s = {0};
    s.train = net.train;
    صحيح i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    increment_layer(&uz, l.ګامونه - 1);
    increment_layer(&ur, l.ګامونه - 1);
    increment_layer(&uh, l.ګامونه - 1);

    increment_layer(&wz, l.ګامونه - 1);
    increment_layer(&wr, l.ګامونه - 1);
    increment_layer(&wh, l.ګامونه - 1);

    net.ننوت_gpu += l.inputs*l.batch*(l.ګامونه-1);
    إذا(net.delta_gpu) net.delta_gpu += l.inputs*l.batch*(l.ګامونه-1);
    l.output_gpu += l.outputs*l.batch*(l.ګامونه-1);
    l.delta_gpu += l.outputs*l.batch*(l.ګامونه-1);
    عائم *پای_ټکی_state = l.output_gpu;
    لكل (i = l.ګامونه-1; i >= 0; --i) {
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

        s.ننوت_gpu = l.forgot_state_gpu;
        s.delta_gpu = l.forgot_delta_gpu;

        backward_connected_layer_gpu(wh, s);
        إذا(prev_delta_gpu) mult_جمع_کړه_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.r_gpu, prev_delta_gpu);
        mult_جمع_کړه_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.state_gpu, ur.delta_gpu);

        gradient_array_gpu(l.r_gpu, l.outputs*l.batch, LOGISTIC, ur.delta_gpu);
        copy_gpu(l.outputs*l.batch, ur.delta_gpu, 1, wr.delta_gpu, 1);

        gradient_array_gpu(l.z_gpu, l.outputs*l.batch, LOGISTIC, uz.delta_gpu);
        copy_gpu(l.outputs*l.batch, uz.delta_gpu, 1, wz.delta_gpu, 1);

        s.ننوت_gpu = l.state_gpu;
        s.delta_gpu = prev_delta_gpu;

        backward_connected_layer_gpu(wr, s);
        backward_connected_layer_gpu(wz, s);

        s.ننوت_gpu = net.ننوت_gpu;
        s.delta_gpu = net.delta_gpu;

        backward_connected_layer_gpu(uh, s);
        backward_connected_layer_gpu(ur, s);
        backward_connected_layer_gpu(uz, s);


        net.ننوت_gpu -= l.inputs*l.batch;
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
    copy_gpu(l.outputs*l.batch, پای_ټکی_state, 1, l.state_gpu, 1);
}
#د_که_پای
