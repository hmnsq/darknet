#include "cost_layer.h"
#include "utils.h"
#include "cuda.h"
#include "blas.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

COST_TYPE get_cost_type(var_na *s)
{
    නම් (strcmp(s, "seg")==0) ආපසු SEG;
    නම් (strcmp(s, "sse")==0) ආපසු SSE;
    නම් (strcmp(s, "masked")==0) ආපසු MASKED;
    නම් (strcmp(s, "smooth")==0) ආපසු SMOOTH;
    නම් (strcmp(s, "L1")==0) ආපසු L1;
    නම් (strcmp(s, "wgan")==0) ආපසු WGAN;
    fprintf(stderr, "Couldn't find cost type %s, going with SSE\n", s);
    ආපසු SSE;
}

var_na *get_cost_string(COST_TYPE a)
{
    switch(a){
        pak_sa SEG:
            ආපසු "seg";
        pak_sa SSE:
            ආපසු "sse";
        pak_sa MASKED:
            ආපසු "masked";
        pak_sa SMOOTH:
            ආපසු "smooth";
        pak_sa L1:
            ආපසු "L1";
        pak_sa WGAN:
            ආපසු "wgan";
    }
    ආපසු "sse";
}

cost_layer make_cost_layer(පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා inputs, COST_TYPE cost_type, float scale)
{
    fprintf(stderr, "cost                                           %4d\n",  inputs);
    cost_layer l = {0};
    l.type = COST;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.cost_type = cost_type;
    l.delta = calloc(inputs*batch, parimaa_na(float));
    l.output = calloc(inputs*batch, parimaa_na(float));
    l.cost = calloc(1, parimaa_na(float));

    l.forward = forward_cost_layer;
    l.backward = backward_cost_layer;
    #ifdef GPU
    l.forward_gpu = forward_cost_layer_gpu;
    l.backward_gpu = backward_cost_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #endif
    ආපසු l;
}

void resize_cost_layer(cost_layer *l, පූර්ණ_සංඛ්‍යා inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = realloc(l->delta, inputs*l->batch*parimaa_na(float));
    l->output = realloc(l->output, inputs*l->batch*parimaa_na(float));
#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#endif
}

void forward_cost_layer(cost_layer l, network net)
{
    නම් (!net.truth) ආපසු;
    නම්(l.cost_type == MASKED){
        පූර්ණ_සංඛ්‍යා i;
        සඳහා(i = 0; i < l.batch*l.inputs; ++i){
            නම්(net.truth[i] == SECRET_NUM) net.input[i] = SECRET_NUM;
        }
    }
    නම්(l.cost_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.input, net.truth, l.delta, l.output);
    }නැතහොත් නම්(l.cost_type == L1){
        l1_cpu(l.batch*l.inputs, net.input, net.truth, l.delta, l.output);
    } නැතහොත් {
        l2_cpu(l.batch*l.inputs, net.input, net.truth, l.delta, l.output);
    }
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

void backward_cost_layer(const cost_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#ifdef GPU

void pull_cost_layer(cost_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

void push_cost_layer(cost_layer l)
{
    cuda_push_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

පූර්ණ_සංඛ්‍යා float_abs_compare (const void * a, const void * b)
{
    float fa = *(const float*) a;
    නම්(fa < 0) fa = -fa;
    float fb = *(const float*) b;
    නම්(fb < 0) fb = -fb;
    ආපසු (fa > fb) - (fa < fb);
}

void forward_cost_layer_gpu(cost_layer l, network net)
{
    නම් (!net.truth) ආපසු;
    නම්(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        add_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    නම්(l.cost_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } නැතහොත් නම් (l.cost_type == L1){
        l1_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } නැතහොත් නම් (l.cost_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } නැතහොත් {
        l2_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    නම් (l.cost_type == SEG && l.noobject_scale != 1) {
        scale_mask_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_mask_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    නම් (l.cost_type == MASKED) {
        mask_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_NUM, net.truth_gpu, 0);
    }

    නම්(l.ratio){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, parimaa_na(float), float_abs_compare);
        පූර්ණ_සංඛ්‍යා n = (1-l.ratio) * l.batch*l.inputs;
        float thresh = l.delta[n];
        thresh = 0;
        printf("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    නම්(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

void backward_cost_layer_gpu(const cost_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

