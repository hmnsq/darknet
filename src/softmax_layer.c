#ಅನ್ತರ್ಭಾವ "softmax_layer.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"

#ಅನ್ತರ್ಭಾವ <float.h>
#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <assert.h>

softmax_layer make_softmax_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ inputs, ಪೂರ್ಣಾಙ್ಕ groups)
{
    ಪ್ರತಿಜ್ಞಾ(inputs%groups == 0);
    fprintf(stderr, "softmax                                        %4d\n",  inputs);
    softmax_layer l = {0};
    l.type = SOFTMAX;
    l.batch = batch;
    l.groups = groups;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = calloc(inputs*batch, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.output = calloc(inputs*batch, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.delta = calloc(inputs*batch, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.cost = calloc(1, ಪರಿಮಾಣ(ಭಿನ್ನ));

    l.forward = forward_softmax_layer;
    l.backward = backward_softmax_layer;
    #ifdef GPU
    l.forward_gpu = forward_softmax_layer_gpu;
    l.backward_gpu = backward_softmax_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ forward_softmax_layer(ಧ್ರುವ softmax_layer l, network net)
{
    ಯದಿ(l.softmax_ವೃಕ್ಷ){
        ಪೂರ್ಣಾಙ್ಕ i;
        ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
        ಪ್ರತಿ (i = 0; i < l.softmax_ವೃಕ್ಷ->groups; ++i) {
            ಪೂರ್ಣಾಙ್ಕ group_size = l.softmax_ವೃಕ್ಷ->group_size[i];
            softmax_cpu(net.ಒಳಹರಿವು + ಎಣಿಕೆ, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output + ಎಣಿಕೆ);
            ಎಣಿಕೆ += group_size;
        }
    } ಅನ್ಯಥಾ {
        softmax_cpu(net.ಒಳಹರಿವು, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output);
    }

    ಯದಿ(net.truth && !l.noloss){
        softmax_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

ಶೂನ್ಯ backward_softmax_layer(ಧ್ರುವ softmax_layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

ಶೂನ್ಯ pull_softmax_layer_output(ಧ್ರುವ softmax_layer layer)
{
    cuda_pull_array(layer.output_gpu, layer.output, layer.inputs*layer.batch);
}

ಶೂನ್ಯ forward_softmax_layer_gpu(ಧ್ರುವ softmax_layer l, network net)
{
    ಯದಿ(l.softmax_ವೃಕ್ಷ){
        softmax_ವೃಕ್ಷ(net.ಒಳಹರಿವು_gpu, 1, l.batch, l.inputs, l.temperature, l.output_gpu, *l.softmax_ವೃಕ್ಷ);
        /*
        int i;
        int count = 0;
        for (i = 0; i < l.softmax_tree->groups; ++i) {
            int group_size = l.softmax_tree->group_size[i];
            softmax_gpu(net.input_gpu + count, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output_gpu + count);
            count += group_size;
        }
        */
    } ಅನ್ಯಥಾ {
        ಯದಿ(l.spatial){
            softmax_gpu(net.ಒಳಹರಿವು_gpu, l.c, l.batch*l.c, l.inputs/l.c, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu);
        }ಅನ್ಯಥಾ{
            softmax_gpu(net.ಒಳಹರಿವು_gpu, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output_gpu);
        }
    }
    ಯದಿ(net.truth && !l.noloss){
        softmax_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        ಯದಿ(l.softmax_ವೃಕ್ಷ){
            mask_gpu(l.batch*l.inputs, l.delta_gpu, SECRET_ಅಂಕೆ, net.truth_gpu, 0);
            mask_gpu(l.batch*l.inputs, l.loss_gpu, SECRET_ಅಂಕೆ, net.truth_gpu, 0);
        }
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

ಶೂನ್ಯ backward_softmax_layer_gpu(ಧ್ರುವ softmax_layer layer, network net)
{
    axpy_gpu(layer.batch*layer.inputs, 1, layer.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
