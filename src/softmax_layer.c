#అన్తర్భావ "softmax_layer.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "cuda.h"

#అన్తర్భావ <float.h>
#అన్తర్భావ <math.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <assert.h>

softmax_layer make_softmax_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, పూర్ణాంకం groups)
{
    ప్రతిజ్ఞా(inputs%groups == 0);
    fprintf(stderr, "softmax                                        %4d\n",  inputs);
    softmax_layer l = {0};
    l.type = SOFTMAX;
    l.batch = batch;
    l.groups = groups;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = సున్నా_జ్ఞాపకం_ఇవ్వు(inputs*batch, పరిమాణం(దశాంశం));
    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(inputs*batch, పరిమాణం(దశాంశం));
    l.delta = సున్నా_జ్ఞాపకం_ఇవ్వు(inputs*batch, పరిమాణం(దశాంశం));
    l.ఖర్చు_2 = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(దశాంశం));

    l.forward = forward_softmax_layer;
    l.backward = backward_softmax_layer;
    #నిర్వచితమైతే GPU
    l.forward_gpu = forward_softmax_layer_gpu;
    l.backward_gpu = backward_softmax_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #అయితే_ముగింపు
    తిరిగి l;
}

శూన్య forward_softmax_layer(స్థిరరాశి softmax_layer l, జాలం net)
{
    ఒకవేళ(l.softmax_వృక్షం){
        పూర్ణాంకం i;
        పూర్ణాంకం లెక్క = 0;
        కొరకు (i = 0; i < l.softmax_వృక్షం->groups; ++i) {
            పూర్ణాంకం సమూహం_కొలత = l.softmax_వృక్షం->సమూహం_కొలత[i];
            softmax_cpu(net.ఆదానం + లెక్క, సమూహం_కొలత, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output + లెక్క);
            లెక్క += సమూహం_కొలత;
        }
    } లేకపోతే {
        softmax_cpu(net.ఆదానం, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output);
    }

    ఒకవేళ(net.truth && !l.noloss){
        softmax_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.ఖర్చు_2[0] = కూడిక_array(l.loss, l.batch*l.inputs);
    }
}

శూన్య backward_softmax_layer(స్థిరరాశి softmax_layer l, జాలం net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#నిర్వచితమైతే GPU

శూన్య pull_softmax_layer_output(స్థిరరాశి softmax_layer layer)
{
    cuda_pull_array(layer.output_gpu, layer.output, layer.inputs*layer.batch);
}

శూన్య forward_softmax_layer_gpu(స్థిరరాశి softmax_layer l, జాలం net)
{
    ఒకవేళ(l.softmax_వృక్షం){
        softmax_వృక్షం(net.ఆదానం_gpu, 1, l.batch, l.inputs, l.temperature, l.output_gpu, *l.softmax_వృక్షం);
        /*
        int i;
        int count = 0;
        for (i = 0; i < l.softmax_tree->groups; ++i) {
            int group_size = l.softmax_tree->group_size[i];
            softmax_gpu(net.input_gpu + count, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output_gpu + count);
            count += group_size;
        }
        */
    } లేకపోతే {
        ఒకవేళ(l.spatial){
            softmax_gpu(net.ఆదానం_gpu, l.c, l.batch*l.c, l.inputs/l.c, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu);
        }లేకపోతే{
            softmax_gpu(net.ఆదానం_gpu, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output_gpu);
        }
    }
    ఒకవేళ(net.truth && !l.noloss){
        softmax_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        ఒకవేళ(l.softmax_వృక్షం){
            ముసుగు_gpu(l.batch*l.inputs, l.delta_gpu, SECRET_అంకె, net.truth_gpu, 0);
            ముసుగు_gpu(l.batch*l.inputs, l.loss_gpu, SECRET_అంకె, net.truth_gpu, 0);
        }
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.ఖర్చు_2[0] = కూడిక_array(l.loss, l.batch*l.inputs);
    }
}

శూన్య backward_softmax_layer_gpu(స్థిరరాశి softmax_layer layer, జాలం net)
{
    axpy_gpu(layer.batch*layer.inputs, 1, layer.delta_gpu, 1, net.delta_gpu, 1);
}

#అయితే_ముగింపు
