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
    ᨀᨘ (strcmp(s, "seg")==0) ᨀᨘᨒ SEG;
    ᨀᨘ (strcmp(s, "sse")==0) ᨀᨘᨒ SSE;
    ᨀᨘ (strcmp(s, "masked")==0) ᨀᨘᨒ MASKED;
    ᨀᨘ (strcmp(s, "smooth")==0) ᨀᨘᨒ SMOOTH;
    ᨀᨘ (strcmp(s, "L1")==0) ᨀᨘᨒ L1;
    ᨀᨘ (strcmp(s, "wgan")==0) ᨀᨘᨒ WGAN;
    fprintf(stderr, "Couldn't find cost type %s, going with SSE\n", s);
    ᨀᨘᨒ SSE;
}

var_na *get_cost_string(COST_TYPE a)
{
    switch(a){
        pak_sa SEG:
            ᨀᨘᨒ "seg";
        pak_sa SSE:
            ᨀᨘᨒ "sse";
        pak_sa MASKED:
            ᨀᨘᨒ "masked";
        pak_sa SMOOTH:
            ᨀᨘᨒ "smooth";
        pak_sa L1:
            ᨀᨘᨒ "L1";
        pak_sa WGAN:
            ᨀᨘᨒ "wgan";
    }
    ᨀᨘᨒ "sse";
}

cost_layer make_cost_layer(puur_naa_nka batch, puur_naa_nka inputs, COST_TYPE cost_type, float scale)
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
    ᨀᨘᨒ l;
}

void resize_cost_layer(cost_layer *l, puur_naa_nka inputs)
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
    ᨀᨘ (!net.truth) ᨀᨘᨒ;
    ᨀᨘ(l.cost_type == MASKED){
        puur_naa_nka i;
        for(i = 0; i < l.batch*l.inputs; ++i){
            ᨀᨘ(net.truth[i] == SECRET_NUM) net.input[i] = SECRET_NUM;
        }
    }
    ᨀᨘ(l.cost_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.input, net.truth, l.delta, l.output);
    }ᨀᨘᨁ ᨀᨘ(l.cost_type == L1){
        l1_cpu(l.batch*l.inputs, net.input, net.truth, l.delta, l.output);
    } ᨀᨘᨁ {
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

puur_naa_nka float_abs_compare (const void * a, const void * b)
{
    float fa = *(const float*) a;
    ᨀᨘ(fa < 0) fa = -fa;
    float fb = *(const float*) b;
    ᨀᨘ(fb < 0) fb = -fb;
    ᨀᨘᨒ (fa > fb) - (fa < fb);
}

void forward_cost_layer_gpu(cost_layer l, network net)
{
    ᨀᨘ (!net.truth) ᨀᨘᨒ;
    ᨀᨘ(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        add_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    ᨀᨘ(l.cost_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } ᨀᨘᨁ ᨀᨘ (l.cost_type == L1){
        l1_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } ᨀᨘᨁ ᨀᨘ (l.cost_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } ᨀᨘᨁ {
        l2_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    ᨀᨘ (l.cost_type == SEG && l.noobject_scale != 1) {
        scale_mask_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_mask_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    ᨀᨘ (l.cost_type == MASKED) {
        mask_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_NUM, net.truth_gpu, 0);
    }

    ᨀᨘ(l.ratio){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, parimaa_na(float), float_abs_compare);
        puur_naa_nka n = (1-l.ratio) * l.batch*l.inputs;
        float thresh = l.delta[n];
        thresh = 0;
        printf("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    ᨀᨘ(l.thresh){
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

