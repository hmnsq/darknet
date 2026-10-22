#ਅਨ੍ਤਰ੍ਭਾਵ "softmax_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <float.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

softmax_layer make_softmax_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਪੂਰਨ groups)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(inputs%groups == 0);
    fprintf(stderr, "softmax                                        %4d\n",  inputs);
    softmax_layer l = {0};
    l.type = SOFTMAX;
    l.batch = batch;
    l.groups = groups;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(inputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(inputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(inputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.ਲਾਗਤ_2 = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.forward = forward_softmax_layer;
    l.backward = backward_softmax_layer;
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_softmax_layer_gpu;
    l.backward_gpu = backward_softmax_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #ਜੇ_ਅੰਤ
    ਵਾਪਸ l;
}

ਖਾਲੀ forward_softmax_layer(ਅਚਲ softmax_layer l, ਜਾਲ net)
{
    ਜੇ(l.softmax_ਰੁੱਖ){
        ਪੂਰਨ i;
        ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
        ਲਈ (i = 0; i < l.softmax_ਰੁੱਖ->groups; ++i) {
            ਪੂਰਨ ਸਮੂਹ_ਮਾਪ = l.softmax_ਰੁੱਖ->ਸਮੂਹ_ਮਾਪ[i];
            softmax_cpu(net.ਨਿਵੇਸ਼ + ਗਿਣਤੀ_2, ਸਮੂਹ_ਮਾਪ, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output + ਗਿਣਤੀ_2);
            ਗਿਣਤੀ_2 += ਸਮੂਹ_ਮਾਪ;
        }
    } ਨਹੀਂਤਾਂ {
        softmax_cpu(net.ਨਿਵੇਸ਼, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output);
    }

    ਜੇ(net.truth && !l.noloss){
        softmax_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.ਲਾਗਤ_2[0] = ਜੋੜ_array(l.loss, l.batch*l.inputs);
    }
}

ਖਾਲੀ backward_softmax_layer(ਅਚਲ softmax_layer l, ਜਾਲ net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ pull_softmax_layer_output(ਅਚਲ softmax_layer layer)
{
    cuda_pull_array(layer.output_gpu, layer.output, layer.inputs*layer.batch);
}

ਖਾਲੀ forward_softmax_layer_gpu(ਅਚਲ softmax_layer l, ਜਾਲ net)
{
    ਜੇ(l.softmax_ਰੁੱਖ){
        softmax_ਰੁੱਖ(net.ਨਿਵੇਸ਼_gpu, 1, l.batch, l.inputs, l.temperature, l.output_gpu, *l.softmax_ਰੁੱਖ);
        /*
        int i;
        int count = 0;
        for (i = 0; i < l.softmax_tree->groups; ++i) {
            int group_size = l.softmax_tree->group_size[i];
            softmax_gpu(net.input_gpu + count, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output_gpu + count);
            count += group_size;
        }
        */
    } ਨਹੀਂਤਾਂ {
        ਜੇ(l.spatial){
            softmax_gpu(net.ਨਿਵੇਸ਼_gpu, l.c, l.batch*l.c, l.inputs/l.c, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu);
        }ਨਹੀਂਤਾਂ{
            softmax_gpu(net.ਨਿਵੇਸ਼_gpu, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output_gpu);
        }
    }
    ਜੇ(net.truth && !l.noloss){
        softmax_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        ਜੇ(l.softmax_ਰੁੱਖ){
            ਨਕਾਬ_gpu(l.batch*l.inputs, l.delta_gpu, SECRET_ਅੰਕ, net.truth_gpu, 0);
            ਨਕਾਬ_gpu(l.batch*l.inputs, l.loss_gpu, SECRET_ਅੰਕ, net.truth_gpu, 0);
        }
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.ਲਾਗਤ_2[0] = ਜੋੜ_array(l.loss, l.batch*l.inputs);
    }
}

ਖਾਲੀ backward_softmax_layer_gpu(ਅਚਲ softmax_layer layer, ਜਾਲ net)
{
    axpy_gpu(layer.batch*layer.inputs, 1, layer.delta_gpu, 1, net.delta_gpu, 1);
}

#ਜੇ_ਅੰਤ
