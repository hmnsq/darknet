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

ಸ್ಥಿರ ಶೂನ್ಯ increment_layer(layer *l, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆಗಳು)
{
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ = l->outputs*l->batch*ಹೆಜ್ಜೆಗಳು;
    l->output += ಅಂಕೆ;
    l->delta += ಅಂಕೆ;
    l->x += ಅಂಕೆ;
    l->x_norm += ಅಂಕೆ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l->output_gpu += ಅಂಕೆ;
    l->delta_gpu += ಅಂಕೆ;
    l->x_gpu += ಅಂಕೆ;
    l->x_norm_gpu += ಅಂಕೆ;
#ಆದರೆ_ಅಂತ್ಯ
}

layer make_rnn_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ಪೂರ್ಣಾಂಕ outputs, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆಗಳು, ACTIVATION activation, ಪೂರ್ಣಾಂಕ batch_normalize, ಪೂರ್ಣಾಂಕ adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / ಹೆಜ್ಜೆಗಳು;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.ಹೆಜ್ಜೆಗಳು = ಹೆಜ್ಜೆಗಳು;
    l.inputs = inputs;

    l.state = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.prev_state = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.ಒಳಹರಿವು_layer = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.ಒಳಹರಿವು_layer) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, inputs, outputs, activation, batch_normalize, adam);
    l.ಒಳಹರಿವು_layer->batch = batch;

    l.self_layer = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.output_layer) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, outputs, outputs, activation, batch_normalize, adam);
    l.output_layer->batch = batch;

    l.outputs = outputs;
    l.output = l.output_layer->output;
    l.delta = l.output_layer->delta;

    l.forward = forward_rnn_layer;
    l.backward = backward_rnn_layer;
    l.update = update_rnn_layer;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_rnn_layer_gpu;
    l.backward_gpu = backward_rnn_layer_gpu;
    l.update_gpu = update_rnn_layer_gpu;
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = l.output_layer->output_gpu;
    l.delta_gpu = l.output_layer->delta_gpu;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CUDNN
    cudnn_ಗಣ_Tensor4d_Descriptor(l.ಒಳಹರಿವು_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.ಒಳಹರಿವು_layer->ಹೊರಹೋಗು_c, l.ಒಳಹರಿವು_layer->ಹೊರಹೋಗು_h, l.ಒಳಹರಿವು_layer->ಹೊರಹೋಗು_w); 
    cudnn_ಗಣ_Tensor4d_Descriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.self_layer->ಹೊರಹೋಗು_c, l.self_layer->ಹೊರಹೋಗು_h, l.self_layer->ಹೊರಹೋಗು_w); 
    cudnn_ಗಣ_Tensor4d_Descriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.output_layer->ಹೊರಹೋಗು_c, l.output_layer->ಹೊರಹೋಗು_h, l.output_layer->ಹೊರಹೋಗು_w); 
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ update_rnn_layer(layer l, update_ವಾದಗಳು a)
{
    update_connected_layer(*(l.ಒಳಹರಿವು_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

ಶೂನ್ಯ forward_rnn_layer(layer l, ಜಾಲ net)
{
    ಜಾಲ s = net;
    s.train = net.train;
    ಪೂರ್ಣಾಂಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, ಒಳಹರಿವು_layer.delta, 1);
    ಆದರೆ(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    ಗಾಗಿ (i = 0; i < l.ಹೆಜ್ಜೆಗಳು; ++i) {
        s.ಒಳಹರಿವು = net.ಒಳಹರಿವು;
        forward_connected_layer(ಒಳಹರಿವು_layer, s);

        s.ಒಳಹರಿವು = l.state;
        forward_connected_layer(self_layer, s);

        ದಶಮಾಂಶ *old_state = l.state;
        ಆದರೆ(net.train) l.state += l.outputs*l.batch;
        ಆದರೆ(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }ಇಲ್ಲದಿದ್ದರೆ{
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

ಶೂನ್ಯ backward_rnn_layer(layer l, ಜಾಲ net)
{
    ಜಾಲ s = net;
    s.train = net.train;
    ಪೂರ್ಣಾಂಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&ಒಳಹರಿವು_layer, l.ಹೆಜ್ಜೆಗಳು-1);
    increment_layer(&self_layer, l.ಹೆಜ್ಜೆಗಳು-1);
    increment_layer(&output_layer, l.ಹೆಜ್ಜೆಗಳು-1);

    l.state += l.outputs*l.batch*l.ಹೆಜ್ಜೆಗಳು;
    ಗಾಗಿ (i = l.ಹೆಜ್ಜೆಗಳು-1; i >= 0; --i) {
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
        ಆದರೆ (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, ಒಳಹರಿವು_layer.delta, 1);
        ಆದರೆ (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.ಒಳಹರಿವು = net.ಒಳಹರಿವು + i*l.inputs*l.batch;
        ಆದರೆ(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        ಇಲ್ಲದಿದ್ದರೆ s.delta = 0;
        backward_connected_layer(ಒಳಹರಿವು_layer, s);

        increment_layer(&ಒಳಹರಿವು_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

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

ಶೂನ್ಯ update_rnn_layer_gpu(layer l, update_ವಾದಗಳು a)
{
    update_connected_layer_gpu(*(l.ಒಳಹರಿವು_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

ಶೂನ್ಯ forward_rnn_layer_gpu(layer l, ಜಾಲ net)
{
    ಜಾಲ s = {0};
    s.train = net.train;
    ಪೂರ್ಣಾಂಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, ಒಳಹರಿವು_layer.delta_gpu, 1);

    ಆದರೆ(net.train) {
        fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    ಗಾಗಿ (i = 0; i < l.ಹೆಜ್ಜೆಗಳು; ++i) {
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

ಶೂನ್ಯ backward_rnn_layer_gpu(layer l, ಜಾಲ net)
{
    ಜಾಲ s = {0};
    s.train = net.train;
    ಪೂರ್ಣಾಂಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&ಒಳಹರಿವು_layer,  l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&self_layer,   l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&output_layer, l.ಹೆಜ್ಜೆಗಳು - 1);
    ದಶಮಾಂಶ *ಕೊನೆಯ_ಒಳಹರಿವು = ಒಳಹರಿವು_layer.output_gpu;
    ದಶಮಾಂಶ *ಕೊನೆಯ_self = self_layer.output_gpu;
    ಗಾಗಿ (i = l.ಹೆಜ್ಜೆಗಳು-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, ಒಳಹರಿವು_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ಒಳಹರಿವು_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        ಆದರೆ(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, ಒಳಹರಿವು_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }ಇಲ್ಲದಿದ್ದರೆ {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, ಒಳಹರಿವು_layer.delta_gpu, 1);

        s.ಒಳಹರಿವು_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        ಆದರೆ (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.ಒಳಹರಿವು_gpu = net.ಒಳಹರಿವು_gpu + i*l.inputs*l.batch;
        ಆದರೆ(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        ಇಲ್ಲದಿದ್ದರೆ s.delta_gpu = 0;
        backward_connected_layer_gpu(ಒಳಹರಿವು_layer, s);

        increment_layer(&ಒಳಹರಿವು_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, ಕೊನೆಯ_ಒಳಹರಿವು, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, ಕೊನೆಯ_self, 1, l.state_gpu, 1);
}
#ಆದರೆ_ಅಂತ್ಯ
