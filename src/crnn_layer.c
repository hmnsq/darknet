#ଅନ୍ତର୍ଭାଵ "crnn_layer.h"
#ଅନ୍ତର୍ଭାଵ "convolutional_layer.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ "gemm.h"

#ଅନ୍ତର୍ଭାଵ <math.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

ସ୍ଥିର ଶୂନ୍ୟ increment_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା steps)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ = l->outputs*l->batch*steps;
    l->output += ଅଙ୍କ;
    l->delta += ଅଙ୍କ;
    l->x += ଅଙ୍କ;
    l->x_norm += ଅଙ୍କ;

#ifdef GPU
    l->output_gpu += ଅଙ୍କ;
    l->delta_gpu += ଅଙ୍କ;
    l->x_gpu += ଅଙ୍କ;
    l->x_norm_gpu += ଅଙ୍କ;
#endif
}

layer make_crnn_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା hidden_filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା steps, ACTIVATION activation, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize)
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

    l.state = calloc(l.hidden*batch*(steps+1), ଆକାର(ଦଶମିକ));

    l.ନିବେଶ_layer = malloc(ଆକାର(layer));
    fprintf(stderr, "\t\t");
    *(l.ନିବେଶ_layer) = make_convolutional_layer(batch*steps, h, w, c, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.ନିବେଶ_layer->batch = batch;

    l.self_layer = malloc(ଆକାର(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_convolutional_layer(batch*steps, h, w, hidden_filters, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.self_layer->batch = batch;

    l.output_layer = malloc(ଆକାର(layer));
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

    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ update_crnn_layer(layer l, update_args a)
{
    update_convolutional_layer(*(l.ନିବେଶ_layer),  a);
    update_convolutional_layer(*(l.self_layer),   a);
    update_convolutional_layer(*(l.output_layer), a);
}

ଶୂନ୍ୟ forward_crnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    layer ନିବେଶ_layer = *(l.ନିବେଶ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, ନିବେଶ_layer.delta, 1);
    ଯଦି(net.train) fill_cpu(l.hidden * l.batch, 0, l.state, 1);

    ପାଇଁ (i = 0; i < l.steps; ++i) {
        s.ନିବେଶ = net.ନିବେଶ;
        forward_convolutional_layer(ନିବେଶ_layer, s);

        s.ନିବେଶ = l.state;
        forward_convolutional_layer(self_layer, s);

        ଦଶମିକ *old_state = l.state;
        ଯଦି(net.train) l.state += l.hidden*l.batch;
        ଯଦି(l.shortcut){
            copy_cpu(l.hidden * l.batch, old_state, 1, l.state, 1);
        }ଅନ୍ୟଥା{
            fill_cpu(l.hidden * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.hidden * l.batch, 1, ନିବେଶ_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ନିବେଶ = l.state;
        forward_convolutional_layer(output_layer, s);

        net.ନିବେଶ += l.inputs*l.batch;
        increment_layer(&ନିବେଶ_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ଶୂନ୍ୟ backward_crnn_layer(layer l, network net)
{
    network s = net;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    layer ନିବେଶ_layer = *(l.ନିବେଶ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&ନିବେଶ_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.hidden*l.batch*l.steps;
    ପାଇଁ (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.hidden * l.batch, ନିବେଶ_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ନିବେଶ = l.state;
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

        s.ନିବେଶ = l.state;
        s.delta = self_layer.delta - l.hidden*l.batch;
        ଯଦି (i == 0) s.delta = 0;
        backward_convolutional_layer(self_layer, s);

        copy_cpu(l.hidden*l.batch, self_layer.delta, 1, ନିବେଶ_layer.delta, 1);
        ଯଦି (i > 0 && l.shortcut) axpy_cpu(l.hidden*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.hidden*l.batch, 1);
        s.ନିବେଶ = net.ନିବେଶ + i*l.inputs*l.batch;
        ଯଦି(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        ଅନ୍ୟଥା s.delta = 0;
        backward_convolutional_layer(ନିବେଶ_layer, s);

        increment_layer(&ନିବେଶ_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

ଶୂନ୍ୟ pull_crnn_layer(layer l)
{
    pull_convolutional_layer(*(l.ନିବେଶ_layer));
    pull_convolutional_layer(*(l.self_layer));
    pull_convolutional_layer(*(l.output_layer));
}

ଶୂନ୍ୟ ଠେଲ_crnn_layer(layer l)
{
    ଠେଲ_convolutional_layer(*(l.ନିବେଶ_layer));
    ଠେଲ_convolutional_layer(*(l.self_layer));
    ଠେଲ_convolutional_layer(*(l.output_layer));
}

ଶୂନ୍ୟ update_crnn_layer_gpu(layer l, update_args a)
{
    update_convolutional_layer_gpu(*(l.ନିବେଶ_layer),  a);
    update_convolutional_layer_gpu(*(l.self_layer),   a);
    update_convolutional_layer_gpu(*(l.output_layer), a);
}

ଶୂନ୍ୟ forward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    layer ନିବେଶ_layer = *(l.ନିବେଶ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, ନିବେଶ_layer.delta_gpu, 1);
    ଯଦି(net.train) fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);

    ପାଇଁ (i = 0; i < l.steps; ++i) {
        s.ନିବେଶ_gpu = net.ନିବେଶ_gpu;
        forward_convolutional_layer_gpu(ନିବେଶ_layer, s);

        s.ନିବେଶ_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(self_layer, s);

        ଦଶମିକ *old_state = l.state_gpu;
        ଯଦି(net.train) l.state_gpu += l.hidden*l.batch;
        ଯଦି(l.shortcut){
            copy_gpu(l.hidden * l.batch, old_state, 1, l.state_gpu, 1);
        }ଅନ୍ୟଥା{
            fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);
        }
        axpy_gpu(l.hidden * l.batch, 1, ନିବେଶ_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ନିବେଶ_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(output_layer, s);

        net.ନିବେଶ_gpu += l.inputs*l.batch;
        increment_layer(&ନିବେଶ_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ଶୂନ୍ୟ backward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    s.train = net.train;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    layer ନିବେଶ_layer = *(l.ନିବେଶ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&ନିବେଶ_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    l.state_gpu += l.hidden*l.batch*l.steps;
    ପାଇଁ (i = l.steps-1; i >= 0; --i) {
        copy_gpu(l.hidden * l.batch, ନିବେଶ_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ନିବେଶ_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_convolutional_layer_gpu(output_layer, s);

        l.state_gpu -= l.hidden*l.batch;

        s.ନିବେଶ_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu - l.hidden*l.batch;
        ଯଦି (i == 0) s.delta_gpu = 0;
        backward_convolutional_layer_gpu(self_layer, s);

        copy_gpu(l.hidden*l.batch, self_layer.delta_gpu, 1, ନିବେଶ_layer.delta_gpu, 1);
        ଯଦି (i > 0 && l.shortcut) axpy_gpu(l.hidden*l.batch, 1, self_layer.delta_gpu, 1, self_layer.delta_gpu - l.hidden*l.batch, 1);
        s.ନିବେଶ_gpu = net.ନିବେଶ_gpu + i*l.inputs*l.batch;
        ଯଦି(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        ଅନ୍ୟଥା s.delta_gpu = 0;
        backward_convolutional_layer_gpu(ନିବେଶ_layer, s);

        increment_layer(&ନିବେଶ_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
}
#endif
