#ضمن "rnn_layer.h"
#ضمن "connected_layer.h"
#ضمن "utils.h"
#ضمن "cuda.h"
#ضمن "blas.h"
#ضمن "gemm.h"

#ضمن <math.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>

ساكن فراغ increment_layer(layer *l, صحيح ګامونه)
{
    صحيح عدد = l->outputs*l->batch*ګامونه;
    l->output += عدد;
    l->delta += عدد;
    l->x += عدد;
    l->x_norm += عدد;

#که_تعریف_شوی GPU
    l->output_gpu += عدد;
    l->delta_gpu += عدد;
    l->x_gpu += عدد;
    l->x_norm_gpu += عدد;
#د_که_پای
}

layer make_rnn_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح ګامونه, ACTIVATION activation, صحيح batch_normalize, صحيح adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / ګامونه;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.ګامونه = ګامونه;
    l.inputs = inputs;

    l.state = حافظه_صفر_ورکړه(batch*outputs, حجم(عائم));
    l.prev_state = حافظه_صفر_ورکړه(batch*outputs, حجم(عائم));

    l.ننوت_layer = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.ننوت_layer) = make_connected_layer(batch*ګامونه, inputs, outputs, activation, batch_normalize, adam);
    l.ننوت_layer->batch = batch;

    l.self_layer = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*ګامونه, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.output_layer) = make_connected_layer(batch*ګامونه, outputs, outputs, activation, batch_normalize, adam);
    l.output_layer->batch = batch;

    l.outputs = outputs;
    l.output = l.output_layer->output;
    l.delta = l.output_layer->delta;

    l.forward = forward_rnn_layer;
    l.backward = backward_rnn_layer;
    l.update = update_rnn_layer;
#که_تعریف_شوی GPU
    l.forward_gpu = forward_rnn_layer_gpu;
    l.backward_gpu = backward_rnn_layer_gpu;
    l.update_gpu = update_rnn_layer_gpu;
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = l.output_layer->output_gpu;
    l.delta_gpu = l.output_layer->delta_gpu;
#که_تعریف_شوی CUDNN
    cudnn_سېټ_Tensor4d_Descriptor(l.ننوت_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.ننوت_layer->بهر_c, l.ننوت_layer->بهر_h, l.ننوت_layer->بهر_w); 
    cudnn_سېټ_Tensor4d_Descriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.self_layer->بهر_c, l.self_layer->بهر_h, l.self_layer->بهر_w); 
    cudnn_سېټ_Tensor4d_Descriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.output_layer->بهر_c, l.output_layer->بهر_h, l.output_layer->بهر_w); 
#د_که_پای
#د_که_پای

    أرجع l;
}

فراغ update_rnn_layer(layer l, update_دلیلونه a)
{
    update_connected_layer(*(l.ننوت_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

فراغ forward_rnn_layer(layer l, شبکه net)
{
    شبکه s = net;
    s.train = net.train;
    صحيح i;
    layer ننوت_layer = *(l.ننوت_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, ننوت_layer.delta, 1);
    إذا(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    لكل (i = 0; i < l.ګامونه; ++i) {
        s.ننوت = net.ننوت;
        forward_connected_layer(ننوت_layer, s);

        s.ننوت = l.state;
        forward_connected_layer(self_layer, s);

        عائم *old_state = l.state;
        إذا(net.train) l.state += l.outputs*l.batch;
        إذا(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }وإلا{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, ننوت_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ننوت = l.state;
        forward_connected_layer(output_layer, s);

        net.ننوت += l.inputs*l.batch;
        increment_layer(&ننوت_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

فراغ backward_rnn_layer(layer l, شبکه net)
{
    شبکه s = net;
    s.train = net.train;
    صحيح i;
    layer ننوت_layer = *(l.ننوت_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&ننوت_layer, l.ګامونه-1);
    increment_layer(&self_layer, l.ګامونه-1);
    increment_layer(&output_layer, l.ګامونه-1);

    l.state += l.outputs*l.batch*l.ګامونه;
    لكل (i = l.ګامونه-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, ننوت_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ننوت = l.state;
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

        s.ننوت = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        إذا (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, ننوت_layer.delta, 1);
        إذا (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.ننوت = net.ننوت + i*l.inputs*l.batch;
        إذا(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        وإلا s.delta = 0;
        backward_connected_layer(ننوت_layer, s);

        increment_layer(&ننوت_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#که_تعریف_شوی GPU

فراغ pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.ننوت_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

فراغ ټېل_rnn_layer(layer l)
{
    ټېل_connected_layer(*(l.ننوت_layer));
    ټېل_connected_layer(*(l.self_layer));
    ټېل_connected_layer(*(l.output_layer));
}

فراغ update_rnn_layer_gpu(layer l, update_دلیلونه a)
{
    update_connected_layer_gpu(*(l.ننوت_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

فراغ forward_rnn_layer_gpu(layer l, شبکه net)
{
    شبکه s = {0};
    s.train = net.train;
    صحيح i;
    layer ننوت_layer = *(l.ننوت_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, ننوت_layer.delta_gpu, 1);

    إذا(net.train) {
        fill_gpu(l.outputs * l.batch * l.ګامونه, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    لكل (i = 0; i < l.ګامونه; ++i) {
        s.ننوت_gpu = net.ننوت_gpu;
        forward_connected_layer_gpu(ننوت_layer, s);

        s.ننوت_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, ننوت_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ننوت_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.ننوت_gpu += l.inputs*l.batch;
        increment_layer(&ننوت_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

فراغ backward_rnn_layer_gpu(layer l, شبکه net)
{
    شبکه s = {0};
    s.train = net.train;
    صحيح i;
    layer ننوت_layer = *(l.ننوت_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&ننوت_layer,  l.ګامونه - 1);
    increment_layer(&self_layer,   l.ګامونه - 1);
    increment_layer(&output_layer, l.ګامونه - 1);
    عائم *وروستی_ننوت = ننوت_layer.output_gpu;
    عائم *وروستی_self = self_layer.output_gpu;
    لكل (i = l.ګامونه-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, ننوت_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ننوت_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        إذا(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, ننوت_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }وإلا {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, ننوت_layer.delta_gpu, 1);

        s.ننوت_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        إذا (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.ننوت_gpu = net.ننوت_gpu + i*l.inputs*l.batch;
        إذا(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        وإلا s.delta_gpu = 0;
        backward_connected_layer_gpu(ننوت_layer, s);

        increment_layer(&ننوت_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, وروستی_ننوت, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, وروستی_self, 1, l.state_gpu, 1);
}
#د_که_پای
