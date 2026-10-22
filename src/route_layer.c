#شامل "route_layer.h"
#شامل "cuda.h"
#شامل "blas.h"

#شامل <stdio.h>

route_layer make_route_layer(صحیح batch, صحیح n, صحیح *درون_داد_layers, صحیح *درون_داد_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.درون_داد_layers = درون_داد_layers;
    l.درون_داد_sizes = درون_داد_sizes;
    صحیح i;
    صحیح outputs = 0;
    برای(i = 0; i < n; ++i){
        fprintf(stderr," %d", درون_داد_layers[i]);
        outputs += درون_داد_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  calloc(outputs*batch, اندازه(اعشاری));
    l.output = calloc(outputs*batch, اندازه(اعشاری));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #ifdef GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #endif
    بازگشت l;
}

تهی resize_route_layer(route_layer *l, network *net)
{
    صحیح i;
    layer first = net->layers[l->درون_داد_layers[0]];
    l->out_w = first.out_w;
    l->out_h = first.out_h;
    l->out_c = first.out_c;
    l->outputs = first.outputs;
    l->درون_داد_sizes[0] = first.outputs;
    برای(i = 1; i < l->n; ++i){
        صحیح index = l->درون_داد_layers[i];
        layer next = net->layers[index];
        l->outputs += next.outputs;
        l->درون_داد_sizes[i] = next.outputs;
        اگر(next.out_w == first.out_w && next.out_h == first.out_h){
            l->out_c += next.out_c;
        }وگرنه{
            printf("%d %d, %d %d\n", next.out_w, next.out_h, first.out_w, first.out_h);
            l->out_h = l->out_w = l->out_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*اندازه(اعشاری));
    l->output = realloc(l->output, l->outputs*l->batch*اندازه(اعشاری));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

تهی forward_route_layer(ثابت route_layer l, network net)
{
    صحیح i, j;
    صحیح offset = 0;
    برای(i = 0; i < l.n; ++i){
        صحیح index = l.درون_داد_layers[i];
        اعشاری *درون_داد = net.layers[index].output;
        صحیح درون_داد_size = l.درون_داد_sizes[i];
        برای(j = 0; j < l.batch; ++j){
            copy_cpu(درون_داد_size, درون_داد + j*درون_داد_size, 1, l.output + offset + j*l.outputs, 1);
        }
        offset += درون_داد_size;
    }
}

تهی backward_route_layer(ثابت route_layer l, network net)
{
    صحیح i, j;
    صحیح offset = 0;
    برای(i = 0; i < l.n; ++i){
        صحیح index = l.درون_داد_layers[i];
        اعشاری *delta = net.layers[index].delta;
        صحیح درون_داد_size = l.درون_داد_sizes[i];
        برای(j = 0; j < l.batch; ++j){
            axpy_cpu(درون_داد_size, 1, l.delta + offset + j*l.outputs, 1, delta + j*درون_داد_size, 1);
        }
        offset += درون_داد_size;
    }
}

#ifdef GPU
تهی forward_route_layer_gpu(ثابت route_layer l, network net)
{
    صحیح i, j;
    صحیح offset = 0;
    برای(i = 0; i < l.n; ++i){
        صحیح index = l.درون_داد_layers[i];
        اعشاری *درون_داد = net.layers[index].output_gpu;
        صحیح درون_داد_size = l.درون_داد_sizes[i];
        برای(j = 0; j < l.batch; ++j){
            copy_gpu(درون_داد_size, درون_داد + j*درون_داد_size, 1, l.output_gpu + offset + j*l.outputs, 1);
        }
        offset += درون_داد_size;
    }
}

تهی backward_route_layer_gpu(ثابت route_layer l, network net)
{
    صحیح i, j;
    صحیح offset = 0;
    برای(i = 0; i < l.n; ++i){
        صحیح index = l.درون_داد_layers[i];
        اعشاری *delta = net.layers[index].delta_gpu;
        صحیح درون_داد_size = l.درون_داد_sizes[i];
        برای(j = 0; j < l.batch; ++j){
            axpy_gpu(درون_داد_size, 1, l.delta_gpu + offset + j*l.outputs, 1, delta + j*درون_داد_size, 1);
        }
        offset += درون_داد_size;
    }
}
#endif
