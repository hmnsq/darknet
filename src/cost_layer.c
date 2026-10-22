#అన్తర్భావ "cost_layer.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ <math.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdio.h>

ఖర్చు_TYPE get_ఖర్చు_type(అక్షరం *s)
{
    ఒకవేళ (strcmp(s, "seg")==0) తిరిగి SEG;
    ఒకవేళ (strcmp(s, "sse")==0) తిరిగి SSE;
    ఒకవేళ (strcmp(s, "masked")==0) తిరిగి MASKED;
    ఒకవేళ (strcmp(s, "smooth")==0) తిరిగి SMOOTH;
    ఒకవేళ (strcmp(s, "L1")==0) తిరిగి L1;
    ఒకవేళ (strcmp(s, "wgan")==0) తిరిగి WGAN;
    fprintf(stderr, "Couldn't find cost type %s, going with SSE\n", s);
    తిరిగి SSE;
}

అక్షరం *get_ఖర్చు_string(ఖర్చు_TYPE a)
{
    ఎంపిక(a){
        స్థితి SEG:
            తిరిగి "seg";
        స్థితి SSE:
            తిరిగి "sse";
        స్థితి MASKED:
            తిరిగి "masked";
        స్థితి SMOOTH:
            తిరిగి "smooth";
        స్థితి L1:
            తిరిగి "L1";
        స్థితి WGAN:
            తిరిగి "wgan";
    }
    తిరిగి "sse";
}

ఖర్చు_layer make_ఖర్చు_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, ఖర్చు_TYPE ఖర్చు_type, దశాంశం scale)
{
    fprintf(stderr, "cost                                           %4d\n",  inputs);
    ఖర్చు_layer l = {0};
    l.type = ఖర్చు;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.ఖర్చు_type = ఖర్చు_type;
    l.delta = సున్నా_జ్ఞాపకం_ఇవ్వు(inputs*batch, పరిమాణం(దశాంశం));
    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(inputs*batch, పరిమాణం(దశాంశం));
    l.ఖర్చు_2 = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(దశాంశం));

    l.forward = forward_ఖర్చు_layer;
    l.backward = backward_ఖర్చు_layer;
    #నిర్వచితమైతే GPU
    l.forward_gpu = forward_ఖర్చు_layer_gpu;
    l.backward_gpu = backward_ఖర్చు_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #అయితే_ముగింపు
    తిరిగి l;
}

శూన్య resize_ఖర్చు_layer(ఖర్చు_layer *l, పూర్ణాంకం inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = realloc(l->delta, inputs*l->batch*పరిమాణం(దశాంశం));
    l->output = realloc(l->output, inputs*l->batch*పరిమాణం(దశాంశం));
#నిర్వచితమైతే GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#అయితే_ముగింపు
}

శూన్య forward_ఖర్చు_layer(ఖర్చు_layer l, జాలం net)
{
    ఒకవేళ (!net.truth) తిరిగి;
    ఒకవేళ(l.ఖర్చు_type == MASKED){
        పూర్ణాంకం i;
        కొరకు(i = 0; i < l.batch*l.inputs; ++i){
            ఒకవేళ(net.truth[i] == SECRET_అంకె) net.ఆదానం[i] = SECRET_అంకె;
        }
    }
    ఒకవేళ(l.ఖర్చు_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.ఆదానం, net.truth, l.delta, l.output);
    }లేకపోతే ఒకవేళ(l.ఖర్చు_type == L1){
        l1_cpu(l.batch*l.inputs, net.ఆదానం, net.truth, l.delta, l.output);
    } లేకపోతే {
        l2_cpu(l.batch*l.inputs, net.ఆదానం, net.truth, l.delta, l.output);
    }
    l.ఖర్చు_2[0] = కూడిక_array(l.output, l.batch*l.inputs);
}

శూన్య backward_ఖర్చు_layer(స్థిరరాశి ఖర్చు_layer l, జాలం net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#నిర్వచితమైతే GPU

శూన్య pull_ఖర్చు_layer(ఖర్చు_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

శూన్య నెట్టు_ఖర్చు_layer(ఖర్చు_layer l)
{
    cuda_నెట్టు_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

పూర్ణాంకం float_abs_compare (స్థిరరాశి శూన్య * a, స్థిరరాశి శూన్య * b)
{
    దశాంశం fa = *(స్థిరరాశి దశాంశం*) a;
    ఒకవేళ(fa < 0) fa = -fa;
    దశాంశం fb = *(స్థిరరాశి దశాంశం*) b;
    ఒకవేళ(fb < 0) fb = -fb;
    తిరిగి (fa > fb) - (fa < fb);
}

శూన్య forward_ఖర్చు_layer_gpu(ఖర్చు_layer l, జాలం net)
{
    ఒకవేళ (!net.truth) తిరిగి;
    ఒకవేళ(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        కూడు_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    ఒకవేళ(l.ఖర్చు_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.ఆదానం_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } లేకపోతే ఒకవేళ (l.ఖర్చు_type == L1){
        l1_gpu(l.batch*l.inputs, net.ఆదానం_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } లేకపోతే ఒకవేళ (l.ఖర్చు_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.ఆదానం_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } లేకపోతే {
        l2_gpu(l.batch*l.inputs, net.ఆదానం_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    ఒకవేళ (l.ఖర్చు_type == SEG && l.noobject_scale != 1) {
        scale_ముసుగు_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_ముసుగు_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    ఒకవేళ (l.ఖర్చు_type == MASKED) {
        ముసుగు_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_అంకె, net.truth_gpu, 0);
    }

    ఒకవేళ(l.నిష్పత్తి){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, పరిమాణం(దశాంశం), float_abs_compare);
        పూర్ణాంకం n = (1-l.నిష్పత్తి) * l.batch*l.inputs;
        దశాంశం thresh = l.delta[n];
        thresh = 0;
        రాయి("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    ఒకవేళ(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.ఖర్చు_2[0] = కూడిక_array(l.output, l.batch*l.inputs);
}

శూన్య backward_ఖర్చు_layer_gpu(స్థిరరాశి ఖర్చు_layer l, జాలం net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#అయితే_ముగింపు

