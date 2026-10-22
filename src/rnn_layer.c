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

स्थिर शून्य increment_layer(layer *l, पूर्णांक steps)
{
    पूर्णांक अंक = l->outputs*l->batch*steps;
    l->output += अंक;
    l->delta += अंक;
    l->x += अंक;
    l->x_norm += अंक;

#ifdef GPU
    l->output_gpu += अंक;
    l->delta_gpu += अंक;
    l->x_gpu += अंक;
    l->x_norm_gpu += अंक;
#endif
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

    l.state = सुस्मृति(batch*outputs, आकार(दशांश));
    l.prev_state = सुस्मृति(batch*outputs, आकार(दशांश));

    l.आदान_layer = दो_स्मृति(आकार(layer));
    ख_लिखो(मानक_त्रुटि, "\t\t");
    *(l.आदान_layer) = make_connected_layer(batch*steps, inputs, outputs, activation, batch_normalize, adam);
    l.आदान_layer->batch = batch;

    l.self_layer = दो_स्मृति(आकार(layer));
    ख_लिखो(मानक_त्रुटि, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*steps, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = दो_स्मृति(आकार(layer));
    ख_लिखो(मानक_त्रुटि, "\t\t");
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
    cudnnSetTensor4dDescriptor(l.आदान_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.आदान_layer->out_c, l.आदान_layer->out_h, l.आदान_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.self_layer->out_c, l.self_layer->out_h, l.self_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.output_layer->out_c, l.output_layer->out_h, l.output_layer->out_w); 
#endif
#endif

    परत l;
}

शून्य update_rnn_layer(layer l, update_args a)
{
    update_connected_layer(*(l.आदान_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

शून्य forward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    पूर्णांक i;
    layer आदान_layer = *(l.आदान_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, आदान_layer.delta, 1);
    जर(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    साठी (i = 0; i < l.steps; ++i) {
        s.आदान = net.आदान;
        forward_connected_layer(आदान_layer, s);

        s.आदान = l.state;
        forward_connected_layer(self_layer, s);

        दशांश *old_state = l.state;
        जर(net.train) l.state += l.outputs*l.batch;
        जर(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }अन्यथा{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, आदान_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.आदान = l.state;
        forward_connected_layer(output_layer, s);

        net.आदान += l.inputs*l.batch;
        increment_layer(&आदान_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

शून्य backward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    पूर्णांक i;
    layer आदान_layer = *(l.आदान_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&आदान_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.outputs*l.batch*l.steps;
    साठी (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, आदान_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.आदान = l.state;
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

        s.आदान = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        जर (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, आदान_layer.delta, 1);
        जर (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.आदान = net.आदान + i*l.inputs*l.batch;
        जर(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        अन्यथा s.delta = 0;
        backward_connected_layer(आदान_layer, s);

        increment_layer(&आदान_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

शून्य pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.आदान_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

शून्य ढकला_rnn_layer(layer l)
{
    ढकला_connected_layer(*(l.आदान_layer));
    ढकला_connected_layer(*(l.self_layer));
    ढकला_connected_layer(*(l.output_layer));
}

शून्य update_rnn_layer_gpu(layer l, update_args a)
{
    update_connected_layer_gpu(*(l.आदान_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

शून्य forward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    पूर्णांक i;
    layer आदान_layer = *(l.आदान_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, आदान_layer.delta_gpu, 1);

    जर(net.train) {
        fill_gpu(l.outputs * l.batch * l.steps, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    साठी (i = 0; i < l.steps; ++i) {
        s.आदान_gpu = net.आदान_gpu;
        forward_connected_layer_gpu(आदान_layer, s);

        s.आदान_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, आदान_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.आदान_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.आदान_gpu += l.inputs*l.batch;
        increment_layer(&आदान_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

शून्य backward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    पूर्णांक i;
    layer आदान_layer = *(l.आदान_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&आदान_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    दशांश *last_आदान = आदान_layer.output_gpu;
    दशांश *last_self = self_layer.output_gpu;
    साठी (i = l.steps-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, आदान_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.आदान_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        जर(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, आदान_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }अन्यथा {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, आदान_layer.delta_gpu, 1);

        s.आदान_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        जर (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.आदान_gpu = net.आदान_gpu + i*l.inputs*l.batch;
        जर(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        अन्यथा s.delta_gpu = 0;
        backward_connected_layer_gpu(आदान_layer, s);

        increment_layer(&आदान_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_आदान, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_self, 1, l.state_gpu, 1);
}
#endif
