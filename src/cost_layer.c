#અન્તર્ભાવ "cost_layer.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ <math.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <stdio.h>

COST_TYPE get_cost_type(અક્ષર *s)
{
    જો (strcmp(s, "seg")==0) પાછા SEG;
    જો (strcmp(s, "sse")==0) પાછા SSE;
    જો (strcmp(s, "masked")==0) પાછા MASKED;
    જો (strcmp(s, "smooth")==0) પાછા SMOOTH;
    જો (strcmp(s, "L1")==0) પાછા L1;
    જો (strcmp(s, "wgan")==0) પાછા WGAN;
    fprintf(stderr, "Couldn't find cost type %s, going with SSE\n", s);
    પાછા SSE;
}

અક્ષર *get_cost_string(COST_TYPE a)
{
    પસંદગી(a){
        સ્થિતિ SEG:
            પાછા "seg";
        સ્થિતિ SSE:
            પાછા "sse";
        સ્થિતિ MASKED:
            પાછા "masked";
        સ્થિતિ SMOOTH:
            પાછા "smooth";
        સ્થિતિ L1:
            પાછા "L1";
        સ્થિતિ WGAN:
            પાછા "wgan";
    }
    પાછા "sse";
}

cost_layer make_cost_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs, COST_TYPE cost_type, દશાંશ scale)
{
    fprintf(stderr, "cost                                           %4d\n",  inputs);
    cost_layer l = {0};
    l.type = COST;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.cost_type = cost_type;
    l.delta = calloc(inputs*batch, કદ(દશાંશ));
    l.output = calloc(inputs*batch, કદ(દશાંશ));
    l.cost = calloc(1, કદ(દશાંશ));

    l.forward = forward_cost_layer;
    l.backward = backward_cost_layer;
    #ifdef GPU
    l.forward_gpu = forward_cost_layer_gpu;
    l.backward_gpu = backward_cost_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #endif
    પાછા l;
}

શૂન્ય resize_cost_layer(cost_layer *l, પૂર્ણાંક inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = realloc(l->delta, inputs*l->batch*કદ(દશાંશ));
    l->output = realloc(l->output, inputs*l->batch*કદ(દશાંશ));
#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#endif
}

શૂન્ય forward_cost_layer(cost_layer l, network net)
{
    જો (!net.truth) પાછા;
    જો(l.cost_type == MASKED){
        પૂર્ણાંક i;
        માટે(i = 0; i < l.batch*l.inputs; ++i){
            જો(net.truth[i] == SECRET_અંક) net.આગત[i] = SECRET_અંક;
        }
    }
    જો(l.cost_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.આગત, net.truth, l.delta, l.output);
    }નહીંતર જો(l.cost_type == L1){
        l1_cpu(l.batch*l.inputs, net.આગત, net.truth, l.delta, l.output);
    } નહીંતર {
        l2_cpu(l.batch*l.inputs, net.આગત, net.truth, l.delta, l.output);
    }
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

શૂન્ય backward_cost_layer(અચલ cost_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#ifdef GPU

શૂન્ય pull_cost_layer(cost_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

શૂન્ય ધકેલો_cost_layer(cost_layer l)
{
    cuda_ધકેલો_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

પૂર્ણાંક float_abs_compare (અચલ શૂન્ય * a, અચલ શૂન્ય * b)
{
    દશાંશ fa = *(અચલ દશાંશ*) a;
    જો(fa < 0) fa = -fa;
    દશાંશ fb = *(અચલ દશાંશ*) b;
    જો(fb < 0) fb = -fb;
    પાછા (fa > fb) - (fa < fb);
}

શૂન્ય forward_cost_layer_gpu(cost_layer l, network net)
{
    જો (!net.truth) પાછા;
    જો(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        add_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    જો(l.cost_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.આગત_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } નહીંતર જો (l.cost_type == L1){
        l1_gpu(l.batch*l.inputs, net.આગત_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } નહીંતર જો (l.cost_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.આગત_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } નહીંતર {
        l2_gpu(l.batch*l.inputs, net.આગત_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    જો (l.cost_type == SEG && l.noobject_scale != 1) {
        scale_mask_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_mask_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    જો (l.cost_type == MASKED) {
        mask_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_અંક, net.truth_gpu, 0);
    }

    જો(l.ratio){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, કદ(દશાંશ), float_abs_compare);
        પૂર્ણાંક n = (1-l.ratio) * l.batch*l.inputs;
        દશાંશ thresh = l.delta[n];
        thresh = 0;
        printf("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    જો(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

શૂન્ય backward_cost_layer_gpu(અચલ cost_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

