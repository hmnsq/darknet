#include "route_layer.h"
#include "cuda.h"
#include "blas.h"

#include <stdio.h>

route_layer make_route_layer(පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා *input_layers, පූර්ණ_සංඛ්‍යා *input_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.input_layers = input_layers;
    l.input_sizes = input_sizes;
    පූර්ණ_සංඛ්‍යා i;
    පූර්ණ_සංඛ්‍යා outputs = 0;
    සඳහා(i = 0; i < n; ++i){
        fprintf(stderr," %d", input_layers[i]);
        outputs += input_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  calloc(outputs*batch, parimaa_na(float));
    l.output = calloc(outputs*batch, parimaa_na(float));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #ifdef GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #endif
    ආපසු l;
}

void resize_route_layer(route_layer *l, network *net)
{
    පූර්ණ_සංඛ්‍යා i;
    layer first = net->layers[l->input_layers[0]];
    l->out_w = first.out_w;
    l->out_h = first.out_h;
    l->out_c = first.out_c;
    l->outputs = first.outputs;
    l->input_sizes[0] = first.outputs;
    සඳහා(i = 1; i < l->n; ++i){
        පූර්ණ_සංඛ්‍යා index = l->input_layers[i];
        layer next = net->layers[index];
        l->outputs += next.outputs;
        l->input_sizes[i] = next.outputs;
        නම්(next.out_w == first.out_w && next.out_h == first.out_h){
            l->out_c += next.out_c;
        }නැතහොත්{
            printf("%d %d, %d %d\n", next.out_w, next.out_h, first.out_w, first.out_h);
            l->out_h = l->out_w = l->out_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*parimaa_na(float));
    l->output = realloc(l->output, l->outputs*l->batch*parimaa_na(float));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

void forward_route_layer(const route_layer l, network net)
{
    පූර්ණ_සංඛ්‍යා i, j;
    පූර්ණ_සංඛ්‍යා offset = 0;
    සඳහා(i = 0; i < l.n; ++i){
        පූර්ණ_සංඛ්‍යා index = l.input_layers[i];
        float *input = net.layers[index].output;
        පූර්ණ_සංඛ්‍යා input_size = l.input_sizes[i];
        සඳහා(j = 0; j < l.batch; ++j){
            copy_cpu(input_size, input + j*input_size, 1, l.output + offset + j*l.outputs, 1);
        }
        offset += input_size;
    }
}

void backward_route_layer(const route_layer l, network net)
{
    පූර්ණ_සංඛ්‍යා i, j;
    පූර්ණ_සංඛ්‍යා offset = 0;
    සඳහා(i = 0; i < l.n; ++i){
        පූර්ණ_සංඛ්‍යා index = l.input_layers[i];
        float *delta = net.layers[index].delta;
        පූර්ණ_සංඛ්‍යා input_size = l.input_sizes[i];
        සඳහා(j = 0; j < l.batch; ++j){
            axpy_cpu(input_size, 1, l.delta + offset + j*l.outputs, 1, delta + j*input_size, 1);
        }
        offset += input_size;
    }
}

#ifdef GPU
void forward_route_layer_gpu(const route_layer l, network net)
{
    පූර්ණ_සංඛ්‍යා i, j;
    පූර්ණ_සංඛ්‍යා offset = 0;
    සඳහා(i = 0; i < l.n; ++i){
        පූර්ණ_සංඛ්‍යා index = l.input_layers[i];
        float *input = net.layers[index].output_gpu;
        පූර්ණ_සංඛ්‍යා input_size = l.input_sizes[i];
        සඳහා(j = 0; j < l.batch; ++j){
            copy_gpu(input_size, input + j*input_size, 1, l.output_gpu + offset + j*l.outputs, 1);
        }
        offset += input_size;
    }
}

void backward_route_layer_gpu(const route_layer l, network net)
{
    පූර්ණ_සංඛ්‍යා i, j;
    පූර්ණ_සංඛ්‍යා offset = 0;
    සඳහා(i = 0; i < l.n; ++i){
        පූර්ණ_සංඛ්‍යා index = l.input_layers[i];
        float *delta = net.layers[index].delta_gpu;
        පූර්ණ_සංඛ්‍යා input_size = l.input_sizes[i];
        සඳහා(j = 0; j < l.batch; ++j){
            axpy_gpu(input_size, 1, l.delta_gpu + offset + j*l.outputs, 1, delta + j*input_size, 1);
        }
        offset += input_size;
    }
}
#endif
