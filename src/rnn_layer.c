#അന്തര്ഭാവ "rnn_layer.h"
#അന്തര്ഭാവ "connected_layer.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "gemm.h"

#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>

സ്ഥിതം ശൂന്യം increment_layer(layer *l, പൂർണ്ണസംഖ്യ steps)
{
    പൂർണ്ണസംഖ്യ അക്കം = l->outputs*l->batch*steps;
    l->output += അക്കം;
    l->delta += അക്കം;
    l->x += അക്കം;
    l->x_norm += അക്കം;

#ifdef GPU
    l->output_gpu += അക്കം;
    l->delta_gpu += അക്കം;
    l->x_gpu += അക്കം;
    l->x_norm_gpu += അക്കം;
#endif
}

layer make_rnn_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, പൂർണ്ണസംഖ്യ outputs, പൂർണ്ണസംഖ്യ steps, ACTIVATION activation, പൂർണ്ണസംഖ്യ batch_normalize, പൂർണ്ണസംഖ്യ adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / steps;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.steps = steps;
    l.inputs = inputs;

    l.state = calloc(batch*outputs, വലുപ്പം(ദശാംശം));
    l.prev_state = calloc(batch*outputs, വലുപ്പം(ദശാംശം));

    l.നിവേശം_layer = malloc(വലുപ്പം(layer));
    fprintf(stderr, "\t\t");
    *(l.നിവേശം_layer) = make_connected_layer(batch*steps, inputs, outputs, activation, batch_normalize, adam);
    l.നിവേശം_layer->batch = batch;

    l.self_layer = malloc(വലുപ്പം(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*steps, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = malloc(വലുപ്പം(layer));
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
    cudnnSetTensor4dDescriptor(l.നിവേശം_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.നിവേശം_layer->out_c, l.നിവേശം_layer->out_h, l.നിവേശം_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.self_layer->out_c, l.self_layer->out_h, l.self_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.output_layer->out_c, l.output_layer->out_h, l.output_layer->out_w); 
#endif
#endif

    തിരികെ l;
}

ശൂന്യം update_rnn_layer(layer l, update_args a)
{
    update_connected_layer(*(l.നിവേശം_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

ശൂന്യം forward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    പൂർണ്ണസംഖ്യ i;
    layer നിവേശം_layer = *(l.നിവേശം_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, നിവേശം_layer.delta, 1);
    എങ്കിൽ(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    വേണ്ടി (i = 0; i < l.steps; ++i) {
        s.നിവേശം = net.നിവേശം;
        forward_connected_layer(നിവേശം_layer, s);

        s.നിവേശം = l.state;
        forward_connected_layer(self_layer, s);

        ദശാംശം *old_state = l.state;
        എങ്കിൽ(net.train) l.state += l.outputs*l.batch;
        എങ്കിൽ(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }അല്ലെങ്കിൽ{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, നിവേശം_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.നിവേശം = l.state;
        forward_connected_layer(output_layer, s);

        net.നിവേശം += l.inputs*l.batch;
        increment_layer(&നിവേശം_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ശൂന്യം backward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    പൂർണ്ണസംഖ്യ i;
    layer നിവേശം_layer = *(l.നിവേശം_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&നിവേശം_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.outputs*l.batch*l.steps;
    വേണ്ടി (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, നിവേശം_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.നിവേശം = l.state;
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

        s.നിവേശം = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        എങ്കിൽ (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, നിവേശം_layer.delta, 1);
        എങ്കിൽ (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.നിവേശം = net.നിവേശം + i*l.inputs*l.batch;
        എങ്കിൽ(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        അല്ലെങ്കിൽ s.delta = 0;
        backward_connected_layer(നിവേശം_layer, s);

        increment_layer(&നിവേശം_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

ശൂന്യം pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.നിവേശം_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

ശൂന്യം തള്ളുക_rnn_layer(layer l)
{
    തള്ളുക_connected_layer(*(l.നിവേശം_layer));
    തള്ളുക_connected_layer(*(l.self_layer));
    തള്ളുക_connected_layer(*(l.output_layer));
}

ശൂന്യം update_rnn_layer_gpu(layer l, update_args a)
{
    update_connected_layer_gpu(*(l.നിവേശം_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

ശൂന്യം forward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    പൂർണ്ണസംഖ്യ i;
    layer നിവേശം_layer = *(l.നിവേശം_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, നിവേശം_layer.delta_gpu, 1);

    എങ്കിൽ(net.train) {
        fill_gpu(l.outputs * l.batch * l.steps, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    വേണ്ടി (i = 0; i < l.steps; ++i) {
        s.നിവേശം_gpu = net.നിവേശം_gpu;
        forward_connected_layer_gpu(നിവേശം_layer, s);

        s.നിവേശം_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, നിവേശം_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.നിവേശം_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.നിവേശം_gpu += l.inputs*l.batch;
        increment_layer(&നിവേശം_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ശൂന്യം backward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    പൂർണ്ണസംഖ്യ i;
    layer നിവേശം_layer = *(l.നിവേശം_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&നിവേശം_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    ദശാംശം *last_നിവേശം = നിവേശം_layer.output_gpu;
    ദശാംശം *last_self = self_layer.output_gpu;
    വേണ്ടി (i = l.steps-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, നിവേശം_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.നിവേശം_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        എങ്കിൽ(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, നിവേശം_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }അല്ലെങ്കിൽ {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, നിവേശം_layer.delta_gpu, 1);

        s.നിവേശം_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        എങ്കിൽ (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.നിവേശം_gpu = net.നിവേശം_gpu + i*l.inputs*l.batch;
        എങ്കിൽ(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        അല്ലെങ്കിൽ s.delta_gpu = 0;
        backward_connected_layer_gpu(നിവേശം_layer, s);

        increment_layer(&നിവേശം_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_നിവേശം, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_self, 1, l.state_gpu, 1);
}
#endif
