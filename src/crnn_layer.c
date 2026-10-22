#अन्तर्भाव "crnn_layer.h"
#अन्तर्भाव "convolutional_layer.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "gemm.h"

#अन्तर्भाव <math.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

स्थिर शून्य increment_layer(layer *l, पूर्णाङ्क steps)
{
    पूर्णाङ्क अङ्कः = l->outputs*l->batch*steps;
    l->output += अङ्कः;
    l->delta += अङ्कः;
    l->x += अङ्कः;
    l->x_norm += अङ्कः;

#ifdef GPU
    l->output_gpu += अङ्कः;
    l->delta_gpu += अङ्कः;
    l->x_gpu += अङ्कः;
    l->x_norm_gpu += अङ्कः;
#endif
}

layer make_crnn_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क hidden_filters, पूर्णाङ्क output_filters, पूर्णाङ्क steps, ACTIVATION activation, पूर्णाङ्क batch_normalize)
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

    l.state = calloc(l.hidden*batch*(steps+1), परिमाण(भिन्न));

    l.निवेशः_layer = malloc(परिमाण(layer));
    fprintf(stderr, "\t\t");
    *(l.निवेशः_layer) = make_convolutional_layer(batch*steps, h, w, c, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.निवेशः_layer->batch = batch;

    l.self_layer = malloc(परिमाण(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_convolutional_layer(batch*steps, h, w, hidden_filters, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.self_layer->batch = batch;

    l.output_layer = malloc(परिमाण(layer));
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

    पटिनिवत्त l;
}

शून्य update_crnn_layer(layer l, update_args a)
{
    update_convolutional_layer(*(l.निवेशः_layer),  a);
    update_convolutional_layer(*(l.self_layer),   a);
    update_convolutional_layer(*(l.output_layer), a);
}

शून्य forward_crnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    पूर्णाङ्क i;
    layer निवेशः_layer = *(l.निवेशः_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, निवेशः_layer.delta, 1);
    सचे(net.train) fill_cpu(l.hidden * l.batch, 0, l.state, 1);

    अत्थाय (i = 0; i < l.steps; ++i) {
        s.निवेशः = net.निवेशः;
        forward_convolutional_layer(निवेशः_layer, s);

        s.निवेशः = l.state;
        forward_convolutional_layer(self_layer, s);

        भिन्न *old_state = l.state;
        सचे(net.train) l.state += l.hidden*l.batch;
        सचे(l.shortcut){
            copy_cpu(l.hidden * l.batch, old_state, 1, l.state, 1);
        }अञ्ञथा{
            fill_cpu(l.hidden * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.hidden * l.batch, 1, निवेशः_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.निवेशः = l.state;
        forward_convolutional_layer(output_layer, s);

        net.निवेशः += l.inputs*l.batch;
        increment_layer(&निवेशः_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

शून्य backward_crnn_layer(layer l, network net)
{
    network s = net;
    पूर्णाङ्क i;
    layer निवेशः_layer = *(l.निवेशः_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&निवेशः_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.hidden*l.batch*l.steps;
    अत्थाय (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.hidden * l.batch, निवेशः_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.निवेशः = l.state;
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

        s.निवेशः = l.state;
        s.delta = self_layer.delta - l.hidden*l.batch;
        सचे (i == 0) s.delta = 0;
        backward_convolutional_layer(self_layer, s);

        copy_cpu(l.hidden*l.batch, self_layer.delta, 1, निवेशः_layer.delta, 1);
        सचे (i > 0 && l.shortcut) axpy_cpu(l.hidden*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.hidden*l.batch, 1);
        s.निवेशः = net.निवेशः + i*l.inputs*l.batch;
        सचे(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        अञ्ञथा s.delta = 0;
        backward_convolutional_layer(निवेशः_layer, s);

        increment_layer(&निवेशः_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

शून्य pull_crnn_layer(layer l)
{
    pull_convolutional_layer(*(l.निवेशः_layer));
    pull_convolutional_layer(*(l.self_layer));
    pull_convolutional_layer(*(l.output_layer));
}

शून्य प्रक्षिप_crnn_layer(layer l)
{
    प्रक्षिप_convolutional_layer(*(l.निवेशः_layer));
    प्रक्षिप_convolutional_layer(*(l.self_layer));
    प्रक्षिप_convolutional_layer(*(l.output_layer));
}

शून्य update_crnn_layer_gpu(layer l, update_args a)
{
    update_convolutional_layer_gpu(*(l.निवेशः_layer),  a);
    update_convolutional_layer_gpu(*(l.self_layer),   a);
    update_convolutional_layer_gpu(*(l.output_layer), a);
}

शून्य forward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    पूर्णाङ्क i;
    layer निवेशः_layer = *(l.निवेशः_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, निवेशः_layer.delta_gpu, 1);
    सचे(net.train) fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);

    अत्थाय (i = 0; i < l.steps; ++i) {
        s.निवेशः_gpu = net.निवेशः_gpu;
        forward_convolutional_layer_gpu(निवेशः_layer, s);

        s.निवेशः_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(self_layer, s);

        भिन्न *old_state = l.state_gpu;
        सचे(net.train) l.state_gpu += l.hidden*l.batch;
        सचे(l.shortcut){
            copy_gpu(l.hidden * l.batch, old_state, 1, l.state_gpu, 1);
        }अञ्ञथा{
            fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);
        }
        axpy_gpu(l.hidden * l.batch, 1, निवेशः_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.निवेशः_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(output_layer, s);

        net.निवेशः_gpu += l.inputs*l.batch;
        increment_layer(&निवेशः_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

शून्य backward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    s.train = net.train;
    पूर्णाङ्क i;
    layer निवेशः_layer = *(l.निवेशः_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&निवेशः_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    l.state_gpu += l.hidden*l.batch*l.steps;
    अत्थाय (i = l.steps-1; i >= 0; --i) {
        copy_gpu(l.hidden * l.batch, निवेशः_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.निवेशः_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_convolutional_layer_gpu(output_layer, s);

        l.state_gpu -= l.hidden*l.batch;

        s.निवेशः_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu - l.hidden*l.batch;
        सचे (i == 0) s.delta_gpu = 0;
        backward_convolutional_layer_gpu(self_layer, s);

        copy_gpu(l.hidden*l.batch, self_layer.delta_gpu, 1, निवेशः_layer.delta_gpu, 1);
        सचे (i > 0 && l.shortcut) axpy_gpu(l.hidden*l.batch, 1, self_layer.delta_gpu, 1, self_layer.delta_gpu - l.hidden*l.batch, 1);
        s.निवेशः_gpu = net.निवेशः_gpu + i*l.inputs*l.batch;
        सचे(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        अञ्ञथा s.delta_gpu = 0;
        backward_convolutional_layer_gpu(निवेशः_layer, s);

        increment_layer(&निवेशः_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
}
#endif
