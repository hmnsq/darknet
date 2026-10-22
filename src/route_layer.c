#અન્તર્ભાવ "route_layer.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "blas.h"

#અન્તર્ભાવ <stdio.h>

route_layer make_route_layer(પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક *આગત_layers, પૂર્ણાંક *આગત_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.આગત_layers = આગત_layers;
    l.આગત_sizes = આગત_sizes;
    પૂર્ણાંક i;
    પૂર્ણાંક outputs = 0;
    માટે(i = 0; i < n; ++i){
        fprintf(stderr," %d", આગત_layers[i]);
        outputs += આગત_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  calloc(outputs*batch, કદ(દશાંશ));
    l.output = calloc(outputs*batch, કદ(દશાંશ));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #ifdef GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #endif
    પાછા l;
}

શૂન્ય resize_route_layer(route_layer *l, network *net)
{
    પૂર્ણાંક i;
    layer first = net->layers[l->આગત_layers[0]];
    l->out_w = first.out_w;
    l->out_h = first.out_h;
    l->out_c = first.out_c;
    l->outputs = first.outputs;
    l->આગત_sizes[0] = first.outputs;
    માટે(i = 1; i < l->n; ++i){
        પૂર્ણાંક index = l->આગત_layers[i];
        layer next = net->layers[index];
        l->outputs += next.outputs;
        l->આગત_sizes[i] = next.outputs;
        જો(next.out_w == first.out_w && next.out_h == first.out_h){
            l->out_c += next.out_c;
        }નહીંતર{
            printf("%d %d, %d %d\n", next.out_w, next.out_h, first.out_w, first.out_h);
            l->out_h = l->out_w = l->out_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*કદ(દશાંશ));
    l->output = realloc(l->output, l->outputs*l->batch*કદ(દશાંશ));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

શૂન્ય forward_route_layer(અચલ route_layer l, network net)
{
    પૂર્ણાંક i, j;
    પૂર્ણાંક offset = 0;
    માટે(i = 0; i < l.n; ++i){
        પૂર્ણાંક index = l.આગત_layers[i];
        દશાંશ *આગત = net.layers[index].output;
        પૂર્ણાંક આગત_size = l.આગત_sizes[i];
        માટે(j = 0; j < l.batch; ++j){
            copy_cpu(આગત_size, આગત + j*આગત_size, 1, l.output + offset + j*l.outputs, 1);
        }
        offset += આગત_size;
    }
}

શૂન્ય backward_route_layer(અચલ route_layer l, network net)
{
    પૂર્ણાંક i, j;
    પૂર્ણાંક offset = 0;
    માટે(i = 0; i < l.n; ++i){
        પૂર્ણાંક index = l.આગત_layers[i];
        દશાંશ *delta = net.layers[index].delta;
        પૂર્ણાંક આગત_size = l.આગત_sizes[i];
        માટે(j = 0; j < l.batch; ++j){
            axpy_cpu(આગત_size, 1, l.delta + offset + j*l.outputs, 1, delta + j*આગત_size, 1);
        }
        offset += આગત_size;
    }
}

#ifdef GPU
શૂન્ય forward_route_layer_gpu(અચલ route_layer l, network net)
{
    પૂર્ણાંક i, j;
    પૂર્ણાંક offset = 0;
    માટે(i = 0; i < l.n; ++i){
        પૂર્ણાંક index = l.આગત_layers[i];
        દશાંશ *આગત = net.layers[index].output_gpu;
        પૂર્ણાંક આગત_size = l.આગત_sizes[i];
        માટે(j = 0; j < l.batch; ++j){
            copy_gpu(આગત_size, આગત + j*આગત_size, 1, l.output_gpu + offset + j*l.outputs, 1);
        }
        offset += આગત_size;
    }
}

શૂન્ય backward_route_layer_gpu(અચલ route_layer l, network net)
{
    પૂર્ણાંક i, j;
    પૂર્ણાંક offset = 0;
    માટે(i = 0; i < l.n; ++i){
        પૂર્ણાંક index = l.આગત_layers[i];
        દશાંશ *delta = net.layers[index].delta_gpu;
        પૂર્ણાંક આગત_size = l.આગત_sizes[i];
        માટે(j = 0; j < l.batch; ++j){
            axpy_gpu(આગત_size, 1, l.delta_gpu + offset + j*l.outputs, 1, delta + j*આગત_size, 1);
        }
        offset += આગત_size;
    }
}
#endif
