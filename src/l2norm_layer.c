#अन्तर्भाव "l2norm_layer.h"
#अन्तर्भाव "activations.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "cuda.h"

#अन्तर्भाव <float.h>
#अन्तर्भाव <math.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <assert.h>

layer make_l2norm_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs)
{
    fprintf(stderr, "l2norm                                         %4d\n",  inputs);
    layer l = {0};
    l.type = L2NORM;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.output = शून्यस्मृतिं_देहि(inputs*batch, परिमाण(भिन्न));
    l.scales = शून्यस्मृतिं_देहि(inputs*batch, परिमाण(भिन्न));
    l.delta = शून्यस्मृतिं_देहि(inputs*batch, परिमाण(भिन्न));

    l.forward = forward_l2norm_layer;
    l.backward = backward_l2norm_layer;
    #यदि_परिभाषितम् GPU
    l.forward_gpu = forward_l2norm_layer_gpu;
    l.backward_gpu = backward_l2norm_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.scales_gpu = cuda_make_array(l.output, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #यदि_अन्तः
    प्रत्यागम l;
}

शून्य forward_l2norm_layer(ध्रुव layer l, जालम् net)
{
    copy_cpu(l.outputs*l.batch, net.निवेशः, 1, l.output, 1);
    l2normalize_cpu(l.output, l.scales, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_w*l.बहिर्गमनम्_h);
}

शून्य backward_l2norm_layer(ध्रुव layer l, जालम् net)
{
    //axpy_cpu(l.inputs*l.batch, 1, l.scales, 1, l.delta, 1);
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#यदि_परिभाषितम् GPU

शून्य forward_l2norm_layer_gpu(ध्रुव layer l, जालम् net)
{
    copy_gpu(l.outputs*l.batch, net.निवेशः_gpu, 1, l.output_gpu, 1);
    l2normalize_gpu(l.output_gpu, l.scales_gpu, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_w*l.बहिर्गमनम्_h);
}

शून्य backward_l2norm_layer_gpu(ध्रुव layer l, जालम् net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.scales_gpu, 1, l.delta_gpu, 1);
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#यदि_अन्तः
