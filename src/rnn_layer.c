#अन्तर्भाव "rnn_layer.h"
#अन्तर्भाव "connected_layer.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "gemm.h"

#अन्तर्भाव <math.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

स्थिर शून्य increment_layer(layer *l, पूर्णाङ्क steps)
{
    पूर्णाङ्क अङ्कः = l->outputs*l->batch*steps;
    l->output += अङ्कः;
    l->delta += अङ्कः;
    l->x += अङ्कः;
    l->x_norm += अङ्कः;

#ifdef GPU
    l->output_gpu += अङ्कः;
    l->delta_gpu += अङ्कः;
    l->x_gpu += अङ्कः;
    l->x_norm_gpu += अङ्कः;
#endif
}

layer make_rnn_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, पूर्णाङ्क outputs, पूर्णाङ्क steps, ACTIVATION activation, पूर्णाङ्क batch_normalize, पूर्णाङ्क adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / steps;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.steps = steps;
    l.inputs = inputs;

    l.state = calloc(batch*outputs, परिमाण(भिन्न));
    l.prev_state = calloc(batch*outputs, परिमाण(भिन्न));

    l.निवेशः_layer = malloc(परिमाण(layer));
    fprintf(stderr, "\t\t");
    *(l.निवेशः_layer) = make_connected_layer(batch*steps, inputs, outputs, activation, batch_normalize, adam);
    l.निवेशः_layer->batch = batch;

    l.self_layer = malloc(परिमाण(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*steps, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = malloc(परिमाण(layer));
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
    cudnnSetTensor4dDescriptor(l.निवेशः_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.निवेशः_layer->out_c, l.निवेशः_layer->out_h, l.निवेशः_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.self_layer->out_c, l.self_layer->out_h, l.self_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.output_layer->out_c, l.output_layer->out_h, l.output_layer->out_w); 
#endif
#endif

    पटिनिवत्त l;
}

शून्य update_rnn_layer(layer l, update_args a)
{
    update_connected_layer(*(l.निवेशः_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

शून्य forward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    पूर्णाङ्क i;
    layer निवेशः_layer = *(l.निवेशः_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, निवेशः_layer.delta, 1);
    सचे(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    अत्थाय (i = 0; i < l.steps; ++i) {
        s.निवेशः = net.निवेशः;
        forward_connected_layer(निवेशः_layer, s);

        s.निवेशः = l.state;
        forward_connected_layer(self_layer, s);

        भिन्न *old_state = l.state;
        सचे(net.train) l.state += l.outputs*l.batch;
        सचे(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }अञ्ञथा{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, निवेशः_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.निवेशः = l.state;
        forward_connected_layer(output_layer, s);

        net.निवेशः += l.inputs*l.batch;
        increment_layer(&निवेशः_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

शून्य backward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    पूर्णाङ्क i;
    layer निवेशः_layer = *(l.निवेशः_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&निवेशः_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.outputs*l.batch*l.steps;
    अत्थाय (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, निवेशः_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.निवेशः = l.state;
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

        s.निवेशः = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        सचे (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, निवेशः_layer.delta, 1);
        सचे (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.निवेशः = net.निवेशः + i*l.inputs*l.batch;
        सचे(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        अञ्ञथा s.delta = 0;
        backward_connected_layer(निवेशः_layer, s);

        increment_layer(&निवेशः_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

शून्य pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.निवेशः_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

शून्य प्रक्षिप_rnn_layer(layer l)
{
    प्रक्षिप_connected_layer(*(l.निवेशः_layer));
    प्रक्षिप_connected_layer(*(l.self_layer));
    प्रक्षिप_connected_layer(*(l.output_layer));
}

शून्य update_rnn_layer_gpu(layer l, update_args a)
{
    update_connected_layer_gpu(*(l.निवेशः_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

शून्य forward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    पूर्णाङ्क i;
    layer निवेशः_layer = *(l.निवेशः_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, निवेशः_layer.delta_gpu, 1);

    सचे(net.train) {
        fill_gpu(l.outputs * l.batch * l.steps, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    अत्थाय (i = 0; i < l.steps; ++i) {
        s.निवेशः_gpu = net.निवेशः_gpu;
        forward_connected_layer_gpu(निवेशः_layer, s);

        s.निवेशः_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, निवेशः_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.निवेशः_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.निवेशः_gpu += l.inputs*l.batch;
        increment_layer(&निवेशः_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

शून्य backward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    पूर्णाङ्क i;
    layer निवेशः_layer = *(l.निवेशः_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&निवेशः_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    भिन्न *last_निवेशः = निवेशः_layer.output_gpu;
    भिन्न *last_self = self_layer.output_gpu;
    अत्थाय (i = l.steps-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, निवेशः_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.निवेशः_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        सचे(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, निवेशः_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }अञ्ञथा {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, निवेशः_layer.delta_gpu, 1);

        s.निवेशः_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        सचे (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.निवेशः_gpu = net.निवेशः_gpu + i*l.inputs*l.batch;
        सचे(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        अञ्ञथा s.delta_gpu = 0;
        backward_connected_layer_gpu(निवेशः_layer, s);

        increment_layer(&निवेशः_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_निवेशः, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_self, 1, l.state_gpu, 1);
}
#endif
