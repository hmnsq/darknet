#ಅನ್ತರ್ಭಾವ "gru_layer.h"
#ಅನ್ತರ್ಭಾವ "connected_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "gemm.h"

#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>

ಸ್ಥಿರ ಶೂನ್ಯ increment_layer(layer *l, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆಗಳು)
{
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ = l->outputs*l->batch*ಹೆಜ್ಜೆಗಳು;
    l->output += ಅಂಕೆ;
    l->delta += ಅಂಕೆ;
    l->x += ಅಂಕೆ;
    l->x_norm += ಅಂಕೆ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l->output_gpu += ಅಂಕೆ;
    l->delta_gpu += ಅಂಕೆ;
    l->x_gpu += ಅಂಕೆ;
    l->x_norm_gpu += ಅಂಕೆ;
#ಆದರೆ_ಅಂತ್ಯ
}

layer make_gru_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ಪೂರ್ಣಾಂಕ outputs, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆಗಳು, ಪೂರ್ಣಾಂಕ batch_normalize, ಪೂರ್ಣಾಂಕ adam)
{
    fprintf(stderr, "GRU Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / ಹೆಜ್ಜೆಗಳು;
    layer l = {0};
    l.batch = batch;
    l.type = GRU;
    l.ಹೆಜ್ಜೆಗಳು = ಹೆಜ್ಜೆಗಳು;
    l.inputs = inputs;

    l.uz = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.uz) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, inputs, outputs, ರೇಖೀಯ, batch_normalize, adam);
    l.uz->batch = batch;

    l.wz = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.wz) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, outputs, outputs, ರೇಖೀಯ, batch_normalize, adam);
    l.wz->batch = batch;

    l.ur = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.ur) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, inputs, outputs, ರೇಖೀಯ, batch_normalize, adam);
    l.ur->batch = batch;

    l.wr = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.wr) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, outputs, outputs, ರೇಖೀಯ, batch_normalize, adam);
    l.wr->batch = batch;



    l.uh = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.uh) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, inputs, outputs, ರೇಖೀಯ, batch_normalize, adam);
    l.uh->batch = batch;

    l.wh = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.wh) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, outputs, outputs, ರೇಖೀಯ, batch_normalize, adam);
    l.wh->batch = batch;

    l.batch_normalize = batch_normalize;


    l.outputs = outputs;
    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs*batch*ಹೆಜ್ಜೆಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.delta = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs*batch*ಹೆಜ್ಜೆಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.state = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.prev_state = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.forgot_state = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.forgot_delta = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.r_cpu = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.z_cpu = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.h_cpu = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.forward = forward_gru_layer;
    l.backward = backward_gru_layer;
    l.update = update_gru_layer;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_gru_layer_gpu;
    l.backward_gpu = backward_gru_layer_gpu;
    l.update_gpu = update_gru_layer_gpu;

    l.forgot_state_gpu = cuda_make_array(0, batch*outputs);
    l.forgot_delta_gpu = cuda_make_array(0, batch*outputs);
    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = cuda_make_array(0, batch*outputs*ಹೆಜ್ಜೆಗಳು);
    l.delta_gpu = cuda_make_array(0, batch*outputs*ಹೆಜ್ಜೆಗಳು);
    l.r_gpu = cuda_make_array(0, batch*outputs);
    l.z_gpu = cuda_make_array(0, batch*outputs);
    l.h_gpu = cuda_make_array(0, batch*outputs);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CUDNN
    cudnn_ಗಣ_Tensor4d_Descriptor(l.uz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.uz->ಹೊರಹೋಗು_c, l.uz->ಹೊರಹೋಗು_h, l.uz->ಹೊರಹೋಗು_w); 
    cudnn_ಗಣ_Tensor4d_Descriptor(l.uh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.uh->ಹೊರಹೋಗು_c, l.uh->ಹೊರಹೋಗು_h, l.uh->ಹೊರಹೋಗು_w); 
    cudnn_ಗಣ_Tensor4d_Descriptor(l.ur->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.ur->ಹೊರಹೋಗು_c, l.ur->ಹೊರಹೋಗು_h, l.ur->ಹೊರಹೋಗು_w); 
    cudnn_ಗಣ_Tensor4d_Descriptor(l.wz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.wz->ಹೊರಹೋಗು_c, l.wz->ಹೊರಹೋಗು_h, l.wz->ಹೊರಹೋಗು_w); 
    cudnn_ಗಣ_Tensor4d_Descriptor(l.wh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.wh->ಹೊರಹೋಗು_c, l.wh->ಹೊರಹೋಗು_h, l.wh->ಹೊರಹೋಗು_w); 
    cudnn_ಗಣ_Tensor4d_Descriptor(l.wr->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.wr->ಹೊರಹೋಗು_c, l.wr->ಹೊರಹೋಗು_h, l.wr->ಹೊರಹೋಗು_w); 
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ update_gru_layer(layer l, update_ವಾದಗಳು a)
{
    update_connected_layer(*(l.ur), a);
    update_connected_layer(*(l.uz), a);
    update_connected_layer(*(l.uh), a);
    update_connected_layer(*(l.wr), a);
    update_connected_layer(*(l.wz), a);
    update_connected_layer(*(l.wh), a);
}

ಶೂನ್ಯ forward_gru_layer(layer l, ಜಾಲ net)
{
    ಜಾಲ s = net;
    s.train = net.train;
    ಪೂರ್ಣಾಂಕ i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, uz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, ur.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, uh.delta, 1);

    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, wz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, wr.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, wh.delta, 1);
    ಆದರೆ(net.train) {
        fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, l.delta, 1);
        copy_cpu(l.outputs*l.batch, l.state, 1, l.prev_state, 1);
    }

    ಗಾಗಿ (i = 0; i < l.ಹೆಜ್ಜೆಗಳು; ++i) {
        s.ಒಳಹರಿವು = l.state;
        forward_connected_layer(wz, s);
        forward_connected_layer(wr, s);

        s.ಒಳಹರಿವು = net.ಒಳಹರಿವು;
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

        s.ಒಳಹರಿವು = l.forgot_state;
        forward_connected_layer(wh, s);

        copy_cpu(l.outputs*l.batch, uh.output, 1, l.h_cpu, 1);
        axpy_cpu(l.outputs*l.batch, 1, wh.output, 1, l.h_cpu, 1);

        ಆದರೆ(l.tanh){
            activate_array(l.h_cpu, l.outputs*l.batch, TANH);
        } ಇಲ್ಲದಿದ್ದರೆ {
            activate_array(l.h_cpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_ಮೊತ್ತ_cpu(l.state, l.h_cpu, l.z_cpu, l.outputs*l.batch, l.output);

        copy_cpu(l.outputs*l.batch, l.output, 1, l.state, 1);

        net.ಒಳಹರಿವು += l.inputs*l.batch;
        l.output += l.outputs*l.batch;
        increment_layer(&uz, 1);
        increment_layer(&ur, 1);
        increment_layer(&uh, 1);

        increment_layer(&wz, 1);
        increment_layer(&wr, 1);
        increment_layer(&wh, 1);
    }
}

ಶೂನ್ಯ backward_gru_layer(layer l, ಜಾಲ net)
{
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

ಶೂನ್ಯ pull_gru_layer(layer l)
{
}

ಶೂನ್ಯ ತಳ್ಳು_gru_layer(layer l)
{
}

ಶೂನ್ಯ update_gru_layer_gpu(layer l, update_ವಾದಗಳು a)
{
    update_connected_layer_gpu(*(l.ur), a);
    update_connected_layer_gpu(*(l.uz), a);
    update_connected_layer_gpu(*(l.uh), a);
    update_connected_layer_gpu(*(l.wr), a);
    update_connected_layer_gpu(*(l.wz), a);
    update_connected_layer_gpu(*(l.wh), a);
}

ಶೂನ್ಯ forward_gru_layer_gpu(layer l, ಜಾಲ net)
{
    ಜಾಲ s = {0};
    s.train = net.train;
    ಪೂರ್ಣಾಂಕ i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, uz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, ur.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, uh.delta_gpu, 1);

    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, wz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, wr.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, wh.delta_gpu, 1);
    ಆದರೆ(net.train) {
        fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    ಗಾಗಿ (i = 0; i < l.ಹೆಜ್ಜೆಗಳು; ++i) {
        s.ಒಳಹರಿವು_gpu = l.state_gpu;
        forward_connected_layer_gpu(wz, s);
        forward_connected_layer_gpu(wr, s);

        s.ಒಳಹರಿವು_gpu = net.ಒಳಹರಿವು_gpu;
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

        s.ಒಳಹರಿವು_gpu = l.forgot_state_gpu;
        forward_connected_layer_gpu(wh, s);

        copy_gpu(l.outputs*l.batch, uh.output_gpu, 1, l.h_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wh.output_gpu, 1, l.h_gpu, 1);

        ಆದರೆ(l.tanh){
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);
        } ಇಲ್ಲದಿದ್ದರೆ {
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_ಮೊತ್ತ_gpu(l.state_gpu, l.h_gpu, l.z_gpu, l.outputs*l.batch, l.output_gpu);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.state_gpu, 1);

        net.ಒಳಹರಿವು_gpu += l.inputs*l.batch;
        l.output_gpu += l.outputs*l.batch;
        increment_layer(&uz, 1);
        increment_layer(&ur, 1);
        increment_layer(&uh, 1);

        increment_layer(&wz, 1);
        increment_layer(&wr, 1);
        increment_layer(&wh, 1);
    }
}

ಶೂನ್ಯ backward_gru_layer_gpu(layer l, ಜಾಲ net)
{
    ಜಾಲ s = {0};
    s.train = net.train;
    ಪೂರ್ಣಾಂಕ i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    increment_layer(&uz, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&ur, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&uh, l.ಹೆಜ್ಜೆಗಳು - 1);

    increment_layer(&wz, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&wr, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&wh, l.ಹೆಜ್ಜೆಗಳು - 1);

    net.ಒಳಹರಿವು_gpu += l.inputs*l.batch*(l.ಹೆಜ್ಜೆಗಳು-1);
    ಆದರೆ(net.delta_gpu) net.delta_gpu += l.inputs*l.batch*(l.ಹೆಜ್ಜೆಗಳು-1);
    l.output_gpu += l.outputs*l.batch*(l.ಹೆಜ್ಜೆಗಳು-1);
    l.delta_gpu += l.outputs*l.batch*(l.ಹೆಜ್ಜೆಗಳು-1);
    ದಶಮಾಂಶ *ಕೊನೆ_state = l.output_gpu;
    ಗಾಗಿ (i = l.ಹೆಜ್ಜೆಗಳು-1; i >= 0; --i) {
        ಆದರೆ(i != 0) copy_gpu(l.outputs*l.batch, l.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        ಇಲ್ಲದಿದ್ದರೆ copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        ದಶಮಾಂಶ *prev_delta_gpu = (i == 0) ? 0 : l.delta_gpu - l.outputs*l.batch;

        copy_gpu(l.outputs*l.batch, uz.output_gpu, 1, l.z_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wz.output_gpu, 1, l.z_gpu, 1);

        copy_gpu(l.outputs*l.batch, ur.output_gpu, 1, l.r_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wr.output_gpu, 1, l.r_gpu, 1);

        activate_array_gpu(l.z_gpu, l.outputs*l.batch, LOGISTIC);
        activate_array_gpu(l.r_gpu, l.outputs*l.batch, LOGISTIC);

        copy_gpu(l.outputs*l.batch, uh.output_gpu, 1, l.h_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wh.output_gpu, 1, l.h_gpu, 1);

        ಆದರೆ(l.tanh){
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);
        } ಇಲ್ಲದಿದ್ದರೆ {
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_delta_gpu(l.state_gpu, l.h_gpu, l.z_gpu, prev_delta_gpu, uh.delta_gpu, uz.delta_gpu, l.outputs*l.batch, l.delta_gpu);

        ಆದರೆ(l.tanh){
            gradient_array_gpu(l.h_gpu, l.outputs*l.batch, TANH, uh.delta_gpu);
        } ಇಲ್ಲದಿದ್ದರೆ {
            gradient_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC, uh.delta_gpu);
        }

        copy_gpu(l.outputs*l.batch, uh.delta_gpu, 1, wh.delta_gpu, 1);

        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.forgot_state_gpu, 1);
        mul_gpu(l.outputs*l.batch, l.r_gpu, 1, l.forgot_state_gpu, 1);
        fill_gpu(l.outputs*l.batch, 0, l.forgot_delta_gpu, 1);

        s.ಒಳಹರಿವು_gpu = l.forgot_state_gpu;
        s.delta_gpu = l.forgot_delta_gpu;

        backward_connected_layer_gpu(wh, s);
        ಆದರೆ(prev_delta_gpu) mult_ಕೂಡಿಸು_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.r_gpu, prev_delta_gpu);
        mult_ಕೂಡಿಸು_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.state_gpu, ur.delta_gpu);

        gradient_array_gpu(l.r_gpu, l.outputs*l.batch, LOGISTIC, ur.delta_gpu);
        copy_gpu(l.outputs*l.batch, ur.delta_gpu, 1, wr.delta_gpu, 1);

        gradient_array_gpu(l.z_gpu, l.outputs*l.batch, LOGISTIC, uz.delta_gpu);
        copy_gpu(l.outputs*l.batch, uz.delta_gpu, 1, wz.delta_gpu, 1);

        s.ಒಳಹರಿವು_gpu = l.state_gpu;
        s.delta_gpu = prev_delta_gpu;

        backward_connected_layer_gpu(wr, s);
        backward_connected_layer_gpu(wz, s);

        s.ಒಳಹರಿವು_gpu = net.ಒಳಹರಿವು_gpu;
        s.delta_gpu = net.delta_gpu;

        backward_connected_layer_gpu(uh, s);
        backward_connected_layer_gpu(ur, s);
        backward_connected_layer_gpu(uz, s);


        net.ಒಳಹರಿವು_gpu -= l.inputs*l.batch;
        ಆದರೆ(net.delta_gpu) net.delta_gpu -= l.inputs*l.batch;
        l.output_gpu -= l.outputs*l.batch;
        l.delta_gpu -= l.outputs*l.batch;
        increment_layer(&uz, -1);
        increment_layer(&ur, -1);
        increment_layer(&uh, -1);

        increment_layer(&wz, -1);
        increment_layer(&wr, -1);
        increment_layer(&wh, -1);
    }
    copy_gpu(l.outputs*l.batch, ಕೊನೆ_state, 1, l.state_gpu, 1);
}
#ಆದರೆ_ಅಂತ್ಯ
