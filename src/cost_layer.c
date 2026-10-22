#अन्तर्भाव "cost_layer.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव <math.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>

COST_TYPE get_cost_type(वर्ण *s)
{
    यदि (वाक्यं_तुलय(s, "seg")==0) प्रत्यागम SEG;
    यदि (वाक्यं_तुलय(s, "sse")==0) प्रत्यागम SSE;
    यदि (वाक्यं_तुलय(s, "masked")==0) प्रत्यागम MASKED;
    यदि (वाक्यं_तुलय(s, "smooth")==0) प्रत्यागम SMOOTH;
    यदि (वाक्यं_तुलय(s, "L1")==0) प्रत्यागम L1;
    यदि (वाक्यं_तुलय(s, "wgan")==0) प्रत्यागम WGAN;
    fprintf(stderr, "Couldn't find cost type %s, going with SSE\n", s);
    प्रत्यागम SSE;
}

वर्ण *get_cost_string(COST_TYPE a)
{
    विकल्प(a){
        पक्ष SEG:
            प्रत्यागम "seg";
        पक्ष SSE:
            प्रत्यागम "sse";
        पक्ष MASKED:
            प्रत्यागम "masked";
        पक्ष SMOOTH:
            प्रत्यागम "smooth";
        पक्ष L1:
            प्रत्यागम "L1";
        पक्ष WGAN:
            प्रत्यागम "wgan";
    }
    प्रत्यागम "sse";
}

cost_layer make_cost_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, COST_TYPE cost_type, भिन्न scale)
{
    fprintf(stderr, "cost                                           %4d\n",  inputs);
    cost_layer l = {0};
    l.type = COST;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.cost_type = cost_type;
    l.delta = शून्यस्मृतिं_देहि(inputs*batch, परिमाण(भिन्न));
    l.output = शून्यस्मृतिं_देहि(inputs*batch, परिमाण(भिन्न));
    l.cost = शून्यस्मृतिं_देहि(1, परिमाण(भिन्न));

    l.forward = forward_cost_layer;
    l.backward = backward_cost_layer;
    #यदि_परिभाषितम् GPU
    l.forward_gpu = forward_cost_layer_gpu;
    l.backward_gpu = backward_cost_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #यदि_अन्तः
    प्रत्यागम l;
}

शून्य resize_cost_layer(cost_layer *l, पूर्णाङ्क inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = realloc(l->delta, inputs*l->batch*परिमाण(भिन्न));
    l->output = realloc(l->output, inputs*l->batch*परिमाण(भिन्न));
#यदि_परिभाषितम् GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#यदि_अन्तः
}

शून्य forward_cost_layer(cost_layer l, जालम् net)
{
    यदि (!net.truth) प्रत्यागम;
    यदि(l.cost_type == MASKED){
        पूर्णाङ्क i;
        प्रति(i = 0; i < l.batch*l.inputs; ++i){
            यदि(net.truth[i] == SECRET_अङ्कः) net.निवेशः[i] = SECRET_अङ्कः;
        }
    }
    यदि(l.cost_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.निवेशः, net.truth, l.delta, l.output);
    }अन्यथा यदि(l.cost_type == L1){
        l1_cpu(l.batch*l.inputs, net.निवेशः, net.truth, l.delta, l.output);
    } अन्यथा {
        l2_cpu(l.batch*l.inputs, net.निवेशः, net.truth, l.delta, l.output);
    }
    l.cost[0] = योग_array(l.output, l.batch*l.inputs);
}

शून्य backward_cost_layer(ध्रुव cost_layer l, जालम् net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#यदि_परिभाषितम् GPU

शून्य pull_cost_layer(cost_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

शून्य प्रक्षिप_cost_layer(cost_layer l)
{
    cuda_प्रक्षिप_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

पूर्णाङ्क float_abs_compare (ध्रुव शून्य * a, ध्रुव शून्य * b)
{
    भिन्न fa = *(ध्रुव भिन्न*) a;
    यदि(fa < 0) fa = -fa;
    भिन्न fb = *(ध्रुव भिन्न*) b;
    यदि(fb < 0) fb = -fb;
    प्रत्यागम (fa > fb) - (fa < fb);
}

शून्य forward_cost_layer_gpu(cost_layer l, जालम् net)
{
    यदि (!net.truth) प्रत्यागम;
    यदि(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        सङ्कलय_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    यदि(l.cost_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.निवेशः_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } अन्यथा यदि (l.cost_type == L1){
        l1_gpu(l.batch*l.inputs, net.निवेशः_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } अन्यथा यदि (l.cost_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.निवेशः_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } अन्यथा {
        l2_gpu(l.batch*l.inputs, net.निवेशः_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    यदि (l.cost_type == SEG && l.noobject_scale != 1) {
        scale_आवरणम्_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_आवरणम्_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    यदि (l.cost_type == MASKED) {
        आवरणम्_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_अङ्कः, net.truth_gpu, 0);
    }

    यदि(l.अनुपातः){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        शीघ्रं_क्रमय(l.delta, l.batch*l.inputs, परिमाण(भिन्न), float_abs_compare);
        पूर्णाङ्क n = (1-l.अनुपातः) * l.batch*l.inputs;
        भिन्न thresh = l.delta[n];
        thresh = 0;
        प्रारूप_लिख("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    यदि(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.cost[0] = योग_array(l.output, l.batch*l.inputs);
}

शून्य backward_cost_layer_gpu(ध्रुव cost_layer l, जालम् net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#यदि_अन्तः

