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

ساكن فراغ increment_layer(layer *l, صحيح قدمن)
{
    صحيح انگ = l->outputs*l->batch*قدمن;
    l->output += انگ;
    l->delta += انگ;
    l->x += انگ;
    l->x_norm += انگ;

#جيڪڏهن_وصف_ٿيل GPU
    l->output_gpu += انگ;
    l->delta_gpu += انگ;
    l->x_gpu += انگ;
    l->x_norm_gpu += انگ;
#جيڪڏهن_پورو
}

layer make_gru_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح قدمن, صحيح batch_normalize, صحيح adam)
{
    fprintf(stderr, "GRU Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / قدمن;
    layer l = {0};
    l.batch = batch;
    l.type = GRU;
    l.قدمن = قدمن;
    l.inputs = inputs;

    l.uz = يادگيري_ڏيو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.uz) = make_connected_layer(batch*قدمن, inputs, outputs, لڪيري, batch_normalize, adam);
    l.uz->batch = batch;

    l.wz = يادگيري_ڏيو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wz) = make_connected_layer(batch*قدمن, outputs, outputs, لڪيري, batch_normalize, adam);
    l.wz->batch = batch;

    l.ur = يادگيري_ڏيو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.ur) = make_connected_layer(batch*قدمن, inputs, outputs, لڪيري, batch_normalize, adam);
    l.ur->batch = batch;

    l.wr = يادگيري_ڏيو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wr) = make_connected_layer(batch*قدمن, outputs, outputs, لڪيري, batch_normalize, adam);
    l.wr->batch = batch;



    l.uh = يادگيري_ڏيو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.uh) = make_connected_layer(batch*قدمن, inputs, outputs, لڪيري, batch_normalize, adam);
    l.uh->batch = batch;

    l.wh = يادگيري_ڏيو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wh) = make_connected_layer(batch*قدمن, outputs, outputs, لڪيري, batch_normalize, adam);
    l.wh->batch = batch;

    l.batch_normalize = batch_normalize;


    l.outputs = outputs;
    l.output = صفر_يادگيري_ڏيو(outputs*batch*قدمن, حجم(عائم));
    l.delta = صفر_يادگيري_ڏيو(outputs*batch*قدمن, حجم(عائم));
    l.state = صفر_يادگيري_ڏيو(outputs*batch, حجم(عائم));
    l.prev_state = صفر_يادگيري_ڏيو(outputs*batch, حجم(عائم));
    l.forgot_state = صفر_يادگيري_ڏيو(outputs*batch, حجم(عائم));
    l.forgot_delta = صفر_يادگيري_ڏيو(outputs*batch, حجم(عائم));

    l.r_cpu = صفر_يادگيري_ڏيو(outputs*batch, حجم(عائم));
    l.z_cpu = صفر_يادگيري_ڏيو(outputs*batch, حجم(عائم));
    l.h_cpu = صفر_يادگيري_ڏيو(outputs*batch, حجم(عائم));

    l.forward = forward_gru_layer;
    l.backward = backward_gru_layer;
    l.update = update_gru_layer;

#جيڪڏهن_وصف_ٿيل GPU
    l.forward_gpu = forward_gru_layer_gpu;
    l.backward_gpu = backward_gru_layer_gpu;
    l.update_gpu = update_gru_layer_gpu;

    l.forgot_state_gpu = cuda_make_array(0, batch*outputs);
    l.forgot_delta_gpu = cuda_make_array(0, batch*outputs);
    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = cuda_make_array(0, batch*outputs*قدمن);
    l.delta_gpu = cuda_make_array(0, batch*outputs*قدمن);
    l.r_gpu = cuda_make_array(0, batch*outputs);
    l.z_gpu = cuda_make_array(0, batch*outputs);
    l.h_gpu = cuda_make_array(0, batch*outputs);

#جيڪڏهن_وصف_ٿيل CUDNN
    cudnn_سيٽ_Tensor4d_Descriptor(l.uz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.uz->ٻاهريون_c, l.uz->ٻاهريون_h, l.uz->ٻاهريون_w); 
    cudnn_سيٽ_Tensor4d_Descriptor(l.uh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.uh->ٻاهريون_c, l.uh->ٻاهريون_h, l.uh->ٻاهريون_w); 
    cudnn_سيٽ_Tensor4d_Descriptor(l.ur->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.ur->ٻاهريون_c, l.ur->ٻاهريون_h, l.ur->ٻاهريون_w); 
    cudnn_سيٽ_Tensor4d_Descriptor(l.wz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.wz->ٻاهريون_c, l.wz->ٻاهريون_h, l.wz->ٻاهريون_w); 
    cudnn_سيٽ_Tensor4d_Descriptor(l.wh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.wh->ٻاهريون_c, l.wh->ٻاهريون_h, l.wh->ٻاهريون_w); 
    cudnn_سيٽ_Tensor4d_Descriptor(l.wr->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.wr->ٻاهريون_c, l.wr->ٻاهريون_h, l.wr->ٻاهريون_w); 
#جيڪڏهن_پورو
#جيڪڏهن_پورو

    أرجع l;
}

فراغ update_gru_layer(layer l, update_دليلون a)
{
    update_connected_layer(*(l.ur), a);
    update_connected_layer(*(l.uz), a);
    update_connected_layer(*(l.uh), a);
    update_connected_layer(*(l.wr), a);
    update_connected_layer(*(l.wz), a);
    update_connected_layer(*(l.wh), a);
}

فراغ forward_gru_layer(layer l, ڄار net)
{
    ڄار s = net;
    s.train = net.train;
    صحيح i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_cpu(l.outputs * l.batch * l.قدمن, 0, uz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.قدمن, 0, ur.delta, 1);
    fill_cpu(l.outputs * l.batch * l.قدمن, 0, uh.delta, 1);

    fill_cpu(l.outputs * l.batch * l.قدمن, 0, wz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.قدمن, 0, wr.delta, 1);
    fill_cpu(l.outputs * l.batch * l.قدمن, 0, wh.delta, 1);
    إذا(net.train) {
        fill_cpu(l.outputs * l.batch * l.قدمن, 0, l.delta, 1);
        copy_cpu(l.outputs*l.batch, l.state, 1, l.prev_state, 1);
    }

    لكل (i = 0; i < l.قدمن; ++i) {
        s.داخلا = l.state;
        forward_connected_layer(wz, s);
        forward_connected_layer(wr, s);

        s.داخلا = net.داخلا;
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

        s.داخلا = l.forgot_state;
        forward_connected_layer(wh, s);

        copy_cpu(l.outputs*l.batch, uh.output, 1, l.h_cpu, 1);
        axpy_cpu(l.outputs*l.batch, 1, wh.output, 1, l.h_cpu, 1);

        إذا(l.tanh){
            activate_array(l.h_cpu, l.outputs*l.batch, TANH);
        } وإلا {
            activate_array(l.h_cpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_جوڙ_cpu(l.state, l.h_cpu, l.z_cpu, l.outputs*l.batch, l.output);

        copy_cpu(l.outputs*l.batch, l.output, 1, l.state, 1);

        net.داخلا += l.inputs*l.batch;
        l.output += l.outputs*l.batch;
        increment_layer(&uz, 1);
        increment_layer(&ur, 1);
        increment_layer(&uh, 1);

        increment_layer(&wz, 1);
        increment_layer(&wr, 1);
        increment_layer(&wh, 1);
    }
}

فراغ backward_gru_layer(layer l, ڄار net)
{
}

#جيڪڏهن_وصف_ٿيل GPU

فراغ pull_gru_layer(layer l)
{
}

فراغ ڌڪو_gru_layer(layer l)
{
}

فراغ update_gru_layer_gpu(layer l, update_دليلون a)
{
    update_connected_layer_gpu(*(l.ur), a);
    update_connected_layer_gpu(*(l.uz), a);
    update_connected_layer_gpu(*(l.uh), a);
    update_connected_layer_gpu(*(l.wr), a);
    update_connected_layer_gpu(*(l.wz), a);
    update_connected_layer_gpu(*(l.wh), a);
}

فراغ forward_gru_layer_gpu(layer l, ڄار net)
{
    ڄار s = {0};
    s.train = net.train;
    صحيح i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_gpu(l.outputs * l.batch * l.قدمن, 0, uz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.قدمن, 0, ur.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.قدمن, 0, uh.delta_gpu, 1);

    fill_gpu(l.outputs * l.batch * l.قدمن, 0, wz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.قدمن, 0, wr.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.قدمن, 0, wh.delta_gpu, 1);
    إذا(net.train) {
        fill_gpu(l.outputs * l.batch * l.قدمن, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    لكل (i = 0; i < l.قدمن; ++i) {
        s.داخلا_gpu = l.state_gpu;
        forward_connected_layer_gpu(wz, s);
        forward_connected_layer_gpu(wr, s);

        s.داخلا_gpu = net.داخلا_gpu;
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

        s.داخلا_gpu = l.forgot_state_gpu;
        forward_connected_layer_gpu(wh, s);

        copy_gpu(l.outputs*l.batch, uh.output_gpu, 1, l.h_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wh.output_gpu, 1, l.h_gpu, 1);

        إذا(l.tanh){
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);
        } وإلا {
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_جوڙ_gpu(l.state_gpu, l.h_gpu, l.z_gpu, l.outputs*l.batch, l.output_gpu);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.state_gpu, 1);

        net.داخلا_gpu += l.inputs*l.batch;
        l.output_gpu += l.outputs*l.batch;
        increment_layer(&uz, 1);
        increment_layer(&ur, 1);
        increment_layer(&uh, 1);

        increment_layer(&wz, 1);
        increment_layer(&wr, 1);
        increment_layer(&wh, 1);
    }
}

فراغ backward_gru_layer_gpu(layer l, ڄار net)
{
    ڄار s = {0};
    s.train = net.train;
    صحيح i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    increment_layer(&uz, l.قدمن - 1);
    increment_layer(&ur, l.قدمن - 1);
    increment_layer(&uh, l.قدمن - 1);

    increment_layer(&wz, l.قدمن - 1);
    increment_layer(&wr, l.قدمن - 1);
    increment_layer(&wh, l.قدمن - 1);

    net.داخلا_gpu += l.inputs*l.batch*(l.قدمن-1);
    إذا(net.delta_gpu) net.delta_gpu += l.inputs*l.batch*(l.قدمن-1);
    l.output_gpu += l.outputs*l.batch*(l.قدمن-1);
    l.delta_gpu += l.outputs*l.batch*(l.قدمن-1);
    عائم *پڇاڙي_state = l.output_gpu;
    لكل (i = l.قدمن-1; i >= 0; --i) {
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

        s.داخلا_gpu = l.forgot_state_gpu;
        s.delta_gpu = l.forgot_delta_gpu;

        backward_connected_layer_gpu(wh, s);
        إذا(prev_delta_gpu) mult_جوڙيو_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.r_gpu, prev_delta_gpu);
        mult_جوڙيو_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.state_gpu, ur.delta_gpu);

        gradient_array_gpu(l.r_gpu, l.outputs*l.batch, LOGISTIC, ur.delta_gpu);
        copy_gpu(l.outputs*l.batch, ur.delta_gpu, 1, wr.delta_gpu, 1);

        gradient_array_gpu(l.z_gpu, l.outputs*l.batch, LOGISTIC, uz.delta_gpu);
        copy_gpu(l.outputs*l.batch, uz.delta_gpu, 1, wz.delta_gpu, 1);

        s.داخلا_gpu = l.state_gpu;
        s.delta_gpu = prev_delta_gpu;

        backward_connected_layer_gpu(wr, s);
        backward_connected_layer_gpu(wz, s);

        s.داخلا_gpu = net.داخلا_gpu;
        s.delta_gpu = net.delta_gpu;

        backward_connected_layer_gpu(uh, s);
        backward_connected_layer_gpu(ur, s);
        backward_connected_layer_gpu(uz, s);


        net.داخلا_gpu -= l.inputs*l.batch;
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
    copy_gpu(l.outputs*l.batch, پڇاڙي_state, 1, l.state_gpu, 1);
}
#جيڪڏهن_پورو
