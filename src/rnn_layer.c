#અન્તર્ભાવ "rnn_layer.h"
#અન્તર્ભાવ "connected_layer.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ "gemm.h"

#અન્તર્ભાવ <math.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>

સ્થિર શૂન્ય increment_layer(layer *l, પૂર્ણાંક steps)
{
    પૂર્ણાંક અંક = l->outputs*l->batch*steps;
    l->output += અંક;
    l->delta += અંક;
    l->x += અંક;
    l->x_norm += અંક;

#ifdef GPU
    l->output_gpu += અંક;
    l->delta_gpu += અંક;
    l->x_gpu += અંક;
    l->x_norm_gpu += અંક;
#endif
}

layer make_rnn_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs, પૂર્ણાંક outputs, પૂર્ણાંક steps, ACTIVATION activation, પૂર્ણાંક batch_normalize, પૂર્ણાંક adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / steps;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.steps = steps;
    l.inputs = inputs;

    l.state = calloc(batch*outputs, કદ(દશાંશ));
    l.prev_state = calloc(batch*outputs, કદ(દશાંશ));

    l.આગત_layer = malloc(કદ(layer));
    fprintf(stderr, "\t\t");
    *(l.આગત_layer) = make_connected_layer(batch*steps, inputs, outputs, activation, batch_normalize, adam);
    l.આગત_layer->batch = batch;

    l.self_layer = malloc(કદ(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*steps, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = malloc(કદ(layer));
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
    cudnnSetTensor4dDescriptor(l.આગત_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.આગત_layer->out_c, l.આગત_layer->out_h, l.આગત_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.self_layer->out_c, l.self_layer->out_h, l.self_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.output_layer->out_c, l.output_layer->out_h, l.output_layer->out_w); 
#endif
#endif

    પાછા l;
}

શૂન્ય update_rnn_layer(layer l, update_args a)
{
    update_connected_layer(*(l.આગત_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

શૂન્ય forward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    પૂર્ણાંક i;
    layer આગત_layer = *(l.આગત_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, આગત_layer.delta, 1);
    જો(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    માટે (i = 0; i < l.steps; ++i) {
        s.આગત = net.આગત;
        forward_connected_layer(આગત_layer, s);

        s.આગત = l.state;
        forward_connected_layer(self_layer, s);

        દશાંશ *old_state = l.state;
        જો(net.train) l.state += l.outputs*l.batch;
        જો(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }નહીંતર{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, આગત_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.આગત = l.state;
        forward_connected_layer(output_layer, s);

        net.આગત += l.inputs*l.batch;
        increment_layer(&આગત_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

શૂન્ય backward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    પૂર્ણાંક i;
    layer આગત_layer = *(l.આગત_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&આગત_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.outputs*l.batch*l.steps;
    માટે (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, આગત_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.આગત = l.state;
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

        s.આગત = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        જો (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, આગત_layer.delta, 1);
        જો (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.આગત = net.આગત + i*l.inputs*l.batch;
        જો(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        નહીંતર s.delta = 0;
        backward_connected_layer(આગત_layer, s);

        increment_layer(&આગત_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

શૂન્ય pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.આગત_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

શૂન્ય ધકેલો_rnn_layer(layer l)
{
    ધકેલો_connected_layer(*(l.આગત_layer));
    ધકેલો_connected_layer(*(l.self_layer));
    ધકેલો_connected_layer(*(l.output_layer));
}

શૂન્ય update_rnn_layer_gpu(layer l, update_args a)
{
    update_connected_layer_gpu(*(l.આગત_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

શૂન્ય forward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    પૂર્ણાંક i;
    layer આગત_layer = *(l.આગત_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, આગત_layer.delta_gpu, 1);

    જો(net.train) {
        fill_gpu(l.outputs * l.batch * l.steps, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    માટે (i = 0; i < l.steps; ++i) {
        s.આગત_gpu = net.આગત_gpu;
        forward_connected_layer_gpu(આગત_layer, s);

        s.આગત_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, આગત_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.આગત_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.આગત_gpu += l.inputs*l.batch;
        increment_layer(&આગત_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

શૂન્ય backward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    પૂર્ણાંક i;
    layer આગત_layer = *(l.આગત_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&આગત_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    દશાંશ *last_આગત = આગત_layer.output_gpu;
    દશાંશ *last_self = self_layer.output_gpu;
    માટે (i = l.steps-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, આગત_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.આગત_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        જો(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, આગત_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }નહીંતર {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, આગત_layer.delta_gpu, 1);

        s.આગત_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        જો (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.આગત_gpu = net.આગત_gpu + i*l.inputs*l.batch;
        જો(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        નહીંતર s.delta_gpu = 0;
        backward_connected_layer_gpu(આગત_layer, s);

        increment_layer(&આગત_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_આગત, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_self, 1, l.state_gpu, 1);
}
#endif
