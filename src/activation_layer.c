#ಅನ್ತರ್ಭಾವ "activation_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "gemm.h"

#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>

layer make_activation_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ACTIVATION activation)
{
    layer l = {0};
    l.type = ಸಕ್ರಿಯ;

    l.inputs = inputs;
    l.outputs = inputs;
    l.batch=batch;

    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*inputs, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    l.delta = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*inputs, ಗಾತ್ರ(ದಶಮಾಂಶ*));

    l.forward = forward_activation_layer;
    l.backward = backward_activation_layer;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_activation_layer_gpu;
    l.backward_gpu = backward_activation_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch);
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch);
#ಆದರೆ_ಅಂತ್ಯ
    l.activation = activation;
    fprintf(stderr, "Activation Layer: %d inputs\n", inputs);
    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ forward_activation_layer(layer l, ಜಾಲ net)
{
    copy_cpu(l.outputs*l.batch, net.ಒಳಹರಿವು, 1, l.output, 1);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ಶೂನ್ಯ backward_activation_layer(layer l, ಜಾಲ net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

ಶೂನ್ಯ forward_activation_layer_gpu(layer l, ಜಾಲ net)
{
    copy_gpu(l.outputs*l.batch, net.ಒಳಹರಿವು_gpu, 1, l.output_gpu, 1);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ಶೂನ್ಯ backward_activation_layer_gpu(layer l, ಜಾಲ net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#ಆದರೆ_ಅಂತ್ಯ
