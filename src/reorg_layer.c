#అన్తర్భావ "reorg_layer.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "blas.h"

#అన్తర్భావ <stdio.h>


layer make_reorg_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c, పూర్ణాంకం stride, పూర్ణాంకం తిరగేయి, పూర్ణాంకం flatten, పూర్ణాంకం extra)
{
    layer l = {0};
    l.type = REORG;
    l.batch = batch;
    l.stride = stride;
    l.extra = extra;
    l.h = h;
    l.w = w;
    l.c = c;
    l.flatten = flatten;
    ఒకవేళ(తిరగేయి){
        l.బహిర్గతం_w = w*stride;
        l.బహిర్గతం_h = h*stride;
        l.బహిర్గతం_c = c/(stride*stride);
    }లేకపోతే{
        l.బహిర్గతం_w = w/stride;
        l.బహిర్గతం_h = h/stride;
        l.బహిర్గతం_c = c*(stride*stride);
    }
    l.తిరగేయి = తిరగేయి;

    l.outputs = l.బహిర్గతం_h * l.బహిర్గతం_w * l.బహిర్గతం_c;
    l.inputs = h*w*c;
    ఒకవేళ(l.extra){
        l.బహిర్గతం_w = l.బహిర్గతం_h = l.బహిర్గతం_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    ఒకవేళ(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } లేకపోతే {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.బహిర్గతం_w, l.బహిర్గతం_h, l.బహిర్గతం_c);
    }
    పూర్ణాంకం output_కొలత = l.outputs * batch;
    l.output =  సున్నా_జ్ఞాపకం_ఇవ్వు(output_కొలత, పరిమాణం(దశాంశం));
    l.delta =   సున్నా_జ్ఞాపకం_ఇవ్వు(output_కొలత, పరిమాణం(దశాంశం));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#నిర్వచితమైతే GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_కొలత);
    l.delta_gpu   = cuda_make_array(l.delta, output_కొలత);
#అయితే_ముగింపు
    తిరిగి l;
}

శూన్య resize_reorg_layer(layer *l, పూర్ణాంకం w, పూర్ణాంకం h)
{
    పూర్ణాంకం stride = l->stride;
    పూర్ణాంకం c = l->c;

    l->h = h;
    l->w = w;

    ఒకవేళ(l->తిరగేయి){
        l->బహిర్గతం_w = w*stride;
        l->బహిర్గతం_h = h*stride;
        l->బహిర్గతం_c = c/(stride*stride);
    }లేకపోతే{
        l->బహిర్గతం_w = w/stride;
        l->బహిర్గతం_h = h/stride;
        l->బహిర్గతం_c = c*(stride*stride);
    }

    l->outputs = l->బహిర్గతం_h * l->బహిర్గతం_w * l->బహిర్గతం_c;
    l->inputs = l->outputs;
    పూర్ణాంకం output_కొలత = l->outputs * l->batch;

    l->output = realloc(l->output, output_కొలత * పరిమాణం(దశాంశం));
    l->delta = realloc(l->delta, output_కొలత * పరిమాణం(దశాంశం));

#నిర్వచితమైతే GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_కొలత);
    l->delta_gpu   = cuda_make_array(l->delta,  output_కొలత);
#అయితే_ముగింపు
}

శూన్య forward_reorg_layer(స్థిరరాశి layer l, జాలం net)
{
    పూర్ణాంకం i;
    ఒకవేళ(l.flatten){
        జ్ఞాపకం_నకలు(l.output, net.ఆదానం, l.outputs*l.batch*పరిమాణం(దశాంశం));
        ఒకవేళ(l.తిరగేయి){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }లేకపోతే{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } లేకపోతే ఒకవేళ (l.extra) {
        కొరకు(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.ఆదానం + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } లేకపోతే ఒకవేళ (l.తిరగేయి){
        reorg_cpu(net.ఆదానం, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } లేకపోతే {
        reorg_cpu(net.ఆదానం, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

శూన్య backward_reorg_layer(స్థిరరాశి layer l, జాలం net)
{
    పూర్ణాంకం i;
    ఒకవేళ(l.flatten){
        జ్ఞాపకం_నకలు(net.delta, l.delta, l.outputs*l.batch*పరిమాణం(దశాంశం));
        ఒకవేళ(l.తిరగేయి){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }లేకపోతే{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } లేకపోతే ఒకవేళ(l.తిరగేయి){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } లేకపోతే ఒకవేళ (l.extra) {
        కొరకు(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }లేకపోతే{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#నిర్వచితమైతే GPU
శూన్య forward_reorg_layer_gpu(layer l, జాలం net)
{
    పూర్ణాంకం i;
    ఒకవేళ(l.flatten){
        ఒకవేళ(l.తిరగేయి){
            flatten_gpu(net.ఆదానం_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }లేకపోతే{
            flatten_gpu(net.ఆదానం_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } లేకపోతే ఒకవేళ (l.extra) {
        కొరకు(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.ఆదానం_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } లేకపోతే ఒకవేళ (l.తిరగేయి) {
        reorg_gpu(net.ఆదానం_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }లేకపోతే {
        reorg_gpu(net.ఆదానం_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

శూన్య backward_reorg_layer_gpu(layer l, జాలం net)
{
    ఒకవేళ(l.flatten){
        ఒకవేళ(l.తిరగేయి){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }లేకపోతే{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } లేకపోతే ఒకవేళ (l.extra) {
        పూర్ణాంకం i;
        కొరకు(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } లేకపోతే ఒకవేళ(l.తిరగేయి){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } లేకపోతే {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#అయితే_ముగింపు
