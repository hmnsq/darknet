#समावेश "rnn_layer.h"
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

layer make_rnn_layer(पूर्णांक batch, पूर्णांक inputs, पूर्णांक outputs, पूर्णांक steps, ACTIVATION activation, पूर्णांक batch_normalize, पूर्णांक adam)
{
    ख_लिखो(मानक_त्रुटि, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / steps;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.steps = steps;
    l.inputs = inputs;

    l.state = सुस्मृति(batch*outputs, माप(भग्न));
    l.पूर्व_state = सुस्मृति(batch*outputs, माप(भग्न));

    l.निवेश_layer = दो_स्मृति(माप(layer));
    ख_लिखो(मानक_त्रुटि, "\t\t");
    *(l.निवेश_layer) = make_connected_layer(batch*steps, inputs, outputs, activation, batch_normalize, adam);
    l.निवेश_layer->batch = batch;

    l.self_layer = दो_स्मृति(माप(layer));
    ख_लिखो(मानक_त्रुटि, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*steps, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = दो_स्मृति(माप(layer));
    ख_लिखो(मानक_त्रुटि, "\t\t");
    *(l.output_layer) = make_connected_layer(batch*steps, outputs, outputs, activation, batch_normalize, adam);
    l.output_layer->batch = batch;

    l.outputs = outputs;
    l.output = l.output_layer->output;
    l.delta = l.output_layer->delta;

    l.forward = forward_rnn_layer;
    l.backward = backward_rnn_layer;
    l.update = update_rnn_layer;
#अगर_घोषित GPU
    l.forward_gpu = forward_rnn_layer_gpu;
    l.backward_gpu = backward_rnn_layer_gpu;
    l.update_gpu = update_rnn_layer_gpu;
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.पूर्व_state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = l.output_layer->output_gpu;
    l.delta_gpu = l.output_layer->delta_gpu;
#अगर_घोषित CUDNN
    cudnnSetTensor4dDescriptor(l.निवेश_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, batch, l.निवेश_layer->out_c, l.निवेश_layer->out_h, l.निवेश_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, batch, l.self_layer->out_c, l.self_layer->out_h, l.self_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, batch, l.output_layer->out_c, l.output_layer->out_h, l.output_layer->out_w); 
#पूर्ण_अगर
#पूर्ण_अगर

    वापस l;
}

व्योम update_rnn_layer(layer l, update_args a)
{
    update_connected_layer(*(l.निवेश_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

व्योम forward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    पूर्णांक i;
    layer निवेश_layer = *(l.निवेश_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, निवेश_layer.delta, 1);
    अगर(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    क्रम (i = 0; i < l.steps; ++i) {
        s.निवेश = net.निवेश;
        forward_connected_layer(निवेश_layer, s);

        s.निवेश = l.state;
        forward_connected_layer(self_layer, s);

        भग्न *old_state = l.state;
        अगर(net.train) l.state += l.outputs*l.batch;
        अगर(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }अन्यथा{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, निवेश_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.निवेश = l.state;
        forward_connected_layer(output_layer, s);

        net.निवेश += l.inputs*l.batch;
        increment_layer(&निवेश_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

व्योम backward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    पूर्णांक i;
    layer निवेश_layer = *(l.निवेश_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&निवेश_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.outputs*l.batch*l.steps;
    क्रम (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, निवेश_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.निवेश = l.state;
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

        s.निवेश = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        अगर (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, निवेश_layer.delta, 1);
        अगर (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.निवेश = net.निवेश + i*l.inputs*l.batch;
        अगर(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        अन्यथा s.delta = 0;
        backward_connected_layer(निवेश_layer, s);

        increment_layer(&निवेश_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#अगर_घोषित GPU

व्योम pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.निवेश_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

व्योम धकेलो_rnn_layer(layer l)
{
    धकेलो_connected_layer(*(l.निवेश_layer));
    धकेलो_connected_layer(*(l.self_layer));
    धकेलो_connected_layer(*(l.output_layer));
}

व्योम update_rnn_layer_gpu(layer l, update_args a)
{
    update_connected_layer_gpu(*(l.निवेश_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

व्योम forward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    पूर्णांक i;
    layer निवेश_layer = *(l.निवेश_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, निवेश_layer.delta_gpu, 1);

    अगर(net.train) {
        fill_gpu(l.outputs * l.batch * l.steps, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.पूर्व_state_gpu, 1);
    }

    क्रम (i = 0; i < l.steps; ++i) {
        s.निवेश_gpu = net.निवेश_gpu;
        forward_connected_layer_gpu(निवेश_layer, s);

        s.निवेश_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, निवेश_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.निवेश_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.निवेश_gpu += l.inputs*l.batch;
        increment_layer(&निवेश_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

व्योम backward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    पूर्णांक i;
    layer निवेश_layer = *(l.निवेश_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&निवेश_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    भग्न *last_निवेश = निवेश_layer.output_gpu;
    भग्न *last_self = self_layer.output_gpu;
    क्रम (i = l.steps-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, निवेश_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.निवेश_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        अगर(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, निवेश_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }अन्यथा {
            copy_gpu(l.outputs*l.batch, l.पूर्व_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, निवेश_layer.delta_gpu, 1);

        s.निवेश_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        अगर (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.निवेश_gpu = net.निवेश_gpu + i*l.inputs*l.batch;
        अगर(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        अन्यथा s.delta_gpu = 0;
        backward_connected_layer_gpu(निवेश_layer, s);

        increment_layer(&निवेश_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_निवेश, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_self, 1, l.state_gpu, 1);
}
#पूर्ण_अगर
