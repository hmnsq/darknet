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

ساکن خالی increment_layer(layer *l, صحیح اقدام)
{
    صحیح عدد = l->outputs*l->batch*اقدام;
    l->output += عدد;
    l->delta += عدد;
    l->x += عدد;
    l->x_norm += عدد;

#اگر_تعریف_شدہ GPU
    l->output_gpu += عدد;
    l->delta_gpu += عدد;
    l->x_gpu += عدد;
    l->x_norm_gpu += عدد;
#اگر_ختم
}

layer make_crnn_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح hidden_filters, صحیح output_filters, صحیح اقدام, ACTIVATION activation, صحیح batch_normalize)
{
    fprintf(stderr, "CRNN Layer: %d x %d x %d image, %d filters\n", h,w,c,output_filters);
    batch = batch / اقدام;
    layer l = {0};
    l.batch = batch;
    l.type = CRNN;
    l.اقدام = اقدام;
    l.h = h;
    l.w = w;
    l.c = c;
    l.برآمد_h = h;
    l.برآمد_w = w;
    l.برآمد_c = output_filters;
    l.inputs = h*w*c;
    l.hidden = h * w * hidden_filters;
    l.outputs = l.برآمد_h * l.برآمد_w * l.برآمد_c;

    l.state = صفر_حافظہ_دو(l.hidden*batch*(اقدام+1), حجم(اعشاری));

    l.داخلہ_layer = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.داخلہ_layer) = make_convolutional_layer(batch*اقدام, h, w, c, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.داخلہ_layer->batch = batch;

    l.self_layer = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_convolutional_layer(batch*اقدام, h, w, hidden_filters, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.self_layer->batch = batch;

    l.output_layer = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.output_layer) = make_convolutional_layer(batch*اقدام, h, w, hidden_filters, output_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.output_layer->batch = batch;

    l.output = l.output_layer->output;
    l.delta = l.output_layer->delta;

    l.forward = forward_crnn_layer;
    l.backward = backward_crnn_layer;
    l.update = update_crnn_layer;

#اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_crnn_layer_gpu;
    l.backward_gpu = backward_crnn_layer_gpu;
    l.update_gpu = update_crnn_layer_gpu;

    l.state_gpu = cuda_make_array(l.state, l.hidden*batch*(اقدام+1));
    l.output_gpu = l.output_layer->output_gpu;
    l.delta_gpu = l.output_layer->delta_gpu;
#اگر_ختم

    واپس l;
}

خالی update_crnn_layer(layer l, update_دلائل a)
{
    update_convolutional_layer(*(l.داخلہ_layer),  a);
    update_convolutional_layer(*(l.self_layer),   a);
    update_convolutional_layer(*(l.output_layer), a);
}

خالی forward_crnn_layer(layer l, جال net)
{
    جال s = net;
    s.train = net.train;
    صحیح i;
    layer داخلہ_layer = *(l.داخلہ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.اقدام, 0, output_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.اقدام, 0, self_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.اقدام, 0, داخلہ_layer.delta, 1);
    اگر(net.train) fill_cpu(l.hidden * l.batch, 0, l.state, 1);

    برائے (i = 0; i < l.اقدام; ++i) {
        s.داخلہ = net.داخلہ;
        forward_convolutional_layer(داخلہ_layer, s);

        s.داخلہ = l.state;
        forward_convolutional_layer(self_layer, s);

        اعشاری *old_state = l.state;
        اگر(net.train) l.state += l.hidden*l.batch;
        اگر(l.shortcut){
            copy_cpu(l.hidden * l.batch, old_state, 1, l.state, 1);
        }ورنہ{
            fill_cpu(l.hidden * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.hidden * l.batch, 1, داخلہ_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.داخلہ = l.state;
        forward_convolutional_layer(output_layer, s);

        net.داخلہ += l.inputs*l.batch;
        increment_layer(&داخلہ_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

خالی backward_crnn_layer(layer l, جال net)
{
    جال s = net;
    صحیح i;
    layer داخلہ_layer = *(l.داخلہ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&داخلہ_layer, l.اقدام-1);
    increment_layer(&self_layer, l.اقدام-1);
    increment_layer(&output_layer, l.اقدام-1);

    l.state += l.hidden*l.batch*l.اقدام;
    برائے (i = l.اقدام-1; i >= 0; --i) {
        copy_cpu(l.hidden * l.batch, داخلہ_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.داخلہ = l.state;
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

        s.داخلہ = l.state;
        s.delta = self_layer.delta - l.hidden*l.batch;
        اگر (i == 0) s.delta = 0;
        backward_convolutional_layer(self_layer, s);

        copy_cpu(l.hidden*l.batch, self_layer.delta, 1, داخلہ_layer.delta, 1);
        اگر (i > 0 && l.shortcut) axpy_cpu(l.hidden*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.hidden*l.batch, 1);
        s.داخلہ = net.داخلہ + i*l.inputs*l.batch;
        اگر(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        ورنہ s.delta = 0;
        backward_convolutional_layer(داخلہ_layer, s);

        increment_layer(&داخلہ_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#اگر_تعریف_شدہ GPU

خالی pull_crnn_layer(layer l)
{
    pull_convolutional_layer(*(l.داخلہ_layer));
    pull_convolutional_layer(*(l.self_layer));
    pull_convolutional_layer(*(l.output_layer));
}

خالی ڈالو_crnn_layer(layer l)
{
    ڈالو_convolutional_layer(*(l.داخلہ_layer));
    ڈالو_convolutional_layer(*(l.self_layer));
    ڈالو_convolutional_layer(*(l.output_layer));
}

خالی update_crnn_layer_gpu(layer l, update_دلائل a)
{
    update_convolutional_layer_gpu(*(l.داخلہ_layer),  a);
    update_convolutional_layer_gpu(*(l.self_layer),   a);
    update_convolutional_layer_gpu(*(l.output_layer), a);
}

خالی forward_crnn_layer_gpu(layer l, جال net)
{
    جال s = net;
    صحیح i;
    layer داخلہ_layer = *(l.داخلہ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.اقدام, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.اقدام, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.اقدام, 0, داخلہ_layer.delta_gpu, 1);
    اگر(net.train) fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);

    برائے (i = 0; i < l.اقدام; ++i) {
        s.داخلہ_gpu = net.داخلہ_gpu;
        forward_convolutional_layer_gpu(داخلہ_layer, s);

        s.داخلہ_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(self_layer, s);

        اعشاری *old_state = l.state_gpu;
        اگر(net.train) l.state_gpu += l.hidden*l.batch;
        اگر(l.shortcut){
            copy_gpu(l.hidden * l.batch, old_state, 1, l.state_gpu, 1);
        }ورنہ{
            fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);
        }
        axpy_gpu(l.hidden * l.batch, 1, داخلہ_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.داخلہ_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(output_layer, s);

        net.داخلہ_gpu += l.inputs*l.batch;
        increment_layer(&داخلہ_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

خالی backward_crnn_layer_gpu(layer l, جال net)
{
    جال s = net;
    s.train = net.train;
    صحیح i;
    layer داخلہ_layer = *(l.داخلہ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&داخلہ_layer,  l.اقدام - 1);
    increment_layer(&self_layer,   l.اقدام - 1);
    increment_layer(&output_layer, l.اقدام - 1);
    l.state_gpu += l.hidden*l.batch*l.اقدام;
    برائے (i = l.اقدام-1; i >= 0; --i) {
        copy_gpu(l.hidden * l.batch, داخلہ_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.داخلہ_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_convolutional_layer_gpu(output_layer, s);

        l.state_gpu -= l.hidden*l.batch;

        s.داخلہ_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu - l.hidden*l.batch;
        اگر (i == 0) s.delta_gpu = 0;
        backward_convolutional_layer_gpu(self_layer, s);

        copy_gpu(l.hidden*l.batch, self_layer.delta_gpu, 1, داخلہ_layer.delta_gpu, 1);
        اگر (i > 0 && l.shortcut) axpy_gpu(l.hidden*l.batch, 1, self_layer.delta_gpu, 1, self_layer.delta_gpu - l.hidden*l.batch, 1);
        s.داخلہ_gpu = net.داخلہ_gpu + i*l.inputs*l.batch;
        اگر(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        ورنہ s.delta_gpu = 0;
        backward_convolutional_layer_gpu(داخلہ_layer, s);

        increment_layer(&داخلہ_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
}
#اگر_ختم
