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
    ܐܢ (strcmp(s, "seg")==0) ܦܢܐ SEG;
    ܐܢ (strcmp(s, "sse")==0) ܦܢܐ SSE;
    ܐܢ (strcmp(s, "masked")==0) ܦܢܐ MASKED;
    ܐܢ (strcmp(s, "smooth")==0) ܦܢܐ SMOOTH;
    ܐܢ (strcmp(s, "L1")==0) ܦܢܐ L1;
    ܐܢ (strcmp(s, "wgan")==0) ܦܢܐ WGAN;
    fprintf(stderr, "Couldn't find cost type %s, going with SSE\n", s);
    ܦܢܐ SSE;
}

حرف *get_cost_string(COST_TYPE a)
{
    بدل(a){
        حالة SEG:
            ܦܢܐ "seg";
        حالة SSE:
            ܦܢܐ "sse";
        حالة MASKED:
            ܦܢܐ "masked";
        حالة SMOOTH:
            ܦܢܐ "smooth";
        حالة L1:
            ܦܢܐ "L1";
        حالة WGAN:
            ܦܢܐ "wgan";
    }
    ܦܢܐ "sse";
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
    ܦܢܐ l;
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
    ܐܢ (!net.truth) ܦܢܐ;
    ܐܢ(l.cost_type == MASKED){
        صحيح i;
        لكل(i = 0; i < l.batch*l.inputs; ++i){
            ܐܢ(net.truth[i] == SECRET_NUM) net.input[i] = SECRET_NUM;
        }
    }
    ܐܢ(l.cost_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.input, net.truth, l.delta, l.output);
    }ܐܚܪܢܐ ܐܢ(l.cost_type == L1){
        l1_cpu(l.batch*l.inputs, net.input, net.truth, l.delta, l.output);
    } ܐܚܪܢܐ {
        l2_cpu(l.batch*l.inputs, net.input, net.truth, l.delta, l.output);
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

فراغ push_cost_layer(cost_layer l)
{
    cuda_push_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

صحيح float_abs_compare (ثابت فراغ * a, ثابت فراغ * b)
{
    عائم fa = *(ثابت عائم*) a;
    ܐܢ(fa < 0) fa = -fa;
    عائم fb = *(ثابت عائم*) b;
    ܐܢ(fb < 0) fb = -fb;
    ܦܢܐ (fa > fb) - (fa < fb);
}

فراغ forward_cost_layer_gpu(cost_layer l, network net)
{
    ܐܢ (!net.truth) ܦܢܐ;
    ܐܢ(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        add_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    ܐܢ(l.cost_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } ܐܚܪܢܐ ܐܢ (l.cost_type == L1){
        l1_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } ܐܚܪܢܐ ܐܢ (l.cost_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } ܐܚܪܢܐ {
        l2_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    ܐܢ (l.cost_type == SEG && l.noobject_scale != 1) {
        scale_mask_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_mask_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    ܐܢ (l.cost_type == MASKED) {
        mask_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_NUM, net.truth_gpu, 0);
    }

    ܐܢ(l.ratio){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, حجم(عائم), float_abs_compare);
        صحيح n = (1-l.ratio) * l.batch*l.inputs;
        عائم thresh = l.delta[n];
        thresh = 0;
        printf("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    ܐܢ(l.thresh){
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

