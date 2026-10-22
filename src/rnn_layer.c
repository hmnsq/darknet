#అన్తర్భావ "rnn_layer.h"
#అన్తర్భావ "connected_layer.h"
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

layer make_rnn_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, పూర్ణాంకం outputs, పూర్ణాంకం అడుగులు, ACTIVATION activation, పూర్ణాంకం batch_normalize, పూర్ణాంకం adam)
{
    fprintf(stderr, "RNN Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / అడుగులు;
    layer l = {0};
    l.batch = batch;
    l.type = RNN;
    l.అడుగులు = అడుగులు;
    l.inputs = inputs;

    l.state = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
    l.prev_state = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));

    l.ఆదానం_layer = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.ఆదానం_layer) = make_connected_layer(batch*అడుగులు, inputs, outputs, activation, batch_normalize, adam);
    l.ఆదానం_layer->batch = batch;

    l.self_layer = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.self_layer) = make_connected_layer(batch*అడుగులు, outputs, outputs, activation, batch_normalize, adam);
    l.self_layer->batch = batch;

    l.output_layer = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.output_layer) = make_connected_layer(batch*అడుగులు, outputs, outputs, activation, batch_normalize, adam);
    l.output_layer->batch = batch;

    l.outputs = outputs;
    l.output = l.output_layer->output;
    l.delta = l.output_layer->delta;

    l.forward = forward_rnn_layer;
    l.backward = backward_rnn_layer;
    l.update = update_rnn_layer;
#నిర్వచితమైతే GPU
    l.forward_gpu = forward_rnn_layer_gpu;
    l.backward_gpu = backward_rnn_layer_gpu;
    l.update_gpu = update_rnn_layer_gpu;
    l.state_gpu = cuda_make_array(0, batch*outputs);
    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.output_gpu = l.output_layer->output_gpu;
    l.delta_gpu = l.output_layer->delta_gpu;
#నిర్వచితమైతే CUDNN
    cudnn_సమితి_Tensor4d_Descriptor(l.ఆదానం_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.ఆదానం_layer->బహిర్గతం_c, l.ఆదానం_layer->బహిర్గతం_h, l.ఆదానం_layer->బహిర్గతం_w); 
    cudnn_సమితి_Tensor4d_Descriptor(l.self_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.self_layer->బహిర్గతం_c, l.self_layer->బహిర్గతం_h, l.self_layer->బహిర్గతం_w); 
    cudnn_సమితి_Tensor4d_Descriptor(l.output_layer->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.output_layer->బహిర్గతం_c, l.output_layer->బహిర్గతం_h, l.output_layer->బహిర్గతం_w); 
#అయితే_ముగింపు
#అయితే_ముగింపు

    తిరిగి l;
}

శూన్య update_rnn_layer(layer l, update_వాదనలు a)
{
    update_connected_layer(*(l.ఆదానం_layer),  a);
    update_connected_layer(*(l.self_layer),   a);
    update_connected_layer(*(l.output_layer), a);
}

శూన్య forward_rnn_layer(layer l, జాలం net)
{
    జాలం s = net;
    s.train = net.train;
    పూర్ణాంకం i;
    layer ఆదానం_layer = *(l.ఆదానం_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, output_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, self_layer.delta, 1);
    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, ఆదానం_layer.delta, 1);
    ఒకవేళ(net.train) fill_cpu(l.outputs * l.batch, 0, l.state, 1);

    కొరకు (i = 0; i < l.అడుగులు; ++i) {
        s.ఆదానం = net.ఆదానం;
        forward_connected_layer(ఆదానం_layer, s);

        s.ఆదానం = l.state;
        forward_connected_layer(self_layer, s);

        దశాంశం *old_state = l.state;
        ఒకవేళ(net.train) l.state += l.outputs*l.batch;
        ఒకవేళ(l.shortcut){
            copy_cpu(l.outputs * l.batch, old_state, 1, l.state, 1);
        }లేకపోతే{
            fill_cpu(l.outputs * l.batch, 0, l.state, 1);
        }
        axpy_cpu(l.outputs * l.batch, 1, ఆదానం_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ఆదానం = l.state;
        forward_connected_layer(output_layer, s);

        net.ఆదానం += l.inputs*l.batch;
        increment_layer(&ఆదానం_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

శూన్య backward_rnn_layer(layer l, జాలం net)
{
    జాలం s = net;
    s.train = net.train;
    పూర్ణాంకం i;
    layer ఆదానం_layer = *(l.ఆదానం_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    increment_layer(&ఆదానం_layer, l.అడుగులు-1);
    increment_layer(&self_layer, l.అడుగులు-1);
    increment_layer(&output_layer, l.అడుగులు-1);

    l.state += l.outputs*l.batch*l.అడుగులు;
    కొరకు (i = l.అడుగులు-1; i >= 0; --i) {
        copy_cpu(l.outputs * l.batch, ఆదానం_layer.output, 1, l.state, 1);
        axpy_cpu(l.outputs * l.batch, 1, self_layer.output, 1, l.state, 1);

        s.ఆదానం = l.state;
        s.delta = self_layer.delta;
        backward_connected_layer(output_layer, s);

        l.state -= l.outputs*l.batch;
        /*
           if(i > 0){
           copy_cpu(l.outputs * l.batch, input_layer.output - l.outputs*l.batch, 1, l.state, 1);
           axpy_cpu(l.outputs * l.batch, 1, self_layer.output - l.outputs*l.batch, 1, l.state, 1);
           }else{
           fill_cpu(l.outputs * l.batch, 0, l.state, 1);
           }
         */

        s.ఆదానం = l.state;
        s.delta = self_layer.delta - l.outputs*l.batch;
        ఒకవేళ (i == 0) s.delta = 0;
        backward_connected_layer(self_layer, s);

        copy_cpu(l.outputs*l.batch, self_layer.delta, 1, ఆదానం_layer.delta, 1);
        ఒకవేళ (i > 0 && l.shortcut) axpy_cpu(l.outputs*l.batch, 1, self_layer.delta, 1, self_layer.delta - l.outputs*l.batch, 1);
        s.ఆదానం = net.ఆదానం + i*l.inputs*l.batch;
        ఒకవేళ(net.delta) s.delta = net.delta + i*l.inputs*l.batch;
        లేకపోతే s.delta = 0;
        backward_connected_layer(ఆదానం_layer, s);

        increment_layer(&ఆదానం_layer, -1);
        increment_layer(&self_layer, -1);
        increment_layer(&output_layer, -1);
    }
}

#నిర్వచితమైతే GPU

శూన్య pull_rnn_layer(layer l)
{
    pull_connected_layer(*(l.ఆదానం_layer));
    pull_connected_layer(*(l.self_layer));
    pull_connected_layer(*(l.output_layer));
}

శూన్య నెట్టు_rnn_layer(layer l)
{
    నెట్టు_connected_layer(*(l.ఆదానం_layer));
    నెట్టు_connected_layer(*(l.self_layer));
    నెట్టు_connected_layer(*(l.output_layer));
}

శూన్య update_rnn_layer_gpu(layer l, update_వాదనలు a)
{
    update_connected_layer_gpu(*(l.ఆదానం_layer),  a);
    update_connected_layer_gpu(*(l.self_layer),   a);
    update_connected_layer_gpu(*(l.output_layer), a);
}

శూన్య forward_rnn_layer_gpu(layer l, జాలం net)
{
    జాలం s = {0};
    s.train = net.train;
    పూర్ణాంకం i;
    layer ఆదానం_layer = *(l.ఆదానం_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);

    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, output_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, self_layer.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, ఆదానం_layer.delta_gpu, 1);

    ఒకవేళ(net.train) {
        fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, l.delta_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.state_gpu, 1, l.prev_state_gpu, 1);
    }

    కొరకు (i = 0; i < l.అడుగులు; ++i) {
        s.ఆదానం_gpu = net.ఆదానం_gpu;
        forward_connected_layer_gpu(ఆదానం_layer, s);

        s.ఆదానం_gpu = l.state_gpu;
        forward_connected_layer_gpu(self_layer, s);

        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, ఆదానం_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ఆదానం_gpu = l.state_gpu;
        forward_connected_layer_gpu(output_layer, s);

        net.ఆదానం_gpu += l.inputs*l.batch;
        increment_layer(&ఆదానం_layer, 1);
        increment_layer(&self_layer, 1);
        increment_layer(&output_layer, 1);
    }
}

శూన్య backward_rnn_layer_gpu(layer l, జాలం net)
{
    జాలం s = {0};
    s.train = net.train;
    పూర్ణాంకం i;
    layer ఆదానం_layer = *(l.ఆదానం_layer);
    layer self_layer = *(l.self_layer);
    layer output_layer = *(l.output_layer);
    increment_layer(&ఆదానం_layer,  l.అడుగులు - 1);
    increment_layer(&self_layer,   l.అడుగులు - 1);
    increment_layer(&output_layer, l.అడుగులు - 1);
    దశాంశం *చివరి_ఆదానం = ఆదానం_layer.output_gpu;
    దశాంశం *చివరి_self = self_layer.output_gpu;
    కొరకు (i = l.అడుగులు-1; i >= 0; --i) {
        fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, ఆదానం_layer.output_gpu, 1, l.state_gpu, 1);
        axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu, 1, l.state_gpu, 1);

        s.ఆదానం_gpu = l.state_gpu;
        s.delta_gpu = self_layer.delta_gpu;
        backward_connected_layer_gpu(output_layer, s);

        ఒకవేళ(i != 0) {
            fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, ఆదానం_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
            axpy_gpu(l.outputs * l.batch, 1, self_layer.output_gpu - l.outputs*l.batch, 1, l.state_gpu, 1);
        }లేకపోతే {
            copy_gpu(l.outputs*l.batch, l.prev_state_gpu, 1, l.state_gpu, 1);
        }

        copy_gpu(l.outputs*l.batch, self_layer.delta_gpu, 1, ఆదానం_layer.delta_gpu, 1);

        s.ఆదానం_gpu = l.state_gpu;
        s.delta_gpu = (i > 0) ? self_layer.delta_gpu - l.outputs*l.batch : 0;
        ఒకవేళ (i == 0) s.delta_gpu = 0;
        backward_connected_layer_gpu(self_layer, s);

        s.ఆదానం_gpu = net.ఆదానం_gpu + i*l.inputs*l.batch;
        ఒకవేళ(net.delta_gpu) s.delta_gpu = net.delta_gpu + i*l.inputs*l.batch;
        లేకపోతే s.delta_gpu = 0;
        backward_connected_layer_gpu(ఆదానం_layer, s);

        increment_layer(&ఆదానం_layer,  -1);
        increment_layer(&self_layer,   -1);
        increment_layer(&output_layer, -1);
    }
    fill_gpu(l.outputs * l.batch, 0, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, చివరి_ఆదానం, 1, l.state_gpu, 1);
    axpy_gpu(l.outputs * l.batch, 1, చివరి_self, 1, l.state_gpu, 1);
}
#అయితే_ముగింపు
