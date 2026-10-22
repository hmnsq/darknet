#ಅನ್ತರ್ಭಾವ "l2norm_layer.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"

#ಅನ್ತರ್ಭಾವ <float.h>
#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <assert.h>

layer make_l2norm_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs)
{
    fprintf(stderr, "l2norm                                         %4d\n",  inputs);
    layer l = {0};
    l.type = L2NORM;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(inputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.scales = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(inputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.delta = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(inputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.forward = forward_l2norm_layer;
    l.backward = backward_l2norm_layer;
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_l2norm_layer_gpu;
    l.backward_gpu = backward_l2norm_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.scales_gpu = cuda_make_array(l.output, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ forward_l2norm_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    copy_cpu(l.outputs*l.batch, net.ಒಳಹರಿವು, 1, l.output, 1);
    l2normalize_cpu(l.output, l.scales, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h);
}

ಶೂನ್ಯ backward_l2norm_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    //axpy_cpu(l.inputs*l.batch, 1, l.scales, 1, l.delta, 1);
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

ಶೂನ್ಯ forward_l2norm_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    copy_gpu(l.outputs*l.batch, net.ಒಳಹರಿವು_gpu, 1, l.output_gpu, 1);
    l2normalize_gpu(l.output_gpu, l.scales_gpu, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h);
}

ಶೂನ್ಯ backward_l2norm_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.scales_gpu, 1, l.delta_gpu, 1);
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#ಆದರೆ_ಅಂತ್ಯ
