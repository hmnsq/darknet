#ضمن "cost_layer.h"
#ضمن "utils.h"
#ضمن "cuda.h"
#ضمن "blas.h"
#ضمن <math.h>
#ضمن <string.h>
#ضمن <stdlib.h>
#ضمن <stdio.h>

لګښت_TYPE get_لګښت_type(حرف *s)
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

حرف *get_لګښت_string(لګښت_TYPE a)
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

لګښت_layer make_لګښت_layer(صحيح batch, صحيح inputs, لګښت_TYPE لګښت_type, عائم scale)
{
    fprintf(stderr, "cost                                           %4d\n",  inputs);
    لګښت_layer l = {0};
    l.type = لګښت;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.لګښت_type = لګښت_type;
    l.delta = حافظه_صفر_ورکړه(inputs*batch, حجم(عائم));
    l.output = حافظه_صفر_ورکړه(inputs*batch, حجم(عائم));
    l.لګښت_2 = حافظه_صفر_ورکړه(1, حجم(عائم));

    l.forward = forward_لګښت_layer;
    l.backward = backward_لګښت_layer;
    #که_تعریف_شوی GPU
    l.forward_gpu = forward_لګښت_layer_gpu;
    l.backward_gpu = backward_لګښت_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #د_که_پای
    أرجع l;
}

فراغ resize_لګښت_layer(لګښت_layer *l, صحيح inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = realloc(l->delta, inputs*l->batch*حجم(عائم));
    l->output = realloc(l->output, inputs*l->batch*حجم(عائم));
#که_تعریف_شوی GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#د_که_پای
}

فراغ forward_لګښت_layer(لګښت_layer l, شبکه net)
{
    إذا (!net.truth) أرجع;
    إذا(l.لګښت_type == MASKED){
        صحيح i;
        لكل(i = 0; i < l.batch*l.inputs; ++i){
            إذا(net.truth[i] == SECRET_عدد) net.ننوت[i] = SECRET_عدد;
        }
    }
    إذا(l.لګښت_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.ننوت, net.truth, l.delta, l.output);
    }وإلا إذا(l.لګښت_type == L1){
        l1_cpu(l.batch*l.inputs, net.ننوت, net.truth, l.delta, l.output);
    } وإلا {
        l2_cpu(l.batch*l.inputs, net.ننوت, net.truth, l.delta, l.output);
    }
    l.لګښت_2[0] = مجموعه_array(l.output, l.batch*l.inputs);
}

فراغ backward_لګښت_layer(ثابت لګښت_layer l, شبکه net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#که_تعریف_شوی GPU

فراغ pull_لګښت_layer(لګښت_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

فراغ ټېل_لګښت_layer(لګښت_layer l)
{
    cuda_ټېل_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

صحيح float_abs_compare (ثابت فراغ * a, ثابت فراغ * b)
{
    عائم fa = *(ثابت عائم*) a;
    إذا(fa < 0) fa = -fa;
    عائم fb = *(ثابت عائم*) b;
    إذا(fb < 0) fb = -fb;
    أرجع (fa > fb) - (fa < fb);
}

فراغ forward_لګښت_layer_gpu(لګښت_layer l, شبکه net)
{
    إذا (!net.truth) أرجع;
    إذا(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        جمع_کړه_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    إذا(l.لګښت_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.ننوت_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } وإلا إذا (l.لګښت_type == L1){
        l1_gpu(l.batch*l.inputs, net.ننوت_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } وإلا إذا (l.لګښت_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.ننوت_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } وإلا {
        l2_gpu(l.batch*l.inputs, net.ننوت_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    إذا (l.لګښت_type == SEG && l.noobject_scale != 1) {
        scale_ماسک_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_ماسک_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    إذا (l.لګښت_type == MASKED) {
        ماسک_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_عدد, net.truth_gpu, 0);
    }

    إذا(l.نسبت){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, حجم(عائم), float_abs_compare);
        صحيح n = (1-l.نسبت) * l.batch*l.inputs;
        عائم thresh = l.delta[n];
        thresh = 0;
        چاپ_کړه("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    إذا(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.لګښت_2[0] = مجموعه_array(l.output, l.batch*l.inputs);
}

فراغ backward_لګښت_layer_gpu(ثابت لګښت_layer l, شبکه net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#د_که_پای

