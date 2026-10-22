#अन्तर्भाव "cost_layer.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव <math.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>

COST_TYPE get_cost_type(अक्षर *s)
{
    जर (म_भेद(s, "seg")==0) परत SEG;
    जर (म_भेद(s, "sse")==0) परत SSE;
    जर (म_भेद(s, "masked")==0) परत MASKED;
    जर (म_भेद(s, "smooth")==0) परत SMOOTH;
    जर (म_भेद(s, "L1")==0) परत L1;
    जर (म_भेद(s, "wgan")==0) परत WGAN;
    ख_लिखो(मानक_त्रुटि, "Couldn't find cost type %s, going with SSE\n", s);
    परत SSE;
}

अक्षर *get_cost_string(COST_TYPE a)
{
    निवड(a){
        स्थिती SEG:
            परत "seg";
        स्थिती SSE:
            परत "sse";
        स्थिती MASKED:
            परत "masked";
        स्थिती SMOOTH:
            परत "smooth";
        स्थिती L1:
            परत "L1";
        स्थिती WGAN:
            परत "wgan";
    }
    परत "sse";
}

cost_layer make_cost_layer(पूर्णांक batch, पूर्णांक inputs, COST_TYPE cost_type, दशांश scale)
{
    ख_लिखो(मानक_त्रुटि, "cost                                           %4d\n",  inputs);
    cost_layer l = {0};
    l.type = COST;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.cost_type = cost_type;
    l.delta = सुस्मृति(inputs*batch, आकार(दशांश));
    l.output = सुस्मृति(inputs*batch, आकार(दशांश));
    l.cost = सुस्मृति(1, आकार(दशांश));

    l.forward = forward_cost_layer;
    l.backward = backward_cost_layer;
    #ifdef GPU
    l.forward_gpu = forward_cost_layer_gpu;
    l.backward_gpu = backward_cost_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #endif
    परत l;
}

शून्य resize_cost_layer(cost_layer *l, पूर्णांक inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = पुनः_स्मृति(l->delta, inputs*l->batch*आकार(दशांश));
    l->output = पुनः_स्मृति(l->output, inputs*l->batch*आकार(दशांश));
#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#endif
}

शून्य forward_cost_layer(cost_layer l, network net)
{
    जर (!net.truth) परत;
    जर(l.cost_type == MASKED){
        पूर्णांक i;
        साठी(i = 0; i < l.batch*l.inputs; ++i){
            जर(net.truth[i] == SECRET_अंक) net.आदान[i] = SECRET_अंक;
        }
    }
    जर(l.cost_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.आदान, net.truth, l.delta, l.output);
    }अन्यथा जर(l.cost_type == L1){
        l1_cpu(l.batch*l.inputs, net.आदान, net.truth, l.delta, l.output);
    } अन्यथा {
        l2_cpu(l.batch*l.inputs, net.आदान, net.truth, l.delta, l.output);
    }
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

शून्य backward_cost_layer(स्थिरांक cost_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#ifdef GPU

शून्य pull_cost_layer(cost_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

शून्य ढकला_cost_layer(cost_layer l)
{
    cuda_ढकला_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

पूर्णांक float_abs_compare (स्थिरांक शून्य * a, स्थिरांक शून्य * b)
{
    दशांश fa = *(स्थिरांक दशांश*) a;
    जर(fa < 0) fa = -fa;
    दशांश fb = *(स्थिरांक दशांश*) b;
    जर(fb < 0) fb = -fb;
    परत (fa > fb) - (fa < fb);
}

शून्य forward_cost_layer_gpu(cost_layer l, network net)
{
    जर (!net.truth) परत;
    जर(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        add_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    जर(l.cost_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.आदान_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } अन्यथा जर (l.cost_type == L1){
        l1_gpu(l.batch*l.inputs, net.आदान_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } अन्यथा जर (l.cost_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.आदान_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } अन्यथा {
        l2_gpu(l.batch*l.inputs, net.आदान_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    जर (l.cost_type == SEG && l.noobject_scale != 1) {
        scale_mask_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_mask_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    जर (l.cost_type == MASKED) {
        mask_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_अंक, net.truth_gpu, 0);
    }

    जर(l.ratio){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        क्विक(l.delta, l.batch*l.inputs, आकार(दशांश), float_abs_compare);
        पूर्णांक n = (1-l.ratio) * l.batch*l.inputs;
        दशांश thresh = l.delta[n];
        thresh = 0;
        म_लिखो("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    जर(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.cost[0] = sum_array(l.output, l.batch*l.inputs);
}

शून्य backward_cost_layer_gpu(स्थिरांक cost_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

