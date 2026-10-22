#അന്തര്ഭാവ "cost_layer.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdio.h>

COST_TYPE get_cost_type(അക്ഷരം *s)
{
    എങ്കിൽ (strcmp(s, "seg")==0) തിരികെ SEG;
    എങ്കിൽ (strcmp(s, "sse")==0) തിരികെ SSE;
    എങ്കിൽ (strcmp(s, "masked")==0) തിരികെ MASKED;
    എങ്കിൽ (strcmp(s, "smooth")==0) തിരികെ SMOOTH;
    എങ്കിൽ (strcmp(s, "L1")==0) തിരികെ L1;
    എങ്കിൽ (strcmp(s, "wgan")==0) തിരികെ WGAN;
    fprintf(stderr, "Couldn't find cost type %s, going with SSE\n", s);
    തിരികെ SSE;
}

അക്ഷരം *get_cost_string(COST_TYPE a)
{
    തിരഞ്ഞെടുപ്പ്(a){
        അവസ്ഥ SEG:
            തിരികെ "seg";
        അവസ്ഥ SSE:
            തിരികെ "sse";
        അവസ്ഥ MASKED:
            തിരികെ "masked";
        അവസ്ഥ SMOOTH:
            തിരികെ "smooth";
        അവസ്ഥ L1:
            തിരികെ "L1";
        അവസ്ഥ WGAN:
            തിരികെ "wgan";
    }
    തിരികെ "sse";
}

cost_layer make_cost_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, COST_TYPE cost_type, ദശാംശം scale)
{
    fprintf(stderr, "cost                                           %4d\n",  inputs);
    cost_layer l = {0};
    l.type = COST;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.cost_type = cost_type;
    l.delta = calloc(inputs*batch, വലുപ്പം(ദശാംശം));
    l.output = calloc(inputs*batch, വലുപ്പം(ദശാംശം));
    l.cost = calloc(1, വലുപ്പം(ദശാംശം));

    l.forward = forward_cost_layer;
    l.backward = backward_cost_layer;
    #ifdef GPU
    l.forward_gpu = forward_cost_layer_gpu;
    l.backward_gpu = backward_cost_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #endif
    തിരികെ l;
}

ശൂന്യം resize_cost_layer(cost_layer *l, പൂർണ്ണസംഖ്യ inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = realloc(l->delta, inputs*l->batch*വലുപ്പം(ദശാംശം));
    l->output = realloc(l->output, inputs*l->batch*വലുപ്പം(ദശാംശം));
#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#endif
}

ശൂന്യം forward_cost_layer(cost_layer l, network net)
{
    എങ്കിൽ (!net.truth) തിരികെ;
    എങ്കിൽ(l.cost_type == MASKED){
        പൂർണ്ണസംഖ്യ i;
        വേണ്ടി(i = 0; i < l.batch*l.inputs; ++i){
            എങ്കിൽ(net.truth[i] == SECRET_അക്കം) net.നിവേശം[i] = SECRET_അക്കം;
        }
    }
    എങ്കിൽ(l.cost_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.നിവേശം, net.truth, l.delta, l.output);
    }അല്ലെങ്കിൽ എങ്കിൽ(l.cost_type == L1){
        l1_cpu(l.batch*l.inputs, net.നിവേശം, net.truth, l.delta, l.output);
    } അല്ലെങ്കിൽ {
        l2_cpu(l.batch*l.inputs, net.നിവേശം, net.truth, l.delta, l.output);
    }
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

ശൂന്യം backward_cost_layer(സ്ഥിരം cost_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#ifdef GPU

ശൂന്യം pull_cost_layer(cost_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

ശൂന്യം തള്ളുക_cost_layer(cost_layer l)
{
    cuda_തള്ളുക_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

പൂർണ്ണസംഖ്യ float_abs_compare (സ്ഥിരം ശൂന്യം * a, സ്ഥിരം ശൂന്യം * b)
{
    ദശാംശം fa = *(സ്ഥിരം ദശാംശം*) a;
    എങ്കിൽ(fa < 0) fa = -fa;
    ദശാംശം fb = *(സ്ഥിരം ദശാംശം*) b;
    എങ്കിൽ(fb < 0) fb = -fb;
    തിരികെ (fa > fb) - (fa < fb);
}

ശൂന്യം forward_cost_layer_gpu(cost_layer l, network net)
{
    എങ്കിൽ (!net.truth) തിരികെ;
    എങ്കിൽ(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        add_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    എങ്കിൽ(l.cost_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.നിവേശം_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } അല്ലെങ്കിൽ എങ്കിൽ (l.cost_type == L1){
        l1_gpu(l.batch*l.inputs, net.നിവേശം_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } അല്ലെങ്കിൽ എങ്കിൽ (l.cost_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.നിവേശം_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } അല്ലെങ്കിൽ {
        l2_gpu(l.batch*l.inputs, net.നിവേശം_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    എങ്കിൽ (l.cost_type == SEG && l.noobject_scale != 1) {
        scale_mask_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_mask_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    എങ്കിൽ (l.cost_type == MASKED) {
        mask_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_അക്കം, net.truth_gpu, 0);
    }

    എങ്കിൽ(l.ratio){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, വലുപ്പം(ദശാംശം), float_abs_compare);
        പൂർണ്ണസംഖ്യ n = (1-l.ratio) * l.batch*l.inputs;
        ദശാംശം thresh = l.delta[n];
        thresh = 0;
        printf("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    എങ്കിൽ(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

ശൂന്യം backward_cost_layer_gpu(സ്ഥിരം cost_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

