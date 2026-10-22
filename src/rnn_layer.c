#অন্তর্ভাব "rnn_layer.h"
#অন্তর্ভাব "connected_layer.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "gemm.h"

#অন্তর্ভাব <math.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>

স্থির শূন্য increment_layer(layer *l, পূর্ণাঙ্ক steps)
{
    পূর্ণাঙ্ক অঙ্ক = l->outputs*l->batch*steps;
    l->output += অঙ্ক;
    l->delta += অঙ্ক;
    l->x += অঙ্ক;
    l->x_norm += অঙ্ক;

#ifdef GPU
    l->output_gpu += অঙ্ক;
    l->delta_gpu += অঙ্ক;
    l->x_gpu += অঙ্ক;
    l->x_norm_gpu += অঙ্ক;
#endif
}

layer make_rnn_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs, পূর্ণাঙ্ক outputs, পূর্ণাঙ্ক steps, ACTIVATION activation, পূর্ণাঙ্ক batch_normalize, পূর্ণাঙ্ক adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / steps;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.steps = steps;
    l.inputs = inputs;

    l.state = calloc(batch*outputs, পরিমাণ(ভিন্ন));
    l.prev_state = calloc(batch*outputs, পরিমাণ(ভিন্ন));

    l.প্রবেশ_layer = malloc(পরিমাণ(layer));
    fprintf(stderr, "\t\t");
    *(l.প্রবেশ_layer) = make_connected_layer(batch*steps, inputs, outputs, activation, batch_normalize, adam);
    l.প্রবেশ_layer->batch = batch;

    l.self_layer = malloc(পরিমাণ(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*steps, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = malloc(পরিমাণ(layer));
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
    cudnnSetTensor4dDescriptor(l.প্রবেশ_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.প্রবেশ_layer->out_c, l.প্রবেশ_layer->out_h, l.প্রবেশ_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.self_layer->out_c, l.self_layer->out_h, l.self_layer->out_w); 
    cudnnSetTensor4dDescriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, batch, l.output_layer->out_c, l.output_layer->out_h, l.output_layer->out_w); 
#endif
#endif

    প্রত্যাগম l;
}

শূন্য update_rnn_layer(layer l, update_args a)
{
    update_connected_layer(*(l.প্রবেশ_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

শূন্য forward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    পূর্ণাঙ্ক i;
    layer প্রবেশ_layer = *(l.প্রবেশ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.steps, 0, প্রবেশ_layer.delta, 1);
    যদি(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    প্রতি (i = 0; i < l.steps; ++i) {
        s.প্রবেশ = net.প্রবেশ;
        forward_connected_layer(প্রবেশ_layer, s);

        s.প্রবেশ = l.state;
        forward_connected_layer(self_layer, s);

        ভিন্ন *old_state = l.state;
        যদি(net.train) l.state += l.outputs*l.batch;
        যদি(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }অন্যথা{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, প্রবেশ_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.প্রবেশ = l.state;
        forward_connected_layer(output_layer, s);

        net.প্রবেশ += l.inputs*l.batch;
        increment_layer(&প্রবেশ_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

শূন্য backward_rnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    পূর্ণাঙ্ক i;
    layer প্রবেশ_layer = *(l.প্রবেশ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&প্রবেশ_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.outputs*l.batch*l.steps;
    প্রতি (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, প্রবেশ_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.প্রবেশ = l.state;
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

        s.প্রবেশ = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        যদি (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, প্রবেশ_layer.delta, 1);
        যদি (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.প্রবেশ = net.প্রবেশ + i*l.inputs*l.batch;
        যদি(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        অন্যথা s.delta = 0;
        backward_connected_layer(প্রবেশ_layer, s);

        increment_layer(&প্রবেশ_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

শূন্য pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.প্রবেশ_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

শূন্য ঠেলো_rnn_layer(layer l)
{
    ঠেলো_connected_layer(*(l.প্রবেশ_layer));
    ঠেলো_connected_layer(*(l.self_layer));
    ঠেলো_connected_layer(*(l.output_layer));
}

শূন্য update_rnn_layer_gpu(layer l, update_args a)
{
    update_connected_layer_gpu(*(l.প্রবেশ_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

শূন্য forward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    পূর্ণাঙ্ক i;
    layer প্রবেশ_layer = *(l.প্রবেশ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.steps, 0, প্রবেশ_layer.delta_gpu, 1);

    যদি(net.train) {
        fill_gpu(l.outputs * l.batch * l.steps, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    প্রতি (i = 0; i < l.steps; ++i) {
        s.প্রবেশ_gpu = net.প্রবেশ_gpu;
        forward_connected_layer_gpu(প্রবেশ_layer, s);

        s.প্রবেশ_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, প্রবেশ_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.প্রবেশ_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.প্রবেশ_gpu += l.inputs*l.batch;
        increment_layer(&প্রবেশ_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

শূন্য backward_rnn_layer_gpu(layer l, network net)
{
    network s = {0};
    s.train = net.train;
    পূর্ণাঙ্ক i;
    layer প্রবেশ_layer = *(l.প্রবেশ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&প্রবেশ_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    ভিন্ন *last_প্রবেশ = প্রবেশ_layer.output_gpu;
    ভিন্ন *last_self = self_layer.output_gpu;
    প্রতি (i = l.steps-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, প্রবেশ_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.প্রবেশ_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        যদি(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, প্রবেশ_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }অন্যথা {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, প্রবেশ_layer.delta_gpu, 1);

        s.প্রবেশ_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        যদি (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.প্রবেশ_gpu = net.প্রবেশ_gpu + i*l.inputs*l.batch;
        যদি(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        অন্যথা s.delta_gpu = 0;
        backward_connected_layer_gpu(প্রবেশ_layer, s);

        increment_layer(&প্রবেশ_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_প্রবেশ, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, last_self, 1, l.state_gpu, 1);
}
#endif
