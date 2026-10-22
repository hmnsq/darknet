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

ایستا تهی increment_layer(layer *l, صحیح steps)
{
    صحیح رقم = l->outputs*l->batch*steps;
    l->output += رقم;
    l->delta += رقم;
    l->x += رقم;
    l->x_norm += رقم;

#ifdef GPU
    l->output_gpu += رقم;
    l->delta_gpu += رقم;
    l->x_gpu += رقم;
    l->x_norm_gpu += رقم;
#endif
}

layer make_rnn_layer(صحیح batch, صحیح inputs, صحیح outputs, صحیح steps, ACTIVATION activation, صحیح batch_normalize, صحیح adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / steps;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.steps = steps;
    l.inputs = inputs;

    l.state = calloc(batch*outputs, اندازه(اعشاری));
    l.prev_state = calloc(batch*outputs, اندازه(اعشاری));

    l.درون_داد_layer = malloc(اندازه(layer));
    fprintf(stderr, "\t\t");
    *(l.درون_داد_layer) = make_connected_layer(batch*steps, inputs, outputs, activation, batch_normalize, adam);
    l.درون_داد_layer->batch = batch;

    l.self_layer = malloc(اندازه(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*steps, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = malloc(اندازه(layer));
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
    cudnnSetTensor4dDescriptor(l.درون_داد_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.درون_داد_layer->out_c, l.درون_داد_layer->out_h, l.درون_داد_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.self_layer->out_c, l.self_layer->out_h, l.self_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.output_layer->out_c, l.output_layer->out_h, l.output_layer->out_w); 
#endif
#endif

    بازگشت l;
}

تهی update_rnn_layer(layer l, update_args a)
{
    update_connected_layer(*(l.درون_داد_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

تهی forward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    صحیح i;
    layer درون_داد_layer = *(l.درون_داد_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, درون_داد_layer.delta, 1);
    اگر(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    برای (i = 0; i < l.steps; ++i) {
        s.درون_داد = net.درون_داد;
        forward_connected_layer(درون_داد_layer, s);

        s.درون_داد = l.state;
        forward_connected_layer(self_layer, s);

        اعشاری *old_state = l.state;
        اگر(net.train) l.state += l.outputs*l.batch;
        اگر(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }وگرنه{
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

تهی backward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    صحیح i;
    layer درون_داد_layer = *(l.درون_داد_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&درون_داد_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.outputs*l.batch*l.steps;
    برای (i = l.steps-1; i >= 0; --i) {
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
        اگر (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, درون_داد_layer.delta, 1);
        اگر (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.درون_داد = net.درون_داد + i*l.inputs*l.batch;
        اگر(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        وگرنه s.delta = 0;
        backward_connected_layer(درون_داد_layer, s);

        increment_layer(&درون_داد_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

تهی pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.درون_داد_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

تهی افزودن_rnn_layer(layer l)
{
    افزودن_connected_layer(*(l.درون_داد_layer));
    افزودن_connected_layer(*(l.self_layer));
    افزودن_connected_layer(*(l.output_layer));
}

تهی update_rnn_layer_gpu(layer l, update_args a)
{
    update_connected_layer_gpu(*(l.درون_داد_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

تهی forward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    صحیح i;
    layer درون_داد_layer = *(l.درون_داد_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, درون_داد_layer.delta_gpu, 1);

    اگر(net.train) {
        fill_gpu(l.outputs * l.batch * l.steps, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    برای (i = 0; i < l.steps; ++i) {
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

تهی backward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    صحیح i;
    layer درون_داد_layer = *(l.درون_داد_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&درون_داد_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    اعشاری *last_درون_داد = درون_داد_layer.output_gpu;
    اعشاری *last_self = self_layer.output_gpu;
    برای (i = l.steps-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, درون_داد_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.درون_داد_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        اگر(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, درون_داد_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }وگرنه {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, درون_داد_layer.delta_gpu, 1);

        s.درون_داد_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        اگر (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.درون_داد_gpu = net.درون_داد_gpu + i*l.inputs*l.batch;
        اگر(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        وگرنه s.delta_gpu = 0;
        backward_connected_layer_gpu(درون_داد_layer, s);

        increment_layer(&درون_داد_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_درون_داد, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_self, 1, l.state_gpu, 1);
}
#endif
