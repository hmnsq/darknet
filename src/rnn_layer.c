#அந்தர்பாவ "rnn_layer.h"
#அந்தர்பாவ "connected_layer.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ "gemm.h"

#அந்தர்பாவ <math.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>

நிலையான வெறுமை increment_layer(layer *l, முழு steps)
{
    முழு இலக்கம் = l->outputs*l->batch*steps;
    l->output += இலக்கம்;
    l->delta += இலக்கம்;
    l->x += இலக்கம்;
    l->x_norm += இலக்கம்;

#ifdef GPU
    l->output_gpu += இலக்கம்;
    l->delta_gpu += இலக்கம்;
    l->x_gpu += இலக்கம்;
    l->x_norm_gpu += இலக்கம்;
#endif
}

layer make_rnn_layer(முழு batch, முழு inputs, முழு outputs, முழு steps, ACTIVATION activation, முழு batch_normalize, முழு adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / steps;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.steps = steps;
    l.inputs = inputs;

    l.state = calloc(batch*outputs, அளவு(தசம));
    l.prev_state = calloc(batch*outputs, அளவு(தசம));

    l.உள்ளீடு_layer = malloc(அளவு(layer));
    fprintf(stderr, "\t\t");
    *(l.உள்ளீடு_layer) = make_connected_layer(batch*steps, inputs, outputs, activation, batch_normalize, adam);
    l.உள்ளீடு_layer->batch = batch;

    l.self_layer = malloc(அளவு(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*steps, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = malloc(அளவு(layer));
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
    cudnnSetTensor4dDescriptor(l.உள்ளீடு_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.உள்ளீடு_layer->out_c, l.உள்ளீடு_layer->out_h, l.உள்ளீடு_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.self_layer->out_c, l.self_layer->out_h, l.self_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.output_layer->out_c, l.output_layer->out_h, l.output_layer->out_w); 
#endif
#endif

    திரும்பு l;
}

வெறுமை update_rnn_layer(layer l, update_args a)
{
    update_connected_layer(*(l.உள்ளீடு_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

வெறுமை forward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    முழு i;
    layer உள்ளீடு_layer = *(l.உள்ளீடு_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, உள்ளீடு_layer.delta, 1);
    எனில்(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    க்கு (i = 0; i < l.steps; ++i) {
        s.உள்ளீடு = net.உள்ளீடு;
        forward_connected_layer(உள்ளீடு_layer, s);

        s.உள்ளீடு = l.state;
        forward_connected_layer(self_layer, s);

        தசம *old_state = l.state;
        எனில்(net.train) l.state += l.outputs*l.batch;
        எனில்(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }இல்லையெனில்{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, உள்ளீடு_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.உள்ளீடு = l.state;
        forward_connected_layer(output_layer, s);

        net.உள்ளீடு += l.inputs*l.batch;
        increment_layer(&உள்ளீடு_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

வெறுமை backward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    முழு i;
    layer உள்ளீடு_layer = *(l.உள்ளீடு_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&உள்ளீடு_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.outputs*l.batch*l.steps;
    க்கு (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, உள்ளீடு_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.உள்ளீடு = l.state;
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

        s.உள்ளீடு = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        எனில் (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, உள்ளீடு_layer.delta, 1);
        எனில் (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.உள்ளீடு = net.உள்ளீடு + i*l.inputs*l.batch;
        எனில்(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        இல்லையெனில் s.delta = 0;
        backward_connected_layer(உள்ளீடு_layer, s);

        increment_layer(&உள்ளீடு_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

வெறுமை pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.உள்ளீடு_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

வெறுமை தள்ளு_rnn_layer(layer l)
{
    தள்ளு_connected_layer(*(l.உள்ளீடு_layer));
    தள்ளு_connected_layer(*(l.self_layer));
    தள்ளு_connected_layer(*(l.output_layer));
}

வெறுமை update_rnn_layer_gpu(layer l, update_args a)
{
    update_connected_layer_gpu(*(l.உள்ளீடு_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

வெறுமை forward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    முழு i;
    layer உள்ளீடு_layer = *(l.உள்ளீடு_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, உள்ளீடு_layer.delta_gpu, 1);

    எனில்(net.train) {
        fill_gpu(l.outputs * l.batch * l.steps, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    க்கு (i = 0; i < l.steps; ++i) {
        s.உள்ளீடு_gpu = net.உள்ளீடு_gpu;
        forward_connected_layer_gpu(உள்ளீடு_layer, s);

        s.உள்ளீடு_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, உள்ளீடு_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.உள்ளீடு_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.உள்ளீடு_gpu += l.inputs*l.batch;
        increment_layer(&உள்ளீடு_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

வெறுமை backward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    முழு i;
    layer உள்ளீடு_layer = *(l.உள்ளீடு_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&உள்ளீடு_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    தசம *last_உள்ளீடு = உள்ளீடு_layer.output_gpu;
    தசம *last_self = self_layer.output_gpu;
    க்கு (i = l.steps-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, உள்ளீடு_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.உள்ளீடு_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        எனில்(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, உள்ளீடு_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }இல்லையெனில் {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, உள்ளீடு_layer.delta_gpu, 1);

        s.உள்ளீடு_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        எனில் (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.உள்ளீடு_gpu = net.உள்ளீடு_gpu + i*l.inputs*l.batch;
        எனில்(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        இல்லையெனில் s.delta_gpu = 0;
        backward_connected_layer_gpu(உள்ளீடு_layer, s);

        increment_layer(&உள்ளீடு_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_உள்ளீடு, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_self, 1, l.state_gpu, 1);
}
#endif
