#ضمن "rnn_layer.h"
#ضمن "connected_layer.h"
#ضمن "utils.h"
#ضمن "cuda.h"
#ضمن "blas.h"
#ضمن "gemm.h"

#ضمن <math.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>

ساكن فراغ increment_layer(layer *l, صحيح گامها)
{
    صحيح رقم = l->outputs*l->batch*گامها;
    l->output += رقم;
    l->delta += رقم;
    l->x += رقم;
    l->x_norm += رقم;

#اگر_تعریف_شده GPU
    l->output_gpu += رقم;
    l->delta_gpu += رقم;
    l->x_gpu += رقم;
    l->x_norm_gpu += رقم;
#پایان_اگر
}

layer make_rnn_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح گامها, ACTIVATION activation, صحيح batch_normalize, صحيح adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / گامها;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.گامها = گامها;
    l.inputs = inputs;

    l.state = حافظه_صفر_بده(batch*outputs, حجم(عائم));
    l.prev_state = حافظه_صفر_بده(batch*outputs, حجم(عائم));

    l.درون_داد_layer = حافظه_بده(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.درون_داد_layer) = make_connected_layer(batch*گامها, inputs, outputs, activation, batch_normalize, adam);
    l.درون_داد_layer->batch = batch;

    l.self_layer = حافظه_بده(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*گامها, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = حافظه_بده(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.output_layer) = make_connected_layer(batch*گامها, outputs, outputs, activation, batch_normalize, adam);
    l.output_layer->batch = batch;

    l.outputs = outputs;
    l.output = l.output_layer->output;
    l.delta = l.output_layer->delta;

    l.forward = forward_rnn_layer;
    l.backward = backward_rnn_layer;
    l.update = update_rnn_layer;
#اگر_تعریف_شده GPU
    l.forward_gpu = forward_rnn_layer_gpu;
    l.backward_gpu = backward_rnn_layer_gpu;
    l.update_gpu = update_rnn_layer_gpu;
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = l.output_layer->output_gpu;
    l.delta_gpu = l.output_layer->delta_gpu;
#اگر_تعریف_شده CUDNN
    cudnn_مجموعه_Tensor4d_Descriptor(l.درون_داد_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_داده_FLOAT, batch, l.درون_داد_layer->بیرون_c, l.درون_داد_layer->بیرون_h, l.درون_داد_layer->بیرون_w); 
    cudnn_مجموعه_Tensor4d_Descriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_داده_FLOAT, batch, l.self_layer->بیرون_c, l.self_layer->بیرون_h, l.self_layer->بیرون_w); 
    cudnn_مجموعه_Tensor4d_Descriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_داده_FLOAT, batch, l.output_layer->بیرون_c, l.output_layer->بیرون_h, l.output_layer->بیرون_w); 
#پایان_اگر
#پایان_اگر

    أرجع l;
}

فراغ update_rnn_layer(layer l, update_آرگومانها a)
{
    update_connected_layer(*(l.درون_داد_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

فراغ forward_rnn_layer(layer l, شبکه net)
{
    شبکه s = net;
    s.train = net.train;
    صحيح i;
    layer درون_داد_layer = *(l.درون_داد_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.گامها, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.گامها, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.گامها, 0, درون_داد_layer.delta, 1);
    إذا(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    لكل (i = 0; i < l.گامها; ++i) {
        s.درون_داد = net.درون_داد;
        forward_connected_layer(درون_داد_layer, s);

        s.درون_داد = l.state;
        forward_connected_layer(self_layer, s);

        عائم *old_state = l.state;
        إذا(net.train) l.state += l.outputs*l.batch;
        إذا(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }وإلا{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, درون_داد_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.درون_داد = l.state;
        forward_connected_layer(output_layer, s);

        net.درون_داد += l.inputs*l.batch;
        increment_layer(&درون_داد_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

فراغ backward_rnn_layer(layer l, شبکه net)
{
    شبکه s = net;
    s.train = net.train;
    صحيح i;
    layer درون_داد_layer = *(l.درون_داد_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&درون_داد_layer, l.گامها-1);
    increment_layer(&self_layer, l.گامها-1);
    increment_layer(&output_layer, l.گامها-1);

    l.state += l.outputs*l.batch*l.گامها;
    لكل (i = l.گامها-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, درون_داد_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.درون_داد = l.state;
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

        s.درون_داد = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        إذا (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, درون_داد_layer.delta, 1);
        إذا (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.درون_داد = net.درون_داد + i*l.inputs*l.batch;
        إذا(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        وإلا s.delta = 0;
        backward_connected_layer(درون_داد_layer, s);

        increment_layer(&درون_داد_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#اگر_تعریف_شده GPU

فراغ pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.درون_داد_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

فراغ افزودن_rnn_layer(layer l)
{
    افزودن_connected_layer(*(l.درون_داد_layer));
    افزودن_connected_layer(*(l.self_layer));
    افزودن_connected_layer(*(l.output_layer));
}

فراغ update_rnn_layer_gpu(layer l, update_آرگومانها a)
{
    update_connected_layer_gpu(*(l.درون_داد_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

فراغ forward_rnn_layer_gpu(layer l, شبکه net)
{
    شبکه s = {0};
    s.train = net.train;
    صحيح i;
    layer درون_داد_layer = *(l.درون_داد_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.گامها, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.گامها, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.گامها, 0, درون_داد_layer.delta_gpu, 1);

    إذا(net.train) {
        fill_gpu(l.outputs * l.batch * l.گامها, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    لكل (i = 0; i < l.گامها; ++i) {
        s.درون_داد_gpu = net.درون_داد_gpu;
        forward_connected_layer_gpu(درون_داد_layer, s);

        s.درون_داد_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, درون_داد_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.درون_داد_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.درون_داد_gpu += l.inputs*l.batch;
        increment_layer(&درون_داد_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

فراغ backward_rnn_layer_gpu(layer l, شبکه net)
{
    شبکه s = {0};
    s.train = net.train;
    صحيح i;
    layer درون_داد_layer = *(l.درون_داد_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&درون_داد_layer,  l.گامها - 1);
    increment_layer(&self_layer,   l.گامها - 1);
    increment_layer(&output_layer, l.گامها - 1);
    عائم *آخرین_درون_داد = درون_داد_layer.output_gpu;
    عائم *آخرین_self = self_layer.output_gpu;
    لكل (i = l.گامها-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, درون_داد_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.درون_داد_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        إذا(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, درون_داد_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }وإلا {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, درون_داد_layer.delta_gpu, 1);

        s.درون_داد_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        إذا (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.درون_داد_gpu = net.درون_داد_gpu + i*l.inputs*l.batch;
        إذا(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        وإلا s.delta_gpu = 0;
        backward_connected_layer_gpu(درون_داد_layer, s);

        increment_layer(&درون_داد_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, آخرین_درون_داد, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, آخرین_self, 1, l.state_gpu, 1);
}
#پایان_اگر
