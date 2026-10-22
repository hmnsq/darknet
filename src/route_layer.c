#అన్తర్భావ "route_layer.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "blas.h"

#అన్తర్భావ <stdio.h>

route_layer make_route_layer(పూర్ణాంకం batch, పూర్ణాంకం n, పూర్ణాంకం *ఆదానం_layers, పూర్ణాంకం *ఆదానం_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.ఆదానం_layers = ఆదానం_layers;
    l.ఆదానం_sizes = ఆదానం_sizes;
    పూర్ణాంకం i;
    పూర్ణాంకం outputs = 0;
    కొరకు(i = 0; i < n; ++i){
        fprintf(stderr," %d", ఆదానం_layers[i]);
        outputs += ఆదానం_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  సున్నా_జ్ఞాపకం_ఇవ్వు(outputs*batch, పరిమాణం(దశాంశం));
    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs*batch, పరిమాణం(దశాంశం));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #నిర్వచితమైతే GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #అయితే_ముగింపు
    తిరిగి l;
}

శూన్య resize_route_layer(route_layer *l, జాలం *net)
{
    పూర్ణాంకం i;
    layer మొదటి = net->layers[l->ఆదానం_layers[0]];
    l->బహిర్గతం_w = మొదటి.బహిర్గతం_w;
    l->బహిర్గతం_h = మొదటి.బహిర్గతం_h;
    l->బహిర్గతం_c = మొదటి.బహిర్గతం_c;
    l->outputs = మొదటి.outputs;
    l->ఆదానం_sizes[0] = మొదటి.outputs;
    కొరకు(i = 1; i < l->n; ++i){
        పూర్ణాంకం సూచీ = l->ఆదానం_layers[i];
        layer తదుపరి = net->layers[సూచీ];
        l->outputs += తదుపరి.outputs;
        l->ఆదానం_sizes[i] = తదుపరి.outputs;
        ఒకవేళ(తదుపరి.బహిర్గతం_w == మొదటి.బహిర్గతం_w && తదుపరి.బహిర్గతం_h == మొదటి.బహిర్గతం_h){
            l->బహిర్గతం_c += తదుపరి.బహిర్గతం_c;
        }లేకపోతే{
            రాయి("%d %d, %d %d\n", తదుపరి.బహిర్గతం_w, తదుపరి.బహిర్గతం_h, మొదటి.బహిర్గతం_w, మొదటి.బహిర్గతం_h);
            l->బహిర్గతం_h = l->బహిర్గతం_w = l->బహిర్గతం_c = 0;
        }
    }
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

శూన్య forward_route_layer(స్థిరరాశి route_layer l, జాలం net)
{
    పూర్ణాంకం i, j;
    పూర్ణాంకం స్థానభ్రంశం = 0;
    కొరకు(i = 0; i < l.n; ++i){
        పూర్ణాంకం సూచీ = l.ఆదానం_layers[i];
        దశాంశం *ఆదానం = net.layers[సూచీ].output;
        పూర్ణాంకం ఆదానం_కొలత = l.ఆదానం_sizes[i];
        కొరకు(j = 0; j < l.batch; ++j){
            copy_cpu(ఆదానం_కొలత, ఆదానం + j*ఆదానం_కొలత, 1, l.output + స్థానభ్రంశం + j*l.outputs, 1);
        }
        స్థానభ్రంశం += ఆదానం_కొలత;
    }
}

శూన్య backward_route_layer(స్థిరరాశి route_layer l, జాలం net)
{
    పూర్ణాంకం i, j;
    పూర్ణాంకం స్థానభ్రంశం = 0;
    కొరకు(i = 0; i < l.n; ++i){
        పూర్ణాంకం సూచీ = l.ఆదానం_layers[i];
        దశాంశం *delta = net.layers[సూచీ].delta;
        పూర్ణాంకం ఆదానం_కొలత = l.ఆదానం_sizes[i];
        కొరకు(j = 0; j < l.batch; ++j){
            axpy_cpu(ఆదానం_కొలత, 1, l.delta + స్థానభ్రంశం + j*l.outputs, 1, delta + j*ఆదానం_కొలత, 1);
        }
        స్థానభ్రంశం += ఆదానం_కొలత;
    }
}

#నిర్వచితమైతే GPU
శూన్య forward_route_layer_gpu(స్థిరరాశి route_layer l, జాలం net)
{
    పూర్ణాంకం i, j;
    పూర్ణాంకం స్థానభ్రంశం = 0;
    కొరకు(i = 0; i < l.n; ++i){
        పూర్ణాంకం సూచీ = l.ఆదానం_layers[i];
        దశాంశం *ఆదానం = net.layers[సూచీ].output_gpu;
        పూర్ణాంకం ఆదానం_కొలత = l.ఆదానం_sizes[i];
        కొరకు(j = 0; j < l.batch; ++j){
            copy_gpu(ఆదానం_కొలత, ఆదానం + j*ఆదానం_కొలత, 1, l.output_gpu + స్థానభ్రంశం + j*l.outputs, 1);
        }
        స్థానభ్రంశం += ఆదానం_కొలత;
    }
}

శూన్య backward_route_layer_gpu(స్థిరరాశి route_layer l, జాలం net)
{
    పూర్ణాంకం i, j;
    పూర్ణాంకం స్థానభ్రంశం = 0;
    కొరకు(i = 0; i < l.n; ++i){
        పూర్ణాంకం సూచీ = l.ఆదానం_layers[i];
        దశాంశం *delta = net.layers[సూచీ].delta_gpu;
        పూర్ణాంకం ఆదానం_కొలత = l.ఆదానం_sizes[i];
        కొరకు(j = 0; j < l.batch; ++j){
            axpy_gpu(ఆదానం_కొలత, 1, l.delta_gpu + స్థానభ్రంశం + j*l.outputs, 1, delta + j*ఆదానం_కొలత, 1);
        }
        స్థానభ్రంశం += ఆదానం_కొలత;
    }
}
#అయితే_ముగింపు
