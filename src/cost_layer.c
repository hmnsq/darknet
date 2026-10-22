#அந்தர்பாவ "cost_layer.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ <math.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdio.h>

COST_TYPE get_cost_type(எழுத்து *s)
{
    எனில் (strcmp(s, "seg")==0) திரும்பு SEG;
    எனில் (strcmp(s, "sse")==0) திரும்பு SSE;
    எனில் (strcmp(s, "masked")==0) திரும்பு MASKED;
    எனில் (strcmp(s, "smooth")==0) திரும்பு SMOOTH;
    எனில் (strcmp(s, "L1")==0) திரும்பு L1;
    எனில் (strcmp(s, "wgan")==0) திரும்பு WGAN;
    fprintf(stderr, "Couldn't find cost type %s, going with SSE\n", s);
    திரும்பு SSE;
}

எழுத்து *get_cost_string(COST_TYPE a)
{
    தேர்வு(a){
        நிலை SEG:
            திரும்பு "seg";
        நிலை SSE:
            திரும்பு "sse";
        நிலை MASKED:
            திரும்பு "masked";
        நிலை SMOOTH:
            திரும்பு "smooth";
        நிலை L1:
            திரும்பு "L1";
        நிலை WGAN:
            திரும்பு "wgan";
    }
    திரும்பு "sse";
}

cost_layer make_cost_layer(முழு batch, முழு inputs, COST_TYPE cost_type, தசம scale)
{
    fprintf(stderr, "cost                                           %4d\n",  inputs);
    cost_layer l = {0};
    l.type = COST;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.cost_type = cost_type;
    l.delta = calloc(inputs*batch, அளவு(தசம));
    l.output = calloc(inputs*batch, அளவு(தசம));
    l.cost = calloc(1, அளவு(தசம));

    l.forward = forward_cost_layer;
    l.backward = backward_cost_layer;
    #ifdef GPU
    l.forward_gpu = forward_cost_layer_gpu;
    l.backward_gpu = backward_cost_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #endif
    திரும்பு l;
}

வெறுமை resize_cost_layer(cost_layer *l, முழு inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = realloc(l->delta, inputs*l->batch*அளவு(தசம));
    l->output = realloc(l->output, inputs*l->batch*அளவு(தசம));
#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#endif
}

வெறுமை forward_cost_layer(cost_layer l, network net)
{
    எனில் (!net.truth) திரும்பு;
    எனில்(l.cost_type == MASKED){
        முழு i;
        க்கு(i = 0; i < l.batch*l.inputs; ++i){
            எனில்(net.truth[i] == SECRET_இலக்கம்) net.உள்ளீடு[i] = SECRET_இலக்கம்;
        }
    }
    எனில்(l.cost_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.உள்ளீடு, net.truth, l.delta, l.output);
    }இல்லையெனில் எனில்(l.cost_type == L1){
        l1_cpu(l.batch*l.inputs, net.உள்ளீடு, net.truth, l.delta, l.output);
    } இல்லையெனில் {
        l2_cpu(l.batch*l.inputs, net.உள்ளீடு, net.truth, l.delta, l.output);
    }
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

வெறுமை backward_cost_layer(மாறிலி cost_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#ifdef GPU

வெறுமை pull_cost_layer(cost_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

வெறுமை தள்ளு_cost_layer(cost_layer l)
{
    cuda_தள்ளு_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

முழு float_abs_compare (மாறிலி வெறுமை * a, மாறிலி வெறுமை * b)
{
    தசம fa = *(மாறிலி தசம*) a;
    எனில்(fa < 0) fa = -fa;
    தசம fb = *(மாறிலி தசம*) b;
    எனில்(fb < 0) fb = -fb;
    திரும்பு (fa > fb) - (fa < fb);
}

வெறுமை forward_cost_layer_gpu(cost_layer l, network net)
{
    எனில் (!net.truth) திரும்பு;
    எனில்(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        add_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    எனில்(l.cost_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.உள்ளீடு_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } இல்லையெனில் எனில் (l.cost_type == L1){
        l1_gpu(l.batch*l.inputs, net.உள்ளீடு_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } இல்லையெனில் எனில் (l.cost_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.உள்ளீடு_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } இல்லையெனில் {
        l2_gpu(l.batch*l.inputs, net.உள்ளீடு_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    எனில் (l.cost_type == SEG && l.noobject_scale != 1) {
        scale_mask_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_mask_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    எனில் (l.cost_type == MASKED) {
        mask_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_இலக்கம், net.truth_gpu, 0);
    }

    எனில்(l.ratio){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, அளவு(தசம), float_abs_compare);
        முழு n = (1-l.ratio) * l.batch*l.inputs;
        தசம thresh = l.delta[n];
        thresh = 0;
        printf("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    எனில்(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

வெறுமை backward_cost_layer_gpu(மாறிலி cost_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

