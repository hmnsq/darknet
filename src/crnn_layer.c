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

layer make_crnn_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ hidden_filters, ಪೂರ್ಣಾಙ್ಕ output_filters, ಪೂರ್ಣಾಙ್ಕ steps, ACTIVATION activation, ಪೂರ್ಣಾಙ್ಕ batch_normalize)
{
    fprintf(stderr, "CRNN Layer: %d x %d x %d image, %d filters\n", h,w,c,output_filters);
    batch = batch / steps;
    layer l = {0};
    l.batch = batch;
    l.type = CRNN;
    l.steps = steps;
    l.h = h;
    l.w = w;
    l.c = c;
    l.out_h = h;
    l.out_w = w;
    l.out_c = output_filters;
    l.inputs = h*w*c;
    l.hidden = h * w * hidden_filters;
    l.outputs = l.out_h * l.out_w * l.out_c;

    l.state = calloc(l.hidden*batch*(steps+1), ಪರಿಮಾಣ(ಭಿನ್ನ));

    l.ಒಳಹರಿವು_layer = malloc(ಪರಿಮಾಣ(layer));
    fprintf(stderr, "\t\t");
    *(l.ಒಳಹರಿವು_layer) = make_convolutional_layer(batch*steps, h, w, c, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.ಒಳಹರಿವು_layer->batch = batch;

    l.self_layer = malloc(ಪರಿಮಾಣ(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_convolutional_layer(batch*steps, h, w, hidden_filters, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.self_layer->batch = batch;

    l.output_layer = malloc(ಪರಿಮಾಣ(layer));
    fprintf(stderr, "\t\t");
    *(l.output_layer) = make_convolutional_layer(batch*steps, h, w, hidden_filters, output_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.output_layer->batch = batch;

    l.output = l.output_layer->output;
    l.delta = l.output_layer->delta;

    l.forward = forward_crnn_layer;
    l.backward = backward_crnn_layer;
    l.update = update_crnn_layer;

#ifdef GPU
    l.forward_gpu = forward_crnn_layer_gpu;
    l.backward_gpu = backward_crnn_layer_gpu;
    l.update_gpu = update_crnn_layer_gpu;

    l.state_gpu = cuda_make_array(l.state, l.hidden*batch*(steps+1));
    l.output_gpu = l.output_layer->output_gpu;
    l.delta_gpu = l.output_layer->delta_gpu;
#endif

    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ update_crnn_layer(layer l, update_args a)
{
    update_convolutional_layer(*(l.ಒಳಹರಿವು_layer),  a);
    update_convolutional_layer(*(l.self_layer),   a);
    update_convolutional_layer(*(l.output_layer), a);
}

ಶೂನ್ಯ forward_crnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    ಪೂರ್ಣಾಙ್ಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, ಒಳಹರಿವು_layer.delta, 1);
    ಯದಿ(net.train) fill_cpu(l.hidden * l.batch, 0, l.state, 1);

    ಪ್ರತಿ (i = 0; i < l.steps; ++i) {
        s.ಒಳಹರಿವು = net.ಒಳಹರಿವು;
        forward_convolutional_layer(ಒಳಹರಿವು_layer, s);

        s.ಒಳಹರಿವು = l.state;
        forward_convolutional_layer(self_layer, s);

        ಭಿನ್ನ *old_state = l.state;
        ಯದಿ(net.train) l.state += l.hidden*l.batch;
        ಯದಿ(l.shortcut){
            copy_cpu(l.hidden * l.batch, old_state, 1, l.state, 1);
        }ಅನ್ಯಥಾ{
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

ಶೂನ್ಯ backward_crnn_layer(layer l, network net)
{
    network s = net;
    ಪೂರ್ಣಾಙ್ಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&ಒಳಹರಿವು_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.hidden*l.batch*l.steps;
    ಪ್ರತಿ (i = l.steps-1; i >= 0; --i) {
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
        ಯದಿ (i == 0) s.delta = 0;
        backward_convolutional_layer(self_layer, s);

        copy_cpu(l.hidden*l.batch, self_layer.delta, 1, ಒಳಹರಿವು_layer.delta, 1);
        ಯದಿ (i > 0 && l.shortcut) axpy_cpu(l.hidden*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.hidden*l.batch, 1);
        s.ಒಳಹರಿವು = net.ಒಳಹರಿವು + i*l.inputs*l.batch;
        ಯದಿ(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        ಅನ್ಯಥಾ s.delta = 0;
        backward_convolutional_layer(ಒಳಹರಿವು_layer, s);

        increment_layer(&ಒಳಹರಿವು_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

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

ಶೂನ್ಯ update_crnn_layer_gpu(layer l, update_args a)
{
    update_convolutional_layer_gpu(*(l.ಒಳಹರಿವು_layer),  a);
    update_convolutional_layer_gpu(*(l.self_layer),   a);
    update_convolutional_layer_gpu(*(l.output_layer), a);
}

ಶೂನ್ಯ forward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    ಪೂರ್ಣಾಙ್ಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, ಒಳಹರಿವು_layer.delta_gpu, 1);
    ಯದಿ(net.train) fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);

    ಪ್ರತಿ (i = 0; i < l.steps; ++i) {
        s.ಒಳಹರಿವು_gpu = net.ಒಳಹರಿವು_gpu;
        forward_convolutional_layer_gpu(ಒಳಹರಿವು_layer, s);

        s.ಒಳಹರಿವು_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(self_layer, s);

        ಭಿನ್ನ *old_state = l.state_gpu;
        ಯದಿ(net.train) l.state_gpu += l.hidden*l.batch;
        ಯದಿ(l.shortcut){
            copy_gpu(l.hidden * l.batch, old_state, 1, l.state_gpu, 1);
        }ಅನ್ಯಥಾ{
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

ಶೂನ್ಯ backward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    s.train = net.train;
    ಪೂರ್ಣಾಙ್ಕ i;
    layer ಒಳಹರಿವು_layer = *(l.ಒಳಹರಿವು_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&ಒಳಹರಿವು_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    l.state_gpu += l.hidden*l.batch*l.steps;
    ಪ್ರತಿ (i = l.steps-1; i >= 0; --i) {
        copy_gpu(l.hidden * l.batch, ಒಳಹರಿವು_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ಒಳಹರಿವು_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_convolutional_layer_gpu(output_layer, s);

        l.state_gpu -= l.hidden*l.batch;

        s.ಒಳಹರಿವು_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu - l.hidden*l.batch;
        ಯದಿ (i == 0) s.delta_gpu = 0;
        backward_convolutional_layer_gpu(self_layer, s);

        copy_gpu(l.hidden*l.batch, self_layer.delta_gpu, 1, ಒಳಹರಿವು_layer.delta_gpu, 1);
        ಯದಿ (i > 0 && l.shortcut) axpy_gpu(l.hidden*l.batch, 1, self_layer.delta_gpu, 1, self_layer.delta_gpu - l.hidden*l.batch, 1);
        s.ಒಳಹರಿವು_gpu = net.ಒಳಹರಿವು_gpu + i*l.inputs*l.batch;
        ಯದಿ(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        ಅನ್ಯಥಾ s.delta_gpu = 0;
        backward_convolutional_layer_gpu(ಒಳಹರಿವು_layer, s);

        increment_layer(&ಒಳಹರಿವು_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
}
#endif
