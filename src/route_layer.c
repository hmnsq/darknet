#ಅನ್ತರ್ಭಾವ "route_layer.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "blas.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>

route_layer make_route_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ *ಒಳಹರಿವು_layers, ಪೂರ್ಣಾಙ್ಕ *ಒಳಹರಿವು_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.ಒಳಹರಿವು_layers = ಒಳಹರಿವು_layers;
    l.ಒಳಹರಿವು_sizes = ಒಳಹರಿವು_sizes;
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ outputs = 0;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        fprintf(stderr," %d", ಒಳಹರಿವು_layers[i]);
        outputs += ಒಳಹರಿವು_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  calloc(outputs*batch, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.output = calloc(outputs*batch, ಪರಿಮಾಣ(ಭಿನ್ನ));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #ifdef GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #endif
    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ resize_route_layer(route_layer *l, network *net)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    layer first = net->layers[l->ಒಳಹರಿವು_layers[0]];
    l->out_w = first.out_w;
    l->out_h = first.out_h;
    l->out_c = first.out_c;
    l->outputs = first.outputs;
    l->ಒಳಹರಿವು_sizes[0] = first.outputs;
    ಪ್ರತಿ(i = 1; i < l->n; ++i){
        ಪೂರ್ಣಾಙ್ಕ index = l->ಒಳಹರಿವು_layers[i];
        layer next = net->layers[index];
        l->outputs += next.outputs;
        l->ಒಳಹರಿವು_sizes[i] = next.outputs;
        ಯದಿ(next.out_w == first.out_w && next.out_h == first.out_h){
            l->out_c += next.out_c;
        }ಅನ್ಯಥಾ{
            printf("%d %d, %d %d\n", next.out_w, next.out_h, first.out_w, first.out_h);
            l->out_h = l->out_w = l->out_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*ಪರಿಮಾಣ(ಭಿನ್ನ));
    l->output = realloc(l->output, l->outputs*l->batch*ಪರಿಮಾಣ(ಭಿನ್ನ));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

ಶೂನ್ಯ forward_route_layer(ಧ್ರುವ route_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪೂರ್ಣಾಙ್ಕ offset = 0;
    ಪ್ರತಿ(i = 0; i < l.n; ++i){
        ಪೂರ್ಣಾಙ್ಕ index = l.ಒಳಹರಿವು_layers[i];
        ಭಿನ್ನ *ಒಳಹರಿವು = net.layers[index].output;
        ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_size = l.ಒಳಹರಿವು_sizes[i];
        ಪ್ರತಿ(j = 0; j < l.batch; ++j){
            copy_cpu(ಒಳಹರಿವು_size, ಒಳಹರಿವು + j*ಒಳಹರಿವು_size, 1, l.output + offset + j*l.outputs, 1);
        }
        offset += ಒಳಹರಿವು_size;
    }
}

ಶೂನ್ಯ backward_route_layer(ಧ್ರುವ route_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪೂರ್ಣಾಙ್ಕ offset = 0;
    ಪ್ರತಿ(i = 0; i < l.n; ++i){
        ಪೂರ್ಣಾಙ್ಕ index = l.ಒಳಹರಿವು_layers[i];
        ಭಿನ್ನ *delta = net.layers[index].delta;
        ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_size = l.ಒಳಹರಿವು_sizes[i];
        ಪ್ರತಿ(j = 0; j < l.batch; ++j){
            axpy_cpu(ಒಳಹರಿವು_size, 1, l.delta + offset + j*l.outputs, 1, delta + j*ಒಳಹರಿವು_size, 1);
        }
        offset += ಒಳಹರಿವು_size;
    }
}

#ifdef GPU
ಶೂನ್ಯ forward_route_layer_gpu(ಧ್ರುವ route_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪೂರ್ಣಾಙ್ಕ offset = 0;
    ಪ್ರತಿ(i = 0; i < l.n; ++i){
        ಪೂರ್ಣಾಙ್ಕ index = l.ಒಳಹರಿವು_layers[i];
        ಭಿನ್ನ *ಒಳಹರಿವು = net.layers[index].output_gpu;
        ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_size = l.ಒಳಹರಿವು_sizes[i];
        ಪ್ರತಿ(j = 0; j < l.batch; ++j){
            copy_gpu(ಒಳಹರಿವು_size, ಒಳಹರಿವು + j*ಒಳಹರಿವು_size, 1, l.output_gpu + offset + j*l.outputs, 1);
        }
        offset += ಒಳಹರಿವು_size;
    }
}

ಶೂನ್ಯ backward_route_layer_gpu(ಧ್ರುವ route_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪೂರ್ಣಾಙ್ಕ offset = 0;
    ಪ್ರತಿ(i = 0; i < l.n; ++i){
        ಪೂರ್ಣಾಙ್ಕ index = l.ಒಳಹರಿವು_layers[i];
        ಭಿನ್ನ *delta = net.layers[index].delta_gpu;
        ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_size = l.ಒಳಹರಿವು_sizes[i];
        ಪ್ರತಿ(j = 0; j < l.batch; ++j){
            axpy_gpu(ಒಳಹರಿವು_size, 1, l.delta_gpu + offset + j*l.outputs, 1, delta + j*ಒಳಹರಿವು_size, 1);
        }
        offset += ಒಳಹರಿವು_size;
    }
}
#endif
