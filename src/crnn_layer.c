#অন্তর্ভাৱ "crnn_layer.h"
#অন্তর্ভাৱ "convolutional_layer.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "cuda.h"
#অন্তর্ভাৱ "blas.h"
#অন্তর্ভাৱ "gemm.h"

#অন্তর্ভাৱ <math.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <string.h>

স্থির শূন্য increment_layer(layer *l, পূর্ণাঙ্ক steps)
{
    পূর্ণাঙ্ক অঙ্ক = l->outputs*l->batch*steps;
    l->output += অঙ্ক;
    l->delta += অঙ্ক;
    l->x += অঙ্ক;
    l->x_norm += অঙ্ক;

#ifdef GPU
    l->output_gpu += অঙ্ক;
    l->delta_gpu += অঙ্ক;
    l->x_gpu += অঙ্ক;
    l->x_norm_gpu += অঙ্ক;
#endif
}

layer make_crnn_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক h, পূর্ণাঙ্ক w, পূর্ণাঙ্ক c, পূর্ণাঙ্ক hidden_filters, পূর্ণাঙ্ক output_filters, পূর্ণাঙ্ক steps, ACTIVATION activation, পূর্ণাঙ্ক batch_normalize)
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

    l.state = calloc(l.hidden*batch*(steps+1), পরিমাণ(ভিন্ন));

    l.প্ৰবেশ_layer = malloc(পরিমাণ(layer));
    fprintf(stderr, "\t\t");
    *(l.প্ৰবেশ_layer) = make_convolutional_layer(batch*steps, h, w, c, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.প্ৰবেশ_layer->batch = batch;

    l.self_layer = malloc(পরিমাণ(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_convolutional_layer(batch*steps, h, w, hidden_filters, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.self_layer->batch = batch;

    l.output_layer = malloc(পরিমাণ(layer));
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

    ঘূৰাই l;
}

শূন্য update_crnn_layer(layer l, update_args a)
{
    update_convolutional_layer(*(l.প্ৰবেশ_layer),  a);
    update_convolutional_layer(*(l.self_layer),   a);
    update_convolutional_layer(*(l.output_layer), a);
}

শূন্য forward_crnn_layer(layer l, network net)
{
    network s = net;
    s.train = net.train;
    পূর্ণাঙ্ক i;
    layer প্ৰবেশ_layer = *(l.প্ৰবেশ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.steps, 0, output_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, self_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.steps, 0, প্ৰবেশ_layer.delta, 1);
    যদি(net.train) fill_cpu(l.hidden * l.batch, 0, l.state, 1);

    বাবে (i = 0; i < l.steps; ++i) {
        s.প্ৰবেশ = net.প্ৰবেশ;
        forward_convolutional_layer(প্ৰবেশ_layer, s);

        s.প্ৰবেশ = l.state;
        forward_convolutional_layer(self_layer, s);

        ভিন্ন *old_state = l.state;
        যদি(net.train) l.state += l.hidden*l.batch;
        যদি(l.shortcut){
            copy_cpu(l.hidden * l.batch, old_state, 1, l.state, 1);
        }অন্যথা{
            fill_cpu(l.hidden * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.hidden * l.batch, 1, প্ৰবেশ_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.প্ৰবেশ = l.state;
        forward_convolutional_layer(output_layer, s);

        net.প্ৰবেশ += l.inputs*l.batch;
        increment_layer(&প্ৰবেশ_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

শূন্য backward_crnn_layer(layer l, network net)
{
    network s = net;
    পূর্ণাঙ্ক i;
    layer প্ৰবেশ_layer = *(l.প্ৰবেশ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&প্ৰবেশ_layer, l.steps-1);
    increment_layer(&self_layer, l.steps-1);
    increment_layer(&output_layer, l.steps-1);

    l.state += l.hidden*l.batch*l.steps;
    বাবে (i = l.steps-1; i >= 0; --i) {
        copy_cpu(l.hidden * l.batch, প্ৰবেশ_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.প্ৰবেশ = l.state;
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

        s.প্ৰবেশ = l.state;
        s.delta = self_layer.delta - l.hidden*l.batch;
        যদি (i == 0) s.delta = 0;
        backward_convolutional_layer(self_layer, s);

        copy_cpu(l.hidden*l.batch, self_layer.delta, 1, প্ৰবেশ_layer.delta, 1);
        যদি (i > 0 && l.shortcut) axpy_cpu(l.hidden*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.hidden*l.batch, 1);
        s.প্ৰবেশ = net.প্ৰবেশ + i*l.inputs*l.batch;
        যদি(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        অন্যথা s.delta = 0;
        backward_convolutional_layer(প্ৰবেশ_layer, s);

        increment_layer(&প্ৰবেশ_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#ifdef GPU

শূন্য pull_crnn_layer(layer l)
{
    pull_convolutional_layer(*(l.প্ৰবেশ_layer));
    pull_convolutional_layer(*(l.self_layer));
    pull_convolutional_layer(*(l.output_layer));
}

শূন্য ঠেলো_crnn_layer(layer l)
{
    ঠেলো_convolutional_layer(*(l.প্ৰবেশ_layer));
    ঠেলো_convolutional_layer(*(l.self_layer));
    ঠেলো_convolutional_layer(*(l.output_layer));
}

শূন্য update_crnn_layer_gpu(layer l, update_args a)
{
    update_convolutional_layer_gpu(*(l.প্ৰবেশ_layer),  a);
    update_convolutional_layer_gpu(*(l.self_layer),   a);
    update_convolutional_layer_gpu(*(l.output_layer), a);
}

শূন্য forward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    পূর্ণাঙ্ক i;
    layer প্ৰবেশ_layer = *(l.প্ৰবেশ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.steps, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.steps, 0, প্ৰবেশ_layer.delta_gpu, 1);
    যদি(net.train) fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);

    বাবে (i = 0; i < l.steps; ++i) {
        s.প্ৰবেশ_gpu = net.প্ৰবেশ_gpu;
        forward_convolutional_layer_gpu(প্ৰবেশ_layer, s);

        s.প্ৰবেশ_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(self_layer, s);

        ভিন্ন *old_state = l.state_gpu;
        যদি(net.train) l.state_gpu += l.hidden*l.batch;
        যদি(l.shortcut){
            copy_gpu(l.hidden * l.batch, old_state, 1, l.state_gpu, 1);
        }অন্যথা{
            fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);
        }
        axpy_gpu(l.hidden * l.batch, 1, প্ৰবেশ_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.প্ৰবেশ_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(output_layer, s);

        net.প্ৰবেশ_gpu += l.inputs*l.batch;
        increment_layer(&প্ৰবেশ_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

শূন্য backward_crnn_layer_gpu(layer l, network net)
{
    network s = net;
    s.train = net.train;
    পূর্ণাঙ্ক i;
    layer প্ৰবেশ_layer = *(l.প্ৰবেশ_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&প্ৰবেশ_layer,  l.steps - 1);
    increment_layer(&self_layer,   l.steps - 1);
    increment_layer(&output_layer, l.steps - 1);
    l.state_gpu += l.hidden*l.batch*l.steps;
    বাবে (i = l.steps-1; i >= 0; --i) {
        copy_gpu(l.hidden * l.batch, প্ৰবেশ_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.প্ৰবেশ_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_convolutional_layer_gpu(output_layer, s);

        l.state_gpu -= l.hidden*l.batch;

        s.প্ৰবেশ_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu - l.hidden*l.batch;
        যদি (i == 0) s.delta_gpu = 0;
        backward_convolutional_layer_gpu(self_layer, s);

        copy_gpu(l.hidden*l.batch, self_layer.delta_gpu, 1, প্ৰবেশ_layer.delta_gpu, 1);
        যদি (i > 0 && l.shortcut) axpy_gpu(l.hidden*l.batch, 1, self_layer.delta_gpu, 1, self_layer.delta_gpu - l.hidden*l.batch, 1);
        s.প্ৰবেশ_gpu = net.প্ৰবেশ_gpu + i*l.inputs*l.batch;
        যদি(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        অন্যথা s.delta_gpu = 0;
        backward_convolutional_layer_gpu(প্ৰবেশ_layer, s);

        increment_layer(&প্ৰবেশ_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
}
#endif
