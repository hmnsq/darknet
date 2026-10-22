#समावेश "softmax_layer.h"
#समावेश "blas.h"
#समावेश "cuda.h"

#समावेश <float.h>
#समावेश <math.h>
#समावेश <stdlib.h>
#समावेश <stdio.h>
#समावेश <assert.h>

softmax_layer make_softmax_layer(पूर्णांक batch, पूर्णांक inputs, पूर्णांक groups)
{
    निश्चित(inputs%groups == 0);
    ख_लिखो(मानक_त्रुटि, "softmax                                        %4d\n",  inputs);
    softmax_layer l = {0};
    l.type = SOFTMAX;
    l.batch = batch;
    l.groups = groups;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = सुस्मृति(inputs*batch, माप(भग्न));
    l.output = सुस्मृति(inputs*batch, माप(भग्न));
    l.delta = सुस्मृति(inputs*batch, माप(भग्न));
    l.cost = सुस्मृति(1, माप(भग्न));

    l.forward = forward_softmax_layer;
    l.backward = backward_softmax_layer;
    #अगर_घोषित GPU
    l.forward_gpu = forward_softmax_layer_gpu;
    l.backward_gpu = backward_softmax_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #पूर्ण_अगर
    वापस l;
}

व्योम forward_softmax_layer(स्थिर softmax_layer l, network net)
{
    अगर(l.softmax_वृक्ष){
        पूर्णांक i;
        पूर्णांक गिनती = 0;
        क्रम (i = 0; i < l.softmax_वृक्ष->groups; ++i) {
            पूर्णांक group_आकार = l.softmax_वृक्ष->group_आकार[i];
            softmax_cpu(net.निवेश + गिनती, group_आकार, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output + गिनती);
            गिनती += group_आकार;
        }
    } अन्यथा {
        softmax_cpu(net.निवेश, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output);
    }

    अगर(net.truth && !l.noloss){
        softmax_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = योग_array(l.loss, l.batch*l.inputs);
    }
}

व्योम backward_softmax_layer(स्थिर softmax_layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#अगर_घोषित GPU

व्योम pull_softmax_layer_output(स्थिर softmax_layer layer)
{
    cuda_pull_array(layer.output_gpu, layer.output, layer.inputs*layer.batch);
}

व्योम forward_softmax_layer_gpu(स्थिर softmax_layer l, network net)
{
    अगर(l.softmax_वृक्ष){
        softmax_वृक्ष(net.निवेश_gpu, 1, l.batch, l.inputs, l.temperature, l.output_gpu, *l.softmax_वृक्ष);
        /*
        int i;
        int count = 0;
        for (i = 0; i < l.softmax_tree->groups; ++i) {
            int group_size = l.softmax_tree->group_size[i];
            softmax_gpu(net.input_gpu + count, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output_gpu + count);
            count += group_size;
        }
        */
    } अन्यथा {
        अगर(l.spatial){
            softmax_gpu(net.निवेश_gpu, l.c, l.batch*l.c, l.inputs/l.c, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu);
        }अन्यथा{
            softmax_gpu(net.निवेश_gpu, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output_gpu);
        }
    }
    अगर(net.truth && !l.noloss){
        softmax_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        अगर(l.softmax_वृक्ष){
            mask_gpu(l.batch*l.inputs, l.delta_gpu, SECRET_अंक, net.truth_gpu, 0);
            mask_gpu(l.batch*l.inputs, l.loss_gpu, SECRET_अंक, net.truth_gpu, 0);
        }
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = योग_array(l.loss, l.batch*l.inputs);
    }
}

व्योम backward_softmax_layer_gpu(स्थिर softmax_layer layer, network net)
{
    axpy_gpu(layer.batch*layer.inputs, 1, layer.delta_gpu, 1, net.delta_gpu, 1);
}

#पूर्ण_अगर
