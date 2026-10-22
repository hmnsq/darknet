#अन्तर्भाव "softmax_layer.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "cuda.h"

#अन्तर्भाव <float.h>
#अन्तर्भाव <math.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <assert.h>

softmax_layer make_softmax_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, पूर्णाङ्क groups)
{
    प्रतिज्ञा(inputs%groups == 0);
    ख_लिखो(मानक_त्रुटि, "softmax                                        %4d\n",  inputs);
    softmax_layer l = {0};
    l.type = SOFTMAX;
    l.batch = batch;
    l.groups = groups;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = सुस्मृति(inputs*batch, परिमाण(भिन्न));
    l.output = सुस्मृति(inputs*batch, परिमाण(भिन्न));
    l.delta = सुस्मृति(inputs*batch, परिमाण(भिन्न));
    l.cost = सुस्मृति(1, परिमाण(भिन्न));

    l.forward = forward_softmax_layer;
    l.backward = backward_softmax_layer;
    #ifdef GPU
    l.forward_gpu = forward_softmax_layer_gpu;
    l.backward_gpu = backward_softmax_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    फर्काऊ l;
}

शून्य forward_softmax_layer(अचर softmax_layer l, network net)
{
    यदि(l.softmax_रूख){
        पूर्णाङ्क i;
        पूर्णाङ्क गन्ती = 0;
        लागि (i = 0; i < l.softmax_रूख->groups; ++i) {
            पूर्णाङ्क group_size = l.softmax_रूख->group_size[i];
            softmax_cpu(net.आगत + गन्ती, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output + गन्ती);
            गन्ती += group_size;
        }
    } अन्यथा {
        softmax_cpu(net.आगत, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output);
    }

    यदि(net.truth && !l.noloss){
        softmax_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

शून्य backward_softmax_layer(अचर softmax_layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

शून्य pull_softmax_layer_output(अचर softmax_layer layer)
{
    cuda_pull_array(layer.output_gpu, layer.output, layer.inputs*layer.batch);
}

शून्य forward_softmax_layer_gpu(अचर softmax_layer l, network net)
{
    यदि(l.softmax_रूख){
        softmax_रूख(net.आगत_gpu, 1, l.batch, l.inputs, l.temperature, l.output_gpu, *l.softmax_रूख);
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
        यदि(l.spatial){
            softmax_gpu(net.आगत_gpu, l.c, l.batch*l.c, l.inputs/l.c, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu);
        }अन्यथा{
            softmax_gpu(net.आगत_gpu, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output_gpu);
        }
    }
    यदि(net.truth && !l.noloss){
        softmax_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        यदि(l.softmax_रूख){
            mask_gpu(l.batch*l.inputs, l.delta_gpu, SECRET_अङ्क, net.truth_gpu, 0);
            mask_gpu(l.batch*l.inputs, l.loss_gpu, SECRET_अङ्क, net.truth_gpu, 0);
        }
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

शून्य backward_softmax_layer_gpu(अचर softmax_layer layer, network net)
{
    axpy_gpu(layer.batch*layer.inputs, 1, layer.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
