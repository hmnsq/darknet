#అన్తర్భావ "shortcut_layer.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "activations.h"

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <assert.h>

layer make_shortcut_layer(పూర్ణాంకం batch, పూర్ణాంకం సూచీ, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c, పూర్ణాంకం w2, పూర్ణాంకం h2, పూర్ణాంకం c2)
{
    fprintf(stderr, "res  %3d                %4d x%4d x%4d   ->  %4d x%4d x%4d\n",సూచీ, w2,h2,c2, w,h,c);
    layer l = {0};
    l.type = SHORTCUT;
    l.batch = batch;
    l.w = w2;
    l.h = h2;
    l.c = c2;
    l.బహిర్గతం_w = w;
    l.బహిర్గతం_h = h;
    l.బహిర్గతం_c = c;
    l.outputs = w*h*c;
    l.inputs = l.outputs;

    l.సూచీ = సూచీ;

    l.delta =  సున్నా_జ్ఞాపకం_ఇవ్వు(l.outputs*batch, పరిమాణం(దశాంశం));
    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(l.outputs*batch, పరిమాణం(దశాంశం));;

    l.forward = forward_shortcut_layer;
    l.backward = backward_shortcut_layer;
    #నిర్వచితమైతే GPU
    l.forward_gpu = forward_shortcut_layer_gpu;
    l.backward_gpu = backward_shortcut_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #అయితే_ముగింపు
    తిరిగి l;
}

శూన్య resize_shortcut_layer(layer *l, పూర్ణాంకం w, పూర్ణాంకం h)
{
    ప్రతిజ్ఞా(l->w == l->బహిర్గతం_w);
    ప్రతిజ్ఞా(l->h == l->బహిర్గతం_h);
    l->w = l->బహిర్గతం_w = w;
    l->h = l->బహిర్గతం_h = h;
    l->outputs = w*h*l->బహిర్గతం_c;
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*పరిమాణం(దశాంశం));
    l->output = realloc(l->output, l->outputs*l->batch*పరిమాణం(దశాంశం));

#నిర్వచితమైతే GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#అయితే_ముగింపు
    
}


శూన్య forward_shortcut_layer(స్థిరరాశి layer l, జాలం net)
{
    copy_cpu(l.outputs*l.batch, net.ఆదానం, 1, l.output, 1);
    shortcut_cpu(l.batch, l.w, l.h, l.c, net.layers[l.సూచీ].output, l.బహిర్గతం_w, l.బహిర్గతం_h, l.బహిర్గతం_c, l.alpha, l.beta, l.output);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

శూన్య backward_shortcut_layer(స్థిరరాశి layer l, జాలం net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    axpy_cpu(l.outputs*l.batch, l.alpha, l.delta, 1, net.delta, 1);
    shortcut_cpu(l.batch, l.బహిర్గతం_w, l.బహిర్గతం_h, l.బహిర్గతం_c, l.delta, l.w, l.h, l.c, 1, l.beta, net.layers[l.సూచీ].delta);
}

#నిర్వచితమైతే GPU
శూన్య forward_shortcut_layer_gpu(స్థిరరాశి layer l, జాలం net)
{
    copy_gpu(l.outputs*l.batch, net.ఆదానం_gpu, 1, l.output_gpu, 1);
    shortcut_gpu(l.batch, l.w, l.h, l.c, net.layers[l.సూచీ].output_gpu, l.బహిర్గతం_w, l.బహిర్గతం_h, l.బహిర్గతం_c, l.alpha, l.beta, l.output_gpu);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

శూన్య backward_shortcut_layer_gpu(స్థిరరాశి layer l, జాలం net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    axpy_gpu(l.outputs*l.batch, l.alpha, l.delta_gpu, 1, net.delta_gpu, 1);
    shortcut_gpu(l.batch, l.బహిర్గతం_w, l.బహిర్గతం_h, l.బహిర్గతం_c, l.delta_gpu, l.w, l.h, l.c, 1, l.beta, net.layers[l.సూచీ].delta_gpu);
}
#అయితే_ముగింపు
