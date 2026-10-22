#अन्तर्भाव "route_layer.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "blas.h"

#अन्तर्भाव <stdio.h>

route_layer make_route_layer(पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क *निवेश_layers, पूर्णाङ्क *निवेश_sizes)
{
    ख_लिखो(मानक_त्रुटि,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.निवेश_layers = निवेश_layers;
    l.निवेश_sizes = निवेश_sizes;
    पूर्णाङ्क i;
    पूर्णाङ्क outputs = 0;
    प्रति(i = 0; i < n; ++i){
        ख_लिखो(मानक_त्रुटि," %d", निवेश_layers[i]);
        outputs += निवेश_sizes[i];
    }
    ख_लिखो(मानक_त्रुटि, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  सुस्मृति(outputs*batch, परिमाण(भिन्न));
    l.output = सुस्मृति(outputs*batch, परिमाण(भिन्न));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #ifdef GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #endif
    प्रत्यागम l;
}

शून्य resize_route_layer(route_layer *l, network *net)
{
    पूर्णाङ्क i;
    layer first = net->layers[l->निवेश_layers[0]];
    l->out_w = first.out_w;
    l->out_h = first.out_h;
    l->out_c = first.out_c;
    l->outputs = first.outputs;
    l->निवेश_sizes[0] = first.outputs;
    प्रति(i = 1; i < l->n; ++i){
        पूर्णाङ्क index = l->निवेश_layers[i];
        layer next = net->layers[index];
        l->outputs += next.outputs;
        l->निवेश_sizes[i] = next.outputs;
        यदि(next.out_w == first.out_w && next.out_h == first.out_h){
            l->out_c += next.out_c;
        }अन्यथा{
            म_लिखो("%d %d, %d %d\n", next.out_w, next.out_h, first.out_w, first.out_h);
            l->out_h = l->out_w = l->out_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  पुनः_स्मृति(l->delta, l->outputs*l->batch*परिमाण(भिन्न));
    l->output = पुनः_स्मृति(l->output, l->outputs*l->batch*परिमाण(भिन्न));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

शून्य forward_route_layer(ध्रुव route_layer l, network net)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क offset = 0;
    प्रति(i = 0; i < l.n; ++i){
        पूर्णाङ्क index = l.निवेश_layers[i];
        भिन्न *निवेश = net.layers[index].output;
        पूर्णाङ्क निवेश_size = l.निवेश_sizes[i];
        प्रति(j = 0; j < l.batch; ++j){
            copy_cpu(निवेश_size, निवेश + j*निवेश_size, 1, l.output + offset + j*l.outputs, 1);
        }
        offset += निवेश_size;
    }
}

शून्य backward_route_layer(ध्रुव route_layer l, network net)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क offset = 0;
    प्रति(i = 0; i < l.n; ++i){
        पूर्णाङ्क index = l.निवेश_layers[i];
        भिन्न *delta = net.layers[index].delta;
        पूर्णाङ्क निवेश_size = l.निवेश_sizes[i];
        प्रति(j = 0; j < l.batch; ++j){
            axpy_cpu(निवेश_size, 1, l.delta + offset + j*l.outputs, 1, delta + j*निवेश_size, 1);
        }
        offset += निवेश_size;
    }
}

#ifdef GPU
शून्य forward_route_layer_gpu(ध्रुव route_layer l, network net)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क offset = 0;
    प्रति(i = 0; i < l.n; ++i){
        पूर्णाङ्क index = l.निवेश_layers[i];
        भिन्न *निवेश = net.layers[index].output_gpu;
        पूर्णाङ्क निवेश_size = l.निवेश_sizes[i];
        प्रति(j = 0; j < l.batch; ++j){
            copy_gpu(निवेश_size, निवेश + j*निवेश_size, 1, l.output_gpu + offset + j*l.outputs, 1);
        }
        offset += निवेश_size;
    }
}

शून्य backward_route_layer_gpu(ध्रुव route_layer l, network net)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क offset = 0;
    प्रति(i = 0; i < l.n; ++i){
        पूर्णाङ्क index = l.निवेश_layers[i];
        भिन्न *delta = net.layers[index].delta_gpu;
        पूर्णाङ्क निवेश_size = l.निवेश_sizes[i];
        प्रति(j = 0; j < l.batch; ++j){
            axpy_gpu(निवेश_size, 1, l.delta_gpu + offset + j*l.outputs, 1, delta + j*निवेश_size, 1);
        }
        offset += निवेश_size;
    }
}
#endif
