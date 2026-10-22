#ଅନ୍ତର୍ଭାଵ "route_layer.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"

#ଅନ୍ତର୍ଭାଵ <stdio.h>

route_layer make_route_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ନିବେଶ_layers, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ନିବେଶ_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.ନିବେଶ_layers = ନିବେଶ_layers;
    l.ନିବେଶ_sizes = ନିବେଶ_sizes;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା outputs = 0;
    ପାଇଁ(i = 0; i < n; ++i){
        fprintf(stderr," %d", ନିବେଶ_layers[i]);
        outputs += ନିବେଶ_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  calloc(outputs*batch, ଆକାର(ଦଶମିକ));
    l.output = calloc(outputs*batch, ଆକାର(ଦଶମିକ));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #ifdef GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #endif
    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ resize_route_layer(route_layer *l, network *net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    layer first = net->layers[l->ନିବେଶ_layers[0]];
    l->out_w = first.out_w;
    l->out_h = first.out_h;
    l->out_c = first.out_c;
    l->outputs = first.outputs;
    l->ନିବେଶ_sizes[0] = first.outputs;
    ପାଇଁ(i = 1; i < l->n; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = l->ନିବେଶ_layers[i];
        layer next = net->layers[index];
        l->outputs += next.outputs;
        l->ନିବେଶ_sizes[i] = next.outputs;
        ଯଦି(next.out_w == first.out_w && next.out_h == first.out_h){
            l->out_c += next.out_c;
        }ଅନ୍ୟଥା{
            printf("%d %d, %d %d\n", next.out_w, next.out_h, first.out_w, first.out_h);
            l->out_h = l->out_w = l->out_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*ଆକାର(ଦଶମିକ));
    l->output = realloc(l->output, l->outputs*l->batch*ଆକାର(ଦଶମିକ));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

ଶୂନ୍ୟ forward_route_layer(ଅଚଳ route_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = 0;
    ପାଇଁ(i = 0; i < l.n; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = l.ନିବେଶ_layers[i];
        ଦଶମିକ *ନିବେଶ = net.layers[index].output;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_size = l.ନିବେଶ_sizes[i];
        ପାଇଁ(j = 0; j < l.batch; ++j){
            copy_cpu(ନିବେଶ_size, ନିବେଶ + j*ନିବେଶ_size, 1, l.output + offset + j*l.outputs, 1);
        }
        offset += ନିବେଶ_size;
    }
}

ଶୂନ୍ୟ backward_route_layer(ଅଚଳ route_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = 0;
    ପାଇଁ(i = 0; i < l.n; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = l.ନିବେଶ_layers[i];
        ଦଶମିକ *delta = net.layers[index].delta;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_size = l.ନିବେଶ_sizes[i];
        ପାଇଁ(j = 0; j < l.batch; ++j){
            axpy_cpu(ନିବେଶ_size, 1, l.delta + offset + j*l.outputs, 1, delta + j*ନିବେଶ_size, 1);
        }
        offset += ନିବେଶ_size;
    }
}

#ifdef GPU
ଶୂନ୍ୟ forward_route_layer_gpu(ଅଚଳ route_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = 0;
    ପାଇଁ(i = 0; i < l.n; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = l.ନିବେଶ_layers[i];
        ଦଶମିକ *ନିବେଶ = net.layers[index].output_gpu;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_size = l.ନିବେଶ_sizes[i];
        ପାଇଁ(j = 0; j < l.batch; ++j){
            copy_gpu(ନିବେଶ_size, ନିବେଶ + j*ନିବେଶ_size, 1, l.output_gpu + offset + j*l.outputs, 1);
        }
        offset += ନିବେଶ_size;
    }
}

ଶୂନ୍ୟ backward_route_layer_gpu(ଅଚଳ route_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = 0;
    ପାଇଁ(i = 0; i < l.n; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = l.ନିବେଶ_layers[i];
        ଦଶମିକ *delta = net.layers[index].delta_gpu;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_size = l.ନିବେଶ_sizes[i];
        ପାଇଁ(j = 0; j < l.batch; ++j){
            axpy_gpu(ନିବେଶ_size, 1, l.delta_gpu + offset + j*l.outputs, 1, delta + j*ନିବେଶ_size, 1);
        }
        offset += ନିବେଶ_size;
    }
}
#endif
