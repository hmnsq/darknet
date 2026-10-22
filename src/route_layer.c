#ਅਨ੍ਤਰ੍ਭਾਵ "route_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

route_layer make_route_layer(ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ *ਨਿਵੇਸ਼_layers, ਪੂਰਨ *ਨਿਵੇਸ਼_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.ਨਿਵੇਸ਼_layers = ਨਿਵੇਸ਼_layers;
    l.ਨਿਵੇਸ਼_sizes = ਨਿਵੇਸ਼_sizes;
    ਪੂਰਨ i;
    ਪੂਰਨ outputs = 0;
    ਲਈ(i = 0; i < n; ++i){
        fprintf(stderr," %d", ਨਿਵੇਸ਼_layers[i]);
        outputs += ਨਿਵੇਸ਼_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #ਜੇ_ਅੰਤ
    ਵਾਪਸ l;
}

ਖਾਲੀ resize_route_layer(route_layer *l, ਜਾਲ *net)
{
    ਪੂਰਨ i;
    layer ਪਹਿਲਾ = net->layers[l->ਨਿਵੇਸ਼_layers[0]];
    l->ਬਾਹਰਲਾ_w = ਪਹਿਲਾ.ਬਾਹਰਲਾ_w;
    l->ਬਾਹਰਲਾ_h = ਪਹਿਲਾ.ਬਾਹਰਲਾ_h;
    l->ਬਾਹਰਲਾ_c = ਪਹਿਲਾ.ਬਾਹਰਲਾ_c;
    l->outputs = ਪਹਿਲਾ.outputs;
    l->ਨਿਵੇਸ਼_sizes[0] = ਪਹਿਲਾ.outputs;
    ਲਈ(i = 1; i < l->n; ++i){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = l->ਨਿਵੇਸ਼_layers[i];
        layer ਅਗਲਾ = net->layers[ਸੂਚਕਾਂਕ];
        l->outputs += ਅਗਲਾ.outputs;
        l->ਨਿਵੇਸ਼_sizes[i] = ਅਗਲਾ.outputs;
        ਜੇ(ਅਗਲਾ.ਬਾਹਰਲਾ_w == ਪਹਿਲਾ.ਬਾਹਰਲਾ_w && ਅਗਲਾ.ਬਾਹਰਲਾ_h == ਪਹਿਲਾ.ਬਾਹਰਲਾ_h){
            l->ਬਾਹਰਲਾ_c += ਅਗਲਾ.ਬਾਹਰਲਾ_c;
        }ਨਹੀਂਤਾਂ{
            ਲਿਖੋ("%d %d, %d %d\n", ਅਗਲਾ.ਬਾਹਰਲਾ_w, ਅਗਲਾ.ਬਾਹਰਲਾ_h, ਪਹਿਲਾ.ਬਾਹਰਲਾ_w, ਪਹਿਲਾ.ਬਾਹਰਲਾ_h);
            l->ਬਾਹਰਲਾ_h = l->ਬਾਹਰਲਾ_w = l->ਬਾਹਰਲਾ_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l->output = realloc(l->output, l->outputs*l->batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#ਜੇ_ਅੰਤ
    
}

ਖਾਲੀ forward_route_layer(ਅਚਲ route_layer l, ਜਾਲ net)
{
    ਪੂਰਨ i, j;
    ਪੂਰਨ ਵਿੱਥ = 0;
    ਲਈ(i = 0; i < l.n; ++i){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = l.ਨਿਵੇਸ਼_layers[i];
        ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼ = net.layers[ਸੂਚਕਾਂਕ].output;
        ਪੂਰਨ ਨਿਵੇਸ਼_ਮਾਪ = l.ਨਿਵੇਸ਼_sizes[i];
        ਲਈ(j = 0; j < l.batch; ++j){
            copy_cpu(ਨਿਵੇਸ਼_ਮਾਪ, ਨਿਵੇਸ਼ + j*ਨਿਵੇਸ਼_ਮਾਪ, 1, l.output + ਵਿੱਥ + j*l.outputs, 1);
        }
        ਵਿੱਥ += ਨਿਵੇਸ਼_ਮਾਪ;
    }
}

ਖਾਲੀ backward_route_layer(ਅਚਲ route_layer l, ਜਾਲ net)
{
    ਪੂਰਨ i, j;
    ਪੂਰਨ ਵਿੱਥ = 0;
    ਲਈ(i = 0; i < l.n; ++i){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = l.ਨਿਵੇਸ਼_layers[i];
        ਦਸ਼ਮਲਵ *delta = net.layers[ਸੂਚਕਾਂਕ].delta;
        ਪੂਰਨ ਨਿਵੇਸ਼_ਮਾਪ = l.ਨਿਵੇਸ਼_sizes[i];
        ਲਈ(j = 0; j < l.batch; ++j){
            axpy_cpu(ਨਿਵੇਸ਼_ਮਾਪ, 1, l.delta + ਵਿੱਥ + j*l.outputs, 1, delta + j*ਨਿਵੇਸ਼_ਮਾਪ, 1);
        }
        ਵਿੱਥ += ਨਿਵੇਸ਼_ਮਾਪ;
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_route_layer_gpu(ਅਚਲ route_layer l, ਜਾਲ net)
{
    ਪੂਰਨ i, j;
    ਪੂਰਨ ਵਿੱਥ = 0;
    ਲਈ(i = 0; i < l.n; ++i){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = l.ਨਿਵੇਸ਼_layers[i];
        ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼ = net.layers[ਸੂਚਕਾਂਕ].output_gpu;
        ਪੂਰਨ ਨਿਵੇਸ਼_ਮਾਪ = l.ਨਿਵੇਸ਼_sizes[i];
        ਲਈ(j = 0; j < l.batch; ++j){
            copy_gpu(ਨਿਵੇਸ਼_ਮਾਪ, ਨਿਵੇਸ਼ + j*ਨਿਵੇਸ਼_ਮਾਪ, 1, l.output_gpu + ਵਿੱਥ + j*l.outputs, 1);
        }
        ਵਿੱਥ += ਨਿਵੇਸ਼_ਮਾਪ;
    }
}

ਖਾਲੀ backward_route_layer_gpu(ਅਚਲ route_layer l, ਜਾਲ net)
{
    ਪੂਰਨ i, j;
    ਪੂਰਨ ਵਿੱਥ = 0;
    ਲਈ(i = 0; i < l.n; ++i){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = l.ਨਿਵੇਸ਼_layers[i];
        ਦਸ਼ਮਲਵ *delta = net.layers[ਸੂਚਕਾਂਕ].delta_gpu;
        ਪੂਰਨ ਨਿਵੇਸ਼_ਮਾਪ = l.ਨਿਵੇਸ਼_sizes[i];
        ਲਈ(j = 0; j < l.batch; ++j){
            axpy_gpu(ਨਿਵੇਸ਼_ਮਾਪ, 1, l.delta_gpu + ਵਿੱਥ + j*l.outputs, 1, delta + j*ਨਿਵੇਸ਼_ਮਾਪ, 1);
        }
        ਵਿੱਥ += ਨਿਵੇਸ਼_ਮਾਪ;
    }
}
#ਜੇ_ਅੰਤ
