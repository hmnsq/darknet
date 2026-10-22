#समावेश "l2norm_layer.h"
#समावेश "activations.h"
#समावेश "blas.h"
#समावेश "cuda.h"

#समावेश <float.h>
#समावेश <math.h>
#समावेश <stdlib.h>
#समावेश <stdio.h>
#समावेश <assert.h>

layer make_l2norm_layer(पूर्णांक batch, पूर्णांक inputs)
{
    ख_लिखो(मानक_त्रुटि, "l2norm                                         %4d\n",  inputs);
    layer l = {0};
    l.type = L2NORM;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.output = सुस्मृति(inputs*batch, माप(भग्न));
    l.scales = सुस्मृति(inputs*batch, माप(भग्न));
    l.delta = सुस्मृति(inputs*batch, माप(भग्न));

    l.forward = forward_l2norm_layer;
    l.backward = backward_l2norm_layer;
    #अगर_घोषित GPU
    l.forward_gpu = forward_l2norm_layer_gpu;
    l.backward_gpu = backward_l2norm_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.scales_gpu = cuda_make_array(l.output, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #पूर्ण_अगर
    वापस l;
}

व्योम forward_l2norm_layer(स्थिर layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.निवेश, 1, l.output, 1);
    l2normalize_cpu(l.output, l.scales, l.batch, l.out_c, l.out_w*l.out_h);
}

व्योम backward_l2norm_layer(स्थिर layer l, network net)
{
    //axpy_cpu(l.inputs*l.batch, 1, l.scales, 1, l.delta, 1);
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#अगर_घोषित GPU

व्योम forward_l2norm_layer_gpu(स्थिर layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.निवेश_gpu, 1, l.output_gpu, 1);
    l2normalize_gpu(l.output_gpu, l.scales_gpu, l.batch, l.out_c, l.out_w*l.out_h);
}

व्योम backward_l2norm_layer_gpu(स्थिर layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.scales_gpu, 1, l.delta_gpu, 1);
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}

#पूर्ण_अगर
