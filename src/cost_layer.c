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
    གལ་སྲིད (strcmp(s, "seg")==0) ལོག SEG;
    གལ་སྲིད (strcmp(s, "sse")==0) ལོག SSE;
    གལ་སྲིད (strcmp(s, "masked")==0) ལོག MASKED;
    གལ་སྲིད (strcmp(s, "smooth")==0) ལོག SMOOTH;
    གལ་སྲིད (strcmp(s, "L1")==0) ལོག L1;
    གལ་སྲིད (strcmp(s, "wgan")==0) ལོག WGAN;
    fprintf(stderr, "Couldn't find cost type %s, going with SSE\n", s);
    ལོག SSE;
}

var_na *get_cost_string(COST_TYPE a)
{
    switch(a){
        pak_sa SEG:
            ལོག "seg";
        pak_sa SSE:
            ལོག "sse";
        pak_sa MASKED:
            ལོག "masked";
        pak_sa SMOOTH:
            ལོག "smooth";
        pak_sa L1:
            ལོག "L1";
        pak_sa WGAN:
            ལོག "wgan";
    }
    ལོག "sse";
}

cost_layer make_cost_layer(ཧྲིལ batch, ཧྲིལ inputs, COST_TYPE cost_type, float scale)
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
    ལོག l;
}

void resize_cost_layer(cost_layer *l, ཧྲིལ inputs)
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
    གལ་སྲིད (!net.truth) ལོག;
    གལ་སྲིད(l.cost_type == MASKED){
        ཧྲིལ i;
        དོན་ལུ(i = 0; i < l.batch*l.inputs; ++i){
            གལ་སྲིད(net.truth[i] == SECRET_NUM) net.input[i] = SECRET_NUM;
        }
    }
    གལ་སྲིད(l.cost_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.input, net.truth, l.delta, l.output);
    }དེ་མིན གལ་སྲིད(l.cost_type == L1){
        l1_cpu(l.batch*l.inputs, net.input, net.truth, l.delta, l.output);
    } དེ་མིན {
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

ཧྲིལ float_abs_compare (const void * a, const void * b)
{
    float fa = *(const float*) a;
    གལ་སྲིད(fa < 0) fa = -fa;
    float fb = *(const float*) b;
    གལ་སྲིད(fb < 0) fb = -fb;
    ལོག (fa > fb) - (fa < fb);
}

void forward_cost_layer_gpu(cost_layer l, network net)
{
    གལ་སྲིད (!net.truth) ལོག;
    གལ་སྲིད(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        add_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    གལ་སྲིད(l.cost_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } དེ་མིན གལ་སྲིད (l.cost_type == L1){
        l1_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } དེ་མིན གལ་སྲིད (l.cost_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } དེ་མིན {
        l2_gpu(l.batch*l.inputs, net.input_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    གལ་སྲིད (l.cost_type == SEG && l.noobject_scale != 1) {
        scale_mask_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_mask_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    གལ་སྲིད (l.cost_type == MASKED) {
        mask_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_NUM, net.truth_gpu, 0);
    }

    གལ་སྲིད(l.ratio){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, parimaa_na(float), float_abs_compare);
        ཧྲིལ n = (1-l.ratio) * l.batch*l.inputs;
        float thresh = l.delta[n];
        thresh = 0;
        printf("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    གལ་སྲིད(l.thresh){
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

