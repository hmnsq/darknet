#ଅନ୍ତର୍ଭାଵ "cost_layer.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ <math.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>

COST_TYPE get_cost_type(ଅକ୍ଷର *s)
{
    ଯଦି (strcmp(s, "seg")==0) ଫେରନ୍ତୁ SEG;
    ଯଦି (strcmp(s, "sse")==0) ଫେରନ୍ତୁ SSE;
    ଯଦି (strcmp(s, "masked")==0) ଫେରନ୍ତୁ MASKED;
    ଯଦି (strcmp(s, "smooth")==0) ଫେରନ୍ତୁ SMOOTH;
    ଯଦି (strcmp(s, "L1")==0) ଫେରନ୍ତୁ L1;
    ଯଦି (strcmp(s, "wgan")==0) ଫେରନ୍ତୁ WGAN;
    fprintf(stderr, "Couldn't find cost type %s, going with SSE\n", s);
    ଫେରନ୍ତୁ SSE;
}

ଅକ୍ଷର *get_cost_string(COST_TYPE a)
{
    ବାଛ(a){
        ସ୍ଥିତି SEG:
            ଫେରନ୍ତୁ "seg";
        ସ୍ଥିତି SSE:
            ଫେରନ୍ତୁ "sse";
        ସ୍ଥିତି MASKED:
            ଫେରନ୍ତୁ "masked";
        ସ୍ଥିତି SMOOTH:
            ଫେରନ୍ତୁ "smooth";
        ସ୍ଥିତି L1:
            ଫେରନ୍ତୁ "L1";
        ସ୍ଥିତି WGAN:
            ଫେରନ୍ତୁ "wgan";
    }
    ଫେରନ୍ତୁ "sse";
}

cost_layer make_cost_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs, COST_TYPE cost_type, ଦଶମିକ scale)
{
    fprintf(stderr, "cost                                           %4d\n",  inputs);
    cost_layer l = {0};
    l.type = COST;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.cost_type = cost_type;
    l.delta = calloc(inputs*batch, ଆକାର(ଦଶମିକ));
    l.output = calloc(inputs*batch, ଆକାର(ଦଶମିକ));
    l.cost = calloc(1, ଆକାର(ଦଶମିକ));

    l.forward = forward_cost_layer;
    l.backward = backward_cost_layer;
    #ifdef GPU
    l.forward_gpu = forward_cost_layer_gpu;
    l.backward_gpu = backward_cost_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #endif
    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ resize_cost_layer(cost_layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = realloc(l->delta, inputs*l->batch*ଆକାର(ଦଶମିକ));
    l->output = realloc(l->output, inputs*l->batch*ଆକାର(ଦଶମିକ));
#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#endif
}

ଶୂନ୍ୟ forward_cost_layer(cost_layer l, network net)
{
    ଯଦି (!net.truth) ଫେରନ୍ତୁ;
    ଯଦି(l.cost_type == MASKED){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
        ପାଇଁ(i = 0; i < l.batch*l.inputs; ++i){
            ଯଦି(net.truth[i] == SECRET_ଅଙ୍କ) net.ନିବେଶ[i] = SECRET_ଅଙ୍କ;
        }
    }
    ଯଦି(l.cost_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.ନିବେଶ, net.truth, l.delta, l.output);
    }ଅନ୍ୟଥା ଯଦି(l.cost_type == L1){
        l1_cpu(l.batch*l.inputs, net.ନିବେଶ, net.truth, l.delta, l.output);
    } ଅନ୍ୟଥା {
        l2_cpu(l.batch*l.inputs, net.ନିବେଶ, net.truth, l.delta, l.output);
    }
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

ଶୂନ୍ୟ backward_cost_layer(ଅଚଳ cost_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#ifdef GPU

ଶୂନ୍ୟ pull_cost_layer(cost_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

ଶୂନ୍ୟ ଠେଲ_cost_layer(cost_layer l)
{
    cuda_ଠେଲ_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା float_abs_compare (ଅଚଳ ଶୂନ୍ୟ * a, ଅଚଳ ଶୂନ୍ୟ * b)
{
    ଦଶମିକ fa = *(ଅଚଳ ଦଶମିକ*) a;
    ଯଦି(fa < 0) fa = -fa;
    ଦଶମିକ fb = *(ଅଚଳ ଦଶମିକ*) b;
    ଯଦି(fb < 0) fb = -fb;
    ଫେରନ୍ତୁ (fa > fb) - (fa < fb);
}

ଶୂନ୍ୟ forward_cost_layer_gpu(cost_layer l, network net)
{
    ଯଦି (!net.truth) ଫେରନ୍ତୁ;
    ଯଦି(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        add_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    ଯଦି(l.cost_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.ନିବେଶ_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } ଅନ୍ୟଥା ଯଦି (l.cost_type == L1){
        l1_gpu(l.batch*l.inputs, net.ନିବେଶ_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } ଅନ୍ୟଥା ଯଦି (l.cost_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.ନିବେଶ_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } ଅନ୍ୟଥା {
        l2_gpu(l.batch*l.inputs, net.ନିବେଶ_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    ଯଦି (l.cost_type == SEG && l.noobject_scale != 1) {
        scale_mask_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_mask_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    ଯଦି (l.cost_type == MASKED) {
        mask_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_ଅଙ୍କ, net.truth_gpu, 0);
    }

    ଯଦି(l.ratio){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, ଆକାର(ଦଶମିକ), float_abs_compare);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = (1-l.ratio) * l.batch*l.inputs;
        ଦଶମିକ thresh = l.delta[n];
        thresh = 0;
        printf("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    ଯଦି(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

ଶୂନ୍ୟ backward_cost_layer_gpu(ଅଚଳ cost_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

