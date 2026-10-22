#అన్తర్భావ "crnn_layer.h"
#అన్తర్భావ "convolutional_layer.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "gemm.h"

#అన్తర్భావ <math.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>

స్థిర శూన్య increment_layer(layer *l, పూర్ణాంకం అడుగులు)
{
    పూర్ణాంకం అంకె = l->outputs*l->batch*అడుగులు;
    l->output += అంకె;
    l->delta += అంకె;
    l->x += అంకె;
    l->x_norm += అంకె;

#నిర్వచితమైతే GPU
    l->output_gpu += అంకె;
    l->delta_gpu += అంకె;
    l->x_gpu += అంకె;
    l->x_norm_gpu += అంకె;
#అయితే_ముగింపు
}

layer make_crnn_layer(పూర్ణాంకం batch, పూర్ణాంకం h, పూర్ణాంకం w, పూర్ణాంకం c, పూర్ణాంకం hidden_filters, పూర్ణాంకం output_filters, పూర్ణాంకం అడుగులు, ACTIVATION activation, పూర్ణాంకం batch_normalize)
{
    fprintf(stderr, "CRNN Layer: %d x %d x %d image, %d filters\n", h,w,c,output_filters);
    batch = batch / అడుగులు;
    layer l = {0};
    l.batch = batch;
    l.type = CRNN;
    l.అడుగులు = అడుగులు;
    l.h = h;
    l.w = w;
    l.c = c;
    l.బహిర్గతం_h = h;
    l.బహిర్గతం_w = w;
    l.బహిర్గతం_c = output_filters;
    l.inputs = h*w*c;
    l.hidden = h * w * hidden_filters;
    l.outputs = l.బహిర్గతం_h * l.బహిర్గతం_w * l.బహిర్గతం_c;

    l.state = సున్నా_జ్ఞాపకం_ఇవ్వు(l.hidden*batch*(అడుగులు+1), పరిమాణం(దశాంశం));

    l.ఆదానం_layer = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.ఆదానం_layer) = make_convolutional_layer(batch*అడుగులు, h, w, c, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.ఆదానం_layer->batch = batch;

    l.self_layer = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_convolutional_layer(batch*అడుగులు, h, w, hidden_filters, hidden_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.self_layer->batch = batch;

    l.output_layer = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.output_layer) = make_convolutional_layer(batch*అడుగులు, h, w, hidden_filters, output_filters, 1, 3, 1, 1,  activation, batch_normalize, 0, 0, 0);
    l.output_layer->batch = batch;

    l.output = l.output_layer->output;
    l.delta = l.output_layer->delta;

    l.forward = forward_crnn_layer;
    l.backward = backward_crnn_layer;
    l.update = update_crnn_layer;

#నిర్వచితమైతే GPU
    l.forward_gpu = forward_crnn_layer_gpu;
    l.backward_gpu = backward_crnn_layer_gpu;
    l.update_gpu = update_crnn_layer_gpu;

    l.state_gpu = cuda_make_array(l.state, l.hidden*batch*(అడుగులు+1));
    l.output_gpu = l.output_layer->output_gpu;
    l.delta_gpu = l.output_layer->delta_gpu;
#అయితే_ముగింపు

    తిరిగి l;
}

శూన్య update_crnn_layer(layer l, update_వాదనలు a)
{
    update_convolutional_layer(*(l.ఆదానం_layer),  a);
    update_convolutional_layer(*(l.self_layer),   a);
    update_convolutional_layer(*(l.output_layer), a);
}

శూన్య forward_crnn_layer(layer l, జాలం net)
{
    జాలం s = net;
    s.train = net.train;
    పూర్ణాంకం i;
    layer ఆదానం_layer = *(l.ఆదానం_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, output_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.అడుగులు, 0, self_layer.delta, 1);
    fill_cpu(l.hidden * l.batch * l.అడుగులు, 0, ఆదానం_layer.delta, 1);
    ఒకవేళ(net.train) fill_cpu(l.hidden * l.batch, 0, l.state, 1);

    కొరకు (i = 0; i < l.అడుగులు; ++i) {
        s.ఆదానం = net.ఆదానం;
        forward_convolutional_layer(ఆదానం_layer, s);

        s.ఆదానం = l.state;
        forward_convolutional_layer(self_layer, s);

        దశాంశం *old_state = l.state;
        ఒకవేళ(net.train) l.state += l.hidden*l.batch;
        ఒకవేళ(l.shortcut){
            copy_cpu(l.hidden * l.batch, old_state, 1, l.state, 1);
        }లేకపోతే{
            fill_cpu(l.hidden * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.hidden * l.batch, 1, ఆదానం_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ఆదానం = l.state;
        forward_convolutional_layer(output_layer, s);

        net.ఆదానం += l.inputs*l.batch;
        increment_layer(&ఆదానం_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

శూన్య backward_crnn_layer(layer l, జాలం net)
{
    జాలం s = net;
    పూర్ణాంకం i;
    layer ఆదానం_layer = *(l.ఆదానం_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&ఆదానం_layer, l.అడుగులు-1);
    increment_layer(&self_layer, l.అడుగులు-1);
    increment_layer(&output_layer, l.అడుగులు-1);

    l.state += l.hidden*l.batch*l.అడుగులు;
    కొరకు (i = l.అడుగులు-1; i >= 0; --i) {
        copy_cpu(l.hidden * l.batch, ఆదానం_layer.output, 1, l.state, 1);
        axpy_cpu(l.hidden * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ఆదానం = l.state;
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

        s.ఆదానం = l.state;
        s.delta = self_layer.delta - l.hidden*l.batch;
        ఒకవేళ (i == 0) s.delta = 0;
        backward_convolutional_layer(self_layer, s);

        copy_cpu(l.hidden*l.batch, self_layer.delta, 1, ఆదానం_layer.delta, 1);
        ఒకవేళ (i > 0 && l.shortcut) axpy_cpu(l.hidden*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.hidden*l.batch, 1);
        s.ఆదానం = net.ఆదానం + i*l.inputs*l.batch;
        ఒకవేళ(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        లేకపోతే s.delta = 0;
        backward_convolutional_layer(ఆదానం_layer, s);

        increment_layer(&ఆదానం_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#నిర్వచితమైతే GPU

శూన్య pull_crnn_layer(layer l)
{
    pull_convolutional_layer(*(l.ఆదానం_layer));
    pull_convolutional_layer(*(l.self_layer));
    pull_convolutional_layer(*(l.output_layer));
}

శూన్య నెట్టు_crnn_layer(layer l)
{
    నెట్టు_convolutional_layer(*(l.ఆదానం_layer));
    నెట్టు_convolutional_layer(*(l.self_layer));
    నెట్టు_convolutional_layer(*(l.output_layer));
}

శూన్య update_crnn_layer_gpu(layer l, update_వాదనలు a)
{
    update_convolutional_layer_gpu(*(l.ఆదానం_layer),  a);
    update_convolutional_layer_gpu(*(l.self_layer),   a);
    update_convolutional_layer_gpu(*(l.output_layer), a);
}

శూన్య forward_crnn_layer_gpu(layer l, జాలం net)
{
    జాలం s = net;
    పూర్ణాంకం i;
    layer ఆదానం_layer = *(l.ఆదానం_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.అడుగులు, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.hidden * l.batch * l.అడుగులు, 0, ఆదానం_layer.delta_gpu, 1);
    ఒకవేళ(net.train) fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);

    కొరకు (i = 0; i < l.అడుగులు; ++i) {
        s.ఆదానం_gpu = net.ఆదానం_gpu;
        forward_convolutional_layer_gpu(ఆదానం_layer, s);

        s.ఆదానం_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(self_layer, s);

        దశాంశం *old_state = l.state_gpu;
        ఒకవేళ(net.train) l.state_gpu += l.hidden*l.batch;
        ఒకవేళ(l.shortcut){
            copy_gpu(l.hidden * l.batch, old_state, 1, l.state_gpu, 1);
        }లేకపోతే{
            fill_gpu(l.hidden * l.batch, 0, l.state_gpu, 1);
        }
        axpy_gpu(l.hidden * l.batch, 1, ఆదానం_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ఆదానం_gpu = l.state_gpu;
        forward_convolutional_layer_gpu(output_layer, s);

        net.ఆదానం_gpu += l.inputs*l.batch;
        increment_layer(&ఆదానం_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

శూన్య backward_crnn_layer_gpu(layer l, జాలం net)
{
    జాలం s = net;
    s.train = net.train;
    పూర్ణాంకం i;
    layer ఆదానం_layer = *(l.ఆదానం_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&ఆదానం_layer,  l.అడుగులు - 1);
    increment_layer(&self_layer,   l.అడుగులు - 1);
    increment_layer(&output_layer, l.అడుగులు - 1);
    l.state_gpu += l.hidden*l.batch*l.అడుగులు;
    కొరకు (i = l.అడుగులు-1; i >= 0; --i) {
        copy_gpu(l.hidden * l.batch, ఆదానం_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.hidden * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ఆదానం_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_convolutional_layer_gpu(output_layer, s);

        l.state_gpu -= l.hidden*l.batch;

        s.ఆదానం_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu - l.hidden*l.batch;
        ఒకవేళ (i == 0) s.delta_gpu = 0;
        backward_convolutional_layer_gpu(self_layer, s);

        copy_gpu(l.hidden*l.batch, self_layer.delta_gpu, 1, ఆదానం_layer.delta_gpu, 1);
        ఒకవేళ (i > 0 && l.shortcut) axpy_gpu(l.hidden*l.batch, 1, self_layer.delta_gpu, 1, self_layer.delta_gpu - l.hidden*l.batch, 1);
        s.ఆదానం_gpu = net.ఆదానం_gpu + i*l.inputs*l.batch;
        ఒకవేళ(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        లేకపోతే s.delta_gpu = 0;
        backward_convolutional_layer_gpu(ఆదానం_layer, s);

        increment_layer(&ఆదానం_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
}
#అయితే_ముగింపు
