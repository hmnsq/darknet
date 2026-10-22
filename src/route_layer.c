#ضمن "route_layer.h"
#ضمن "cuda.h"
#ضمن "blas.h"

#ضمن <stdio.h>

route_layer make_route_layer(صحيح batch, صحيح n, صحيح *درون_داد_layers, صحيح *درون_داد_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.درون_داد_layers = درون_داد_layers;
    l.درون_داد_sizes = درون_داد_sizes;
    صحيح i;
    صحيح outputs = 0;
    لكل(i = 0; i < n; ++i){
        fprintf(stderr," %d", درون_داد_layers[i]);
        outputs += درون_داد_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  حافظه_صفر_بده(outputs*batch, حجم(عائم));
    l.output = حافظه_صفر_بده(outputs*batch, حجم(عائم));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #اگر_تعریف_شده GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #پایان_اگر
    أرجع l;
}

فراغ resize_route_layer(route_layer *l, شبکه *net)
{
    صحيح i;
    layer اولی = net->layers[l->درون_داد_layers[0]];
    l->بیرون_w = اولی.بیرون_w;
    l->بیرون_h = اولی.بیرون_h;
    l->بیرون_c = اولی.بیرون_c;
    l->outputs = اولی.outputs;
    l->درون_داد_sizes[0] = اولی.outputs;
    لكل(i = 1; i < l->n; ++i){
        صحيح شاخص = l->درون_داد_layers[i];
        layer بعدی = net->layers[شاخص];
        l->outputs += بعدی.outputs;
        l->درون_داد_sizes[i] = بعدی.outputs;
        إذا(بعدی.بیرون_w == اولی.بیرون_w && بعدی.بیرون_h == اولی.بیرون_h){
            l->بیرون_c += بعدی.بیرون_c;
        }وإلا{
            چاپ_کن("%d %d, %d %d\n", بعدی.بیرون_w, بعدی.بیرون_h, اولی.بیرون_w, اولی.بیرون_h);
            l->بیرون_h = l->بیرون_w = l->بیرون_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*حجم(عائم));
    l->output = realloc(l->output, l->outputs*l->batch*حجم(عائم));

#اگر_تعریف_شده GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#پایان_اگر
    
}

فراغ forward_route_layer(ثابت route_layer l, شبکه net)
{
    صحيح i, j;
    صحيح جابه_جایی = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح شاخص = l.درون_داد_layers[i];
        عائم *درون_داد = net.layers[شاخص].output;
        صحيح درون_داد_اندازه = l.درون_داد_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            copy_cpu(درون_داد_اندازه, درون_داد + j*درون_داد_اندازه, 1, l.output + جابه_جایی + j*l.outputs, 1);
        }
        جابه_جایی += درون_داد_اندازه;
    }
}

فراغ backward_route_layer(ثابت route_layer l, شبکه net)
{
    صحيح i, j;
    صحيح جابه_جایی = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح شاخص = l.درون_داد_layers[i];
        عائم *delta = net.layers[شاخص].delta;
        صحيح درون_داد_اندازه = l.درون_داد_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            axpy_cpu(درون_داد_اندازه, 1, l.delta + جابه_جایی + j*l.outputs, 1, delta + j*درون_داد_اندازه, 1);
        }
        جابه_جایی += درون_داد_اندازه;
    }
}

#اگر_تعریف_شده GPU
فراغ forward_route_layer_gpu(ثابت route_layer l, شبکه net)
{
    صحيح i, j;
    صحيح جابه_جایی = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح شاخص = l.درون_داد_layers[i];
        عائم *درون_داد = net.layers[شاخص].output_gpu;
        صحيح درون_داد_اندازه = l.درون_داد_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            copy_gpu(درون_داد_اندازه, درون_داد + j*درون_داد_اندازه, 1, l.output_gpu + جابه_جایی + j*l.outputs, 1);
        }
        جابه_جایی += درون_داد_اندازه;
    }
}

فراغ backward_route_layer_gpu(ثابت route_layer l, شبکه net)
{
    صحيح i, j;
    صحيح جابه_جایی = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح شاخص = l.درون_داد_layers[i];
        عائم *delta = net.layers[شاخص].delta_gpu;
        صحيح درون_داد_اندازه = l.درون_داد_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            axpy_gpu(درون_داد_اندازه, 1, l.delta_gpu + جابه_جایی + j*l.outputs, 1, delta + j*درون_داد_اندازه, 1);
        }
        جابه_جایی += درون_داد_اندازه;
    }
}
#پایان_اگر
