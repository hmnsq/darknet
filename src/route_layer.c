#ضمن "route_layer.h"
#ضمن "cuda.h"
#ضمن "blas.h"

#ضمن <stdio.h>

route_layer make_route_layer(صحيح batch, صحيح n, صحيح *ننوت_layers, صحيح *ننوت_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.ننوت_layers = ننوت_layers;
    l.ننوت_sizes = ننوت_sizes;
    صحيح i;
    صحيح outputs = 0;
    لكل(i = 0; i < n; ++i){
        fprintf(stderr," %d", ننوت_layers[i]);
        outputs += ننوت_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  حافظه_صفر_ورکړه(outputs*batch, حجم(عائم));
    l.output = حافظه_صفر_ورکړه(outputs*batch, حجم(عائم));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #که_تعریف_شوی GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #د_که_پای
    أرجع l;
}

فراغ resize_route_layer(route_layer *l, شبکه *net)
{
    صحيح i;
    layer لومړی = net->layers[l->ننوت_layers[0]];
    l->بهر_w = لومړی.بهر_w;
    l->بهر_h = لومړی.بهر_h;
    l->بهر_c = لومړی.بهر_c;
    l->outputs = لومړی.outputs;
    l->ننوت_sizes[0] = لومړی.outputs;
    لكل(i = 1; i < l->n; ++i){
        صحيح شاخص = l->ننوت_layers[i];
        layer بل = net->layers[شاخص];
        l->outputs += بل.outputs;
        l->ننوت_sizes[i] = بل.outputs;
        إذا(بل.بهر_w == لومړی.بهر_w && بل.بهر_h == لومړی.بهر_h){
            l->بهر_c += بل.بهر_c;
        }وإلا{
            چاپ_کړه("%d %d, %d %d\n", بل.بهر_w, بل.بهر_h, لومړی.بهر_w, لومړی.بهر_h);
            l->بهر_h = l->بهر_w = l->بهر_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*حجم(عائم));
    l->output = realloc(l->output, l->outputs*l->batch*حجم(عائم));

#که_تعریف_شوی GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#د_که_پای
    
}

فراغ forward_route_layer(ثابت route_layer l, شبکه net)
{
    صحيح i, j;
    صحيح دننه_واټن = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح شاخص = l.ننوت_layers[i];
        عائم *ننوت = net.layers[شاخص].output;
        صحيح ننوت_اندازه = l.ننوت_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            copy_cpu(ننوت_اندازه, ننوت + j*ننوت_اندازه, 1, l.output + دننه_واټن + j*l.outputs, 1);
        }
        دننه_واټن += ننوت_اندازه;
    }
}

فراغ backward_route_layer(ثابت route_layer l, شبکه net)
{
    صحيح i, j;
    صحيح دننه_واټن = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح شاخص = l.ننوت_layers[i];
        عائم *delta = net.layers[شاخص].delta;
        صحيح ننوت_اندازه = l.ننوت_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            axpy_cpu(ننوت_اندازه, 1, l.delta + دننه_واټن + j*l.outputs, 1, delta + j*ننوت_اندازه, 1);
        }
        دننه_واټن += ننوت_اندازه;
    }
}

#که_تعریف_شوی GPU
فراغ forward_route_layer_gpu(ثابت route_layer l, شبکه net)
{
    صحيح i, j;
    صحيح دننه_واټن = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح شاخص = l.ننوت_layers[i];
        عائم *ننوت = net.layers[شاخص].output_gpu;
        صحيح ننوت_اندازه = l.ننوت_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            copy_gpu(ننوت_اندازه, ننوت + j*ننوت_اندازه, 1, l.output_gpu + دننه_واټن + j*l.outputs, 1);
        }
        دننه_واټن += ننوت_اندازه;
    }
}

فراغ backward_route_layer_gpu(ثابت route_layer l, شبکه net)
{
    صحيح i, j;
    صحيح دننه_واټن = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح شاخص = l.ننوت_layers[i];
        عائم *delta = net.layers[شاخص].delta_gpu;
        صحيح ننوت_اندازه = l.ننوت_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            axpy_gpu(ننوت_اندازه, 1, l.delta_gpu + دننه_واټن + j*l.outputs, 1, delta + j*ننوت_اندازه, 1);
        }
        دننه_واټن += ننوت_اندازه;
    }
}
#د_که_پای
