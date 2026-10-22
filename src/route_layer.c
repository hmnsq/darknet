#अन्तर्भाव "route_layer.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "blas.h"

#अन्तर्भाव <stdio.h>

route_layer make_route_layer(पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क *निवेशः_layers, पूर्णाङ्क *निवेशः_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.निवेशः_layers = निवेशः_layers;
    l.निवेशः_sizes = निवेशः_sizes;
    पूर्णाङ्क i;
    पूर्णाङ्क outputs = 0;
    प्रति(i = 0; i < n; ++i){
        fprintf(stderr," %d", निवेशः_layers[i]);
        outputs += निवेशः_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  शून्यस्मृतिं_देहि(outputs*batch, परिमाण(भिन्न));
    l.output = शून्यस्मृतिं_देहि(outputs*batch, परिमाण(भिन्न));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #यदि_परिभाषितम् GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #यदि_अन्तः
    प्रत्यागम l;
}

शून्य resize_route_layer(route_layer *l, जालम् *net)
{
    पूर्णाङ्क i;
    layer first = net->layers[l->निवेशः_layers[0]];
    l->बहिर्गमनम्_w = first.बहिर्गमनम्_w;
    l->बहिर्गमनम्_h = first.बहिर्गमनम्_h;
    l->बहिर्गमनम्_c = first.बहिर्गमनम्_c;
    l->outputs = first.outputs;
    l->निवेशः_sizes[0] = first.outputs;
    प्रति(i = 1; i < l->n; ++i){
        पूर्णाङ्क सूचकांक = l->निवेशः_layers[i];
        layer अग्र = net->layers[सूचकांक];
        l->outputs += अग्र.outputs;
        l->निवेशः_sizes[i] = अग्र.outputs;
        यदि(अग्र.बहिर्गमनम्_w == first.बहिर्गमनम्_w && अग्र.बहिर्गमनम्_h == first.बहिर्गमनम्_h){
            l->बहिर्गमनम्_c += अग्र.बहिर्गमनम्_c;
        }अन्यथा{
            प्रारूप_लिख("%d %d, %d %d\n", अग्र.बहिर्गमनम्_w, अग्र.बहिर्गमनम्_h, first.बहिर्गमनम्_w, first.बहिर्गमनम्_h);
            l->बहिर्गमनम्_h = l->बहिर्गमनम्_w = l->बहिर्गमनम्_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*परिमाण(भिन्न));
    l->output = realloc(l->output, l->outputs*l->batch*परिमाण(भिन्न));

#यदि_परिभाषितम् GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#यदि_अन्तः
    
}

शून्य forward_route_layer(ध्रुव route_layer l, जालम् net)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क विस्थापनम् = 0;
    प्रति(i = 0; i < l.n; ++i){
        पूर्णाङ्क सूचकांक = l.निवेशः_layers[i];
        भिन्न *निवेशः = net.layers[सूचकांक].output;
        पूर्णाङ्क निवेशः_आकार = l.निवेशः_sizes[i];
        प्रति(j = 0; j < l.batch; ++j){
            copy_cpu(निवेशः_आकार, निवेशः + j*निवेशः_आकार, 1, l.output + विस्थापनम् + j*l.outputs, 1);
        }
        विस्थापनम् += निवेशः_आकार;
    }
}

शून्य backward_route_layer(ध्रुव route_layer l, जालम् net)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क विस्थापनम् = 0;
    प्रति(i = 0; i < l.n; ++i){
        पूर्णाङ्क सूचकांक = l.निवेशः_layers[i];
        भिन्न *delta = net.layers[सूचकांक].delta;
        पूर्णाङ्क निवेशः_आकार = l.निवेशः_sizes[i];
        प्रति(j = 0; j < l.batch; ++j){
            axpy_cpu(निवेशः_आकार, 1, l.delta + विस्थापनम् + j*l.outputs, 1, delta + j*निवेशः_आकार, 1);
        }
        विस्थापनम् += निवेशः_आकार;
    }
}

#यदि_परिभाषितम् GPU
शून्य forward_route_layer_gpu(ध्रुव route_layer l, जालम् net)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क विस्थापनम् = 0;
    प्रति(i = 0; i < l.n; ++i){
        पूर्णाङ्क सूचकांक = l.निवेशः_layers[i];
        भिन्न *निवेशः = net.layers[सूचकांक].output_gpu;
        पूर्णाङ्क निवेशः_आकार = l.निवेशः_sizes[i];
        प्रति(j = 0; j < l.batch; ++j){
            copy_gpu(निवेशः_आकार, निवेशः + j*निवेशः_आकार, 1, l.output_gpu + विस्थापनम् + j*l.outputs, 1);
        }
        विस्थापनम् += निवेशः_आकार;
    }
}

शून्य backward_route_layer_gpu(ध्रुव route_layer l, जालम् net)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क विस्थापनम् = 0;
    प्रति(i = 0; i < l.n; ++i){
        पूर्णाङ्क सूचकांक = l.निवेशः_layers[i];
        भिन्न *delta = net.layers[सूचकांक].delta_gpu;
        पूर्णाङ्क निवेशः_आकार = l.निवेशः_sizes[i];
        प्रति(j = 0; j < l.batch; ++j){
            axpy_gpu(निवेशः_आकार, 1, l.delta_gpu + विस्थापनम् + j*l.outputs, 1, delta + j*निवेशः_आकार, 1);
        }
        विस्थापनम् += निवेशः_आकार;
    }
}
#यदि_अन्तः
