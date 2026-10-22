#شامل "cost_layer.h"
#شامل "utils.h"
#شامل "cuda.h"
#شامل "blas.h"
#شامل <math.h>
#شامل <string.h>
#شامل <stdlib.h>
#شامل <stdio.h>

COST_TYPE get_cost_type(نویسه *s)
{
    اگر (strcmp(s, "seg")==0) بازگشت SEG;
    اگر (strcmp(s, "sse")==0) بازگشت SSE;
    اگر (strcmp(s, "masked")==0) بازگشت MASKED;
    اگر (strcmp(s, "smooth")==0) بازگشت SMOOTH;
    اگر (strcmp(s, "L1")==0) بازگشت L1;
    اگر (strcmp(s, "wgan")==0) بازگشت WGAN;
    fprintf(stderr, "Couldn't find cost type %s, going with SSE\n", s);
    بازگشت SSE;
}

نویسه *get_cost_string(COST_TYPE a)
{
    انتخاب(a){
        حالت SEG:
            بازگشت "seg";
        حالت SSE:
            بازگشت "sse";
        حالت MASKED:
            بازگشت "masked";
        حالت SMOOTH:
            بازگشت "smooth";
        حالت L1:
            بازگشت "L1";
        حالت WGAN:
            بازگشت "wgan";
    }
    بازگشت "sse";
}

cost_layer make_cost_layer(صحیح batch, صحیح inputs, COST_TYPE cost_type, اعشاری scale)
{
    fprintf(stderr, "cost                                           %4d\n",  inputs);
    cost_layer l = {0};
    l.type = COST;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.cost_type = cost_type;
    l.delta = calloc(inputs*batch, اندازه(اعشاری));
    l.output = calloc(inputs*batch, اندازه(اعشاری));
    l.cost = calloc(1, اندازه(اعشاری));

    l.forward = forward_cost_layer;
    l.backward = backward_cost_layer;
    #ifdef GPU
    l.forward_gpu = forward_cost_layer_gpu;
    l.backward_gpu = backward_cost_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #endif
    بازگشت l;
}

تهی resize_cost_layer(cost_layer *l, صحیح inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = realloc(l->delta, inputs*l->batch*اندازه(اعشاری));
    l->output = realloc(l->output, inputs*l->batch*اندازه(اعشاری));
#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#endif
}

تهی forward_cost_layer(cost_layer l, network net)
{
    اگر (!net.truth) بازگشت;
    اگر(l.cost_type == MASKED){
        صحیح i;
        برای(i = 0; i < l.batch*l.inputs; ++i){
            اگر(net.truth[i] == SECRET_رقم) net.درون_داد[i] = SECRET_رقم;
        }
    }
    اگر(l.cost_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.درون_داد, net.truth, l.delta, l.output);
    }وگرنه اگر(l.cost_type == L1){
        l1_cpu(l.batch*l.inputs, net.درون_داد, net.truth, l.delta, l.output);
    } وگرنه {
        l2_cpu(l.batch*l.inputs, net.درون_داد, net.truth, l.delta, l.output);
    }
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

تهی backward_cost_layer(ثابت cost_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#ifdef GPU

تهی pull_cost_layer(cost_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

تهی افزودن_cost_layer(cost_layer l)
{
    cuda_افزودن_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

صحیح float_abs_compare (ثابت تهی * a, ثابت تهی * b)
{
    اعشاری fa = *(ثابت اعشاری*) a;
    اگر(fa < 0) fa = -fa;
    اعشاری fb = *(ثابت اعشاری*) b;
    اگر(fb < 0) fb = -fb;
    بازگشت (fa > fb) - (fa < fb);
}

تهی forward_cost_layer_gpu(cost_layer l, network net)
{
    اگر (!net.truth) بازگشت;
    اگر(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        add_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    اگر(l.cost_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.درون_داد_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } وگرنه اگر (l.cost_type == L1){
        l1_gpu(l.batch*l.inputs, net.درون_داد_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } وگرنه اگر (l.cost_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.درون_داد_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } وگرنه {
        l2_gpu(l.batch*l.inputs, net.درون_داد_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    اگر (l.cost_type == SEG && l.noobject_scale != 1) {
        scale_mask_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_mask_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    اگر (l.cost_type == MASKED) {
        mask_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_رقم, net.truth_gpu, 0);
    }

    اگر(l.ratio){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, اندازه(اعشاری), float_abs_compare);
        صحیح n = (1-l.ratio) * l.batch*l.inputs;
        اعشاری thresh = l.delta[n];
        thresh = 0;
        printf("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    اگر(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

تهی backward_cost_layer_gpu(ثابت cost_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

