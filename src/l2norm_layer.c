#ಅನ್ತರ್ಭಾವ "l2norm_layer.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"

#ಅನ್ತರ್ಭಾವ <float.h>
#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <assert.h>

layer make_l2norm_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ inputs)
{
    fprintf(stderr, "l2norm                                         %4d\n",  inputs);
    layer l = {0};
    l.type = L2NORM;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.output = calloc(inputs*batch, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.scales = calloc(inputs*batch, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.delta = calloc(inputs*batch, ಪರಿಮಾಣ(ಭಿನ್ನ));

    l.forward = forward_l2norm_layer;
    l.backward = backward_l2norm_layer;
    #ifdef GPU
    l.forward_gpu = forward_l2norm_layer_gpu;
    l.backward_gpu = backward_l2norm_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.scales_gpu = cuda_make_array(l.output, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ forward_l2norm_layer(ಧ್ರುವ layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.ಒಳಹರಿವು, 1, l.output, 1);
    l2normalize_cpu(l.output, l.scales, l.batch, l.out_c, l.out_w*l.out_h);
}

ಶೂನ್ಯ backward_l2norm_layer(ಧ್ರುವ layer l, network net)
{
    //axpy_cpu(l.inputs*l.batch, 1, l.scales, 1, l.delta, 1);
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

ಶೂನ್ಯ forward_l2norm_layer_gpu(ಧ್ರುವ layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.ಒಳಹರಿವು_gpu, 1, l.output_gpu, 1);
    l2normalize_gpu(l.output_gpu, l.scales_gpu, l.batch, l.out_c, l.out_w*l.out_h);
}

ಶೂನ್ಯ backward_l2norm_layer_gpu(ಧ್ರುವ layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.scales_gpu, 1, l.delta_gpu, 1);
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
