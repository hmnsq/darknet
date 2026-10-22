#ਅਨ੍ਤਰ੍ਭਾਵ "rnn_layer.h"
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

layer make_rnn_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਪੂਰਨ outputs, ਪੂਰਨ ਕਦਮਾਂ, ACTIVATION activation, ਪੂਰਨ batch_normalize, ਪੂਰਨ adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / ਕਦਮਾਂ;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.ਕਦਮਾਂ = ਕਦਮਾਂ;
    l.inputs = inputs;

    l.state = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.prev_state = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.ਨਿਵੇਸ਼_layer = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.ਨਿਵੇਸ਼_layer) = make_connected_layer(batch*ਕਦਮਾਂ, inputs, outputs, activation, batch_normalize, adam);
    l.ਨਿਵੇਸ਼_layer->batch = batch;

    l.self_layer = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*ਕਦਮਾਂ, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.output_layer) = make_connected_layer(batch*ਕਦਮਾਂ, outputs, outputs, activation, batch_normalize, adam);
    l.output_layer->batch = batch;

    l.outputs = outputs;
    l.output = l.output_layer->output;
    l.delta = l.output_layer->delta;

    l.forward = forward_rnn_layer;
    l.backward = backward_rnn_layer;
    l.update = update_rnn_layer;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_rnn_layer_gpu;
    l.backward_gpu = backward_rnn_layer_gpu;
    l.update_gpu = update_rnn_layer_gpu;
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = l.output_layer->output_gpu;
    l.delta_gpu = l.output_layer->delta_gpu;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.ਨਿਵੇਸ਼_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.ਨਿਵੇਸ਼_layer->ਬਾਹਰਲਾ_c, l.ਨਿਵੇਸ਼_layer->ਬਾਹਰਲਾ_h, l.ਨਿਵੇਸ਼_layer->ਬਾਹਰਲਾ_w); 
    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.self_layer->ਬਾਹਰਲਾ_c, l.self_layer->ਬਾਹਰਲਾ_h, l.self_layer->ਬਾਹਰਲਾ_w); 
    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.output_layer->ਬਾਹਰਲਾ_c, l.output_layer->ਬਾਹਰਲਾ_h, l.output_layer->ਬਾਹਰਲਾ_w); 
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

    ਵਾਪਸ l;
}

ਖਾਲੀ update_rnn_layer(layer l, update_ਦਲੀਲਾਂ a)
{
    update_connected_layer(*(l.ਨਿਵੇਸ਼_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

ਖਾਲੀ forward_rnn_layer(layer l, ਜਾਲ net)
{
    ਜਾਲ s = net;
    s.train = net.train;
    ਪੂਰਨ i;
    layer ਨਿਵੇਸ਼_layer = *(l.ਨਿਵੇਸ਼_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, ਨਿਵੇਸ਼_layer.delta, 1);
    ਜੇ(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    ਲਈ (i = 0; i < l.ਕਦਮਾਂ; ++i) {
        s.ਨਿਵੇਸ਼ = net.ਨਿਵੇਸ਼;
        forward_connected_layer(ਨਿਵੇਸ਼_layer, s);

        s.ਨਿਵੇਸ਼ = l.state;
        forward_connected_layer(self_layer, s);

        ਦਸ਼ਮਲਵ *old_state = l.state;
        ਜੇ(net.train) l.state += l.outputs*l.batch;
        ਜੇ(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }ਨਹੀਂਤਾਂ{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, ਨਿਵੇਸ਼_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ਨਿਵੇਸ਼ = l.state;
        forward_connected_layer(output_layer, s);

        net.ਨਿਵੇਸ਼ += l.inputs*l.batch;
        increment_layer(&ਨਿਵੇਸ਼_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ਖਾਲੀ backward_rnn_layer(layer l, ਜਾਲ net)
{
    ਜਾਲ s = net;
    s.train = net.train;
    ਪੂਰਨ i;
    layer ਨਿਵੇਸ਼_layer = *(l.ਨਿਵੇਸ਼_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&ਨਿਵੇਸ਼_layer, l.ਕਦਮਾਂ-1);
    increment_layer(&self_layer, l.ਕਦਮਾਂ-1);
    increment_layer(&output_layer, l.ਕਦਮਾਂ-1);

    l.state += l.outputs*l.batch*l.ਕਦਮਾਂ;
    ਲਈ (i = l.ਕਦਮਾਂ-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, ਨਿਵੇਸ਼_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ਨਿਵੇਸ਼ = l.state;
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

        s.ਨਿਵੇਸ਼ = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        ਜੇ (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, ਨਿਵੇਸ਼_layer.delta, 1);
        ਜੇ (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.ਨਿਵੇਸ਼ = net.ਨਿਵੇਸ਼ + i*l.inputs*l.batch;
        ਜੇ(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        ਨਹੀਂਤਾਂ s.delta = 0;
        backward_connected_layer(ਨਿਵੇਸ਼_layer, s);

        increment_layer(&ਨਿਵੇਸ਼_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.ਨਿਵੇਸ਼_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

ਖਾਲੀ ਧੱਕੋ_rnn_layer(layer l)
{
    ਧੱਕੋ_connected_layer(*(l.ਨਿਵੇਸ਼_layer));
    ਧੱਕੋ_connected_layer(*(l.self_layer));
    ਧੱਕੋ_connected_layer(*(l.output_layer));
}

ਖਾਲੀ update_rnn_layer_gpu(layer l, update_ਦਲੀਲਾਂ a)
{
    update_connected_layer_gpu(*(l.ਨਿਵੇਸ਼_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

ਖਾਲੀ forward_rnn_layer_gpu(layer l, ਜਾਲ net)
{
    ਜਾਲ s = {0};
    s.train = net.train;
    ਪੂਰਨ i;
    layer ਨਿਵੇਸ਼_layer = *(l.ਨਿਵੇਸ਼_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, ਨਿਵੇਸ਼_layer.delta_gpu, 1);

    ਜੇ(net.train) {
        fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    ਲਈ (i = 0; i < l.ਕਦਮਾਂ; ++i) {
        s.ਨਿਵੇਸ਼_gpu = net.ਨਿਵੇਸ਼_gpu;
        forward_connected_layer_gpu(ਨਿਵੇਸ਼_layer, s);

        s.ਨਿਵੇਸ਼_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, ਨਿਵੇਸ਼_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ਨਿਵੇਸ਼_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.ਨਿਵੇਸ਼_gpu += l.inputs*l.batch;
        increment_layer(&ਨਿਵੇਸ਼_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ਖਾਲੀ backward_rnn_layer_gpu(layer l, ਜਾਲ net)
{
    ਜਾਲ s = {0};
    s.train = net.train;
    ਪੂਰਨ i;
    layer ਨਿਵੇਸ਼_layer = *(l.ਨਿਵੇਸ਼_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&ਨਿਵੇਸ਼_layer,  l.ਕਦਮਾਂ - 1);
    increment_layer(&self_layer,   l.ਕਦਮਾਂ - 1);
    increment_layer(&output_layer, l.ਕਦਮਾਂ - 1);
    ਦਸ਼ਮਲਵ *ਆਖਰੀ_ਨਿਵੇਸ਼ = ਨਿਵੇਸ਼_layer.output_gpu;
    ਦਸ਼ਮਲਵ *ਆਖਰੀ_self = self_layer.output_gpu;
    ਲਈ (i = l.ਕਦਮਾਂ-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, ਨਿਵੇਸ਼_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ਨਿਵੇਸ਼_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        ਜੇ(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, ਨਿਵੇਸ਼_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }ਨਹੀਂਤਾਂ {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, ਨਿਵੇਸ਼_layer.delta_gpu, 1);

        s.ਨਿਵੇਸ਼_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        ਜੇ (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.ਨਿਵੇਸ਼_gpu = net.ਨਿਵੇਸ਼_gpu + i*l.inputs*l.batch;
        ਜੇ(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        ਨਹੀਂਤਾਂ s.delta_gpu = 0;
        backward_connected_layer_gpu(ਨਿਵੇਸ਼_layer, s);

        increment_layer(&ਨਿਵੇਸ਼_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, ਆਖਰੀ_ਨਿਵੇਸ਼, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, ਆਖਰੀ_self, 1, l.state_gpu, 1);
}
#ਜੇ_ਅੰਤ
