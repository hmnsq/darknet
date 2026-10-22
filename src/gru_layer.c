#అన్తర్భావ "gru_layer.h"
#అన్తర్భావ "connected_layer.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "gemm.h"

#అన్తర్భావ <math.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>

స్థిర శూన్య increment_layer(layer *l, పూర్ణాంకం అడుగులు)
{
    పూర్ణాంకం అంకె = l->outputs*l->batch*అడుగులు;
    l->output += అంకె;
    l->delta += అంకె;
    l->x += అంకె;
    l->x_norm += అంకె;

#నిర్వచితమైతే GPU
    l->output_gpu += అంకె;
    l->delta_gpu += అంకె;
    l->x_gpu += అంకె;
    l->x_norm_gpu += అంకె;
#అయితే_ముగింపు
}

layer make_gru_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, పూర్ణాంకం outputs, పూర్ణాంకం అడుగులు, పూర్ణాంకం batch_normalize, పూర్ణాంకం adam)
{
    fprintf(stderr, "GRU Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / అడుగులు;
    layer l = {0};
    l.batch = batch;
    l.type = GRU;
    l.అడుగులు = అడుగులు;
    l.inputs = inputs;

    l.uz = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.uz) = make_connected_layer(batch*అడుగులు, inputs, outputs, రేఖీయ, batch_normalize, adam);
    l.uz->batch = batch;

    l.wz = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.wz) = make_connected_layer(batch*అడుగులు, outputs, outputs, రేఖీయ, batch_normalize, adam);
    l.wz->batch = batch;

    l.ur = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.ur) = make_connected_layer(batch*అడుగులు, inputs, outputs, రేఖీయ, batch_normalize, adam);
    l.ur->batch = batch;

    l.wr = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.wr) = make_connected_layer(batch*అడుగులు, outputs, outputs, రేఖీయ, batch_normalize, adam);
    l.wr->batch = batch;



    l.uh = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.uh) = make_connected_layer(batch*అడుగులు, inputs, outputs, రేఖీయ, batch_normalize, adam);
    l.uh->batch = batch;

    l.wh = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.wh) = make_connected_layer(batch*అడుగులు, outputs, outputs, రేఖీయ, batch_normalize, adam);
    l.wh->batch = batch;

    l.batch_normalize = batch_normalize;


    l.outputs = outputs;
    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs*batch*అడుగులు, పరిమాణం(దశాంశం));
    l.delta = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs*batch*అడుగులు, పరిమాణం(దశాంశం));
    l.state = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs*batch, పరిమాణం(దశాంశం));
    l.prev_state = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs*batch, పరిమాణం(దశాంశం));
    l.forgot_state = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs*batch, పరిమాణం(దశాంశం));
    l.forgot_delta = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs*batch, పరిమాణం(దశాంశం));

    l.r_cpu = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs*batch, పరిమాణం(దశాంశం));
    l.z_cpu = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs*batch, పరిమాణం(దశాంశం));
    l.h_cpu = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs*batch, పరిమాణం(దశాంశం));

    l.forward = forward_gru_layer;
    l.backward = backward_gru_layer;
    l.update = update_gru_layer;

#నిర్వచితమైతే GPU
    l.forward_gpu = forward_gru_layer_gpu;
    l.backward_gpu = backward_gru_layer_gpu;
    l.update_gpu = update_gru_layer_gpu;

    l.forgot_state_gpu = cuda_make_array(0, batch*outputs);
    l.forgot_delta_gpu = cuda_make_array(0, batch*outputs);
    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = cuda_make_array(0, batch*outputs*అడుగులు);
    l.delta_gpu = cuda_make_array(0, batch*outputs*అడుగులు);
    l.r_gpu = cuda_make_array(0, batch*outputs);
    l.z_gpu = cuda_make_array(0, batch*outputs);
    l.h_gpu = cuda_make_array(0, batch*outputs);

#నిర్వచితమైతే CUDNN
    cudnn_సమితి_Tensor4d_Descriptor(l.uz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.uz->బహిర్గతం_c, l.uz->బహిర్గతం_h, l.uz->బహిర్గతం_w); 
    cudnn_సమితి_Tensor4d_Descriptor(l.uh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.uh->బహిర్గతం_c, l.uh->బహిర్గతం_h, l.uh->బహిర్గతం_w); 
    cudnn_సమితి_Tensor4d_Descriptor(l.ur->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.ur->బహిర్గతం_c, l.ur->బహిర్గతం_h, l.ur->బహిర్గతం_w); 
    cudnn_సమితి_Tensor4d_Descriptor(l.wz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.wz->బహిర్గతం_c, l.wz->బహిర్గతం_h, l.wz->బహిర్గతం_w); 
    cudnn_సమితి_Tensor4d_Descriptor(l.wh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.wh->బహిర్గతం_c, l.wh->బహిర్గతం_h, l.wh->బహిర్గతం_w); 
    cudnn_సమితి_Tensor4d_Descriptor(l.wr->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.wr->బహిర్గతం_c, l.wr->బహిర్గతం_h, l.wr->బహిర్గతం_w); 
#అయితే_ముగింపు
#అయితే_ముగింపు

    తిరిగి l;
}

శూన్య update_gru_layer(layer l, update_వాదనలు a)
{
    update_connected_layer(*(l.ur), a);
    update_connected_layer(*(l.uz), a);
    update_connected_layer(*(l.uh), a);
    update_connected_layer(*(l.wr), a);
    update_connected_layer(*(l.wz), a);
    update_connected_layer(*(l.wh), a);
}

శూన్య forward_gru_layer(layer l, జాలం net)
{
    జాలం s = net;
    s.train = net.train;
    పూర్ణాంకం i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, uz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, ur.delta, 1);
    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, uh.delta, 1);

    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, wz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, wr.delta, 1);
    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, wh.delta, 1);
    ఒకవేళ(net.train) {
        fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, l.delta, 1);
        copy_cpu(l.outputs*l.batch, l.state, 1, l.prev_state, 1);
    }

    కొరకు (i = 0; i < l.అడుగులు; ++i) {
        s.ఆదానం = l.state;
        forward_connected_layer(wz, s);
        forward_connected_layer(wr, s);

        s.ఆదానం = net.ఆదానం;
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

        s.ఆదానం = l.forgot_state;
        forward_connected_layer(wh, s);

        copy_cpu(l.outputs*l.batch, uh.output, 1, l.h_cpu, 1);
        axpy_cpu(l.outputs*l.batch, 1, wh.output, 1, l.h_cpu, 1);

        ఒకవేళ(l.tanh){
            activate_array(l.h_cpu, l.outputs*l.batch, TANH);
        } లేకపోతే {
            activate_array(l.h_cpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_కూడిక_cpu(l.state, l.h_cpu, l.z_cpu, l.outputs*l.batch, l.output);

        copy_cpu(l.outputs*l.batch, l.output, 1, l.state, 1);

        net.ఆదానం += l.inputs*l.batch;
        l.output += l.outputs*l.batch;
        increment_layer(&uz, 1);
        increment_layer(&ur, 1);
        increment_layer(&uh, 1);

        increment_layer(&wz, 1);
        increment_layer(&wr, 1);
        increment_layer(&wh, 1);
    }
}

శూన్య backward_gru_layer(layer l, జాలం net)
{
}

#నిర్వచితమైతే GPU

శూన్య pull_gru_layer(layer l)
{
}

శూన్య నెట్టు_gru_layer(layer l)
{
}

శూన్య update_gru_layer_gpu(layer l, update_వాదనలు a)
{
    update_connected_layer_gpu(*(l.ur), a);
    update_connected_layer_gpu(*(l.uz), a);
    update_connected_layer_gpu(*(l.uh), a);
    update_connected_layer_gpu(*(l.wr), a);
    update_connected_layer_gpu(*(l.wz), a);
    update_connected_layer_gpu(*(l.wh), a);
}

శూన్య forward_gru_layer_gpu(layer l, జాలం net)
{
    జాలం s = {0};
    s.train = net.train;
    పూర్ణాంకం i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, uz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, ur.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, uh.delta_gpu, 1);

    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, wz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, wr.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, wh.delta_gpu, 1);
    ఒకవేళ(net.train) {
        fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    కొరకు (i = 0; i < l.అడుగులు; ++i) {
        s.ఆదానం_gpu = l.state_gpu;
        forward_connected_layer_gpu(wz, s);
        forward_connected_layer_gpu(wr, s);

        s.ఆదానం_gpu = net.ఆదానం_gpu;
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

        s.ఆదానం_gpu = l.forgot_state_gpu;
        forward_connected_layer_gpu(wh, s);

        copy_gpu(l.outputs*l.batch, uh.output_gpu, 1, l.h_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wh.output_gpu, 1, l.h_gpu, 1);

        ఒకవేళ(l.tanh){
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);
        } లేకపోతే {
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_కూడిక_gpu(l.state_gpu, l.h_gpu, l.z_gpu, l.outputs*l.batch, l.output_gpu);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.state_gpu, 1);

        net.ఆదానం_gpu += l.inputs*l.batch;
        l.output_gpu += l.outputs*l.batch;
        increment_layer(&uz, 1);
        increment_layer(&ur, 1);
        increment_layer(&uh, 1);

        increment_layer(&wz, 1);
        increment_layer(&wr, 1);
        increment_layer(&wh, 1);
    }
}

శూన్య backward_gru_layer_gpu(layer l, జాలం net)
{
    జాలం s = {0};
    s.train = net.train;
    పూర్ణాంకం i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    increment_layer(&uz, l.అడుగులు - 1);
    increment_layer(&ur, l.అడుగులు - 1);
    increment_layer(&uh, l.అడుగులు - 1);

    increment_layer(&wz, l.అడుగులు - 1);
    increment_layer(&wr, l.అడుగులు - 1);
    increment_layer(&wh, l.అడుగులు - 1);

    net.ఆదానం_gpu += l.inputs*l.batch*(l.అడుగులు-1);
    ఒకవేళ(net.delta_gpu) net.delta_gpu += l.inputs*l.batch*(l.అడుగులు-1);
    l.output_gpu += l.outputs*l.batch*(l.అడుగులు-1);
    l.delta_gpu += l.outputs*l.batch*(l.అడుగులు-1);
    దశాంశం *చివర_state = l.output_gpu;
    కొరకు (i = l.అడుగులు-1; i >= 0; --i) {
        ఒకవేళ(i != 0) copy_gpu(l.outputs*l.batch, l.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        లేకపోతే copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        దశాంశం *prev_delta_gpu = (i == 0) ? 0 : l.delta_gpu - l.outputs*l.batch;

        copy_gpu(l.outputs*l.batch, uz.output_gpu, 1, l.z_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wz.output_gpu, 1, l.z_gpu, 1);

        copy_gpu(l.outputs*l.batch, ur.output_gpu, 1, l.r_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wr.output_gpu, 1, l.r_gpu, 1);

        activate_array_gpu(l.z_gpu, l.outputs*l.batch, LOGISTIC);
        activate_array_gpu(l.r_gpu, l.outputs*l.batch, LOGISTIC);

        copy_gpu(l.outputs*l.batch, uh.output_gpu, 1, l.h_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wh.output_gpu, 1, l.h_gpu, 1);

        ఒకవేళ(l.tanh){
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);
        } లేకపోతే {
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_delta_gpu(l.state_gpu, l.h_gpu, l.z_gpu, prev_delta_gpu, uh.delta_gpu, uz.delta_gpu, l.outputs*l.batch, l.delta_gpu);

        ఒకవేళ(l.tanh){
            gradient_array_gpu(l.h_gpu, l.outputs*l.batch, TANH, uh.delta_gpu);
        } లేకపోతే {
            gradient_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC, uh.delta_gpu);
        }

        copy_gpu(l.outputs*l.batch, uh.delta_gpu, 1, wh.delta_gpu, 1);

        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.forgot_state_gpu, 1);
        mul_gpu(l.outputs*l.batch, l.r_gpu, 1, l.forgot_state_gpu, 1);
        fill_gpu(l.outputs*l.batch, 0, l.forgot_delta_gpu, 1);

        s.ఆదానం_gpu = l.forgot_state_gpu;
        s.delta_gpu = l.forgot_delta_gpu;

        backward_connected_layer_gpu(wh, s);
        ఒకవేళ(prev_delta_gpu) mult_కూడు_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.r_gpu, prev_delta_gpu);
        mult_కూడు_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.state_gpu, ur.delta_gpu);

        gradient_array_gpu(l.r_gpu, l.outputs*l.batch, LOGISTIC, ur.delta_gpu);
        copy_gpu(l.outputs*l.batch, ur.delta_gpu, 1, wr.delta_gpu, 1);

        gradient_array_gpu(l.z_gpu, l.outputs*l.batch, LOGISTIC, uz.delta_gpu);
        copy_gpu(l.outputs*l.batch, uz.delta_gpu, 1, wz.delta_gpu, 1);

        s.ఆదానం_gpu = l.state_gpu;
        s.delta_gpu = prev_delta_gpu;

        backward_connected_layer_gpu(wr, s);
        backward_connected_layer_gpu(wz, s);

        s.ఆదానం_gpu = net.ఆదానం_gpu;
        s.delta_gpu = net.delta_gpu;

        backward_connected_layer_gpu(uh, s);
        backward_connected_layer_gpu(ur, s);
        backward_connected_layer_gpu(uz, s);


        net.ఆదానం_gpu -= l.inputs*l.batch;
        ఒకవేళ(net.delta_gpu) net.delta_gpu -= l.inputs*l.batch;
        l.output_gpu -= l.outputs*l.batch;
        l.delta_gpu -= l.outputs*l.batch;
        increment_layer(&uz, -1);
        increment_layer(&ur, -1);
        increment_layer(&uh, -1);

        increment_layer(&wz, -1);
        increment_layer(&wr, -1);
        increment_layer(&wh, -1);
    }
    copy_gpu(l.outputs*l.batch, చివర_state, 1, l.state_gpu, 1);
}
#అయితే_ముగింపు
