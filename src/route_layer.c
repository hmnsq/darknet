#അന്തര്ഭാവ "route_layer.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "blas.h"

#അന്തര്ഭാവ <stdio.h>

route_layer make_route_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ *നിവേശം_layers, പൂർണ്ണസംഖ്യ *നിവേശം_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.നിവേശം_layers = നിവേശം_layers;
    l.നിവേശം_sizes = നിവേശം_sizes;
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ outputs = 0;
    വേണ്ടി(i = 0; i < n; ++i){
        fprintf(stderr," %d", നിവേശം_layers[i]);
        outputs += നിവേശം_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  calloc(outputs*batch, വലുപ്പം(ദശാംശം));
    l.output = calloc(outputs*batch, വലുപ്പം(ദശാംശം));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #ifdef GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #endif
    തിരികെ l;
}

ശൂന്യം resize_route_layer(route_layer *l, network *net)
{
    പൂർണ്ണസംഖ്യ i;
    layer first = net->layers[l->നിവേശം_layers[0]];
    l->out_w = first.out_w;
    l->out_h = first.out_h;
    l->out_c = first.out_c;
    l->outputs = first.outputs;
    l->നിവേശം_sizes[0] = first.outputs;
    വേണ്ടി(i = 1; i < l->n; ++i){
        പൂർണ്ണസംഖ്യ index = l->നിവേശം_layers[i];
        layer next = net->layers[index];
        l->outputs += next.outputs;
        l->നിവേശം_sizes[i] = next.outputs;
        എങ്കിൽ(next.out_w == first.out_w && next.out_h == first.out_h){
            l->out_c += next.out_c;
        }അല്ലെങ്കിൽ{
            printf("%d %d, %d %d\n", next.out_w, next.out_h, first.out_w, first.out_h);
            l->out_h = l->out_w = l->out_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*വലുപ്പം(ദശാംശം));
    l->output = realloc(l->output, l->outputs*l->batch*വലുപ്പം(ദശാംശം));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

ശൂന്യം forward_route_layer(സ്ഥിരം route_layer l, network net)
{
    പൂർണ്ണസംഖ്യ i, j;
    പൂർണ്ണസംഖ്യ offset = 0;
    വേണ്ടി(i = 0; i < l.n; ++i){
        പൂർണ്ണസംഖ്യ index = l.നിവേശം_layers[i];
        ദശാംശം *നിവേശം = net.layers[index].output;
        പൂർണ്ണസംഖ്യ നിവേശം_size = l.നിവേശം_sizes[i];
        വേണ്ടി(j = 0; j < l.batch; ++j){
            copy_cpu(നിവേശം_size, നിവേശം + j*നിവേശം_size, 1, l.output + offset + j*l.outputs, 1);
        }
        offset += നിവേശം_size;
    }
}

ശൂന്യം backward_route_layer(സ്ഥിരം route_layer l, network net)
{
    പൂർണ്ണസംഖ്യ i, j;
    പൂർണ്ണസംഖ്യ offset = 0;
    വേണ്ടി(i = 0; i < l.n; ++i){
        പൂർണ്ണസംഖ്യ index = l.നിവേശം_layers[i];
        ദശാംശം *delta = net.layers[index].delta;
        പൂർണ്ണസംഖ്യ നിവേശം_size = l.നിവേശം_sizes[i];
        വേണ്ടി(j = 0; j < l.batch; ++j){
            axpy_cpu(നിവേശം_size, 1, l.delta + offset + j*l.outputs, 1, delta + j*നിവേശം_size, 1);
        }
        offset += നിവേശം_size;
    }
}

#ifdef GPU
ശൂന്യം forward_route_layer_gpu(സ്ഥിരം route_layer l, network net)
{
    പൂർണ്ണസംഖ്യ i, j;
    പൂർണ്ണസംഖ്യ offset = 0;
    വേണ്ടി(i = 0; i < l.n; ++i){
        പൂർണ്ണസംഖ്യ index = l.നിവേശം_layers[i];
        ദശാംശം *നിവേശം = net.layers[index].output_gpu;
        പൂർണ്ണസംഖ്യ നിവേശം_size = l.നിവേശം_sizes[i];
        വേണ്ടി(j = 0; j < l.batch; ++j){
            copy_gpu(നിവേശം_size, നിവേശം + j*നിവേശം_size, 1, l.output_gpu + offset + j*l.outputs, 1);
        }
        offset += നിവേശം_size;
    }
}

ശൂന്യം backward_route_layer_gpu(സ്ഥിരം route_layer l, network net)
{
    പൂർണ്ണസംഖ്യ i, j;
    പൂർണ്ണസംഖ്യ offset = 0;
    വേണ്ടി(i = 0; i < l.n; ++i){
        പൂർണ്ണസംഖ്യ index = l.നിവേശം_layers[i];
        ദശാംശം *delta = net.layers[index].delta_gpu;
        പൂർണ്ണസംഖ്യ നിവേശം_size = l.നിവേശം_sizes[i];
        വേണ്ടി(j = 0; j < l.batch; ++j){
            axpy_gpu(നിവേശം_size, 1, l.delta_gpu + offset + j*l.outputs, 1, delta + j*നിവേശം_size, 1);
        }
        offset += നിവേശം_size;
    }
}
#endif
