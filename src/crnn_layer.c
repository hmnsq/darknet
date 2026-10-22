#ਅਨ੍ਤਰ੍ਭਾਵ "crnn_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "convolutional_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gemm.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

ਸਥਿਰ ਖਾਲੀ increment_layer(layer *l, ਪੂਰਨ ਕਦਮਾਂ)
{
    ਪੂਰਨ ਅੰਕ = l->outputs*l->batch*ਕਦਮਾਂ;
    l->output += ਅੰਕ;
    l->delta += ਅੰਕ;
    l->x += ਅੰਕ;
    l->x_norm += ਅੰਕ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l->output_gpu += ਅੰਕ;
    l->delta_gpu += ਅੰਕ;
    l->x_gpu += ਅੰਕ;
    l->x_norm_gpu += ਅੰਕ;
#ਜੇ_ਅੰਤ
}

layer make_crnn_layer(ਪੂਰਨ batch, ਪੂਰਨ h, ਪੂਰਨ w, ਪੂਰਨ c, ਪੂਰਨ hidden_filters, ਪੂਰਨ output_filters, ਪੂਰਨ ਕਦਮਾਂ, ACTIVATION activation, ਪੂਰਨ batch_normalize)
{
    fprintf(stderr, "CRNN Layer: %d x %d x %d image, %d filters\n", h,w,c,output_filters);
    batch = batch / ਕਦਮਾਂ;
    layer l = {0};
    l.batch = batch;
    l.type = CRNN;
    l.ਕਦਮਾਂ = ਕਦਮਾਂ;
    l.h = h;
    l.w = w;
    l.c = c;
    l.ਬਾਹਰਲਾ_h = h;
    l.ਬਾਹਰਲਾ_w = w;
    l.ਬਾਹਰਲਾ_c = output_filters;
    l.inputs = h*w*c;
    l.hidden = h * w * hidden_filters;
    l.outputs = l.ਬਾਹਰਲਾ_h * l.ਬਾਹਰਲਾ_w * l.ਬਾਹਰਲਾ_c;

    l.state = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.hidden*batch*(ਕਦਮਾਂ+1), ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.ਨਿਵੇਸ਼_layer = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.ਨਿਵੇਸ਼_layer) = make_convolutional_layer(batch*ਕਦਮਾਂ, h, w, c, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.ਨਿਵੇਸ਼_layer->batch = batch;

    l.self_layer = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_convolutional_layer(batch*ਕਦਮਾਂ, h, w, hidden_filters, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.self_layer->batch = batch;

    l.output_layer = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.output_layer) = make_convolutional_layer(batch*ਕਦਮਾਂ, h, w, hidden_filters, output_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.output_layer->batch = batch;

    l.output = l.output_layer->output;
    l.delta = l.output_layer->delta;

    l.forward = forward_crnn_layer;
    l.backward = backward_crnn_layer;
    l.update = update_crnn_layer;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_crnn_layer_gpu;
    l.backward_gpu = backward_crnn_layer_gpu;
    l.update_gpu = update_crnn_layer_gpu;

    l.state_gpu = cuda_make_array(l.state, l.hidden*batch*(ਕਦਮਾਂ+1));
    l.output_gpu = l.output_layer->output_gpu;
    l.delta_gpu = l.output_layer->delta_gpu;
#ਜੇ_ਅੰਤ

    ਵਾਪਸ l;
}

ਖਾਲੀ update_crnn_layer(layer l, update_ਦਲੀਲਾਂ a)
{
    update_convolutional_layer(*(l.ਨਿਵੇਸ਼_layer),  a);
    update_convolutional_layer(*(l.self_layer),   a);
    update_convolutional_layer(*(l.output_layer), a);
}

ਖਾਲੀ forward_crnn_layer(layer l, ਜਾਲ net)
{
    ਜਾਲ s = net;
    s.train = net.train;
    ਪੂਰਨ i;
    layer ਨਿਵੇਸ਼_layer = *(l.ਨਿਵੇਸ਼_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, output_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.ਕਦਮਾਂ, 0, self_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.ਕਦਮਾਂ, 0, ਨਿਵੇਸ਼_layer.delta, 1);
    ਜੇ(net.train) fill_cpu(l.hidden * l.batch, 0, l.state, 1);

    ਲਈ (i = 0; i < l.ਕਦਮਾਂ; ++i) {
        s.ਨਿਵੇਸ਼ = net.ਨਿਵੇਸ਼;
        forward_convolutional_layer(ਨਿਵੇਸ਼_layer, s);

        s.ਨਿਵੇਸ਼ = l.state;
        forward_convolutional_layer(self_layer, s);

        ਦਸ਼ਮਲਵ *old_state = l.state;
        ਜੇ(net.train) l.state += l.hidden*l.batch;
        ਜੇ(l.shortcut){
            copy_cpu(l.hidden * l.batch, old_state, 1, l.state, 1);
        }ਨਹੀਂਤਾਂ{
            fill_cpu(l.hidden * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.hidden * l.batch, 1, ਨਿਵੇਸ਼_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ਨਿਵੇਸ਼ = l.state;
        forward_convolutional_layer(output_layer, s);

        net.ਨਿਵੇਸ਼ += l.inputs*l.batch;
        increment_layer(&ਨਿਵੇਸ਼_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ਖਾਲੀ backward_crnn_layer(layer l, ਜਾਲ net)
{
    ਜਾਲ s = net;
    ਪੂਰਨ i;
    layer ਨਿਵੇਸ਼_layer = *(l.ਨਿਵੇਸ਼_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&ਨਿਵੇਸ਼_layer, l.ਕਦਮਾਂ-1);
    increment_layer(&self_layer, l.ਕਦਮਾਂ-1);
    increment_layer(&output_layer, l.ਕਦਮਾਂ-1);

    l.state += l.hidden*l.batch*l.ਕਦਮਾਂ;
    ਲਈ (i = l.ਕਦਮਾਂ-1; i >= 0; --i) {
        copy_cpu(l.hidden * l.batch, ਨਿਵੇਸ਼_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ਨਿਵੇਸ਼ = l.state;
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

        s.ਨਿਵੇਸ਼ = l.state;
        s.delta = self_layer.delta - l.hidden*l.batch;
        ਜੇ (i == 0) s.delta = 0;
        backward_convolutional_layer(self_layer, s);

        copy_cpu(l.hidden*l.batch, self_layer.delta, 1, ਨਿਵੇਸ਼_layer.delta, 1);
        ਜੇ (i > 0 && l.shortcut) axpy_cpu(l.hidden*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.hidden*l.batch, 1);
        s.ਨਿਵੇਸ਼ = net.ਨਿਵੇਸ਼ + i*l.inputs*l.batch;
        ਜੇ(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        ਨਹੀਂਤਾਂ s.delta = 0;
        backward_convolutional_layer(ਨਿਵੇਸ਼_layer, s);

        increment_layer(&ਨਿਵੇਸ਼_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ pull_crnn_layer(layer l)
{
    pull_convolutional_layer(*(l.ਨਿਵੇਸ਼_layer));
    pull_convolutional_layer(*(l.self_layer));
    pull_convolutional_layer(*(l.output_layer));
}

ਖਾਲੀ ਧੱਕੋ_crnn_layer(layer l)
{
    ਧੱਕੋ_convolutional_layer(*(l.ਨਿਵੇਸ਼_layer));
    ਧੱਕੋ_convolutional_layer(*(l.self_layer));
    ਧੱਕੋ_convolutional_layer(*(l.output_layer));
}

ਖਾਲੀ update_crnn_layer_gpu(layer l, update_ਦਲੀਲਾਂ a)
{
    update_convolutional_layer_gpu(*(l.ਨਿਵੇਸ਼_layer),  a);
    update_convolutional_layer_gpu(*(l.self_layer),   a);
    update_convolutional_layer_gpu(*(l.output_layer), a);
}

ਖਾਲੀ forward_crnn_layer_gpu(layer l, ਜਾਲ net)
{
    ਜਾਲ s = net;
    ਪੂਰਨ i;
    layer ਨਿਵੇਸ਼_layer = *(l.ਨਿਵੇਸ਼_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.ਕਦਮਾਂ, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.ਕਦਮਾਂ, 0, ਨਿਵੇਸ਼_layer.delta_gpu, 1);
    ਜੇ(net.train) fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);

    ਲਈ (i = 0; i < l.ਕਦਮਾਂ; ++i) {
        s.ਨਿਵੇਸ਼_gpu = net.ਨਿਵੇਸ਼_gpu;
        forward_convolutional_layer_gpu(ਨਿਵੇਸ਼_layer, s);

        s.ਨਿਵੇਸ਼_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(self_layer, s);

        ਦਸ਼ਮਲਵ *old_state = l.state_gpu;
        ਜੇ(net.train) l.state_gpu += l.hidden*l.batch;
        ਜੇ(l.shortcut){
            copy_gpu(l.hidden * l.batch, old_state, 1, l.state_gpu, 1);
        }ਨਹੀਂਤਾਂ{
            fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);
        }
        axpy_gpu(l.hidden * l.batch, 1, ਨਿਵੇਸ਼_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ਨਿਵੇਸ਼_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(output_layer, s);

        net.ਨਿਵੇਸ਼_gpu += l.inputs*l.batch;
        increment_layer(&ਨਿਵੇਸ਼_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

ਖਾਲੀ backward_crnn_layer_gpu(layer l, ਜਾਲ net)
{
    ਜਾਲ s = net;
    s.train = net.train;
    ਪੂਰਨ i;
    layer ਨਿਵੇਸ਼_layer = *(l.ਨਿਵੇਸ਼_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&ਨਿਵੇਸ਼_layer,  l.ਕਦਮਾਂ - 1);
    increment_layer(&self_layer,   l.ਕਦਮਾਂ - 1);
    increment_layer(&output_layer, l.ਕਦਮਾਂ - 1);
    l.state_gpu += l.hidden*l.batch*l.ਕਦਮਾਂ;
    ਲਈ (i = l.ਕਦਮਾਂ-1; i >= 0; --i) {
        copy_gpu(l.hidden * l.batch, ਨਿਵੇਸ਼_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ਨਿਵੇਸ਼_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_convolutional_layer_gpu(output_layer, s);

        l.state_gpu -= l.hidden*l.batch;

        s.ਨਿਵੇਸ਼_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu - l.hidden*l.batch;
        ਜੇ (i == 0) s.delta_gpu = 0;
        backward_convolutional_layer_gpu(self_layer, s);

        copy_gpu(l.hidden*l.batch, self_layer.delta_gpu, 1, ਨਿਵੇਸ਼_layer.delta_gpu, 1);
        ਜੇ (i > 0 && l.shortcut) axpy_gpu(l.hidden*l.batch, 1, self_layer.delta_gpu, 1, self_layer.delta_gpu - l.hidden*l.batch, 1);
        s.ਨਿਵੇਸ਼_gpu = net.ਨਿਵੇਸ਼_gpu + i*l.inputs*l.batch;
        ਜੇ(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        ਨਹੀਂਤਾਂ s.delta_gpu = 0;
        backward_convolutional_layer_gpu(ਨਿਵੇਸ਼_layer, s);

        increment_layer(&ਨਿਵੇਸ਼_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
}
#ਜੇ_ਅੰਤ
