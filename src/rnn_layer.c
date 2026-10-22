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

ساكن فراغ increment_layer(layer *l, صحيح قدمن)
{
    صحيح انگ = l->outputs*l->batch*قدمن;
    l->output += انگ;
    l->delta += انگ;
    l->x += انگ;
    l->x_norm += انگ;

#جيڪڏهن_وصف_ٿيل GPU
    l->output_gpu += انگ;
    l->delta_gpu += انگ;
    l->x_gpu += انگ;
    l->x_norm_gpu += انگ;
#جيڪڏهن_پورو
}

layer make_rnn_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح قدمن, ACTIVATION activation, صحيح batch_normalize, صحيح adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / قدمن;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.قدمن = قدمن;
    l.inputs = inputs;

    l.state = صفر_يادگيري_ڏيو(batch*outputs, حجم(عائم));
    l.prev_state = صفر_يادگيري_ڏيو(batch*outputs, حجم(عائم));

    l.داخلا_layer = يادگيري_ڏيو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.داخلا_layer) = make_connected_layer(batch*قدمن, inputs, outputs, activation, batch_normalize, adam);
    l.داخلا_layer->batch = batch;

    l.self_layer = يادگيري_ڏيو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*قدمن, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = يادگيري_ڏيو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.output_layer) = make_connected_layer(batch*قدمن, outputs, outputs, activation, batch_normalize, adam);
    l.output_layer->batch = batch;

    l.outputs = outputs;
    l.output = l.output_layer->output;
    l.delta = l.output_layer->delta;

    l.forward = forward_rnn_layer;
    l.backward = backward_rnn_layer;
    l.update = update_rnn_layer;
#جيڪڏهن_وصف_ٿيل GPU
    l.forward_gpu = forward_rnn_layer_gpu;
    l.backward_gpu = backward_rnn_layer_gpu;
    l.update_gpu = update_rnn_layer_gpu;
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = l.output_layer->output_gpu;
    l.delta_gpu = l.output_layer->delta_gpu;
#جيڪڏهن_وصف_ٿيل CUDNN
    cudnn_سيٽ_Tensor4d_Descriptor(l.داخلا_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.داخلا_layer->ٻاهريون_c, l.داخلا_layer->ٻاهريون_h, l.داخلا_layer->ٻاهريون_w); 
    cudnn_سيٽ_Tensor4d_Descriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.self_layer->ٻاهريون_c, l.self_layer->ٻاهريون_h, l.self_layer->ٻاهريون_w); 
    cudnn_سيٽ_Tensor4d_Descriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.output_layer->ٻاهريون_c, l.output_layer->ٻاهريون_h, l.output_layer->ٻاهريون_w); 
#جيڪڏهن_پورو
#جيڪڏهن_پورو

    أرجع l;
}

فراغ update_rnn_layer(layer l, update_دليلون a)
{
    update_connected_layer(*(l.داخلا_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

فراغ forward_rnn_layer(layer l, ڄار net)
{
    ڄار s = net;
    s.train = net.train;
    صحيح i;
    layer داخلا_layer = *(l.داخلا_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.قدمن, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.قدمن, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.قدمن, 0, داخلا_layer.delta, 1);
    إذا(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    لكل (i = 0; i < l.قدمن; ++i) {
        s.داخلا = net.داخلا;
        forward_connected_layer(داخلا_layer, s);

        s.داخلا = l.state;
        forward_connected_layer(self_layer, s);

        عائم *old_state = l.state;
        إذا(net.train) l.state += l.outputs*l.batch;
        إذا(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }وإلا{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, داخلا_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.داخلا = l.state;
        forward_connected_layer(output_layer, s);

        net.داخلا += l.inputs*l.batch;
        increment_layer(&داخلا_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

فراغ backward_rnn_layer(layer l, ڄار net)
{
    ڄار s = net;
    s.train = net.train;
    صحيح i;
    layer داخلا_layer = *(l.داخلا_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&داخلا_layer, l.قدمن-1);
    increment_layer(&self_layer, l.قدمن-1);
    increment_layer(&output_layer, l.قدمن-1);

    l.state += l.outputs*l.batch*l.قدمن;
    لكل (i = l.قدمن-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, داخلا_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.داخلا = l.state;
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

        s.داخلا = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        إذا (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, داخلا_layer.delta, 1);
        إذا (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.داخلا = net.داخلا + i*l.inputs*l.batch;
        إذا(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        وإلا s.delta = 0;
        backward_connected_layer(داخلا_layer, s);

        increment_layer(&داخلا_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#جيڪڏهن_وصف_ٿيل GPU

فراغ pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.داخلا_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

فراغ ڌڪو_rnn_layer(layer l)
{
    ڌڪو_connected_layer(*(l.داخلا_layer));
    ڌڪو_connected_layer(*(l.self_layer));
    ڌڪو_connected_layer(*(l.output_layer));
}

فراغ update_rnn_layer_gpu(layer l, update_دليلون a)
{
    update_connected_layer_gpu(*(l.داخلا_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

فراغ forward_rnn_layer_gpu(layer l, ڄار net)
{
    ڄار s = {0};
    s.train = net.train;
    صحيح i;
    layer داخلا_layer = *(l.داخلا_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.قدمن, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.قدمن, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.قدمن, 0, داخلا_layer.delta_gpu, 1);

    إذا(net.train) {
        fill_gpu(l.outputs * l.batch * l.قدمن, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    لكل (i = 0; i < l.قدمن; ++i) {
        s.داخلا_gpu = net.داخلا_gpu;
        forward_connected_layer_gpu(داخلا_layer, s);

        s.داخلا_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, داخلا_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.داخلا_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.داخلا_gpu += l.inputs*l.batch;
        increment_layer(&داخلا_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

فراغ backward_rnn_layer_gpu(layer l, ڄار net)
{
    ڄار s = {0};
    s.train = net.train;
    صحيح i;
    layer داخلا_layer = *(l.داخلا_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&داخلا_layer,  l.قدمن - 1);
    increment_layer(&self_layer,   l.قدمن - 1);
    increment_layer(&output_layer, l.قدمن - 1);
    عائم *آخري_داخلا = داخلا_layer.output_gpu;
    عائم *آخري_self = self_layer.output_gpu;
    لكل (i = l.قدمن-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, داخلا_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.داخلا_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        إذا(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, داخلا_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }وإلا {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, داخلا_layer.delta_gpu, 1);

        s.داخلا_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        إذا (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.داخلا_gpu = net.داخلا_gpu + i*l.inputs*l.batch;
        إذا(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        وإلا s.delta_gpu = 0;
        backward_connected_layer_gpu(داخلا_layer, s);

        increment_layer(&داخلا_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, آخري_داخلا, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, آخري_self, 1, l.state_gpu, 1);
}
#جيڪڏهن_پورو
