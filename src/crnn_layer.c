#അന്തര്ഭാവ "crnn_layer.h"
#അന്തര്ഭാവ "convolutional_layer.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "gemm.h"

#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>

സ്ഥിതം ശൂന്യം increment_layer(layer *l, പൂർണ്ണസംഖ്യ steps)
{
    പൂർണ്ണസംഖ്യ അക്കം = l->outputs*l->batch*steps;
    l->output += അക്കം;
    l->delta += അക്കം;
    l->x += അക്കം;
    l->x_norm += അക്കം;

#ifdef GPU
    l->output_gpu += അക്കം;
    l->delta_gpu += അക്കം;
    l->x_gpu += അക്കം;
    l->x_norm_gpu += അക്കം;
#endif
}

layer make_crnn_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ hidden_filters, പൂർണ്ണസംഖ്യ output_filters, പൂർണ്ണസംഖ്യ steps, ACTIVATION activation, പൂർണ്ണസംഖ്യ batch_normalize)
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

    l.state = calloc(l.hidden*batch*(steps+1), വലുപ്പം(ദശാംശം));

    l.നിവേശം_layer = malloc(വലുപ്പം(layer));
    fprintf(stderr, "\t\t");
    *(l.നിവേശം_layer) = make_convolutional_layer(batch*steps, h, w, c, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.നിവേശം_layer->batch = batch;

    l.self_layer = malloc(വലുപ്പം(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_convolutional_layer(batch*steps, h, w, hidden_filters, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.self_layer->batch = batch;

    l.output_layer = malloc(വലുപ്പം(layer));
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

    തിരികെ l;
}

ശൂന്യം update_crnn_layer(layer l, update_args a)
{
    update_convolutional_layer(*(l.നിവേശം_layer),  a);
    update_convolutional_layer(*(l.self_layer),   a);
    update_convolutional_layer(*(l.output_layer), a);
}

ശൂന്യം forward_crnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    പൂർണ്ണസംഖ്യ i;
    layer നിവേശം_layer = *(l.നിവേശം_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, നിവേശം_layer.delta, 1);
    എങ്കിൽ(net.train) fill_cpu(l.hidden * l.batch, 0, l.state, 1);

    വേണ്ടി (i = 0; i < l.steps; ++i) {
        s.നിവേശം = net.നിവേശം;
        forward_convolutional_layer(നിവേശം_layer, s);

        s.നിവേശം = l.state;
        forward_convolutional_layer(self_layer, s);

        ദശാംശം *old_state = l.state;
        എങ്കിൽ(net.train) l.state += l.hidden*l.batch;
        എങ്കിൽ(l.shortcut){
            copy_cpu(l.hidden * l.batch, old_state, 1, l.state, 1);
        }അല്ലെങ്കിൽ{
            fill_cpu(l.hidden * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.hidden * l.batch, 1, നിവേശം_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.നിവേശം = l.state;
        forward_convolutional_layer(output_layer, s);

        net.നിവേശം += l.inputs*l.batch;
        increment_layer(&നിവേശം_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ശൂന്യം backward_crnn_layer(layer l, network net)
{
    network s = net;
    പൂർണ്ണസംഖ്യ i;
    layer നിവേശം_layer = *(l.നിവേശം_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&നിവേശം_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.hidden*l.batch*l.steps;
    വേണ്ടി (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.hidden * l.batch, നിവേശം_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.നിവേശം = l.state;
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

        s.നിവേശം = l.state;
        s.delta = self_layer.delta - l.hidden*l.batch;
        എങ്കിൽ (i == 0) s.delta = 0;
        backward_convolutional_layer(self_layer, s);

        copy_cpu(l.hidden*l.batch, self_layer.delta, 1, നിവേശം_layer.delta, 1);
        എങ്കിൽ (i > 0 && l.shortcut) axpy_cpu(l.hidden*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.hidden*l.batch, 1);
        s.നിവേശം = net.നിവേശം + i*l.inputs*l.batch;
        എങ്കിൽ(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        അല്ലെങ്കിൽ s.delta = 0;
        backward_convolutional_layer(നിവേശം_layer, s);

        increment_layer(&നിവേശം_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

ശൂന്യം pull_crnn_layer(layer l)
{
    pull_convolutional_layer(*(l.നിവേശം_layer));
    pull_convolutional_layer(*(l.self_layer));
    pull_convolutional_layer(*(l.output_layer));
}

ശൂന്യം തള്ളുക_crnn_layer(layer l)
{
    തള്ളുക_convolutional_layer(*(l.നിവേശം_layer));
    തള്ളുക_convolutional_layer(*(l.self_layer));
    തള്ളുക_convolutional_layer(*(l.output_layer));
}

ശൂന്യം update_crnn_layer_gpu(layer l, update_args a)
{
    update_convolutional_layer_gpu(*(l.നിവേശം_layer),  a);
    update_convolutional_layer_gpu(*(l.self_layer),   a);
    update_convolutional_layer_gpu(*(l.output_layer), a);
}

ശൂന്യം forward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    പൂർണ്ണസംഖ്യ i;
    layer നിവേശം_layer = *(l.നിവേശം_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, നിവേശം_layer.delta_gpu, 1);
    എങ്കിൽ(net.train) fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);

    വേണ്ടി (i = 0; i < l.steps; ++i) {
        s.നിവേശം_gpu = net.നിവേശം_gpu;
        forward_convolutional_layer_gpu(നിവേശം_layer, s);

        s.നിവേശം_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(self_layer, s);

        ദശാംശം *old_state = l.state_gpu;
        എങ്കിൽ(net.train) l.state_gpu += l.hidden*l.batch;
        എങ്കിൽ(l.shortcut){
            copy_gpu(l.hidden * l.batch, old_state, 1, l.state_gpu, 1);
        }അല്ലെങ്കിൽ{
            fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);
        }
        axpy_gpu(l.hidden * l.batch, 1, നിവേശം_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.നിവേശം_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(output_layer, s);

        net.നിവേശം_gpu += l.inputs*l.batch;
        increment_layer(&നിവേശം_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ശൂന്യം backward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    s.train = net.train;
    പൂർണ്ണസംഖ്യ i;
    layer നിവേശം_layer = *(l.നിവേശം_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&നിവേശം_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    l.state_gpu += l.hidden*l.batch*l.steps;
    വേണ്ടി (i = l.steps-1; i >= 0; --i) {
        copy_gpu(l.hidden * l.batch, നിവേശം_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.നിവേശം_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_convolutional_layer_gpu(output_layer, s);

        l.state_gpu -= l.hidden*l.batch;

        s.നിവേശം_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu - l.hidden*l.batch;
        എങ്കിൽ (i == 0) s.delta_gpu = 0;
        backward_convolutional_layer_gpu(self_layer, s);

        copy_gpu(l.hidden*l.batch, self_layer.delta_gpu, 1, നിവേശം_layer.delta_gpu, 1);
        എങ്കിൽ (i > 0 && l.shortcut) axpy_gpu(l.hidden*l.batch, 1, self_layer.delta_gpu, 1, self_layer.delta_gpu - l.hidden*l.batch, 1);
        s.നിവേശം_gpu = net.നിവേശം_gpu + i*l.inputs*l.batch;
        എങ്കിൽ(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        അല്ലെങ്കിൽ s.delta_gpu = 0;
        backward_convolutional_layer_gpu(നിവേശം_layer, s);

        increment_layer(&നിവേശം_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
}
#endif
