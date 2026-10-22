#ضمن "cost_layer.h"
#ضمن "utils.h"
#ضمن "cuda.h"
#ضمن "blas.h"
#ضمن <math.h>
#ضمن <string.h>
#ضمن <stdlib.h>
#ضمن <stdio.h>

COST_TYPE get_cost_type(حرف *s)
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

حرف *get_cost_string(COST_TYPE a)
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

cost_layer make_cost_layer(صحيح batch, صحيح inputs, COST_TYPE cost_type, عائم scale)
{
    fprintf(stderr, "cost                                           %4d\n",  inputs);
    cost_layer l = {0};
    l.type = COST;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.cost_type = cost_type;
    l.delta = calloc(inputs*batch, حجم(عائم));
    l.output = calloc(inputs*batch, حجم(عائم));
    l.cost = calloc(1, حجم(عائم));

    l.forward = forward_cost_layer;
    l.backward = backward_cost_layer;
    #ifdef GPU
    l.forward_gpu = forward_cost_layer_gpu;
    l.backward_gpu = backward_cost_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #endif
    أرجع l;
}

فراغ resize_cost_layer(cost_layer *l, صحيح inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = realloc(l->delta, inputs*l->batch*حجم(عائم));
    l->output = realloc(l->output, inputs*l->batch*حجم(عائم));
#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#endif
}

فراغ forward_cost_layer(cost_layer l, network net)
{
    إذا (!net.truth) أرجع;
    إذا(l.cost_type == MASKED){
        صحيح i;
        لكل(i = 0; i < l.batch*l.inputs; ++i){
            إذا(net.truth[i] == SECRET_رقم) net.مدخل[i] = SECRET_رقم;
        }
    }
    إذا(l.cost_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.مدخل, net.truth, l.delta, l.output);
    }وإلا إذا(l.cost_type == L1){
        l1_cpu(l.batch*l.inputs, net.مدخل, net.truth, l.delta, l.output);
    } وإلا {
        l2_cpu(l.batch*l.inputs, net.مدخل, net.truth, l.delta, l.output);
    }
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

فراغ backward_cost_layer(ثابت cost_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#ifdef GPU

فراغ pull_cost_layer(cost_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

فراغ ادفع_cost_layer(cost_layer l)
{
    cuda_ادفع_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

صحيح float_abs_compare (ثابت فراغ * a, ثابت فراغ * b)
{
    عائم fa = *(ثابت عائم*) a;
    إذا(fa < 0) fa = -fa;
    عائم fb = *(ثابت عائم*) b;
    إذا(fb < 0) fb = -fb;
    أرجع (fa > fb) - (fa < fb);
}

فراغ forward_cost_layer_gpu(cost_layer l, network net)
{
    إذا (!net.truth) أرجع;
    إذا(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        add_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    إذا(l.cost_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.مدخل_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } وإلا إذا (l.cost_type == L1){
        l1_gpu(l.batch*l.inputs, net.مدخل_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } وإلا إذا (l.cost_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.مدخل_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } وإلا {
        l2_gpu(l.batch*l.inputs, net.مدخل_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    إذا (l.cost_type == SEG && l.noobject_scale != 1) {
        scale_mask_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_mask_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    إذا (l.cost_type == MASKED) {
        mask_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_رقم, net.truth_gpu, 0);
    }

    إذا(l.ratio){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, حجم(عائم), float_abs_compare);
        صحيح n = (1-l.ratio) * l.batch*l.inputs;
        عائم thresh = l.delta[n];
        thresh = 0;
        printf("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    إذا(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

فراغ backward_cost_layer_gpu(ثابت cost_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

