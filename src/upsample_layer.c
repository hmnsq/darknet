#అన్తర్భావ "upsample_layer.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "blas.h"

#అన్తర్భావ <stdio.h>

layer make_upsample_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c, పూర్ణాంకం stride)
{
    layer l = {0};
    l.type = UPSAMPLE;
    l.batch = batch;
    l.w = w;
    l.h = h;
    l.c = c;
    l.బహిర్గతం_w = w*stride;
    l.బహిర్గతం_h = h*stride;
    l.బహిర్గతం_c = c;
    ఒకవేళ(stride < 0){
        stride = -stride;
        l.తిరగేయి=1;
        l.బహిర్గతం_w = w/stride;
        l.బహిర్గతం_h = h/stride;
    }
    l.stride = stride;
    l.outputs = l.బహిర్గతం_w*l.బహిర్గతం_h*l.బహిర్గతం_c;
    l.inputs = l.w*l.h*l.c;
    l.delta =  సున్నా_జ్ఞాపకం_ఇవ్వు(l.outputs*batch, పరిమాణం(దశాంశం));
    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(l.outputs*batch, పరిమాణం(దశాంశం));;

    l.forward = forward_upsample_layer;
    l.backward = backward_upsample_layer;
    #నిర్వచితమైతే GPU
    l.forward_gpu = forward_upsample_layer_gpu;
    l.backward_gpu = backward_upsample_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #అయితే_ముగింపు
    ఒకవేళ(l.తిరగేయి) fprintf(stderr, "downsample         %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.బహిర్గతం_w, l.బహిర్గతం_h, l.బహిర్గతం_c);
    లేకపోతే fprintf(stderr, "upsample           %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.బహిర్గతం_w, l.బహిర్గతం_h, l.బహిర్గతం_c);
    తిరిగి l;
}

శూన్య resize_upsample_layer(layer *l, పూర్ణాంకం w, పూర్ణాంకం h)
{
    l->w = w;
    l->h = h;
    l->బహిర్గతం_w = w*l->stride;
    l->బహిర్గతం_h = h*l->stride;
    ఒకవేళ(l->తిరగేయి){
        l->బహిర్గతం_w = w/l->stride;
        l->బహిర్గతం_h = h/l->stride;
    }
    l->outputs = l->బహిర్గతం_w*l->బహిర్గతం_h*l->బహిర్గతం_c;
    l->inputs = l->h*l->w*l->c;
    l->delta =  realloc(l->delta, l->outputs*l->batch*పరిమాణం(దశాంశం));
    l->output = realloc(l->output, l->outputs*l->batch*పరిమాణం(దశాంశం));

#నిర్వచితమైతే GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#అయితే_ముగింపు
    
}

శూన్య forward_upsample_layer(స్థిరరాశి layer l, జాలం net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    ఒకవేళ(l.తిరగేయి){
        upsample_cpu(l.output, l.బహిర్గతం_w, l.బహిర్గతం_h, l.c, l.batch, l.stride, 0, l.scale, net.ఆదానం);
    }లేకపోతే{
        upsample_cpu(net.ఆదానం, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output);
    }
}

శూన్య backward_upsample_layer(స్థిరరాశి layer l, జాలం net)
{
    ఒకవేళ(l.తిరగేయి){
        upsample_cpu(l.delta, l.బహిర్గతం_w, l.బహిర్గతం_h, l.c, l.batch, l.stride, 1, l.scale, net.delta);
    }లేకపోతే{
        upsample_cpu(net.delta, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta);
    }
}

#నిర్వచితమైతే GPU
శూన్య forward_upsample_layer_gpu(స్థిరరాశి layer l, జాలం net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);
    ఒకవేళ(l.తిరగేయి){
        upsample_gpu(l.output_gpu, l.బహిర్గతం_w, l.బహిర్గతం_h, l.c, l.batch, l.stride, 0, l.scale, net.ఆదానం_gpu);
    }లేకపోతే{
        upsample_gpu(net.ఆదానం_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output_gpu);
    }
}

శూన్య backward_upsample_layer_gpu(స్థిరరాశి layer l, జాలం net)
{
    ఒకవేళ(l.తిరగేయి){
        upsample_gpu(l.delta_gpu, l.బహిర్గతం_w, l.బహిర్గతం_h, l.c, l.batch, l.stride, 1, l.scale, net.delta_gpu);
    }లేకపోతే{
        upsample_gpu(net.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta_gpu);
    }
}
#అయితే_ముగింపు
