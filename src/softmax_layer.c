#ضمن "softmax_layer.h"
#ضمن "blas.h"
#ضمن "cuda.h"

#ضمن <float.h>
#ضمن <math.h>
#ضمن <stdlib.h>
#ضمن <stdio.h>
#ضمن <assert.h>

softmax_layer make_softmax_layer(صحيح batch, صحيح inputs, صحيح groups)
{
    أكد(inputs%groups == 0);
    fprintf(stderr, "softmax                                        %4d\n",  inputs);
    softmax_layer l = {0};
    l.type = SOFTMAX;
    l.batch = batch;
    l.groups = groups;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = حافظه_صفر_بده(inputs*batch, حجم(عائم));
    l.output = حافظه_صفر_بده(inputs*batch, حجم(عائم));
    l.delta = حافظه_صفر_بده(inputs*batch, حجم(عائم));
    l.هزینه_2 = حافظه_صفر_بده(1, حجم(عائم));

    l.forward = forward_softmax_layer;
    l.backward = backward_softmax_layer;
    #اگر_تعریف_شده GPU
    l.forward_gpu = forward_softmax_layer_gpu;
    l.backward_gpu = backward_softmax_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #پایان_اگر
    أرجع l;
}

فراغ forward_softmax_layer(ثابت softmax_layer l, شبکه net)
{
    إذا(l.softmax_درخت){
        صحيح i;
        صحيح شمار = 0;
        لكل (i = 0; i < l.softmax_درخت->groups; ++i) {
            صحيح گروه_اندازه = l.softmax_درخت->گروه_اندازه[i];
            softmax_cpu(net.درون_داد + شمار, گروه_اندازه, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output + شمار);
            شمار += گروه_اندازه;
        }
    } وإلا {
        softmax_cpu(net.درون_داد, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output);
    }

    إذا(net.truth && !l.noloss){
        softmax_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.هزینه_2[0] = جمع_array(l.loss, l.batch*l.inputs);
    }
}

فراغ backward_softmax_layer(ثابت softmax_layer l, شبکه net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#اگر_تعریف_شده GPU

فراغ pull_softmax_layer_output(ثابت softmax_layer layer)
{
    cuda_pull_array(layer.output_gpu, layer.output, layer.inputs*layer.batch);
}

فراغ forward_softmax_layer_gpu(ثابت softmax_layer l, شبکه net)
{
    إذا(l.softmax_درخت){
        softmax_درخت(net.درون_داد_gpu, 1, l.batch, l.inputs, l.temperature, l.output_gpu, *l.softmax_درخت);
        /*
        int i;
        int count = 0;
        for (i = 0; i < l.softmax_tree->groups; ++i) {
            int group_size = l.softmax_tree->group_size[i];
            softmax_gpu(net.input_gpu + count, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output_gpu + count);
            count += group_size;
        }
        */
    } وإلا {
        إذا(l.spatial){
            softmax_gpu(net.درون_داد_gpu, l.c, l.batch*l.c, l.inputs/l.c, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu);
        }وإلا{
            softmax_gpu(net.درون_داد_gpu, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output_gpu);
        }
    }
    إذا(net.truth && !l.noloss){
        softmax_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        إذا(l.softmax_درخت){
            ماسک_gpu(l.batch*l.inputs, l.delta_gpu, SECRET_رقم, net.truth_gpu, 0);
            ماسک_gpu(l.batch*l.inputs, l.loss_gpu, SECRET_رقم, net.truth_gpu, 0);
        }
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.هزینه_2[0] = جمع_array(l.loss, l.batch*l.inputs);
    }
}

فراغ backward_softmax_layer_gpu(ثابت softmax_layer layer, شبکه net)
{
    axpy_gpu(layer.batch*layer.inputs, 1, layer.delta_gpu, 1, net.delta_gpu, 1);
}

#پایان_اگر
