#ضمن "cost_layer.h"
#ضمن "utils.h"
#ضمن "cuda.h"
#ضمن "blas.h"
#ضمن <math.h>
#ضمن <string.h>
#ضمن <stdlib.h>
#ضمن <stdio.h>

هزینه_TYPE get_هزینه_type(حرف *s)
{
    إذا (strcmp(s, "seg")==0) أرجع SEG;
    إذا (strcmp(s, "sse")==0) أرجع SSE;
    إذا (strcmp(s, "masked")==0) أرجع MASKED;
    إذا (strcmp(s, "smooth")==0) أرجع SMOOTH;
    إذا (strcmp(s, "L1")==0) أرجع L1;
    إذا (strcmp(s, "wgan")==0) أرجع WGAN;
    fprintf(stderr, "Couldn't find cost type %s, going with SSE\n", s);
    أرجع SSE;
}

حرف *get_هزینه_string(هزینه_TYPE a)
{
    بدل(a){
        حالة SEG:
            أرجع "seg";
        حالة SSE:
            أرجع "sse";
        حالة MASKED:
            أرجع "masked";
        حالة SMOOTH:
            أرجع "smooth";
        حالة L1:
            أرجع "L1";
        حالة WGAN:
            أرجع "wgan";
    }
    أرجع "sse";
}

هزینه_layer make_هزینه_layer(صحيح batch, صحيح inputs, هزینه_TYPE هزینه_type, عائم scale)
{
    fprintf(stderr, "cost                                           %4d\n",  inputs);
    هزینه_layer l = {0};
    l.type = هزینه;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.هزینه_type = هزینه_type;
    l.delta = حافظه_صفر_بده(inputs*batch, حجم(عائم));
    l.output = حافظه_صفر_بده(inputs*batch, حجم(عائم));
    l.هزینه_2 = حافظه_صفر_بده(1, حجم(عائم));

    l.forward = forward_هزینه_layer;
    l.backward = backward_هزینه_layer;
    #اگر_تعریف_شده GPU
    l.forward_gpu = forward_هزینه_layer_gpu;
    l.backward_gpu = backward_هزینه_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #پایان_اگر
    أرجع l;
}

فراغ resize_هزینه_layer(هزینه_layer *l, صحيح inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = realloc(l->delta, inputs*l->batch*حجم(عائم));
    l->output = realloc(l->output, inputs*l->batch*حجم(عائم));
#اگر_تعریف_شده GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#پایان_اگر
}

فراغ forward_هزینه_layer(هزینه_layer l, شبکه net)
{
    إذا (!net.truth) أرجع;
    إذا(l.هزینه_type == MASKED){
        صحيح i;
        لكل(i = 0; i < l.batch*l.inputs; ++i){
            إذا(net.truth[i] == SECRET_رقم) net.درون_داد[i] = SECRET_رقم;
        }
    }
    إذا(l.هزینه_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.درون_داد, net.truth, l.delta, l.output);
    }وإلا إذا(l.هزینه_type == L1){
        l1_cpu(l.batch*l.inputs, net.درون_داد, net.truth, l.delta, l.output);
    } وإلا {
        l2_cpu(l.batch*l.inputs, net.درون_داد, net.truth, l.delta, l.output);
    }
    l.هزینه_2[0] = جمع_array(l.output, l.batch*l.inputs);
}

فراغ backward_هزینه_layer(ثابت هزینه_layer l, شبکه net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#اگر_تعریف_شده GPU

فراغ pull_هزینه_layer(هزینه_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

فراغ افزودن_هزینه_layer(هزینه_layer l)
{
    cuda_افزودن_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

صحيح float_abs_compare (ثابت فراغ * a, ثابت فراغ * b)
{
    عائم fa = *(ثابت عائم*) a;
    إذا(fa < 0) fa = -fa;
    عائم fb = *(ثابت عائم*) b;
    إذا(fb < 0) fb = -fb;
    أرجع (fa > fb) - (fa < fb);
}

فراغ forward_هزینه_layer_gpu(هزینه_layer l, شبکه net)
{
    إذا (!net.truth) أرجع;
    إذا(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        جمع_کن_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    إذا(l.هزینه_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.درون_داد_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } وإلا إذا (l.هزینه_type == L1){
        l1_gpu(l.batch*l.inputs, net.درون_داد_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } وإلا إذا (l.هزینه_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.درون_داد_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } وإلا {
        l2_gpu(l.batch*l.inputs, net.درون_داد_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    إذا (l.هزینه_type == SEG && l.noobject_scale != 1) {
        scale_ماسک_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_ماسک_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    إذا (l.هزینه_type == MASKED) {
        ماسک_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_رقم, net.truth_gpu, 0);
    }

    إذا(l.نسبت){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, حجم(عائم), float_abs_compare);
        صحيح n = (1-l.نسبت) * l.batch*l.inputs;
        عائم thresh = l.delta[n];
        thresh = 0;
        چاپ_کن("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    إذا(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.هزینه_2[0] = جمع_array(l.output, l.batch*l.inputs);
}

فراغ backward_هزینه_layer_gpu(ثابت هزینه_layer l, شبکه net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#پایان_اگر

