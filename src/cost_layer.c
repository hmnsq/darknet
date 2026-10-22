#ضمن "cost_layer.h"
#ضمن "utils.h"
#ضمن "cuda.h"
#ضمن "blas.h"
#ضمن <math.h>
#ضمن <string.h>
#ضمن <stdlib.h>
#ضمن <stdio.h>

لاگت_TYPE get_لاگت_type(حرف *s)
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

حرف *get_لاگت_string(لاگت_TYPE a)
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

لاگت_layer make_لاگت_layer(صحيح batch, صحيح inputs, لاگت_TYPE لاگت_type, عائم scale)
{
    fprintf(stderr, "cost                                           %4d\n",  inputs);
    لاگت_layer l = {0};
    l.type = لاگت;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.لاگت_type = لاگت_type;
    l.delta = صفر_حافظہ_دیو(inputs*batch, حجم(عائم));
    l.output = صفر_حافظہ_دیو(inputs*batch, حجم(عائم));
    l.لاگت_2 = صفر_حافظہ_دیو(1, حجم(عائم));

    l.forward = forward_لاگت_layer;
    l.backward = backward_لاگت_layer;
    #جے_تعریف_شدہ GPU
    l.forward_gpu = forward_لاگت_layer_gpu;
    l.backward_gpu = backward_لاگت_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #جے_انت
    أرجع l;
}

فراغ resize_لاگت_layer(لاگت_layer *l, صحيح inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = realloc(l->delta, inputs*l->batch*حجم(عائم));
    l->output = realloc(l->output, inputs*l->batch*حجم(عائم));
#جے_تعریف_شدہ GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#جے_انت
}

فراغ forward_لاگت_layer(لاگت_layer l, جال net)
{
    إذا (!net.truth) أرجع;
    إذا(l.لاگت_type == MASKED){
        صحيح i;
        لكل(i = 0; i < l.batch*l.inputs; ++i){
            إذا(net.truth[i] == SECRET_عدد) net.داخلہ[i] = SECRET_عدد;
        }
    }
    إذا(l.لاگت_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.داخلہ, net.truth, l.delta, l.output);
    }وإلا إذا(l.لاگت_type == L1){
        l1_cpu(l.batch*l.inputs, net.داخلہ, net.truth, l.delta, l.output);
    } وإلا {
        l2_cpu(l.batch*l.inputs, net.داخلہ, net.truth, l.delta, l.output);
    }
    l.لاگت_2[0] = جوڑ_array(l.output, l.batch*l.inputs);
}

فراغ backward_لاگت_layer(ثابت لاگت_layer l, جال net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#جے_تعریف_شدہ GPU

فراغ pull_لاگت_layer(لاگت_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

فراغ ڈالو_لاگت_layer(لاگت_layer l)
{
    cuda_ڈالو_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

صحيح float_abs_compare (ثابت فراغ * a, ثابت فراغ * b)
{
    عائم fa = *(ثابت عائم*) a;
    إذا(fa < 0) fa = -fa;
    عائم fb = *(ثابت عائم*) b;
    إذا(fb < 0) fb = -fb;
    أرجع (fa > fb) - (fa < fb);
}

فراغ forward_لاگت_layer_gpu(لاگت_layer l, جال net)
{
    إذا (!net.truth) أرجع;
    إذا(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        جوڑو_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    إذا(l.لاگت_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.داخلہ_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } وإلا إذا (l.لاگت_type == L1){
        l1_gpu(l.batch*l.inputs, net.داخلہ_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } وإلا إذا (l.لاگت_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.داخلہ_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } وإلا {
        l2_gpu(l.batch*l.inputs, net.داخلہ_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    إذا (l.لاگت_type == SEG && l.noobject_scale != 1) {
        scale_نقاب_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_نقاب_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    إذا (l.لاگت_type == MASKED) {
        نقاب_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_عدد, net.truth_gpu, 0);
    }

    إذا(l.تناسب){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, حجم(عائم), float_abs_compare);
        صحيح n = (1-l.تناسب) * l.batch*l.inputs;
        عائم thresh = l.delta[n];
        thresh = 0;
        لکھو("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    إذا(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.لاگت_2[0] = جوڑ_array(l.output, l.batch*l.inputs);
}

فراغ backward_لاگت_layer_gpu(ثابت لاگت_layer l, جال net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#جے_انت

