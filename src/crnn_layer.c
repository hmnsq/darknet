#અન્તર્ભાવ "crnn_layer.h"
#અન્તર્ભાવ "convolutional_layer.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ "gemm.h"

#અન્તર્ભાવ <math.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>

સ્થિર શૂન્ય increment_layer(layer *l, પૂર્ણાંક steps)
{
    પૂર્ણાંક અંક = l->outputs*l->batch*steps;
    l->output += અંક;
    l->delta += અંક;
    l->x += અંક;
    l->x_norm += અંક;

#ifdef GPU
    l->output_gpu += અંક;
    l->delta_gpu += અંક;
    l->x_gpu += અંક;
    l->x_norm_gpu += અંક;
#endif
}

layer make_crnn_layer(પૂર્ણાંક batch, પૂર્ણાંક h, પૂર્ણાંક w, પૂર્ણાંક c, પૂર્ણાંક hidden_filters, પૂર્ણાંક output_filters, પૂર્ણાંક steps, ACTIVATION activation, પૂર્ણાંક batch_normalize)
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

    l.state = calloc(l.hidden*batch*(steps+1), કદ(દશાંશ));

    l.આગત_layer = malloc(કદ(layer));
    fprintf(stderr, "\t\t");
    *(l.આગત_layer) = make_convolutional_layer(batch*steps, h, w, c, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.આગત_layer->batch = batch;

    l.self_layer = malloc(કદ(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_convolutional_layer(batch*steps, h, w, hidden_filters, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.self_layer->batch = batch;

    l.output_layer = malloc(કદ(layer));
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

    પાછા l;
}

શૂન્ય update_crnn_layer(layer l, update_args a)
{
    update_convolutional_layer(*(l.આગત_layer),  a);
    update_convolutional_layer(*(l.self_layer),   a);
    update_convolutional_layer(*(l.output_layer), a);
}

શૂન્ય forward_crnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    પૂર્ણાંક i;
    layer આગત_layer = *(l.આગત_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, આગત_layer.delta, 1);
    જો(net.train) fill_cpu(l.hidden * l.batch, 0, l.state, 1);

    માટે (i = 0; i < l.steps; ++i) {
        s.આગત = net.આગત;
        forward_convolutional_layer(આગત_layer, s);

        s.આગત = l.state;
        forward_convolutional_layer(self_layer, s);

        દશાંશ *old_state = l.state;
        જો(net.train) l.state += l.hidden*l.batch;
        જો(l.shortcut){
            copy_cpu(l.hidden * l.batch, old_state, 1, l.state, 1);
        }નહીંતર{
            fill_cpu(l.hidden * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.hidden * l.batch, 1, આગત_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.આગત = l.state;
        forward_convolutional_layer(output_layer, s);

        net.આગત += l.inputs*l.batch;
        increment_layer(&આગત_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

શૂન્ય backward_crnn_layer(layer l, network net)
{
    network s = net;
    પૂર્ણાંક i;
    layer આગત_layer = *(l.આગત_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&આગત_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.hidden*l.batch*l.steps;
    માટે (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.hidden * l.batch, આગત_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.આગત = l.state;
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

        s.આગત = l.state;
        s.delta = self_layer.delta - l.hidden*l.batch;
        જો (i == 0) s.delta = 0;
        backward_convolutional_layer(self_layer, s);

        copy_cpu(l.hidden*l.batch, self_layer.delta, 1, આગત_layer.delta, 1);
        જો (i > 0 && l.shortcut) axpy_cpu(l.hidden*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.hidden*l.batch, 1);
        s.આગત = net.આગત + i*l.inputs*l.batch;
        જો(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        નહીંતર s.delta = 0;
        backward_convolutional_layer(આગત_layer, s);

        increment_layer(&આગત_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

શૂન્ય pull_crnn_layer(layer l)
{
    pull_convolutional_layer(*(l.આગત_layer));
    pull_convolutional_layer(*(l.self_layer));
    pull_convolutional_layer(*(l.output_layer));
}

શૂન્ય ધકેલો_crnn_layer(layer l)
{
    ધકેલો_convolutional_layer(*(l.આગત_layer));
    ધકેલો_convolutional_layer(*(l.self_layer));
    ધકેલો_convolutional_layer(*(l.output_layer));
}

શૂન્ય update_crnn_layer_gpu(layer l, update_args a)
{
    update_convolutional_layer_gpu(*(l.આગત_layer),  a);
    update_convolutional_layer_gpu(*(l.self_layer),   a);
    update_convolutional_layer_gpu(*(l.output_layer), a);
}

શૂન્ય forward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    પૂર્ણાંક i;
    layer આગત_layer = *(l.આગત_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, આગત_layer.delta_gpu, 1);
    જો(net.train) fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);

    માટે (i = 0; i < l.steps; ++i) {
        s.આગત_gpu = net.આગત_gpu;
        forward_convolutional_layer_gpu(આગત_layer, s);

        s.આગત_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(self_layer, s);

        દશાંશ *old_state = l.state_gpu;
        જો(net.train) l.state_gpu += l.hidden*l.batch;
        જો(l.shortcut){
            copy_gpu(l.hidden * l.batch, old_state, 1, l.state_gpu, 1);
        }નહીંતર{
            fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);
        }
        axpy_gpu(l.hidden * l.batch, 1, આગત_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.આગત_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(output_layer, s);

        net.આગત_gpu += l.inputs*l.batch;
        increment_layer(&આગત_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

શૂન્ય backward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    s.train = net.train;
    પૂર્ણાંક i;
    layer આગત_layer = *(l.આગત_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&આગત_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    l.state_gpu += l.hidden*l.batch*l.steps;
    માટે (i = l.steps-1; i >= 0; --i) {
        copy_gpu(l.hidden * l.batch, આગત_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.આગત_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_convolutional_layer_gpu(output_layer, s);

        l.state_gpu -= l.hidden*l.batch;

        s.આગત_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu - l.hidden*l.batch;
        જો (i == 0) s.delta_gpu = 0;
        backward_convolutional_layer_gpu(self_layer, s);

        copy_gpu(l.hidden*l.batch, self_layer.delta_gpu, 1, આગત_layer.delta_gpu, 1);
        જો (i > 0 && l.shortcut) axpy_gpu(l.hidden*l.batch, 1, self_layer.delta_gpu, 1, self_layer.delta_gpu - l.hidden*l.batch, 1);
        s.આગત_gpu = net.આગત_gpu + i*l.inputs*l.batch;
        જો(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        નહીંતર s.delta_gpu = 0;
        backward_convolutional_layer_gpu(આગત_layer, s);

        increment_layer(&આગત_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
}
#endif
