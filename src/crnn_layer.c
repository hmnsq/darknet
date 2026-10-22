#ಅನ್ತರ್ಭಾವ "crnn_layer.h"
#ಅನ್ತರ್ಭಾವ "convolutional_layer.h"
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

layer make_crnn_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ hidden_filters, ಪೂರ್ಣಾಂಕ output_filters, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆಗಳು, ACTIVATION activation, ಪೂರ್ಣಾಂಕ batch_normalize)
{
    fprintf(stderr, "CRNN Layer: %d x %d x %d image, %d filters\n", h,w,c,output_filters);
    batch = batch / ಹೆಜ್ಜೆಗಳು;
    layer l = {0};
    l.batch = batch;
    l.type = CRNN;
    l.ಹೆಜ್ಜೆಗಳು = ಹೆಜ್ಜೆಗಳು;
    l.h = h;
    l.w = w;
    l.c = c;
    l.ಹೊರಹೋಗು_h = h;
    l.ಹೊರಹೋಗು_w = w;
    l.ಹೊರಹೋಗು_c = output_filters;
    l.inputs = h*w*c;
    l.hidden = h * w * hidden_filters;
    l.outputs = l.ಹೊರಹೋಗು_h * l.ಹೊರಹೋಗು_w * l.ಹೊರಹೋಗು_c;

    l.state = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.hidden*batch*(ಹೆಜ್ಜೆಗಳು+1), ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.ಒಳಹರಿವು_layer = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.ಒಳಹರಿವು_layer) = make_convolutional_layer(batch*ಹೆಜ್ಜೆಗಳು, h, w, c, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.ಒಳಹರಿವು_layer->batch = batch;

    l.self_layer = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_convolutional_layer(batch*ಹೆಜ್ಜೆಗಳು, h, w, hidden_filters, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.self_layer->batch = batch;

    l.output_layer = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.output_layer) = make_convolutional_layer(batch*ಹೆಜ್ಜೆಗಳು, h, w, hidden_filters, output_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.output_layer->batch = batch;

    l.output = l.output_layer->output;
    l.delta = l.output_layer->delta;

    l.forward = forward_crnn_layer;
    l.backward = backward_crnn_layer;
    l.update = update_crnn_layer;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_crnn_layer_gpu;
    l.backward_gpu = backward_crnn_layer_gpu;
    l.update_gpu = update_crnn_layer_gpu;

    l.state_gpu = cuda_make_array(l.state, l.hidden*batch*(ಹೆಜ್ಜೆಗಳು+1));
    l.output_gpu = l.output_layer->output_gpu;
    l.delta_gpu = l.output_layer->delta_gpu;
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ update_crnn_layer(layer l, update_ವಾದಗಳು a)
{
    update_convolutional_layer(*(l.ಒಳಹರಿವು_layer),  a);
    update_convolutional_layer(*(l.self_layer),   a);
    update_convolutional_layer(*(l.output_layer), a);
}

ಶೂನ್ಯ forward_crnn_layer(layer l, ಜಾಲ net)
{
    ಜಾಲ s = net;
    s.train = net.train;
    ಪೂರ್ಣಾಂಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, output_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, self_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, ಒಳಹರಿವು_layer.delta, 1);
    ಆದರೆ(net.train) fill_cpu(l.hidden * l.batch, 0, l.state, 1);

    ಗಾಗಿ (i = 0; i < l.ಹೆಜ್ಜೆಗಳು; ++i) {
        s.ಒಳಹರಿವು = net.ಒಳಹರಿವು;
        forward_convolutional_layer(ಒಳಹರಿವು_layer, s);

        s.ಒಳಹರಿವು = l.state;
        forward_convolutional_layer(self_layer, s);

        ದಶಮಾಂಶ *old_state = l.state;
        ಆದರೆ(net.train) l.state += l.hidden*l.batch;
        ಆದರೆ(l.shortcut){
            copy_cpu(l.hidden * l.batch, old_state, 1, l.state, 1);
        }ಇಲ್ಲದಿದ್ದರೆ{
            fill_cpu(l.hidden * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.hidden * l.batch, 1, ಒಳಹರಿವು_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ಒಳಹರಿವು = l.state;
        forward_convolutional_layer(output_layer, s);

        net.ಒಳಹರಿವು += l.inputs*l.batch;
        increment_layer(&ಒಳಹರಿವು_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ಶೂನ್ಯ backward_crnn_layer(layer l, ಜಾಲ net)
{
    ಜಾಲ s = net;
    ಪೂರ್ಣಾಂಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&ಒಳಹರಿವು_layer, l.ಹೆಜ್ಜೆಗಳು-1);
    increment_layer(&self_layer, l.ಹೆಜ್ಜೆಗಳು-1);
    increment_layer(&output_layer, l.ಹೆಜ್ಜೆಗಳು-1);

    l.state += l.hidden*l.batch*l.ಹೆಜ್ಜೆಗಳು;
    ಗಾಗಿ (i = l.ಹೆಜ್ಜೆಗಳು-1; i >= 0; --i) {
        copy_cpu(l.hidden * l.batch, ಒಳಹರಿವು_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ಒಳಹರಿವು = l.state;
        s.delta = self_layer.delta;
        backward_convolutional_layer(output_layer, s);

        l.state -= l.hidden*l.batch;
        /*
           if(i > 0){
           copy_cpu(l.hidden * l.batch, input_layer.output - l.hidden*l.batch, 1, l.state, 1);
           axpy_cpu(l.hidden * l.batch, 1, self_layer.output - l.hidden*l.batch, 1, l.state, 1);
           }else{
           fill_cpu(l.hidden * l.batch, 0, l.state, 1);
           }
         */

        s.ಒಳಹರಿವು = l.state;
        s.delta = self_layer.delta - l.hidden*l.batch;
        ಆದರೆ (i == 0) s.delta = 0;
        backward_convolutional_layer(self_layer, s);

        copy_cpu(l.hidden*l.batch, self_layer.delta, 1, ಒಳಹರಿವು_layer.delta, 1);
        ಆದರೆ (i > 0 && l.shortcut) axpy_cpu(l.hidden*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.hidden*l.batch, 1);
        s.ಒಳಹರಿವು = net.ಒಳಹರಿವು + i*l.inputs*l.batch;
        ಆದರೆ(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        ಇಲ್ಲದಿದ್ದರೆ s.delta = 0;
        backward_convolutional_layer(ಒಳಹರಿವು_layer, s);

        increment_layer(&ಒಳಹರಿವು_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

ಶೂನ್ಯ pull_crnn_layer(layer l)
{
    pull_convolutional_layer(*(l.ಒಳಹರಿವು_layer));
    pull_convolutional_layer(*(l.self_layer));
    pull_convolutional_layer(*(l.output_layer));
}

ಶೂನ್ಯ ತಳ್ಳು_crnn_layer(layer l)
{
    ತಳ್ಳು_convolutional_layer(*(l.ಒಳಹರಿವು_layer));
    ತಳ್ಳು_convolutional_layer(*(l.self_layer));
    ತಳ್ಳು_convolutional_layer(*(l.output_layer));
}

ಶೂನ್ಯ update_crnn_layer_gpu(layer l, update_ವಾದಗಳು a)
{
    update_convolutional_layer_gpu(*(l.ಒಳಹರಿವು_layer),  a);
    update_convolutional_layer_gpu(*(l.self_layer),   a);
    update_convolutional_layer_gpu(*(l.output_layer), a);
}

ಶೂನ್ಯ forward_crnn_layer_gpu(layer l, ಜಾಲ net)
{
    ಜಾಲ s = net;
    ಪೂರ್ಣಾಂಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, ಒಳಹರಿವು_layer.delta_gpu, 1);
    ಆದರೆ(net.train) fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);

    ಗಾಗಿ (i = 0; i < l.ಹೆಜ್ಜೆಗಳು; ++i) {
        s.ಒಳಹರಿವು_gpu = net.ಒಳಹರಿವು_gpu;
        forward_convolutional_layer_gpu(ಒಳಹರಿವು_layer, s);

        s.ಒಳಹರಿವು_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(self_layer, s);

        ದಶಮಾಂಶ *old_state = l.state_gpu;
        ಆದರೆ(net.train) l.state_gpu += l.hidden*l.batch;
        ಆದರೆ(l.shortcut){
            copy_gpu(l.hidden * l.batch, old_state, 1, l.state_gpu, 1);
        }ಇಲ್ಲದಿದ್ದರೆ{
            fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);
        }
        axpy_gpu(l.hidden * l.batch, 1, ಒಳಹರಿವು_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ಒಳಹರಿವು_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(output_layer, s);

        net.ಒಳಹರಿವು_gpu += l.inputs*l.batch;
        increment_layer(&ಒಳಹರಿವು_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ಶೂನ್ಯ backward_crnn_layer_gpu(layer l, ಜಾಲ net)
{
    ಜಾಲ s = net;
    s.train = net.train;
    ಪೂರ್ಣಾಂಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&ಒಳಹರಿವು_layer,  l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&self_layer,   l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&output_layer, l.ಹೆಜ್ಜೆಗಳು - 1);
    l.state_gpu += l.hidden*l.batch*l.ಹೆಜ್ಜೆಗಳು;
    ಗಾಗಿ (i = l.ಹೆಜ್ಜೆಗಳು-1; i >= 0; --i) {
        copy_gpu(l.hidden * l.batch, ಒಳಹರಿವು_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ಒಳಹರಿವು_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_convolutional_layer_gpu(output_layer, s);

        l.state_gpu -= l.hidden*l.batch;

        s.ಒಳಹರಿವು_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu - l.hidden*l.batch;
        ಆದರೆ (i == 0) s.delta_gpu = 0;
        backward_convolutional_layer_gpu(self_layer, s);

        copy_gpu(l.hidden*l.batch, self_layer.delta_gpu, 1, ಒಳಹರಿವು_layer.delta_gpu, 1);
        ಆದರೆ (i > 0 && l.shortcut) axpy_gpu(l.hidden*l.batch, 1, self_layer.delta_gpu, 1, self_layer.delta_gpu - l.hidden*l.batch, 1);
        s.ಒಳಹರಿವು_gpu = net.ಒಳಹರಿವು_gpu + i*l.inputs*l.batch;
        ಆದರೆ(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        ಇಲ್ಲದಿದ್ದರೆ s.delta_gpu = 0;
        backward_convolutional_layer_gpu(ಒಳಹರಿವು_layer, s);

        increment_layer(&ಒಳಹರಿವು_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
}
#ಆದರೆ_ಅಂತ್ಯ
