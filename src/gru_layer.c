#समावेश "gru_layer.h"
#समावेश "connected_layer.h"
#समावेश "utils.h"
#समावेश "cuda.h"
#समावेश "blas.h"
#समावेश "gemm.h"

#समावेश <math.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <string.h>

अटल व्योम increment_layer(layer *l, पूर्णांक steps)
{
    पूर्णांक अंक = l->outputs*l->batch*steps;
    l->output += अंक;
    l->delta += अंक;
    l->x += अंक;
    l->x_norm += अंक;

#अगर_घोषित GPU
    l->output_gpu += अंक;
    l->delta_gpu += अंक;
    l->x_gpu += अंक;
    l->x_norm_gpu += अंक;
#पूर्ण_अगर
}

layer make_gru_layer(पूर्णांक batch, पूर्णांक inputs, पूर्णांक outputs, पूर्णांक steps, पूर्णांक batch_normalize, पूर्णांक adam)
{
    ख_लिखो(मानक_त्रुटि, "GRU Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / steps;
    layer l = {0};
    l.batch = batch;
    l.type = GRU;
    l.steps = steps;
    l.inputs = inputs;

    l.uz = दो_स्मृति(माप(layer));
    ख_लिखो(मानक_त्रुटि, "\t\t");
    *(l.uz) = make_connected_layer(batch*steps, inputs, outputs, LINEAR, batch_normalize, adam);
    l.uz->batch = batch;

    l.wz = दो_स्मृति(माप(layer));
    ख_लिखो(मानक_त्रुटि, "\t\t");
    *(l.wz) = make_connected_layer(batch*steps, outputs, outputs, LINEAR, batch_normalize, adam);
    l.wz->batch = batch;

    l.ur = दो_स्मृति(माप(layer));
    ख_लिखो(मानक_त्रुटि, "\t\t");
    *(l.ur) = make_connected_layer(batch*steps, inputs, outputs, LINEAR, batch_normalize, adam);
    l.ur->batch = batch;

    l.wr = दो_स्मृति(माप(layer));
    ख_लिखो(मानक_त्रुटि, "\t\t");
    *(l.wr) = make_connected_layer(batch*steps, outputs, outputs, LINEAR, batch_normalize, adam);
    l.wr->batch = batch;



    l.uh = दो_स्मृति(माप(layer));
    ख_लिखो(मानक_त्रुटि, "\t\t");
    *(l.uh) = make_connected_layer(batch*steps, inputs, outputs, LINEAR, batch_normalize, adam);
    l.uh->batch = batch;

    l.wh = दो_स्मृति(माप(layer));
    ख_लिखो(मानक_त्रुटि, "\t\t");
    *(l.wh) = make_connected_layer(batch*steps, outputs, outputs, LINEAR, batch_normalize, adam);
    l.wh->batch = batch;

    l.batch_normalize = batch_normalize;


    l.outputs = outputs;
    l.output = सुस्मृति(outputs*batch*steps, माप(भग्न));
    l.delta = सुस्मृति(outputs*batch*steps, माप(भग्न));
    l.state = सुस्मृति(outputs*batch, माप(भग्न));
    l.पूर्व_state = सुस्मृति(outputs*batch, माप(भग्न));
    l.forgot_state = सुस्मृति(outputs*batch, माप(भग्न));
    l.forgot_delta = सुस्मृति(outputs*batch, माप(भग्न));

    l.r_cpu = सुस्मृति(outputs*batch, माप(भग्न));
    l.z_cpu = सुस्मृति(outputs*batch, माप(भग्न));
    l.h_cpu = सुस्मृति(outputs*batch, माप(भग्न));

    l.forward = forward_gru_layer;
    l.backward = backward_gru_layer;
    l.update = update_gru_layer;

#अगर_घोषित GPU
    l.forward_gpu = forward_gru_layer_gpu;
    l.backward_gpu = backward_gru_layer_gpu;
    l.update_gpu = update_gru_layer_gpu;

    l.forgot_state_gpu = cuda_make_array(0, batch*outputs);
    l.forgot_delta_gpu = cuda_make_array(0, batch*outputs);
    l.पूर्व_state_gpu = cuda_make_array(0, batch*outputs);
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = cuda_make_array(0, batch*outputs*steps);
    l.delta_gpu = cuda_make_array(0, batch*outputs*steps);
    l.r_gpu = cuda_make_array(0, batch*outputs);
    l.z_gpu = cuda_make_array(0, batch*outputs);
    l.h_gpu = cuda_make_array(0, batch*outputs);

#अगर_घोषित CUDNN
    cudnnSetTensor4dDescriptor(l.uz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, batch, l.uz->out_c, l.uz->out_h, l.uz->out_w); 
    cudnnSetTensor4dDescriptor(l.uh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, batch, l.uh->out_c, l.uh->out_h, l.uh->out_w); 
    cudnnSetTensor4dDescriptor(l.ur->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, batch, l.ur->out_c, l.ur->out_h, l.ur->out_w); 
    cudnnSetTensor4dDescriptor(l.wz->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, batch, l.wz->out_c, l.wz->out_h, l.wz->out_w); 
    cudnnSetTensor4dDescriptor(l.wh->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, batch, l.wh->out_c, l.wh->out_h, l.wh->out_w); 
    cudnnSetTensor4dDescriptor(l.wr->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, batch, l.wr->out_c, l.wr->out_h, l.wr->out_w); 
#पूर्ण_अगर
#पूर्ण_अगर

    वापस l;
}

व्योम update_gru_layer(layer l, update_args a)
{
    update_connected_layer(*(l.ur), a);
    update_connected_layer(*(l.uz), a);
    update_connected_layer(*(l.uh), a);
    update_connected_layer(*(l.wr), a);
    update_connected_layer(*(l.wz), a);
    update_connected_layer(*(l.wh), a);
}

व्योम forward_gru_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    पूर्णांक i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_cpu(l.outputs * l.batch * l.steps, 0, uz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, ur.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, uh.delta, 1);

    fill_cpu(l.outputs * l.batch * l.steps, 0, wz.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, wr.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, wh.delta, 1);
    अगर(net.train) {
        fill_cpu(l.outputs * l.batch * l.steps, 0, l.delta, 1);
        copy_cpu(l.outputs*l.batch, l.state, 1, l.पूर्व_state, 1);
    }

    क्रम (i = 0; i < l.steps; ++i) {
        s.निवेश = l.state;
        forward_connected_layer(wz, s);
        forward_connected_layer(wr, s);

        s.निवेश = net.निवेश;
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

        s.निवेश = l.forgot_state;
        forward_connected_layer(wh, s);

        copy_cpu(l.outputs*l.batch, uh.output, 1, l.h_cpu, 1);
        axpy_cpu(l.outputs*l.batch, 1, wh.output, 1, l.h_cpu, 1);

        अगर(l.tanh){
            activate_array(l.h_cpu, l.outputs*l.batch, TANH);
        } अन्यथा {
            activate_array(l.h_cpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_योग_cpu(l.state, l.h_cpu, l.z_cpu, l.outputs*l.batch, l.output);

        copy_cpu(l.outputs*l.batch, l.output, 1, l.state, 1);

        net.निवेश += l.inputs*l.batch;
        l.output += l.outputs*l.batch;
        increment_layer(&uz, 1);
        increment_layer(&ur, 1);
        increment_layer(&uh, 1);

        increment_layer(&wz, 1);
        increment_layer(&wr, 1);
        increment_layer(&wh, 1);
    }
}

व्योम backward_gru_layer(layer l, network net)
{
}

#अगर_घोषित GPU

व्योम pull_gru_layer(layer l)
{
}

व्योम धकेलो_gru_layer(layer l)
{
}

व्योम update_gru_layer_gpu(layer l, update_args a)
{
    update_connected_layer_gpu(*(l.ur), a);
    update_connected_layer_gpu(*(l.uz), a);
    update_connected_layer_gpu(*(l.uh), a);
    update_connected_layer_gpu(*(l.wr), a);
    update_connected_layer_gpu(*(l.wz), a);
    update_connected_layer_gpu(*(l.wh), a);
}

व्योम forward_gru_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    पूर्णांक i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    fill_gpu(l.outputs * l.batch * l.steps, 0, uz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, ur.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, uh.delta_gpu, 1);

    fill_gpu(l.outputs * l.batch * l.steps, 0, wz.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, wr.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, wh.delta_gpu, 1);
    अगर(net.train) {
        fill_gpu(l.outputs * l.batch * l.steps, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.पूर्व_state_gpu, 1);
    }

    क्रम (i = 0; i < l.steps; ++i) {
        s.निवेश_gpu = l.state_gpu;
        forward_connected_layer_gpu(wz, s);
        forward_connected_layer_gpu(wr, s);

        s.निवेश_gpu = net.निवेश_gpu;
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

        s.निवेश_gpu = l.forgot_state_gpu;
        forward_connected_layer_gpu(wh, s);

        copy_gpu(l.outputs*l.batch, uh.output_gpu, 1, l.h_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wh.output_gpu, 1, l.h_gpu, 1);

        अगर(l.tanh){
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);
        } अन्यथा {
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_योग_gpu(l.state_gpu, l.h_gpu, l.z_gpu, l.outputs*l.batch, l.output_gpu);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.state_gpu, 1);

        net.निवेश_gpu += l.inputs*l.batch;
        l.output_gpu += l.outputs*l.batch;
        increment_layer(&uz, 1);
        increment_layer(&ur, 1);
        increment_layer(&uh, 1);

        increment_layer(&wz, 1);
        increment_layer(&wr, 1);
        increment_layer(&wh, 1);
    }
}

व्योम backward_gru_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    पूर्णांक i;
    layer uz = *(l.uz);
    layer ur = *(l.ur);
    layer uh = *(l.uh);

    layer wz = *(l.wz);
    layer wr = *(l.wr);
    layer wh = *(l.wh);

    increment_layer(&uz, l.steps - 1);
    increment_layer(&ur, l.steps - 1);
    increment_layer(&uh, l.steps - 1);

    increment_layer(&wz, l.steps - 1);
    increment_layer(&wr, l.steps - 1);
    increment_layer(&wh, l.steps - 1);

    net.निवेश_gpu += l.inputs*l.batch*(l.steps-1);
    अगर(net.delta_gpu) net.delta_gpu += l.inputs*l.batch*(l.steps-1);
    l.output_gpu += l.outputs*l.batch*(l.steps-1);
    l.delta_gpu += l.outputs*l.batch*(l.steps-1);
    भग्न *end_state = l.output_gpu;
    क्रम (i = l.steps-1; i >= 0; --i) {
        अगर(i != 0) copy_gpu(l.outputs*l.batch, l.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        अन्यथा copy_gpu(l.outputs*l.batch, l.पूर्व_state_gpu, 1, l.state_gpu, 1);
        भग्न *पूर्व_delta_gpu = (i == 0) ? 0 : l.delta_gpu - l.outputs*l.batch;

        copy_gpu(l.outputs*l.batch, uz.output_gpu, 1, l.z_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wz.output_gpu, 1, l.z_gpu, 1);

        copy_gpu(l.outputs*l.batch, ur.output_gpu, 1, l.r_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wr.output_gpu, 1, l.r_gpu, 1);

        activate_array_gpu(l.z_gpu, l.outputs*l.batch, LOGISTIC);
        activate_array_gpu(l.r_gpu, l.outputs*l.batch, LOGISTIC);

        copy_gpu(l.outputs*l.batch, uh.output_gpu, 1, l.h_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, wh.output_gpu, 1, l.h_gpu, 1);

        अगर(l.tanh){
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);
        } अन्यथा {
            activate_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC);
        }

        weighted_delta_gpu(l.state_gpu, l.h_gpu, l.z_gpu, पूर्व_delta_gpu, uh.delta_gpu, uz.delta_gpu, l.outputs*l.batch, l.delta_gpu);

        अगर(l.tanh){
            gradient_array_gpu(l.h_gpu, l.outputs*l.batch, TANH, uh.delta_gpu);
        } अन्यथा {
            gradient_array_gpu(l.h_gpu, l.outputs*l.batch, LOGISTIC, uh.delta_gpu);
        }

        copy_gpu(l.outputs*l.batch, uh.delta_gpu, 1, wh.delta_gpu, 1);

        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.forgot_state_gpu, 1);
        mul_gpu(l.outputs*l.batch, l.r_gpu, 1, l.forgot_state_gpu, 1);
        fill_gpu(l.outputs*l.batch, 0, l.forgot_delta_gpu, 1);

        s.निवेश_gpu = l.forgot_state_gpu;
        s.delta_gpu = l.forgot_delta_gpu;

        backward_connected_layer_gpu(wh, s);
        अगर(पूर्व_delta_gpu) mult_add_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.r_gpu, पूर्व_delta_gpu);
        mult_add_into_gpu(l.outputs*l.batch, l.forgot_delta_gpu, l.state_gpu, ur.delta_gpu);

        gradient_array_gpu(l.r_gpu, l.outputs*l.batch, LOGISTIC, ur.delta_gpu);
        copy_gpu(l.outputs*l.batch, ur.delta_gpu, 1, wr.delta_gpu, 1);

        gradient_array_gpu(l.z_gpu, l.outputs*l.batch, LOGISTIC, uz.delta_gpu);
        copy_gpu(l.outputs*l.batch, uz.delta_gpu, 1, wz.delta_gpu, 1);

        s.निवेश_gpu = l.state_gpu;
        s.delta_gpu = पूर्व_delta_gpu;

        backward_connected_layer_gpu(wr, s);
        backward_connected_layer_gpu(wz, s);

        s.निवेश_gpu = net.निवेश_gpu;
        s.delta_gpu = net.delta_gpu;

        backward_connected_layer_gpu(uh, s);
        backward_connected_layer_gpu(ur, s);
        backward_connected_layer_gpu(uz, s);


        net.निवेश_gpu -= l.inputs*l.batch;
        अगर(net.delta_gpu) net.delta_gpu -= l.inputs*l.batch;
        l.output_gpu -= l.outputs*l.batch;
        l.delta_gpu -= l.outputs*l.batch;
        increment_layer(&uz, -1);
        increment_layer(&ur, -1);
        increment_layer(&uh, -1);

        increment_layer(&wz, -1);
        increment_layer(&wr, -1);
        increment_layer(&wh, -1);
    }
    copy_gpu(l.outputs*l.batch, end_state, 1, l.state_gpu, 1);
}
#पूर्ण_अगर
