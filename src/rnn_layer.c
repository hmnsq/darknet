#ଅନ୍ତର୍ଭାଵ "rnn_layer.h"
#ଅନ୍ତର୍ଭାଵ "connected_layer.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ "gemm.h"

#ଅନ୍ତର୍ଭାଵ <math.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

ସ୍ଥିର ଶୂନ୍ୟ increment_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା steps)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ = l->outputs*l->batch*steps;
    l->output += ଅଙ୍କ;
    l->delta += ଅଙ୍କ;
    l->x += ଅଙ୍କ;
    l->x_norm += ଅଙ୍କ;

#ifdef GPU
    l->output_gpu += ଅଙ୍କ;
    l->delta_gpu += ଅଙ୍କ;
    l->x_gpu += ଅଙ୍କ;
    l->x_norm_gpu += ଅଙ୍କ;
#endif
}

layer make_rnn_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା outputs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା steps, ACTIVATION activation, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / steps;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.steps = steps;
    l.inputs = inputs;

    l.state = calloc(batch*outputs, ଆକାର(ଦଶମିକ));
    l.prev_state = calloc(batch*outputs, ଆକାର(ଦଶମିକ));

    l.ନିବେଶ_layer = malloc(ଆକାର(layer));
    fprintf(stderr, "\t\t");
    *(l.ନିବେଶ_layer) = make_connected_layer(batch*steps, inputs, outputs, activation, batch_normalize, adam);
    l.ନିବେଶ_layer->batch = batch;

    l.self_layer = malloc(ଆକାର(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*steps, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = malloc(ଆକାର(layer));
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
    cudnnSetTensor4dDescriptor(l.ନିବେଶ_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.ନିବେଶ_layer->out_c, l.ନିବେଶ_layer->out_h, l.ନିବେଶ_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.self_layer->out_c, l.self_layer->out_h, l.self_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.output_layer->out_c, l.output_layer->out_h, l.output_layer->out_w); 
#endif
#endif

    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ update_rnn_layer(layer l, update_args a)
{
    update_connected_layer(*(l.ନିବେଶ_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

ଶୂନ୍ୟ forward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    layer ନିବେଶ_layer = *(l.ନିବେଶ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, ନିବେଶ_layer.delta, 1);
    ଯଦି(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    ପାଇଁ (i = 0; i < l.steps; ++i) {
        s.ନିବେଶ = net.ନିବେଶ;
        forward_connected_layer(ନିବେଶ_layer, s);

        s.ନିବେଶ = l.state;
        forward_connected_layer(self_layer, s);

        ଦଶମିକ *old_state = l.state;
        ଯଦି(net.train) l.state += l.outputs*l.batch;
        ଯଦି(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }ଅନ୍ୟଥା{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, ନିବେଶ_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ନିବେଶ = l.state;
        forward_connected_layer(output_layer, s);

        net.ନିବେଶ += l.inputs*l.batch;
        increment_layer(&ନିବେଶ_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ଶୂନ୍ୟ backward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    layer ନିବେଶ_layer = *(l.ନିବେଶ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&ନିବେଶ_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.outputs*l.batch*l.steps;
    ପାଇଁ (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, ନିବେଶ_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ନିବେଶ = l.state;
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

        s.ନିବେଶ = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        ଯଦି (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, ନିବେଶ_layer.delta, 1);
        ଯଦି (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.ନିବେଶ = net.ନିବେଶ + i*l.inputs*l.batch;
        ଯଦି(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        ଅନ୍ୟଥା s.delta = 0;
        backward_connected_layer(ନିବେଶ_layer, s);

        increment_layer(&ନିବେଶ_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

ଶୂନ୍ୟ pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.ନିବେଶ_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

ଶୂନ୍ୟ ଠେଲ_rnn_layer(layer l)
{
    ଠେଲ_connected_layer(*(l.ନିବେଶ_layer));
    ଠେଲ_connected_layer(*(l.self_layer));
    ଠେଲ_connected_layer(*(l.output_layer));
}

ଶୂନ୍ୟ update_rnn_layer_gpu(layer l, update_args a)
{
    update_connected_layer_gpu(*(l.ନିବେଶ_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

ଶୂନ୍ୟ forward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    layer ନିବେଶ_layer = *(l.ନିବେଶ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, ନିବେଶ_layer.delta_gpu, 1);

    ଯଦି(net.train) {
        fill_gpu(l.outputs * l.batch * l.steps, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    ପାଇଁ (i = 0; i < l.steps; ++i) {
        s.ନିବେଶ_gpu = net.ନିବେଶ_gpu;
        forward_connected_layer_gpu(ନିବେଶ_layer, s);

        s.ନିବେଶ_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, ନିବେଶ_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ନିବେଶ_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.ନିବେଶ_gpu += l.inputs*l.batch;
        increment_layer(&ନିବେଶ_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ଶୂନ୍ୟ backward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    layer ନିବେଶ_layer = *(l.ନିବେଶ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&ନିବେଶ_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    ଦଶମିକ *last_ନିବେଶ = ନିବେଶ_layer.output_gpu;
    ଦଶମିକ *last_self = self_layer.output_gpu;
    ପାଇଁ (i = l.steps-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, ନିବେଶ_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ନିବେଶ_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        ଯଦି(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, ନିବେଶ_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }ଅନ୍ୟଥା {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, ନିବେଶ_layer.delta_gpu, 1);

        s.ନିବେଶ_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        ଯଦି (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.ନିବେଶ_gpu = net.ନିବେଶ_gpu + i*l.inputs*l.batch;
        ଯଦି(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        ଅନ୍ୟଥା s.delta_gpu = 0;
        backward_connected_layer_gpu(ନିବେଶ_layer, s);

        increment_layer(&ନିବେଶ_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_ନିବେଶ, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_self, 1, l.state_gpu, 1);
}
#endif
