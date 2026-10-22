#ਅਨ੍ਤਰ੍ਭਾਵ "cost_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

ਲਾਗਤ_TYPE get_ਲਾਗਤ_type(ਅੱਖਰ *s)
{
    ਜੇ (strcmp(s, "seg")==0) ਵਾਪਸ SEG;
    ਜੇ (strcmp(s, "sse")==0) ਵਾਪਸ SSE;
    ਜੇ (strcmp(s, "masked")==0) ਵਾਪਸ MASKED;
    ਜੇ (strcmp(s, "smooth")==0) ਵਾਪਸ SMOOTH;
    ਜੇ (strcmp(s, "L1")==0) ਵਾਪਸ L1;
    ਜੇ (strcmp(s, "wgan")==0) ਵਾਪਸ WGAN;
    fprintf(stderr, "Couldn't find cost type %s, going with SSE\n", s);
    ਵਾਪਸ SSE;
}

ਅੱਖਰ *get_ਲਾਗਤ_string(ਲਾਗਤ_TYPE a)
{
    ਚੋਣ(a){
        ਹਾਲਤ SEG:
            ਵਾਪਸ "seg";
        ਹਾਲਤ SSE:
            ਵਾਪਸ "sse";
        ਹਾਲਤ MASKED:
            ਵਾਪਸ "masked";
        ਹਾਲਤ SMOOTH:
            ਵਾਪਸ "smooth";
        ਹਾਲਤ L1:
            ਵਾਪਸ "L1";
        ਹਾਲਤ WGAN:
            ਵਾਪਸ "wgan";
    }
    ਵਾਪਸ "sse";
}

ਲਾਗਤ_layer make_ਲਾਗਤ_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਲਾਗਤ_TYPE ਲਾਗਤ_type, ਦਸ਼ਮਲਵ scale)
{
    fprintf(stderr, "cost                                           %4d\n",  inputs);
    ਲਾਗਤ_layer l = {0};
    l.type = ਲਾਗਤ;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.ਲਾਗਤ_type = ਲਾਗਤ_type;
    l.delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(inputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(inputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.ਲਾਗਤ_2 = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.forward = forward_ਲਾਗਤ_layer;
    l.backward = backward_ਲਾਗਤ_layer;
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_ਲਾਗਤ_layer_gpu;
    l.backward_gpu = backward_ਲਾਗਤ_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #ਜੇ_ਅੰਤ
    ਵਾਪਸ l;
}

ਖਾਲੀ resize_ਲਾਗਤ_layer(ਲਾਗਤ_layer *l, ਪੂਰਨ inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = realloc(l->delta, inputs*l->batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l->output = realloc(l->output, inputs*l->batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ forward_ਲਾਗਤ_layer(ਲਾਗਤ_layer l, ਜਾਲ net)
{
    ਜੇ (!net.truth) ਵਾਪਸ;
    ਜੇ(l.ਲਾਗਤ_type == MASKED){
        ਪੂਰਨ i;
        ਲਈ(i = 0; i < l.batch*l.inputs; ++i){
            ਜੇ(net.truth[i] == SECRET_ਅੰਕ) net.ਨਿਵੇਸ਼[i] = SECRET_ਅੰਕ;
        }
    }
    ਜੇ(l.ਲਾਗਤ_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.ਨਿਵੇਸ਼, net.truth, l.delta, l.output);
    }ਨਹੀਂਤਾਂ ਜੇ(l.ਲਾਗਤ_type == L1){
        l1_cpu(l.batch*l.inputs, net.ਨਿਵੇਸ਼, net.truth, l.delta, l.output);
    } ਨਹੀਂਤਾਂ {
        l2_cpu(l.batch*l.inputs, net.ਨਿਵੇਸ਼, net.truth, l.delta, l.output);
    }
    l.ਲਾਗਤ_2[0] = ਜੋੜ_array(l.output, l.batch*l.inputs);
}

ਖਾਲੀ backward_ਲਾਗਤ_layer(ਅਚਲ ਲਾਗਤ_layer l, ਜਾਲ net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ pull_ਲਾਗਤ_layer(ਲਾਗਤ_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

ਖਾਲੀ ਧੱਕੋ_ਲਾਗਤ_layer(ਲਾਗਤ_layer l)
{
    cuda_ਧੱਕੋ_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

ਪੂਰਨ float_abs_compare (ਅਚਲ ਖਾਲੀ * a, ਅਚਲ ਖਾਲੀ * b)
{
    ਦਸ਼ਮਲਵ fa = *(ਅਚਲ ਦਸ਼ਮਲਵ*) a;
    ਜੇ(fa < 0) fa = -fa;
    ਦਸ਼ਮਲਵ fb = *(ਅਚਲ ਦਸ਼ਮਲਵ*) b;
    ਜੇ(fb < 0) fb = -fb;
    ਵਾਪਸ (fa > fb) - (fa < fb);
}

ਖਾਲੀ forward_ਲਾਗਤ_layer_gpu(ਲਾਗਤ_layer l, ਜਾਲ net)
{
    ਜੇ (!net.truth) ਵਾਪਸ;
    ਜੇ(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        ਜੋੜੋ_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    ਜੇ(l.ਲਾਗਤ_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.ਨਿਵੇਸ਼_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } ਨਹੀਂਤਾਂ ਜੇ (l.ਲਾਗਤ_type == L1){
        l1_gpu(l.batch*l.inputs, net.ਨਿਵੇਸ਼_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } ਨਹੀਂਤਾਂ ਜੇ (l.ਲਾਗਤ_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.ਨਿਵੇਸ਼_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } ਨਹੀਂਤਾਂ {
        l2_gpu(l.batch*l.inputs, net.ਨਿਵੇਸ਼_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    ਜੇ (l.ਲਾਗਤ_type == SEG && l.noobject_scale != 1) {
        scale_ਨਕਾਬ_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_ਨਕਾਬ_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    ਜੇ (l.ਲਾਗਤ_type == MASKED) {
        ਨਕਾਬ_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_ਅੰਕ, net.truth_gpu, 0);
    }

    ਜੇ(l.ਅਨੁਪਾਤ){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ), float_abs_compare);
        ਪੂਰਨ n = (1-l.ਅਨੁਪਾਤ) * l.batch*l.inputs;
        ਦਸ਼ਮਲਵ thresh = l.delta[n];
        thresh = 0;
        ਲਿਖੋ("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    ਜੇ(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.ਲਾਗਤ_2[0] = ਜੋੜ_array(l.output, l.batch*l.inputs);
}

ਖਾਲੀ backward_ਲਾਗਤ_layer_gpu(ਅਚਲ ਲਾਗਤ_layer l, ਜਾਲ net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#ਜੇ_ਅੰਤ

