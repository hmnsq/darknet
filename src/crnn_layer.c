#அந்தர்பாவ "crnn_layer.h"
#அந்தர்பாவ "convolutional_layer.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ "gemm.h"

#அந்தர்பாவ <math.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>

நிலையான வெறுமை increment_layer(layer *l, முழு steps)
{
    முழு இலக்கம் = l->outputs*l->batch*steps;
    l->output += இலக்கம்;
    l->delta += இலக்கம்;
    l->x += இலக்கம்;
    l->x_norm += இலக்கம்;

#ifdef GPU
    l->output_gpu += இலக்கம்;
    l->delta_gpu += இலக்கம்;
    l->x_gpu += இலக்கம்;
    l->x_norm_gpu += இலக்கம்;
#endif
}

layer make_crnn_layer(முழு batch, முழு h, முழு w, முழு c, முழு hidden_filters, முழு output_filters, முழு steps, ACTIVATION activation, முழு batch_normalize)
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

    l.state = calloc(l.hidden*batch*(steps+1), அளவு(தசம));

    l.உள்ளீடு_layer = malloc(அளவு(layer));
    fprintf(stderr, "\t\t");
    *(l.உள்ளீடு_layer) = make_convolutional_layer(batch*steps, h, w, c, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.உள்ளீடு_layer->batch = batch;

    l.self_layer = malloc(அளவு(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_convolutional_layer(batch*steps, h, w, hidden_filters, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.self_layer->batch = batch;

    l.output_layer = malloc(அளவு(layer));
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

    திரும்பு l;
}

வெறுமை update_crnn_layer(layer l, update_args a)
{
    update_convolutional_layer(*(l.உள்ளீடு_layer),  a);
    update_convolutional_layer(*(l.self_layer),   a);
    update_convolutional_layer(*(l.output_layer), a);
}

வெறுமை forward_crnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    முழு i;
    layer உள்ளீடு_layer = *(l.உள்ளீடு_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, உள்ளீடு_layer.delta, 1);
    எனில்(net.train) fill_cpu(l.hidden * l.batch, 0, l.state, 1);

    க்கு (i = 0; i < l.steps; ++i) {
        s.உள்ளீடு = net.உள்ளீடு;
        forward_convolutional_layer(உள்ளீடு_layer, s);

        s.உள்ளீடு = l.state;
        forward_convolutional_layer(self_layer, s);

        தசம *old_state = l.state;
        எனில்(net.train) l.state += l.hidden*l.batch;
        எனில்(l.shortcut){
            copy_cpu(l.hidden * l.batch, old_state, 1, l.state, 1);
        }இல்லையெனில்{
            fill_cpu(l.hidden * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.hidden * l.batch, 1, உள்ளீடு_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.உள்ளீடு = l.state;
        forward_convolutional_layer(output_layer, s);

        net.உள்ளீடு += l.inputs*l.batch;
        increment_layer(&உள்ளீடு_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

வெறுமை backward_crnn_layer(layer l, network net)
{
    network s = net;
    முழு i;
    layer உள்ளீடு_layer = *(l.உள்ளீடு_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&உள்ளீடு_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.hidden*l.batch*l.steps;
    க்கு (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.hidden * l.batch, உள்ளீடு_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.உள்ளீடு = l.state;
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

        s.உள்ளீடு = l.state;
        s.delta = self_layer.delta - l.hidden*l.batch;
        எனில் (i == 0) s.delta = 0;
        backward_convolutional_layer(self_layer, s);

        copy_cpu(l.hidden*l.batch, self_layer.delta, 1, உள்ளீடு_layer.delta, 1);
        எனில் (i > 0 && l.shortcut) axpy_cpu(l.hidden*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.hidden*l.batch, 1);
        s.உள்ளீடு = net.உள்ளீடு + i*l.inputs*l.batch;
        எனில்(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        இல்லையெனில் s.delta = 0;
        backward_convolutional_layer(உள்ளீடு_layer, s);

        increment_layer(&உள்ளீடு_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

வெறுமை pull_crnn_layer(layer l)
{
    pull_convolutional_layer(*(l.உள்ளீடு_layer));
    pull_convolutional_layer(*(l.self_layer));
    pull_convolutional_layer(*(l.output_layer));
}

வெறுமை தள்ளு_crnn_layer(layer l)
{
    தள்ளு_convolutional_layer(*(l.உள்ளீடு_layer));
    தள்ளு_convolutional_layer(*(l.self_layer));
    தள்ளு_convolutional_layer(*(l.output_layer));
}

வெறுமை update_crnn_layer_gpu(layer l, update_args a)
{
    update_convolutional_layer_gpu(*(l.உள்ளீடு_layer),  a);
    update_convolutional_layer_gpu(*(l.self_layer),   a);
    update_convolutional_layer_gpu(*(l.output_layer), a);
}

வெறுமை forward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    முழு i;
    layer உள்ளீடு_layer = *(l.உள்ளீடு_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, உள்ளீடு_layer.delta_gpu, 1);
    எனில்(net.train) fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);

    க்கு (i = 0; i < l.steps; ++i) {
        s.உள்ளீடு_gpu = net.உள்ளீடு_gpu;
        forward_convolutional_layer_gpu(உள்ளீடு_layer, s);

        s.உள்ளீடு_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(self_layer, s);

        தசம *old_state = l.state_gpu;
        எனில்(net.train) l.state_gpu += l.hidden*l.batch;
        எனில்(l.shortcut){
            copy_gpu(l.hidden * l.batch, old_state, 1, l.state_gpu, 1);
        }இல்லையெனில்{
            fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);
        }
        axpy_gpu(l.hidden * l.batch, 1, உள்ளீடு_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.உள்ளீடு_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(output_layer, s);

        net.உள்ளீடு_gpu += l.inputs*l.batch;
        increment_layer(&உள்ளீடு_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

வெறுமை backward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    s.train = net.train;
    முழு i;
    layer உள்ளீடு_layer = *(l.உள்ளீடு_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&உள்ளீடு_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    l.state_gpu += l.hidden*l.batch*l.steps;
    க்கு (i = l.steps-1; i >= 0; --i) {
        copy_gpu(l.hidden * l.batch, உள்ளீடு_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.உள்ளீடு_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_convolutional_layer_gpu(output_layer, s);

        l.state_gpu -= l.hidden*l.batch;

        s.உள்ளீடு_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu - l.hidden*l.batch;
        எனில் (i == 0) s.delta_gpu = 0;
        backward_convolutional_layer_gpu(self_layer, s);

        copy_gpu(l.hidden*l.batch, self_layer.delta_gpu, 1, உள்ளீடு_layer.delta_gpu, 1);
        எனில் (i > 0 && l.shortcut) axpy_gpu(l.hidden*l.batch, 1, self_layer.delta_gpu, 1, self_layer.delta_gpu - l.hidden*l.batch, 1);
        s.உள்ளீடு_gpu = net.உள்ளீடு_gpu + i*l.inputs*l.batch;
        எனில்(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        இல்லையெனில் s.delta_gpu = 0;
        backward_convolutional_layer_gpu(உள்ளீடு_layer, s);

        increment_layer(&உள்ளீடு_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
}
#endif
