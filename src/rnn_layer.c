#ಅನ್ತರ್ಭಾವ "rnn_layer.h"
#ಅನ್ತರ್ಭಾವ "connected_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "gemm.h"

#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>

ಸ್ಥಿರ ಶೂನ್ಯ increment_layer(layer *l, ಪೂರ್ಣಾಙ್ಕ steps)
{
    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ = l->outputs*l->batch*steps;
    l->output += ಅಂಕೆ;
    l->delta += ಅಂಕೆ;
    l->x += ಅಂಕೆ;
    l->x_norm += ಅಂಕೆ;

#ifdef GPU
    l->output_gpu += ಅಂಕೆ;
    l->delta_gpu += ಅಂಕೆ;
    l->x_gpu += ಅಂಕೆ;
    l->x_norm_gpu += ಅಂಕೆ;
#endif
}

layer make_rnn_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ inputs, ಪೂರ್ಣಾಙ್ಕ outputs, ಪೂರ್ಣಾಙ್ಕ steps, ACTIVATION activation, ಪೂರ್ಣಾಙ್ಕ batch_normalize, ಪೂರ್ಣಾಙ್ಕ adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / steps;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.steps = steps;
    l.inputs = inputs;

    l.state = calloc(batch*outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.prev_state = calloc(batch*outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));

    l.ಒಳಹರಿವು_layer = malloc(ಪರಿಮಾಣ(layer));
    fprintf(stderr, "\t\t");
    *(l.ಒಳಹರಿವು_layer) = make_connected_layer(batch*steps, inputs, outputs, activation, batch_normalize, adam);
    l.ಒಳಹರಿವು_layer->batch = batch;

    l.self_layer = malloc(ಪರಿಮಾಣ(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*steps, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = malloc(ಪರಿಮಾಣ(layer));
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
    cudnnSetTensor4dDescriptor(l.ಒಳಹರಿವು_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.ಒಳಹರಿವು_layer->out_c, l.ಒಳಹರಿವು_layer->out_h, l.ಒಳಹರಿವು_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.self_layer->out_c, l.self_layer->out_h, l.self_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.output_layer->out_c, l.output_layer->out_h, l.output_layer->out_w); 
#endif
#endif

    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ update_rnn_layer(layer l, update_args a)
{
    update_connected_layer(*(l.ಒಳಹರಿವು_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

ಶೂನ್ಯ forward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    ಪೂರ್ಣಾಙ್ಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, ಒಳಹರಿವು_layer.delta, 1);
    ಯದಿ(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    ಪ್ರತಿ (i = 0; i < l.steps; ++i) {
        s.ಒಳಹರಿವು = net.ಒಳಹರಿವು;
        forward_connected_layer(ಒಳಹರಿವು_layer, s);

        s.ಒಳಹರಿವು = l.state;
        forward_connected_layer(self_layer, s);

        ಭಿನ್ನ *old_state = l.state;
        ಯದಿ(net.train) l.state += l.outputs*l.batch;
        ಯದಿ(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }ಅನ್ಯಥಾ{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, ಒಳಹರಿವು_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ಒಳಹರಿವು = l.state;
        forward_connected_layer(output_layer, s);

        net.ಒಳಹರಿವು += l.inputs*l.batch;
        increment_layer(&ಒಳಹರಿವು_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ಶೂನ್ಯ backward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    ಪೂರ್ಣಾಙ್ಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&ಒಳಹರಿವು_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.outputs*l.batch*l.steps;
    ಪ್ರತಿ (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, ಒಳಹರಿವು_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ಒಳಹರಿವು = l.state;
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

        s.ಒಳಹರಿವು = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        ಯದಿ (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, ಒಳಹರಿವು_layer.delta, 1);
        ಯದಿ (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.ಒಳಹರಿವು = net.ಒಳಹರಿವು + i*l.inputs*l.batch;
        ಯದಿ(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        ಅನ್ಯಥಾ s.delta = 0;
        backward_connected_layer(ಒಳಹರಿವು_layer, s);

        increment_layer(&ಒಳಹರಿವು_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

ಶೂನ್ಯ pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.ಒಳಹರಿವು_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

ಶೂನ್ಯ ತಳ್ಳು_rnn_layer(layer l)
{
    ತಳ್ಳು_connected_layer(*(l.ಒಳಹರಿವು_layer));
    ತಳ್ಳು_connected_layer(*(l.self_layer));
    ತಳ್ಳು_connected_layer(*(l.output_layer));
}

ಶೂನ್ಯ update_rnn_layer_gpu(layer l, update_args a)
{
    update_connected_layer_gpu(*(l.ಒಳಹರಿವು_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

ಶೂನ್ಯ forward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    ಪೂರ್ಣಾಙ್ಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, ಒಳಹರಿವು_layer.delta_gpu, 1);

    ಯದಿ(net.train) {
        fill_gpu(l.outputs * l.batch * l.steps, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    ಪ್ರತಿ (i = 0; i < l.steps; ++i) {
        s.ಒಳಹರಿವು_gpu = net.ಒಳಹರಿವು_gpu;
        forward_connected_layer_gpu(ಒಳಹರಿವು_layer, s);

        s.ಒಳಹರಿವು_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, ಒಳಹರಿವು_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ಒಳಹರಿವು_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.ಒಳಹರಿವು_gpu += l.inputs*l.batch;
        increment_layer(&ಒಳಹರಿವು_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ಶೂನ್ಯ backward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    ಪೂರ್ಣಾಙ್ಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&ಒಳಹರಿವು_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    ಭಿನ್ನ *last_ಒಳಹರಿವು = ಒಳಹರಿವು_layer.output_gpu;
    ಭಿನ್ನ *last_self = self_layer.output_gpu;
    ಪ್ರತಿ (i = l.steps-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, ಒಳಹರಿವು_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ಒಳಹರಿವು_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        ಯದಿ(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, ಒಳಹರಿವು_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }ಅನ್ಯಥಾ {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, ಒಳಹರಿವು_layer.delta_gpu, 1);

        s.ಒಳಹರಿವು_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        ಯದಿ (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.ಒಳಹರಿವು_gpu = net.ಒಳಹರಿವು_gpu + i*l.inputs*l.batch;
        ಯದಿ(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        ಅನ್ಯಥಾ s.delta_gpu = 0;
        backward_connected_layer_gpu(ಒಳಹರಿವು_layer, s);

        increment_layer(&ಒಳಹರಿವು_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_ಒಳಹರಿವು, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_self, 1, l.state_gpu, 1);
}
#endif
