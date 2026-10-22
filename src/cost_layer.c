#समावेश "cost_layer.h"
#समावेश "utils.h"
#समावेश "cuda.h"
#समावेश "blas.h"
#समावेश <math.h>
#समावेश <string.h>
#समावेश <stdlib.h>
#समावेश <stdio.h>

COST_TYPE get_cost_type(अक्षर *s)
{
    अगर (म_भेद(s, "seg")==0) वापस SEG;
    अगर (म_भेद(s, "sse")==0) वापस SSE;
    अगर (म_भेद(s, "masked")==0) वापस MASKED;
    अगर (म_भेद(s, "smooth")==0) वापस SMOOTH;
    अगर (म_भेद(s, "L1")==0) वापस L1;
    अगर (म_भेद(s, "wgan")==0) वापस WGAN;
    ख_लिखो(मानक_त्रुटि, "Couldn't find cost type %s, going with SSE\n", s);
    वापस SSE;
}

अक्षर *get_cost_string(COST_TYPE a)
{
    चयन(a){
        हाल SEG:
            वापस "seg";
        हाल SSE:
            वापस "sse";
        हाल MASKED:
            वापस "masked";
        हाल SMOOTH:
            वापस "smooth";
        हाल L1:
            वापस "L1";
        हाल WGAN:
            वापस "wgan";
    }
    वापस "sse";
}

cost_layer make_cost_layer(पूर्णांक batch, पूर्णांक inputs, COST_TYPE cost_type, भग्न scale)
{
    ख_लिखो(मानक_त्रुटि, "cost                                           %4d\n",  inputs);
    cost_layer l = {0};
    l.type = COST;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.cost_type = cost_type;
    l.delta = सुस्मृति(inputs*batch, माप(भग्न));
    l.output = सुस्मृति(inputs*batch, माप(भग्न));
    l.cost = सुस्मृति(1, माप(भग्न));

    l.forward = forward_cost_layer;
    l.backward = backward_cost_layer;
    #अगर_घोषित GPU
    l.forward_gpu = forward_cost_layer_gpu;
    l.backward_gpu = backward_cost_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #पूर्ण_अगर
    वापस l;
}

व्योम resize_cost_layer(cost_layer *l, पूर्णांक inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = पुनः_स्मृति(l->delta, inputs*l->batch*माप(भग्न));
    l->output = पुनः_स्मृति(l->output, inputs*l->batch*माप(भग्न));
#अगर_घोषित GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#पूर्ण_अगर
}

व्योम forward_cost_layer(cost_layer l, network net)
{
    अगर (!net.truth) वापस;
    अगर(l.cost_type == MASKED){
        पूर्णांक i;
        क्रम(i = 0; i < l.batch*l.inputs; ++i){
            अगर(net.truth[i] == SECRET_अंक) net.निवेश[i] = SECRET_अंक;
        }
    }
    अगर(l.cost_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.निवेश, net.truth, l.delta, l.output);
    }अन्यथा अगर(l.cost_type == L1){
        l1_cpu(l.batch*l.inputs, net.निवेश, net.truth, l.delta, l.output);
    } अन्यथा {
        l2_cpu(l.batch*l.inputs, net.निवेश, net.truth, l.delta, l.output);
    }
    l.cost[0] = योग_array(l.output, l.batch*l.inputs);
}

व्योम backward_cost_layer(स्थिर cost_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#अगर_घोषित GPU

व्योम pull_cost_layer(cost_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

व्योम धकेलो_cost_layer(cost_layer l)
{
    cuda_धकेलो_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

पूर्णांक float_abs_compare (स्थिर व्योम * a, स्थिर व्योम * b)
{
    भग्न fa = *(स्थिर भग्न*) a;
    अगर(fa < 0) fa = -fa;
    भग्न fb = *(स्थिर भग्न*) b;
    अगर(fb < 0) fb = -fb;
    वापस (fa > fb) - (fa < fb);
}

व्योम forward_cost_layer_gpu(cost_layer l, network net)
{
    अगर (!net.truth) वापस;
    अगर(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        add_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    अगर(l.cost_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.निवेश_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } अन्यथा अगर (l.cost_type == L1){
        l1_gpu(l.batch*l.inputs, net.निवेश_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } अन्यथा अगर (l.cost_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.निवेश_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } अन्यथा {
        l2_gpu(l.batch*l.inputs, net.निवेश_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    अगर (l.cost_type == SEG && l.noobject_scale != 1) {
        scale_mask_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_mask_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    अगर (l.cost_type == MASKED) {
        mask_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_अंक, net.truth_gpu, 0);
    }

    अगर(l.ratio){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        क्विक(l.delta, l.batch*l.inputs, माप(भग्न), float_abs_compare);
        पूर्णांक n = (1-l.ratio) * l.batch*l.inputs;
        भग्न thresh = l.delta[n];
        thresh = 0;
        म_लिखो("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    अगर(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.cost[0] = योग_array(l.output, l.batch*l.inputs);
}

व्योम backward_cost_layer_gpu(स्थिर cost_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#पूर्ण_अगर

