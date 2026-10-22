#समावेश "route_layer.h"
#समावेश "cuda.h"
#समावेश "blas.h"

#समावेश <stdio.h>

route_layer make_route_layer(पूर्णांक batch, पूर्णांक n, पूर्णांक *निवेश_layers, पूर्णांक *निवेश_sizes)
{
    ख_लिखो(मानक_त्रुटि,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.निवेश_layers = निवेश_layers;
    l.निवेश_sizes = निवेश_sizes;
    पूर्णांक i;
    पूर्णांक outputs = 0;
    क्रम(i = 0; i < n; ++i){
        ख_लिखो(मानक_त्रुटि," %d", निवेश_layers[i]);
        outputs += निवेश_sizes[i];
    }
    ख_लिखो(मानक_त्रुटि, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  सुस्मृति(outputs*batch, माप(भग्न));
    l.output = सुस्मृति(outputs*batch, माप(भग्न));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #अगर_घोषित GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #पूर्ण_अगर
    वापस l;
}

व्योम resize_route_layer(route_layer *l, network *net)
{
    पूर्णांक i;
    layer first = net->layers[l->निवेश_layers[0]];
    l->out_w = first.out_w;
    l->out_h = first.out_h;
    l->out_c = first.out_c;
    l->outputs = first.outputs;
    l->निवेश_sizes[0] = first.outputs;
    क्रम(i = 1; i < l->n; ++i){
        पूर्णांक सूचकांक = l->निवेश_layers[i];
        layer अग्र = net->layers[सूचकांक];
        l->outputs += अग्र.outputs;
        l->निवेश_sizes[i] = अग्र.outputs;
        अगर(अग्र.out_w == first.out_w && अग्र.out_h == first.out_h){
            l->out_c += अग्र.out_c;
        }अन्यथा{
            म_लिखो("%d %d, %d %d\n", अग्र.out_w, अग्र.out_h, first.out_w, first.out_h);
            l->out_h = l->out_w = l->out_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  पुनः_स्मृति(l->delta, l->outputs*l->batch*माप(भग्न));
    l->output = पुनः_स्मृति(l->output, l->outputs*l->batch*माप(भग्न));

#अगर_घोषित GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#पूर्ण_अगर
    
}

व्योम forward_route_layer(स्थिर route_layer l, network net)
{
    पूर्णांक i, j;
    पूर्णांक offset = 0;
    क्रम(i = 0; i < l.n; ++i){
        पूर्णांक सूचकांक = l.निवेश_layers[i];
        भग्न *निवेश = net.layers[सूचकांक].output;
        पूर्णांक निवेश_आकार = l.निवेश_sizes[i];
        क्रम(j = 0; j < l.batch; ++j){
            copy_cpu(निवेश_आकार, निवेश + j*निवेश_आकार, 1, l.output + offset + j*l.outputs, 1);
        }
        offset += निवेश_आकार;
    }
}

व्योम backward_route_layer(स्थिर route_layer l, network net)
{
    पूर्णांक i, j;
    पूर्णांक offset = 0;
    क्रम(i = 0; i < l.n; ++i){
        पूर्णांक सूचकांक = l.निवेश_layers[i];
        भग्न *delta = net.layers[सूचकांक].delta;
        पूर्णांक निवेश_आकार = l.निवेश_sizes[i];
        क्रम(j = 0; j < l.batch; ++j){
            axpy_cpu(निवेश_आकार, 1, l.delta + offset + j*l.outputs, 1, delta + j*निवेश_आकार, 1);
        }
        offset += निवेश_आकार;
    }
}

#अगर_घोषित GPU
व्योम forward_route_layer_gpu(स्थिर route_layer l, network net)
{
    पूर्णांक i, j;
    पूर्णांक offset = 0;
    क्रम(i = 0; i < l.n; ++i){
        पूर्णांक सूचकांक = l.निवेश_layers[i];
        भग्न *निवेश = net.layers[सूचकांक].output_gpu;
        पूर्णांक निवेश_आकार = l.निवेश_sizes[i];
        क्रम(j = 0; j < l.batch; ++j){
            copy_gpu(निवेश_आकार, निवेश + j*निवेश_आकार, 1, l.output_gpu + offset + j*l.outputs, 1);
        }
        offset += निवेश_आकार;
    }
}

व्योम backward_route_layer_gpu(स्थिर route_layer l, network net)
{
    पूर्णांक i, j;
    पूर्णांक offset = 0;
    क्रम(i = 0; i < l.n; ++i){
        पूर्णांक सूचकांक = l.निवेश_layers[i];
        भग्न *delta = net.layers[सूचकांक].delta_gpu;
        पूर्णांक निवेश_आकार = l.निवेश_sizes[i];
        क्रम(j = 0; j < l.batch; ++j){
            axpy_gpu(निवेश_आकार, 1, l.delta_gpu + offset + j*l.outputs, 1, delta + j*निवेश_आकार, 1);
        }
        offset += निवेश_आकार;
    }
}
#पूर्ण_अगर
