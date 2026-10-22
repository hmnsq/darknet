#समावेश "crnn_layer.h"
#समावेश "convolutional_layer.h"
#समावेश "utils.h"
#समावेश "cuda.h"
#समावेश "blas.h"
#समावेश "gemm.h"

#समावेश <math.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <string.h>

अटल व्योम increment_layer(layer *l, पूर्णांक steps)
{
    पूर्णांक अंक = l->outputs*l->batch*steps;
    l->output += अंक;
    l->delta += अंक;
    l->x += अंक;
    l->x_norm += अंक;

#अगर_घोषित GPU
    l->output_gpu += अंक;
    l->delta_gpu += अंक;
    l->x_gpu += अंक;
    l->x_norm_gpu += अंक;
#पूर्ण_अगर
}

layer make_crnn_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक hidden_filters, पूर्णांक output_filters, पूर्णांक steps, ACTIVATION activation, पूर्णांक batch_normalize)
{
    ख_लिखो(मानक_त्रुटि, "CRNN Layer: %d x %d x %d image, %d filters\n", h,w,c,output_filters);
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

    l.state = सुस्मृति(l.hidden*batch*(steps+1), माप(भग्न));

    l.निवेश_layer = दो_स्मृति(माप(layer));
    ख_लिखो(मानक_त्रुटि, "\t\t");
    *(l.निवेश_layer) = make_convolutional_layer(batch*steps, h, w, c, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.निवेश_layer->batch = batch;

    l.self_layer = दो_स्मृति(माप(layer));
    ख_लिखो(मानक_त्रुटि, "\t\t");
    *(l.self_layer) = make_convolutional_layer(batch*steps, h, w, hidden_filters, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.self_layer->batch = batch;

    l.output_layer = दो_स्मृति(माप(layer));
    ख_लिखो(मानक_त्रुटि, "\t\t");
    *(l.output_layer) = make_convolutional_layer(batch*steps, h, w, hidden_filters, output_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.output_layer->batch = batch;

    l.output = l.output_layer->output;
    l.delta = l.output_layer->delta;

    l.forward = forward_crnn_layer;
    l.backward = backward_crnn_layer;
    l.update = update_crnn_layer;

#अगर_घोषित GPU
    l.forward_gpu = forward_crnn_layer_gpu;
    l.backward_gpu = backward_crnn_layer_gpu;
    l.update_gpu = update_crnn_layer_gpu;

    l.state_gpu = cuda_make_array(l.state, l.hidden*batch*(steps+1));
    l.output_gpu = l.output_layer->output_gpu;
    l.delta_gpu = l.output_layer->delta_gpu;
#पूर्ण_अगर

    वापस l;
}

व्योम update_crnn_layer(layer l, update_args a)
{
    update_convolutional_layer(*(l.निवेश_layer),  a);
    update_convolutional_layer(*(l.self_layer),   a);
    update_convolutional_layer(*(l.output_layer), a);
}

व्योम forward_crnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    पूर्णांक i;
    layer निवेश_layer = *(l.निवेश_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, निवेश_layer.delta, 1);
    अगर(net.train) fill_cpu(l.hidden * l.batch, 0, l.state, 1);

    क्रम (i = 0; i < l.steps; ++i) {
        s.निवेश = net.निवेश;
        forward_convolutional_layer(निवेश_layer, s);

        s.निवेश = l.state;
        forward_convolutional_layer(self_layer, s);

        भग्न *old_state = l.state;
        अगर(net.train) l.state += l.hidden*l.batch;
        अगर(l.shortcut){
            copy_cpu(l.hidden * l.batch, old_state, 1, l.state, 1);
        }अन्यथा{
            fill_cpu(l.hidden * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.hidden * l.batch, 1, निवेश_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.निवेश = l.state;
        forward_convolutional_layer(output_layer, s);

        net.निवेश += l.inputs*l.batch;
        increment_layer(&निवेश_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

व्योम backward_crnn_layer(layer l, network net)
{
    network s = net;
    पूर्णांक i;
    layer निवेश_layer = *(l.निवेश_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&निवेश_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.hidden*l.batch*l.steps;
    क्रम (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.hidden * l.batch, निवेश_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.निवेश = l.state;
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

        s.निवेश = l.state;
        s.delta = self_layer.delta - l.hidden*l.batch;
        अगर (i == 0) s.delta = 0;
        backward_convolutional_layer(self_layer, s);

        copy_cpu(l.hidden*l.batch, self_layer.delta, 1, निवेश_layer.delta, 1);
        अगर (i > 0 && l.shortcut) axpy_cpu(l.hidden*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.hidden*l.batch, 1);
        s.निवेश = net.निवेश + i*l.inputs*l.batch;
        अगर(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        अन्यथा s.delta = 0;
        backward_convolutional_layer(निवेश_layer, s);

        increment_layer(&निवेश_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#अगर_घोषित GPU

व्योम pull_crnn_layer(layer l)
{
    pull_convolutional_layer(*(l.निवेश_layer));
    pull_convolutional_layer(*(l.self_layer));
    pull_convolutional_layer(*(l.output_layer));
}

व्योम धकेलो_crnn_layer(layer l)
{
    धकेलो_convolutional_layer(*(l.निवेश_layer));
    धकेलो_convolutional_layer(*(l.self_layer));
    धकेलो_convolutional_layer(*(l.output_layer));
}

व्योम update_crnn_layer_gpu(layer l, update_args a)
{
    update_convolutional_layer_gpu(*(l.निवेश_layer),  a);
    update_convolutional_layer_gpu(*(l.self_layer),   a);
    update_convolutional_layer_gpu(*(l.output_layer), a);
}

व्योम forward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    पूर्णांक i;
    layer निवेश_layer = *(l.निवेश_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, निवेश_layer.delta_gpu, 1);
    अगर(net.train) fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);

    क्रम (i = 0; i < l.steps; ++i) {
        s.निवेश_gpu = net.निवेश_gpu;
        forward_convolutional_layer_gpu(निवेश_layer, s);

        s.निवेश_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(self_layer, s);

        भग्न *old_state = l.state_gpu;
        अगर(net.train) l.state_gpu += l.hidden*l.batch;
        अगर(l.shortcut){
            copy_gpu(l.hidden * l.batch, old_state, 1, l.state_gpu, 1);
        }अन्यथा{
            fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);
        }
        axpy_gpu(l.hidden * l.batch, 1, निवेश_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.निवेश_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(output_layer, s);

        net.निवेश_gpu += l.inputs*l.batch;
        increment_layer(&निवेश_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

व्योम backward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    s.train = net.train;
    पूर्णांक i;
    layer निवेश_layer = *(l.निवेश_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&निवेश_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    l.state_gpu += l.hidden*l.batch*l.steps;
    क्रम (i = l.steps-1; i >= 0; --i) {
        copy_gpu(l.hidden * l.batch, निवेश_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.निवेश_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_convolutional_layer_gpu(output_layer, s);

        l.state_gpu -= l.hidden*l.batch;

        s.निवेश_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu - l.hidden*l.batch;
        अगर (i == 0) s.delta_gpu = 0;
        backward_convolutional_layer_gpu(self_layer, s);

        copy_gpu(l.hidden*l.batch, self_layer.delta_gpu, 1, निवेश_layer.delta_gpu, 1);
        अगर (i > 0 && l.shortcut) axpy_gpu(l.hidden*l.batch, 1, self_layer.delta_gpu, 1, self_layer.delta_gpu - l.hidden*l.batch, 1);
        s.निवेश_gpu = net.निवेश_gpu + i*l.inputs*l.batch;
        अगर(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        अन्यथा s.delta_gpu = 0;
        backward_convolutional_layer_gpu(निवेश_layer, s);

        increment_layer(&निवेश_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
}
#पूर्ण_अगर
