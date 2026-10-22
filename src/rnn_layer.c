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

ساكن فراغ increment_layer(layer *l, صحيح steps)
{
    صحيح اڠک = l->outputs*l->batch*steps;
    l->output += اڠک;
    l->delta += اڠک;
    l->x += اڠک;
    l->x_norm += اڠک;

#ifdef GPU
    l->output_gpu += اڠک;
    l->delta_gpu += اڠک;
    l->x_gpu += اڠک;
    l->x_norm_gpu += اڠک;
#endif
}

layer make_rnn_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح steps, ACTIVATION activation, صحيح batch_normalize, صحيح adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / steps;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.steps = steps;
    l.inputs = inputs;

    l.state = calloc(batch*outputs, حجم(عائم));
    l.prev_state = calloc(batch*outputs, حجم(عائم));

    l.ماسوقن_layer = malloc(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.ماسوقن_layer) = make_connected_layer(batch*steps, inputs, outputs, activation, batch_normalize, adam);
    l.ماسوقن_layer->batch = batch;

    l.self_layer = malloc(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*steps, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = malloc(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.output_layer) = make_connected_layer(batch*steps, outputs, outputs, activation, batch_normalize, adam);
    l.output_layer->batch = batch;

    l.outputs = outputs;
    l.output = l.output_layer->output;
    l.delta = l.output_layer->delta;

    l.forward = forward_rnn_layer;
    l.backward = backward_rnn_layer;
    l.update = update_rnn_layer;
#ifdef GPU
    l.forward_gpu = forward_rnn_layer_gpu;
    l.backward_gpu = backward_rnn_layer_gpu;
    l.update_gpu = update_rnn_layer_gpu;
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = l.output_layer->output_gpu;
    l.delta_gpu = l.output_layer->delta_gpu;
#ifdef CUDNN
    cudnnSetTensor4dDescriptor(l.ماسوقن_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.ماسوقن_layer->out_c, l.ماسوقن_layer->out_h, l.ماسوقن_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.self_layer->out_c, l.self_layer->out_h, l.self_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.output_layer->out_c, l.output_layer->out_h, l.output_layer->out_w); 
#endif
#endif

    أرجع l;
}

فراغ update_rnn_layer(layer l, update_args a)
{
    update_connected_layer(*(l.ماسوقن_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

فراغ forward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    صحيح i;
    layer ماسوقن_layer = *(l.ماسوقن_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, ماسوقن_layer.delta, 1);
    إذا(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    لكل (i = 0; i < l.steps; ++i) {
        s.ماسوقن = net.ماسوقن;
        forward_connected_layer(ماسوقن_layer, s);

        s.ماسوقن = l.state;
        forward_connected_layer(self_layer, s);

        عائم *old_state = l.state;
        إذا(net.train) l.state += l.outputs*l.batch;
        إذا(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }وإلا{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, ماسوقن_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ماسوقن = l.state;
        forward_connected_layer(output_layer, s);

        net.ماسوقن += l.inputs*l.batch;
        increment_layer(&ماسوقن_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

فراغ backward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    صحيح i;
    layer ماسوقن_layer = *(l.ماسوقن_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&ماسوقن_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.outputs*l.batch*l.steps;
    لكل (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, ماسوقن_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ماسوقن = l.state;
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

        s.ماسوقن = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        إذا (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, ماسوقن_layer.delta, 1);
        إذا (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.ماسوقن = net.ماسوقن + i*l.inputs*l.batch;
        إذا(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        وإلا s.delta = 0;
        backward_connected_layer(ماسوقن_layer, s);

        increment_layer(&ماسوقن_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

فراغ pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.ماسوقن_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

فراغ تولق_rnn_layer(layer l)
{
    تولق_connected_layer(*(l.ماسوقن_layer));
    تولق_connected_layer(*(l.self_layer));
    تولق_connected_layer(*(l.output_layer));
}

فراغ update_rnn_layer_gpu(layer l, update_args a)
{
    update_connected_layer_gpu(*(l.ماسوقن_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

فراغ forward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    صحيح i;
    layer ماسوقن_layer = *(l.ماسوقن_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, ماسوقن_layer.delta_gpu, 1);

    إذا(net.train) {
        fill_gpu(l.outputs * l.batch * l.steps, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    لكل (i = 0; i < l.steps; ++i) {
        s.ماسوقن_gpu = net.ماسوقن_gpu;
        forward_connected_layer_gpu(ماسوقن_layer, s);

        s.ماسوقن_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, ماسوقن_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ماسوقن_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.ماسوقن_gpu += l.inputs*l.batch;
        increment_layer(&ماسوقن_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

فراغ backward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    صحيح i;
    layer ماسوقن_layer = *(l.ماسوقن_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&ماسوقن_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    عائم *last_ماسوقن = ماسوقن_layer.output_gpu;
    عائم *last_self = self_layer.output_gpu;
    لكل (i = l.steps-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, ماسوقن_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ماسوقن_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        إذا(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, ماسوقن_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }وإلا {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, ماسوقن_layer.delta_gpu, 1);

        s.ماسوقن_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        إذا (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.ماسوقن_gpu = net.ماسوقن_gpu + i*l.inputs*l.batch;
        إذا(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        وإلا s.delta_gpu = 0;
        backward_connected_layer_gpu(ماسوقن_layer, s);

        increment_layer(&ماسوقن_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_ماسوقن, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_self, 1, l.state_gpu, 1);
}
#endif
