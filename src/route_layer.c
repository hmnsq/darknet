#ضمن "route_layer.h"
#ضمن "cuda.h"
#ضمن "blas.h"

#ضمن <stdio.h>

route_layer make_route_layer(صحيح batch, صحيح n, صحيح *input_layers, صحيح *input_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.input_layers = input_layers;
    l.input_sizes = input_sizes;
    صحيح i;
    صحيح outputs = 0;
    עבור(i = 0; i < n; ++i){
        fprintf(stderr," %d", input_layers[i]);
        outputs += input_sizes[i];
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
    החזר l;
}

فراغ resize_route_layer(route_layer *l, network *net)
{
    صحيح i;
    layer first = net->layers[l->input_layers[0]];
    l->out_w = first.out_w;
    l->out_h = first.out_h;
    l->out_c = first.out_c;
    l->outputs = first.outputs;
    l->input_sizes[0] = first.outputs;
    עבור(i = 1; i < l->n; ++i){
        صحيح index = l->input_layers[i];
        layer next = net->layers[index];
        l->outputs += next.outputs;
        l->input_sizes[i] = next.outputs;
        אם(next.out_w == first.out_w && next.out_h == first.out_h){
            l->out_c += next.out_c;
        }אחרת{
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
    עבור(i = 0; i < l.n; ++i){
        صحيح index = l.input_layers[i];
        عائم *input = net.layers[index].output;
        صحيح input_size = l.input_sizes[i];
        עבור(j = 0; j < l.batch; ++j){
            copy_cpu(input_size, input + j*input_size, 1, l.output + offset + j*l.outputs, 1);
        }
        offset += input_size;
    }
}

فراغ backward_route_layer(ثابت route_layer l, network net)
{
    صحيح i, j;
    صحيح offset = 0;
    עבור(i = 0; i < l.n; ++i){
        صحيح index = l.input_layers[i];
        عائم *delta = net.layers[index].delta;
        صحيح input_size = l.input_sizes[i];
        עבור(j = 0; j < l.batch; ++j){
            axpy_cpu(input_size, 1, l.delta + offset + j*l.outputs, 1, delta + j*input_size, 1);
        }
        offset += input_size;
    }
}

#ifdef GPU
فراغ forward_route_layer_gpu(ثابت route_layer l, network net)
{
    صحيح i, j;
    صحيح offset = 0;
    עבור(i = 0; i < l.n; ++i){
        صحيح index = l.input_layers[i];
        عائم *input = net.layers[index].output_gpu;
        صحيح input_size = l.input_sizes[i];
        עבור(j = 0; j < l.batch; ++j){
            copy_gpu(input_size, input + j*input_size, 1, l.output_gpu + offset + j*l.outputs, 1);
        }
        offset += input_size;
    }
}

فراغ backward_route_layer_gpu(ثابت route_layer l, network net)
{
    صحيح i, j;
    صحيح offset = 0;
    עבור(i = 0; i < l.n; ++i){
        صحيح index = l.input_layers[i];
        عائم *delta = net.layers[index].delta_gpu;
        صحيح input_size = l.input_sizes[i];
        עבור(j = 0; j < l.batch; ++j){
            axpy_gpu(input_size, 1, l.delta_gpu + offset + j*l.outputs, 1, delta + j*input_size, 1);
        }
        offset += input_size;
    }
}
#endif
