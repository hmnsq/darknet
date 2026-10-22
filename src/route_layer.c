#ضمن "route_layer.h"
#ضمن "cuda.h"
#ضمن "blas.h"

#ضمن <stdio.h>

route_layer make_route_layer(صحيح batch, صحيح n, صحيح *إدخال_layers, صحيح *إدخال_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.إدخال_layers = إدخال_layers;
    l.إدخال_sizes = إدخال_sizes;
    صحيح i;
    صحيح outputs = 0;
    لكل(i = 0; i < n; ++i){
        fprintf(stderr," %d", إدخال_layers[i]);
        outputs += إدخال_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  calloc(outputs*batch, حجم(عائم));
    l.output = calloc(outputs*batch, حجم(عائم));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #ifdef GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #endif
    إرجاع l;
}

فراغ resize_route_layer(route_layer *l, network *net)
{
    صحيح i;
    layer first = net->layers[l->إدخال_layers[0]];
    l->out_w = first.out_w;
    l->out_h = first.out_h;
    l->out_c = first.out_c;
    l->outputs = first.outputs;
    l->إدخال_sizes[0] = first.outputs;
    لكل(i = 1; i < l->n; ++i){
        صحيح index = l->إدخال_layers[i];
        layer next = net->layers[index];
        l->outputs += next.outputs;
        l->إدخال_sizes[i] = next.outputs;
        إذا(next.out_w == first.out_w && next.out_h == first.out_h){
            l->out_c += next.out_c;
        }وإلا{
            printf("%d %d, %d %d\n", next.out_w, next.out_h, first.out_w, first.out_h);
            l->out_h = l->out_w = l->out_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*حجم(عائم));
    l->output = realloc(l->output, l->outputs*l->batch*حجم(عائم));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

فراغ forward_route_layer(ثابت route_layer l, network net)
{
    صحيح i, j;
    صحيح offset = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح index = l.إدخال_layers[i];
        عائم *إدخال = net.layers[index].output;
        صحيح إدخال_size = l.إدخال_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            copy_cpu(إدخال_size, إدخال + j*إدخال_size, 1, l.output + offset + j*l.outputs, 1);
        }
        offset += إدخال_size;
    }
}

فراغ backward_route_layer(ثابت route_layer l, network net)
{
    صحيح i, j;
    صحيح offset = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح index = l.إدخال_layers[i];
        عائم *delta = net.layers[index].delta;
        صحيح إدخال_size = l.إدخال_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            axpy_cpu(إدخال_size, 1, l.delta + offset + j*l.outputs, 1, delta + j*إدخال_size, 1);
        }
        offset += إدخال_size;
    }
}

#ifdef GPU
فراغ forward_route_layer_gpu(ثابت route_layer l, network net)
{
    صحيح i, j;
    صحيح offset = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح index = l.إدخال_layers[i];
        عائم *إدخال = net.layers[index].output_gpu;
        صحيح إدخال_size = l.إدخال_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            copy_gpu(إدخال_size, إدخال + j*إدخال_size, 1, l.output_gpu + offset + j*l.outputs, 1);
        }
        offset += إدخال_size;
    }
}

فراغ backward_route_layer_gpu(ثابت route_layer l, network net)
{
    صحيح i, j;
    صحيح offset = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح index = l.إدخال_layers[i];
        عائم *delta = net.layers[index].delta_gpu;
        صحيح إدخال_size = l.إدخال_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            axpy_gpu(إدخال_size, 1, l.delta_gpu + offset + j*l.outputs, 1, delta + j*إدخال_size, 1);
        }
        offset += إدخال_size;
    }
}
#endif
