#अन्तर्भाव "route_layer.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "blas.h"

#अन्तर्भाव <stdio.h>

route_layer make_route_layer(पूर्णांक batch, पूर्णांक n, पूर्णांक *आदान_layers, पूर्णांक *आदान_sizes)
{
    ख_लिखो(मानक_त्रुटि,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.आदान_layers = आदान_layers;
    l.आदान_sizes = आदान_sizes;
    पूर्णांक i;
    पूर्णांक outputs = 0;
    साठी(i = 0; i < n; ++i){
        ख_लिखो(मानक_त्रुटि," %d", आदान_layers[i]);
        outputs += आदान_sizes[i];
    }
    ख_लिखो(मानक_त्रुटि, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  सुस्मृति(outputs*batch, आकार(दशांश));
    l.output = सुस्मृति(outputs*batch, आकार(दशांश));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #ifdef GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #endif
    परत l;
}

शून्य resize_route_layer(route_layer *l, network *net)
{
    पूर्णांक i;
    layer first = net->layers[l->आदान_layers[0]];
    l->out_w = first.out_w;
    l->out_h = first.out_h;
    l->out_c = first.out_c;
    l->outputs = first.outputs;
    l->आदान_sizes[0] = first.outputs;
    साठी(i = 1; i < l->n; ++i){
        पूर्णांक index = l->आदान_layers[i];
        layer next = net->layers[index];
        l->outputs += next.outputs;
        l->आदान_sizes[i] = next.outputs;
        जर(next.out_w == first.out_w && next.out_h == first.out_h){
            l->out_c += next.out_c;
        }अन्यथा{
            म_लिखो("%d %d, %d %d\n", next.out_w, next.out_h, first.out_w, first.out_h);
            l->out_h = l->out_w = l->out_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  पुनः_स्मृति(l->delta, l->outputs*l->batch*आकार(दशांश));
    l->output = पुनः_स्मृति(l->output, l->outputs*l->batch*आकार(दशांश));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

शून्य forward_route_layer(स्थिरांक route_layer l, network net)
{
    पूर्णांक i, j;
    पूर्णांक offset = 0;
    साठी(i = 0; i < l.n; ++i){
        पूर्णांक index = l.आदान_layers[i];
        दशांश *आदान = net.layers[index].output;
        पूर्णांक आदान_size = l.आदान_sizes[i];
        साठी(j = 0; j < l.batch; ++j){
            copy_cpu(आदान_size, आदान + j*आदान_size, 1, l.output + offset + j*l.outputs, 1);
        }
        offset += आदान_size;
    }
}

शून्य backward_route_layer(स्थिरांक route_layer l, network net)
{
    पूर्णांक i, j;
    पूर्णांक offset = 0;
    साठी(i = 0; i < l.n; ++i){
        पूर्णांक index = l.आदान_layers[i];
        दशांश *delta = net.layers[index].delta;
        पूर्णांक आदान_size = l.आदान_sizes[i];
        साठी(j = 0; j < l.batch; ++j){
            axpy_cpu(आदान_size, 1, l.delta + offset + j*l.outputs, 1, delta + j*आदान_size, 1);
        }
        offset += आदान_size;
    }
}

#ifdef GPU
शून्य forward_route_layer_gpu(स्थिरांक route_layer l, network net)
{
    पूर्णांक i, j;
    पूर्णांक offset = 0;
    साठी(i = 0; i < l.n; ++i){
        पूर्णांक index = l.आदान_layers[i];
        दशांश *आदान = net.layers[index].output_gpu;
        पूर्णांक आदान_size = l.आदान_sizes[i];
        साठी(j = 0; j < l.batch; ++j){
            copy_gpu(आदान_size, आदान + j*आदान_size, 1, l.output_gpu + offset + j*l.outputs, 1);
        }
        offset += आदान_size;
    }
}

शून्य backward_route_layer_gpu(स्थिरांक route_layer l, network net)
{
    पूर्णांक i, j;
    पूर्णांक offset = 0;
    साठी(i = 0; i < l.n; ++i){
        पूर्णांक index = l.आदान_layers[i];
        दशांश *delta = net.layers[index].delta_gpu;
        पूर्णांक आदान_size = l.आदान_sizes[i];
        साठी(j = 0; j < l.batch; ++j){
            axpy_gpu(आदान_size, 1, l.delta_gpu + offset + j*l.outputs, 1, delta + j*आदान_size, 1);
        }
        offset += आदान_size;
    }
}
#endif
