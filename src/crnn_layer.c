#ضمن "crnn_layer.h"
#ضمن "convolutional_layer.h"
#ضمن "utils.h"
#ضمن "cuda.h"
#ضمن "blas.h"
#ضمن "gemm.h"

#ضمن <math.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>

ساكن فراغ increment_layer(layer *l, صحيح steps)
{
    صحيح رقم = l->outputs*l->batch*steps;
    l->output += رقم;
    l->delta += رقم;
    l->x += رقم;
    l->x_norm += رقم;

#ifdef GPU
    l->output_gpu += رقم;
    l->delta_gpu += رقم;
    l->x_gpu += رقم;
    l->x_norm_gpu += رقم;
#endif
}

layer make_crnn_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح hidden_filters, صحيح output_filters, صحيح steps, ACTIVATION activation, صحيح batch_normalize)
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

    l.state = calloc(l.hidden*batch*(steps+1), حجم(عائم));

    l.إدخال_layer = malloc(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.إدخال_layer) = make_convolutional_layer(batch*steps, h, w, c, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.إدخال_layer->batch = batch;

    l.self_layer = malloc(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_convolutional_layer(batch*steps, h, w, hidden_filters, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.self_layer->batch = batch;

    l.output_layer = malloc(حجم(layer));
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

    إرجاع l;
}

فراغ update_crnn_layer(layer l, update_args a)
{
    update_convolutional_layer(*(l.إدخال_layer),  a);
    update_convolutional_layer(*(l.self_layer),   a);
    update_convolutional_layer(*(l.output_layer), a);
}

فراغ forward_crnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    صحيح i;
    layer إدخال_layer = *(l.إدخال_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, إدخال_layer.delta, 1);
    إذا(net.train) fill_cpu(l.hidden * l.batch, 0, l.state, 1);

    لكل (i = 0; i < l.steps; ++i) {
        s.إدخال = net.إدخال;
        forward_convolutional_layer(إدخال_layer, s);

        s.إدخال = l.state;
        forward_convolutional_layer(self_layer, s);

        عائم *old_state = l.state;
        إذا(net.train) l.state += l.hidden*l.batch;
        إذا(l.shortcut){
            copy_cpu(l.hidden * l.batch, old_state, 1, l.state, 1);
        }وإلا{
            fill_cpu(l.hidden * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.hidden * l.batch, 1, إدخال_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.إدخال = l.state;
        forward_convolutional_layer(output_layer, s);

        net.إدخال += l.inputs*l.batch;
        increment_layer(&إدخال_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

فراغ backward_crnn_layer(layer l, network net)
{
    network s = net;
    صحيح i;
    layer إدخال_layer = *(l.إدخال_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&إدخال_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.hidden*l.batch*l.steps;
    لكل (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.hidden * l.batch, إدخال_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.إدخال = l.state;
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

        s.إدخال = l.state;
        s.delta = self_layer.delta - l.hidden*l.batch;
        إذا (i == 0) s.delta = 0;
        backward_convolutional_layer(self_layer, s);

        copy_cpu(l.hidden*l.batch, self_layer.delta, 1, إدخال_layer.delta, 1);
        إذا (i > 0 && l.shortcut) axpy_cpu(l.hidden*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.hidden*l.batch, 1);
        s.إدخال = net.إدخال + i*l.inputs*l.batch;
        إذا(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        وإلا s.delta = 0;
        backward_convolutional_layer(إدخال_layer, s);

        increment_layer(&إدخال_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

فراغ pull_crnn_layer(layer l)
{
    pull_convolutional_layer(*(l.إدخال_layer));
    pull_convolutional_layer(*(l.self_layer));
    pull_convolutional_layer(*(l.output_layer));
}

فراغ ادفع_crnn_layer(layer l)
{
    ادفع_convolutional_layer(*(l.إدخال_layer));
    ادفع_convolutional_layer(*(l.self_layer));
    ادفع_convolutional_layer(*(l.output_layer));
}

فراغ update_crnn_layer_gpu(layer l, update_args a)
{
    update_convolutional_layer_gpu(*(l.إدخال_layer),  a);
    update_convolutional_layer_gpu(*(l.self_layer),   a);
    update_convolutional_layer_gpu(*(l.output_layer), a);
}

فراغ forward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    صحيح i;
    layer إدخال_layer = *(l.إدخال_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, إدخال_layer.delta_gpu, 1);
    إذا(net.train) fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);

    لكل (i = 0; i < l.steps; ++i) {
        s.إدخال_gpu = net.إدخال_gpu;
        forward_convolutional_layer_gpu(إدخال_layer, s);

        s.إدخال_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(self_layer, s);

        عائم *old_state = l.state_gpu;
        إذا(net.train) l.state_gpu += l.hidden*l.batch;
        إذا(l.shortcut){
            copy_gpu(l.hidden * l.batch, old_state, 1, l.state_gpu, 1);
        }وإلا{
            fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);
        }
        axpy_gpu(l.hidden * l.batch, 1, إدخال_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.إدخال_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(output_layer, s);

        net.إدخال_gpu += l.inputs*l.batch;
        increment_layer(&إدخال_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

فراغ backward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    s.train = net.train;
    صحيح i;
    layer إدخال_layer = *(l.إدخال_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&إدخال_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    l.state_gpu += l.hidden*l.batch*l.steps;
    لكل (i = l.steps-1; i >= 0; --i) {
        copy_gpu(l.hidden * l.batch, إدخال_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.إدخال_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_convolutional_layer_gpu(output_layer, s);

        l.state_gpu -= l.hidden*l.batch;

        s.إدخال_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu - l.hidden*l.batch;
        إذا (i == 0) s.delta_gpu = 0;
        backward_convolutional_layer_gpu(self_layer, s);

        copy_gpu(l.hidden*l.batch, self_layer.delta_gpu, 1, إدخال_layer.delta_gpu, 1);
        إذا (i > 0 && l.shortcut) axpy_gpu(l.hidden*l.batch, 1, self_layer.delta_gpu, 1, self_layer.delta_gpu - l.hidden*l.batch, 1);
        s.إدخال_gpu = net.إدخال_gpu + i*l.inputs*l.batch;
        إذا(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        وإلا s.delta_gpu = 0;
        backward_convolutional_layer_gpu(إدخال_layer, s);

        increment_layer(&إدخال_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
}
#endif
