#شامل "crnn_layer.h"
#شامل "convolutional_layer.h"
#شامل "utils.h"
#شامل "cuda.h"
#شامل "blas.h"
#شامل "gemm.h"

#شامل <math.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>

ایستا تهی increment_layer(layer *l, صحیح steps)
{
    صحیح رقم = l->outputs*l->batch*steps;
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

layer make_crnn_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح hidden_filters, صحیح output_filters, صحیح steps, ACTIVATION activation, صحیح batch_normalize)
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

    l.state = calloc(l.hidden*batch*(steps+1), اندازه(اعشاری));

    l.درون_داد_layer = malloc(اندازه(layer));
    fprintf(stderr, "\t\t");
    *(l.درون_داد_layer) = make_convolutional_layer(batch*steps, h, w, c, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.درون_داد_layer->batch = batch;

    l.self_layer = malloc(اندازه(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_convolutional_layer(batch*steps, h, w, hidden_filters, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.self_layer->batch = batch;

    l.output_layer = malloc(اندازه(layer));
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

    بازگشت l;
}

تهی update_crnn_layer(layer l, update_args a)
{
    update_convolutional_layer(*(l.درون_داد_layer),  a);
    update_convolutional_layer(*(l.self_layer),   a);
    update_convolutional_layer(*(l.output_layer), a);
}

تهی forward_crnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    صحیح i;
    layer درون_داد_layer = *(l.درون_داد_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, درون_داد_layer.delta, 1);
    اگر(net.train) fill_cpu(l.hidden * l.batch, 0, l.state, 1);

    برای (i = 0; i < l.steps; ++i) {
        s.درون_داد = net.درون_داد;
        forward_convolutional_layer(درون_داد_layer, s);

        s.درون_داد = l.state;
        forward_convolutional_layer(self_layer, s);

        اعشاری *old_state = l.state;
        اگر(net.train) l.state += l.hidden*l.batch;
        اگر(l.shortcut){
            copy_cpu(l.hidden * l.batch, old_state, 1, l.state, 1);
        }وگرنه{
            fill_cpu(l.hidden * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.hidden * l.batch, 1, درون_داد_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.درون_داد = l.state;
        forward_convolutional_layer(output_layer, s);

        net.درون_داد += l.inputs*l.batch;
        increment_layer(&درون_داد_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

تهی backward_crnn_layer(layer l, network net)
{
    network s = net;
    صحیح i;
    layer درون_داد_layer = *(l.درون_داد_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&درون_داد_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.hidden*l.batch*l.steps;
    برای (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.hidden * l.batch, درون_داد_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.درون_داد = l.state;
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

        s.درون_داد = l.state;
        s.delta = self_layer.delta - l.hidden*l.batch;
        اگر (i == 0) s.delta = 0;
        backward_convolutional_layer(self_layer, s);

        copy_cpu(l.hidden*l.batch, self_layer.delta, 1, درون_داد_layer.delta, 1);
        اگر (i > 0 && l.shortcut) axpy_cpu(l.hidden*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.hidden*l.batch, 1);
        s.درون_داد = net.درون_داد + i*l.inputs*l.batch;
        اگر(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        وگرنه s.delta = 0;
        backward_convolutional_layer(درون_داد_layer, s);

        increment_layer(&درون_داد_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

تهی pull_crnn_layer(layer l)
{
    pull_convolutional_layer(*(l.درون_داد_layer));
    pull_convolutional_layer(*(l.self_layer));
    pull_convolutional_layer(*(l.output_layer));
}

تهی افزودن_crnn_layer(layer l)
{
    افزودن_convolutional_layer(*(l.درون_داد_layer));
    افزودن_convolutional_layer(*(l.self_layer));
    افزودن_convolutional_layer(*(l.output_layer));
}

تهی update_crnn_layer_gpu(layer l, update_args a)
{
    update_convolutional_layer_gpu(*(l.درون_داد_layer),  a);
    update_convolutional_layer_gpu(*(l.self_layer),   a);
    update_convolutional_layer_gpu(*(l.output_layer), a);
}

تهی forward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    صحیح i;
    layer درون_داد_layer = *(l.درون_داد_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, درون_داد_layer.delta_gpu, 1);
    اگر(net.train) fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);

    برای (i = 0; i < l.steps; ++i) {
        s.درون_داد_gpu = net.درون_داد_gpu;
        forward_convolutional_layer_gpu(درون_داد_layer, s);

        s.درون_داد_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(self_layer, s);

        اعشاری *old_state = l.state_gpu;
        اگر(net.train) l.state_gpu += l.hidden*l.batch;
        اگر(l.shortcut){
            copy_gpu(l.hidden * l.batch, old_state, 1, l.state_gpu, 1);
        }وگرنه{
            fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);
        }
        axpy_gpu(l.hidden * l.batch, 1, درون_داد_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.درون_داد_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(output_layer, s);

        net.درون_داد_gpu += l.inputs*l.batch;
        increment_layer(&درون_داد_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

تهی backward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    s.train = net.train;
    صحیح i;
    layer درون_داد_layer = *(l.درون_داد_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&درون_داد_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    l.state_gpu += l.hidden*l.batch*l.steps;
    برای (i = l.steps-1; i >= 0; --i) {
        copy_gpu(l.hidden * l.batch, درون_داد_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.درون_داد_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_convolutional_layer_gpu(output_layer, s);

        l.state_gpu -= l.hidden*l.batch;

        s.درون_داد_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu - l.hidden*l.batch;
        اگر (i == 0) s.delta_gpu = 0;
        backward_convolutional_layer_gpu(self_layer, s);

        copy_gpu(l.hidden*l.batch, self_layer.delta_gpu, 1, درون_داد_layer.delta_gpu, 1);
        اگر (i > 0 && l.shortcut) axpy_gpu(l.hidden*l.batch, 1, self_layer.delta_gpu, 1, self_layer.delta_gpu - l.hidden*l.batch, 1);
        s.درون_داد_gpu = net.درون_داد_gpu + i*l.inputs*l.batch;
        اگر(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        وگرنه s.delta_gpu = 0;
        backward_convolutional_layer_gpu(درون_داد_layer, s);

        increment_layer(&درون_داد_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
}
#endif
