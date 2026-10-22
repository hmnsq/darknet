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
    यदि (म_भेद(s, "seg")==0) प्रत्यागम SEG;
    यदि (म_भेद(s, "sse")==0) प्रत्यागम SSE;
    यदि (म_भेद(s, "masked")==0) प्रत्यागम MASKED;
    यदि (म_भेद(s, "smooth")==0) प्रत्यागम SMOOTH;
    यदि (म_भेद(s, "L1")==0) प्रत्यागम L1;
    यदि (म_भेद(s, "wgan")==0) प्रत्यागम WGAN;
    ख_लिखो(मानक_त्रुटि, "Couldn't find cost type %s, going with SSE\n", s);
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
    ख_लिखो(मानक_त्रुटि, "cost                                           %4d\n",  inputs);
    cost_layer l = {0};
    l.type = COST;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.cost_type = cost_type;
    l.delta = सुस्मृति(inputs*batch, परिमाण(भिन्न));
    l.output = सुस्मृति(inputs*batch, परिमाण(भिन्न));
    l.cost = सुस्मृति(1, परिमाण(भिन्न));

    l.forward = forward_cost_layer;
    l.backward = backward_cost_layer;
    #ifdef GPU
    l.forward_gpu = forward_cost_layer_gpu;
    l.backward_gpu = backward_cost_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #endif
    प्रत्यागम l;
}

शून्य resize_cost_layer(cost_layer *l, पूर्णाङ्क inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = पुनः_स्मृति(l->delta, inputs*l->batch*परिमाण(भिन्न));
    l->output = पुनः_स्मृति(l->output, inputs*l->batch*परिमाण(भिन्न));
#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#endif
}

शून्य forward_cost_layer(cost_layer l, network net)
{
    यदि (!net.truth) प्रत्यागम;
    यदि(l.cost_type == MASKED){
        पूर्णाङ्क i;
        प्रति(i = 0; i < l.batch*l.inputs; ++i){
            यदि(net.truth[i] == SECRET_अंक) net.निवेश[i] = SECRET_अंक;
        }
    }
    यदि(l.cost_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.निवेश, net.truth, l.delta, l.output);
    }अन्यथा यदि(l.cost_type == L1){
        l1_cpu(l.batch*l.inputs, net.निवेश, net.truth, l.delta, l.output);
    } अन्यथा {
        l2_cpu(l.batch*l.inputs, net.निवेश, net.truth, l.delta, l.output);
    }
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

शून्य backward_cost_layer(ध्रुव cost_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#ifdef GPU

शून्य pull_cost_layer(cost_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

शून्य धकेलो_cost_layer(cost_layer l)
{
    cuda_धकेलो_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

पूर्णाङ्क float_abs_compare (ध्रुव शून्य * a, ध्रुव शून्य * b)
{
    भिन्न fa = *(ध्रुव भिन्न*) a;
    यदि(fa < 0) fa = -fa;
    भिन्न fb = *(ध्रुव भिन्न*) b;
    यदि(fb < 0) fb = -fb;
    प्रत्यागम (fa > fb) - (fa < fb);
}

शून्य forward_cost_layer_gpu(cost_layer l, network net)
{
    यदि (!net.truth) प्रत्यागम;
    यदि(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        add_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    यदि(l.cost_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.निवेश_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } अन्यथा यदि (l.cost_type == L1){
        l1_gpu(l.batch*l.inputs, net.निवेश_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } अन्यथा यदि (l.cost_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.निवेश_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } अन्यथा {
        l2_gpu(l.batch*l.inputs, net.निवेश_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    यदि (l.cost_type == SEG && l.noobject_scale != 1) {
        scale_mask_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_mask_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    यदि (l.cost_type == MASKED) {
        mask_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_अंक, net.truth_gpu, 0);
    }

    यदि(l.ratio){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        क्विक(l.delta, l.batch*l.inputs, परिमाण(भिन्न), float_abs_compare);
        पूर्णाङ्क n = (1-l.ratio) * l.batch*l.inputs;
        भिन्न thresh = l.delta[n];
        thresh = 0;
        म_लिखो("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    यदि(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

शून्य backward_cost_layer_gpu(ध्रुव cost_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

