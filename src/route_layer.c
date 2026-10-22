#include "route_layer.h"
#include "cuda.h"
#include "blas.h"

#include <stdio.h>

route_layer make_route_layer(int batch, int n, int *पासोक_layers, int *पासोक_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.पासोक_layers = पासोक_layers;
    l.पासोक_sizes = पासोक_sizes;
    int i;
    int outputs = 0;
    for(i = 0; i < n; ++i){
        fprintf(stderr," %d", पासोक_layers[i]);
        outputs += पासोक_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  calloc(outputs*batch, sizeof(float));
    l.output = calloc(outputs*batch, sizeof(float));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #ifdef GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #endif
    return l;
}

void resize_route_layer(route_layer *l, network *net)
{
    int i;
    layer first = net->layers[l->पासोक_layers[0]];
    l->out_w = first.out_w;
    l->out_h = first.out_h;
    l->out_c = first.out_c;
    l->outputs = first.outputs;
    l->पासोक_sizes[0] = first.outputs;
    for(i = 1; i < l->n; ++i){
        int index = l->पासोक_layers[i];
        layer next = net->layers[index];
        l->outputs += next.outputs;
        l->पासोक_sizes[i] = next.outputs;
        if(next.out_w == first.out_w && next.out_h == first.out_h){
            l->out_c += next.out_c;
        }else{
            printf("%d %d, %d %d\n", next.out_w, next.out_h, first.out_w, first.out_h);
            l->out_h = l->out_w = l->out_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*sizeof(float));
    l->output = realloc(l->output, l->outputs*l->batch*sizeof(float));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

void forward_route_layer(const route_layer l, network net)
{
    int i, j;
    int offset = 0;
    for(i = 0; i < l.n; ++i){
        int index = l.पासोक_layers[i];
        float *पासोक = net.layers[index].output;
        int पासोक_size = l.पासोक_sizes[i];
        for(j = 0; j < l.batch; ++j){
            copy_cpu(पासोक_size, पासोक + j*पासोक_size, 1, l.output + offset + j*l.outputs, 1);
        }
        offset += पासोक_size;
    }
}

void backward_route_layer(const route_layer l, network net)
{
    int i, j;
    int offset = 0;
    for(i = 0; i < l.n; ++i){
        int index = l.पासोक_layers[i];
        float *delta = net.layers[index].delta;
        int पासोक_size = l.पासोक_sizes[i];
        for(j = 0; j < l.batch; ++j){
            axpy_cpu(पासोक_size, 1, l.delta + offset + j*l.outputs, 1, delta + j*पासोक_size, 1);
        }
        offset += पासोक_size;
    }
}

#ifdef GPU
void forward_route_layer_gpu(const route_layer l, network net)
{
    int i, j;
    int offset = 0;
    for(i = 0; i < l.n; ++i){
        int index = l.पासोक_layers[i];
        float *पासोक = net.layers[index].output_gpu;
        int पासोक_size = l.पासोक_sizes[i];
        for(j = 0; j < l.batch; ++j){
            copy_gpu(पासोक_size, पासोक + j*पासोक_size, 1, l.output_gpu + offset + j*l.outputs, 1);
        }
        offset += पासोक_size;
    }
}

void backward_route_layer_gpu(const route_layer l, network net)
{
    int i, j;
    int offset = 0;
    for(i = 0; i < l.n; ++i){
        int index = l.पासोक_layers[i];
        float *delta = net.layers[index].delta_gpu;
        int पासोक_size = l.पासोक_sizes[i];
        for(j = 0; j < l.batch; ++j){
            axpy_gpu(पासोक_size, 1, l.delta_gpu + offset + j*l.outputs, 1, delta + j*पासोक_size, 1);
        }
        offset += पासोक_size;
    }
}
#endif
