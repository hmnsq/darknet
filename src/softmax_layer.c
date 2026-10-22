#ಅನ್ತರ್ಭಾವ "softmax_layer.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"

#ಅನ್ತರ್ಭಾವ <float.h>
#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <assert.h>

softmax_layer make_softmax_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ಪೂರ್ಣಾಂಕ groups)
{
    ಪ್ರತಿಜ್ಞಾ(inputs%groups == 0);
    fprintf(stderr, "softmax                                        %4d\n",  inputs);
    softmax_layer l = {0};
    l.type = SOFTMAX;
    l.batch = batch;
    l.groups = groups;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(inputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(inputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.delta = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(inputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.ವೆಚ್ಚ_2 = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.forward = forward_softmax_layer;
    l.backward = backward_softmax_layer;
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_softmax_layer_gpu;
    l.backward_gpu = backward_softmax_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ forward_softmax_layer(ಸ್ಥಿರಾಂಕ softmax_layer l, ಜಾಲ net)
{
    ಆದರೆ(l.softmax_ವೃಕ್ಷ){
        ಪೂರ್ಣಾಂಕ i;
        ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
        ಗಾಗಿ (i = 0; i < l.softmax_ವೃಕ್ಷ->groups; ++i) {
            ಪೂರ್ಣಾಂಕ ಗುಂಪು_ಗಾತ್ರ = l.softmax_ವೃಕ್ಷ->ಗುಂಪು_ಗಾತ್ರ[i];
            softmax_cpu(net.ಒಳಹರಿವು + ಎಣಿಕೆ_2, ಗುಂಪು_ಗಾತ್ರ, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output + ಎಣಿಕೆ_2);
            ಎಣಿಕೆ_2 += ಗುಂಪು_ಗಾತ್ರ;
        }
    } ಇಲ್ಲದಿದ್ದರೆ {
        softmax_cpu(net.ಒಳಹರಿವು, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output);
    }

    ಆದರೆ(net.truth && !l.noloss){
        softmax_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.ವೆಚ್ಚ_2[0] = ಮೊತ್ತ_array(l.loss, l.batch*l.inputs);
    }
}

ಶೂನ್ಯ backward_softmax_layer(ಸ್ಥಿರಾಂಕ softmax_layer l, ಜಾಲ net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

ಶೂನ್ಯ pull_softmax_layer_output(ಸ್ಥಿರಾಂಕ softmax_layer layer)
{
    cuda_pull_array(layer.output_gpu, layer.output, layer.inputs*layer.batch);
}

ಶೂನ್ಯ forward_softmax_layer_gpu(ಸ್ಥಿರಾಂಕ softmax_layer l, ಜಾಲ net)
{
    ಆದರೆ(l.softmax_ವೃಕ್ಷ){
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
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ(l.spatial){
            softmax_gpu(net.ಒಳಹರಿವು_gpu, l.c, l.batch*l.c, l.inputs/l.c, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu);
        }ಇಲ್ಲದಿದ್ದರೆ{
            softmax_gpu(net.ಒಳಹರಿವು_gpu, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output_gpu);
        }
    }
    ಆದರೆ(net.truth && !l.noloss){
        softmax_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        ಆದರೆ(l.softmax_ವೃಕ್ಷ){
            ಮುಖವಾಡ_gpu(l.batch*l.inputs, l.delta_gpu, SECRET_ಅಂಕೆ, net.truth_gpu, 0);
            ಮುಖವಾಡ_gpu(l.batch*l.inputs, l.loss_gpu, SECRET_ಅಂಕೆ, net.truth_gpu, 0);
        }
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.ವೆಚ್ಚ_2[0] = ಮೊತ್ತ_array(l.loss, l.batch*l.inputs);
    }
}

ಶೂನ್ಯ backward_softmax_layer_gpu(ಸ್ಥಿರಾಂಕ softmax_layer layer, ಜಾಲ net)
{
    axpy_gpu(layer.batch*layer.inputs, 1, layer.delta_gpu, 1, net.delta_gpu, 1);
}

#ಆದರೆ_ಅಂತ್ಯ
