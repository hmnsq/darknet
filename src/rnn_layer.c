#شامل "rnn_layer.h"
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

layer make_rnn_layer(صحیح batch, صحیح inputs, صحیح outputs, صحیح اقدام, ACTIVATION activation, صحیح batch_normalize, صحیح adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / اقدام;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.اقدام = اقدام;
    l.inputs = inputs;

    l.state = صفر_حافظہ_دو(batch*outputs, حجم(اعشاری));
    l.پچھلا_state = صفر_حافظہ_دو(batch*outputs, حجم(اعشاری));

    l.داخلہ_layer = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.داخلہ_layer) = make_connected_layer(batch*اقدام, inputs, outputs, activation, batch_normalize, adam);
    l.داخلہ_layer->batch = batch;

    l.self_layer = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*اقدام, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.output_layer) = make_connected_layer(batch*اقدام, outputs, outputs, activation, batch_normalize, adam);
    l.output_layer->batch = batch;

    l.outputs = outputs;
    l.output = l.output_layer->output;
    l.delta = l.output_layer->delta;

    l.forward = forward_rnn_layer;
    l.backward = backward_rnn_layer;
    l.update = update_rnn_layer;
#اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_rnn_layer_gpu;
    l.backward_gpu = backward_rnn_layer_gpu;
    l.update_gpu = update_rnn_layer_gpu;
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.پچھلا_state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = l.output_layer->output_gpu;
    l.delta_gpu = l.output_layer->delta_gpu;
#اگر_تعریف_شدہ CUDNN
    cudnn_سیٹ_Tensor4d_Descriptor(l.داخلہ_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.داخلہ_layer->برآمد_c, l.داخلہ_layer->برآمد_h, l.داخلہ_layer->برآمد_w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.self_layer->برآمد_c, l.self_layer->برآمد_h, l.self_layer->برآمد_w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.output_layer->برآمد_c, l.output_layer->برآمد_h, l.output_layer->برآمد_w); 
#اگر_ختم
#اگر_ختم

    واپس l;
}

خالی update_rnn_layer(layer l, update_دلائل a)
{
    update_connected_layer(*(l.داخلہ_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

خالی forward_rnn_layer(layer l, جال net)
{
    جال s = net;
    s.train = net.train;
    صحیح i;
    layer داخلہ_layer = *(l.داخلہ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.اقدام, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.اقدام, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.اقدام, 0, داخلہ_layer.delta, 1);
    اگر(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    برائے (i = 0; i < l.اقدام; ++i) {
        s.داخلہ = net.داخلہ;
        forward_connected_layer(داخلہ_layer, s);

        s.داخلہ = l.state;
        forward_connected_layer(self_layer, s);

        اعشاری *old_state = l.state;
        اگر(net.train) l.state += l.outputs*l.batch;
        اگر(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }ورنہ{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, داخلہ_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.داخلہ = l.state;
        forward_connected_layer(output_layer, s);

        net.داخلہ += l.inputs*l.batch;
        increment_layer(&داخلہ_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

خالی backward_rnn_layer(layer l, جال net)
{
    جال s = net;
    s.train = net.train;
    صحیح i;
    layer داخلہ_layer = *(l.داخلہ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&داخلہ_layer, l.اقدام-1);
    increment_layer(&self_layer, l.اقدام-1);
    increment_layer(&output_layer, l.اقدام-1);

    l.state += l.outputs*l.batch*l.اقدام;
    برائے (i = l.اقدام-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, داخلہ_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.داخلہ = l.state;
        s.delta = self_layer.delta;
        backward_connected_layer(output_layer, s);

        l.state -= l.outputs*l.batch;
        /*
           if(i > 0){
           copy_cpu(l.outputs * l.batch, input_layer.output - l.outputs*l.batch, 1, l.state, 1);
           axpy_cpu(l.outputs * l.batch, 1, self_layer.output - l.outputs*l.batch, 1, l.state, 1);
           }else{
           fill_cpu(l.outputs * l.batch, 0, l.state, 1);
           }
         */

        s.داخلہ = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        اگر (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, داخلہ_layer.delta, 1);
        اگر (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.داخلہ = net.داخلہ + i*l.inputs*l.batch;
        اگر(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        ورنہ s.delta = 0;
        backward_connected_layer(داخلہ_layer, s);

        increment_layer(&داخلہ_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#اگر_تعریف_شدہ GPU

خالی pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.داخلہ_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

خالی ڈالو_rnn_layer(layer l)
{
    ڈالو_connected_layer(*(l.داخلہ_layer));
    ڈالو_connected_layer(*(l.self_layer));
    ڈالو_connected_layer(*(l.output_layer));
}

خالی update_rnn_layer_gpu(layer l, update_دلائل a)
{
    update_connected_layer_gpu(*(l.داخلہ_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

خالی forward_rnn_layer_gpu(layer l, جال net)
{
    جال s = {0};
    s.train = net.train;
    صحیح i;
    layer داخلہ_layer = *(l.داخلہ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.اقدام, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.اقدام, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.اقدام, 0, داخلہ_layer.delta_gpu, 1);

    اگر(net.train) {
        fill_gpu(l.outputs * l.batch * l.اقدام, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.پچھلا_state_gpu, 1);
    }

    برائے (i = 0; i < l.اقدام; ++i) {
        s.داخلہ_gpu = net.داخلہ_gpu;
        forward_connected_layer_gpu(داخلہ_layer, s);

        s.داخلہ_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, داخلہ_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.داخلہ_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.داخلہ_gpu += l.inputs*l.batch;
        increment_layer(&داخلہ_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

خالی backward_rnn_layer_gpu(layer l, جال net)
{
    جال s = {0};
    s.train = net.train;
    صحیح i;
    layer داخلہ_layer = *(l.داخلہ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&داخلہ_layer,  l.اقدام - 1);
    increment_layer(&self_layer,   l.اقدام - 1);
    increment_layer(&output_layer, l.اقدام - 1);
    اعشاری *آخری_داخلہ = داخلہ_layer.output_gpu;
    اعشاری *آخری_self = self_layer.output_gpu;
    برائے (i = l.اقدام-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, داخلہ_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.داخلہ_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        اگر(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, داخلہ_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }ورنہ {
            copy_gpu(l.outputs*l.batch, l.پچھلا_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, داخلہ_layer.delta_gpu, 1);

        s.داخلہ_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        اگر (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.داخلہ_gpu = net.داخلہ_gpu + i*l.inputs*l.batch;
        اگر(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        ورنہ s.delta_gpu = 0;
        backward_connected_layer_gpu(داخلہ_layer, s);

        increment_layer(&داخلہ_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, آخری_داخلہ, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, آخری_self, 1, l.state_gpu, 1);
}
#اگر_ختم
