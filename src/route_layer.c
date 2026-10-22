#ಅನ್ತರ್ಭಾವ "route_layer.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "blas.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>

route_layer make_route_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ *ಒಳಹರಿವು_layers, ಪೂರ್ಣಾಂಕ *ಒಳಹರಿವು_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.ಒಳಹರಿವು_layers = ಒಳಹರಿವು_layers;
    l.ಒಳಹರಿವು_sizes = ಒಳಹರಿವು_sizes;
    ಪೂರ್ಣಾಂಕ i;
    ಪೂರ್ಣಾಂಕ outputs = 0;
    ಗಾಗಿ(i = 0; i < n; ++i){
        fprintf(stderr," %d", ಒಳಹರಿವು_layers[i]);
        outputs += ಒಳಹರಿವು_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ resize_route_layer(route_layer *l, ಜಾಲ *net)
{
    ಪೂರ್ಣಾಂಕ i;
    layer ಮೊದಲ = net->layers[l->ಒಳಹರಿವು_layers[0]];
    l->ಹೊರಹೋಗು_w = ಮೊದಲ.ಹೊರಹೋಗು_w;
    l->ಹೊರಹೋಗು_h = ಮೊದಲ.ಹೊರಹೋಗು_h;
    l->ಹೊರಹೋಗು_c = ಮೊದಲ.ಹೊರಹೋಗು_c;
    l->outputs = ಮೊದಲ.outputs;
    l->ಒಳಹರಿವು_sizes[0] = ಮೊದಲ.outputs;
    ಗಾಗಿ(i = 1; i < l->n; ++i){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = l->ಒಳಹರಿವು_layers[i];
        layer ಮುಂದಿನ = net->layers[ಸೂಚ್ಯಂಕ];
        l->outputs += ಮುಂದಿನ.outputs;
        l->ಒಳಹರಿವು_sizes[i] = ಮುಂದಿನ.outputs;
        ಆದರೆ(ಮುಂದಿನ.ಹೊರಹೋಗು_w == ಮೊದಲ.ಹೊರಹೋಗು_w && ಮುಂದಿನ.ಹೊರಹೋಗು_h == ಮೊದಲ.ಹೊರಹೋಗು_h){
            l->ಹೊರಹೋಗು_c += ಮುಂದಿನ.ಹೊರಹೋಗು_c;
        }ಇಲ್ಲದಿದ್ದರೆ{
            ಬರೆ("%d %d, %d %d\n", ಮುಂದಿನ.ಹೊರಹೋಗು_w, ಮುಂದಿನ.ಹೊರಹೋಗು_h, ಮೊದಲ.ಹೊರಹೋಗು_w, ಮೊದಲ.ಹೊರಹೋಗು_h);
            l->ಹೊರಹೋಗು_h = l->ಹೊರಹೋಗು_w = l->ಹೊರಹೋಗು_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*ಗಾತ್ರ(ದಶಮಾಂಶ));
    l->output = realloc(l->output, l->outputs*l->batch*ಗಾತ್ರ(ದಶಮಾಂಶ));

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#ಆದರೆ_ಅಂತ್ಯ
    
}

ಶೂನ್ಯ forward_route_layer(ಸ್ಥಿರಾಂಕ route_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = 0;
    ಗಾಗಿ(i = 0; i < l.n; ++i){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = l.ಒಳಹರಿವು_layers[i];
        ದಶಮಾಂಶ *ಒಳಹರಿವು = net.layers[ಸೂಚ್ಯಂಕ].output;
        ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_ಗಾತ್ರ = l.ಒಳಹರಿವು_sizes[i];
        ಗಾಗಿ(j = 0; j < l.batch; ++j){
            copy_cpu(ಒಳಹರಿವು_ಗಾತ್ರ, ಒಳಹರಿವು + j*ಒಳಹರಿವು_ಗಾತ್ರ, 1, l.output + ಸ್ಥಾನಾಂತರ + j*l.outputs, 1);
        }
        ಸ್ಥಾನಾಂತರ += ಒಳಹರಿವು_ಗಾತ್ರ;
    }
}

ಶೂನ್ಯ backward_route_layer(ಸ್ಥಿರಾಂಕ route_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = 0;
    ಗಾಗಿ(i = 0; i < l.n; ++i){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = l.ಒಳಹರಿವು_layers[i];
        ದಶಮಾಂಶ *delta = net.layers[ಸೂಚ್ಯಂಕ].delta;
        ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_ಗಾತ್ರ = l.ಒಳಹರಿವು_sizes[i];
        ಗಾಗಿ(j = 0; j < l.batch; ++j){
            axpy_cpu(ಒಳಹರಿವು_ಗಾತ್ರ, 1, l.delta + ಸ್ಥಾನಾಂತರ + j*l.outputs, 1, delta + j*ಒಳಹರಿವು_ಗಾತ್ರ, 1);
        }
        ಸ್ಥಾನಾಂತರ += ಒಳಹರಿವು_ಗಾತ್ರ;
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_route_layer_gpu(ಸ್ಥಿರಾಂಕ route_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = 0;
    ಗಾಗಿ(i = 0; i < l.n; ++i){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = l.ಒಳಹರಿವು_layers[i];
        ದಶಮಾಂಶ *ಒಳಹರಿವು = net.layers[ಸೂಚ್ಯಂಕ].output_gpu;
        ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_ಗಾತ್ರ = l.ಒಳಹರಿವು_sizes[i];
        ಗಾಗಿ(j = 0; j < l.batch; ++j){
            copy_gpu(ಒಳಹರಿವು_ಗಾತ್ರ, ಒಳಹರಿವು + j*ಒಳಹರಿವು_ಗಾತ್ರ, 1, l.output_gpu + ಸ್ಥಾನಾಂತರ + j*l.outputs, 1);
        }
        ಸ್ಥಾನಾಂತರ += ಒಳಹರಿವು_ಗಾತ್ರ;
    }
}

ಶೂನ್ಯ backward_route_layer_gpu(ಸ್ಥಿರಾಂಕ route_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = 0;
    ಗಾಗಿ(i = 0; i < l.n; ++i){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = l.ಒಳಹರಿವು_layers[i];
        ದಶಮಾಂಶ *delta = net.layers[ಸೂಚ್ಯಂಕ].delta_gpu;
        ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_ಗಾತ್ರ = l.ಒಳಹರಿವು_sizes[i];
        ಗಾಗಿ(j = 0; j < l.batch; ++j){
            axpy_gpu(ಒಳಹರಿವು_ಗಾತ್ರ, 1, l.delta_gpu + ಸ್ಥಾನಾಂತರ + j*l.outputs, 1, delta + j*ಒಳಹರಿವು_ಗಾತ್ರ, 1);
        }
        ಸ್ಥಾನಾಂತರ += ಒಳಹರಿವು_ಗಾತ್ರ;
    }
}
#ಆದರೆ_ಅಂತ್ಯ
