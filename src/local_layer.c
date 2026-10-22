#ਅਨ੍ਤਰ੍ਭਾਵ "local_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "im2col.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "col2im.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gemm.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

ਪੂਰਨ local_ਬਾਹਰਲਾ_ਉਚਾਈ(local_layer l)
{
    ਪੂਰਨ h = l.h;
    ਜੇ (!l.pad) h -= l.ਮਾਪ;
    ਨਹੀਂਤਾਂ h -= 1;
    ਵਾਪਸ h/l.stride + 1;
}

ਪੂਰਨ local_ਬਾਹਰਲਾ_ਚੌੜਾਈ(local_layer l)
{
    ਪੂਰਨ w = l.w;
    ਜੇ (!l.pad) w -= l.ਮਾਪ;
    ਨਹੀਂਤਾਂ w -= 1;
    ਵਾਪਸ w/l.stride + 1;
}

local_layer make_local_layer(ਪੂਰਨ batch, ਪੂਰਨ h, ਪੂਰਨ w, ਪੂਰਨ c, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ, ਪੂਰਨ stride, ਪੂਰਨ pad, ACTIVATION activation)
{
    ਪੂਰਨ i;
    local_layer l = {0};
    l.type = LOCAL;

    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.batch = batch;
    l.stride = stride;
    l.ਮਾਪ = ਮਾਪ;
    l.pad = pad;

    ਪੂਰਨ ਬਾਹਰਲਾ_h = local_ਬਾਹਰਲਾ_ਉਚਾਈ(l);
    ਪੂਰਨ ਬਾਹਰਲਾ_w = local_ਬਾਹਰਲਾ_ਚੌੜਾਈ(l);
    ਪੂਰਨ locations = ਬਾਹਰਲਾ_h*ਬਾਹਰਲਾ_w;
    l.ਬਾਹਰਲਾ_h = ਬਾਹਰਲਾ_h;
    l.ਬਾਹਰਲਾ_w = ਬਾਹਰਲਾ_w;
    l.ਬਾਹਰਲਾ_c = n;
    l.outputs = l.ਬਾਹਰਲਾ_h * l.ਬਾਹਰਲਾ_w * l.ਬਾਹਰਲਾ_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c*n*ਮਾਪ*ਮਾਪ*locations, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.ਭਾਰ_updates = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c*n*ਮਾਪ*ਮਾਪ*locations, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.biases = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.bias_updates = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    // float scale = 1./sqrt(size*size*c);
    ਦਸ਼ਮਲਵ scale = ਵਰਗਮੂਲ(2./(ਮਾਪ*ਮਾਪ*c));
    ਲਈ(i = 0; i < c*n*ਮਾਪ*ਮਾਪ; ++i) l.weights[i] = scale*rand_ਇਕਸਾਰ(-1,1);

    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.batch*ਬਾਹਰਲਾ_h * ਬਾਹਰਲਾ_w * n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.delta  = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.batch*ਬਾਹਰਲਾ_h * ਬਾਹਰਲਾ_w * n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.workspace_ਮਾਪ = ਬਾਹਰਲਾ_h*ਬਾਹਰਲਾ_w*ਮਾਪ*ਮਾਪ*c;
    
    l.forward = forward_local_layer;
    l.backward = backward_local_layer;
    l.update = update_local_layer;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_local_layer_gpu;
    l.backward_gpu = backward_local_layer_gpu;
    l.update_gpu = update_local_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, c*n*ਮਾਪ*ਮਾਪ*locations);
    l.ਭਾਰ_updates_gpu = cuda_make_array(l.ਭਾਰ_updates, c*n*ਮਾਪ*ਮਾਪ*locations);

    l.biases_gpu = cuda_make_array(l.biases, l.outputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, l.outputs);

    l.delta_gpu = cuda_make_array(l.delta, l.batch*ਬਾਹਰਲਾ_h*ਬਾਹਰਲਾ_w*n);
    l.output_gpu = cuda_make_array(l.output, l.batch*ਬਾਹਰਲਾ_h*ਬਾਹਰਲਾ_w*n);

#ਜੇ_ਅੰਤ
    l.activation = activation;

    fprintf(stderr, "Local Layer: %d x %d x %d image, %d filters -> %d x %d x %d image\n", h,w,c,n, ਬਾਹਰਲਾ_h, ਬਾਹਰਲਾ_w, n);

    ਵਾਪਸ l;
}

ਖਾਲੀ forward_local_layer(ਅਚਲ local_layer l, ਜਾਲ net)
{
    ਪੂਰਨ ਬਾਹਰਲਾ_h = local_ਬਾਹਰਲਾ_ਉਚਾਈ(l);
    ਪੂਰਨ ਬਾਹਰਲਾ_w = local_ਬਾਹਰਲਾ_ਚੌੜਾਈ(l);
    ਪੂਰਨ i, j;
    ਪੂਰਨ locations = ਬਾਹਰਲਾ_h * ਬਾਹਰਲਾ_w;

    ਲਈ(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    ਲਈ(i = 0; i < l.batch; ++i){
        ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼ = net.ਨਿਵੇਸ਼ + i*l.w*l.h*l.c;
        im2col_cpu(ਨਿਵੇਸ਼, l.c, l.h, l.w, 
                l.ਮਾਪ, l.stride, l.pad, net.workspace);
        ਦਸ਼ਮਲਵ *output = l.output + i*l.outputs;
        ਲਈ(j = 0; j < locations; ++j){
            ਦਸ਼ਮਲਵ *a = l.weights + j*l.ਮਾਪ*l.ਮਾਪ*l.c*l.n;
            ਦਸ਼ਮਲਵ *b = net.workspace + j;
            ਦਸ਼ਮਲਵ *c = output + j;

            ਪੂਰਨ m = l.n;
            ਪੂਰਨ n = 1;
            ਪੂਰਨ k = l.ਮਾਪ*l.ਮਾਪ*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ਖਾਲੀ backward_local_layer(local_layer l, ਜਾਲ net)
{
    ਪੂਰਨ i, j;
    ਪੂਰਨ locations = l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ਲਈ(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    ਲਈ(i = 0; i < l.batch; ++i){
        ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼ = net.ਨਿਵੇਸ਼ + i*l.w*l.h*l.c;
        im2col_cpu(ਨਿਵੇਸ਼, l.c, l.h, l.w, 
                l.ਮਾਪ, l.stride, l.pad, net.workspace);

        ਲਈ(j = 0; j < locations; ++j){ 
            ਦਸ਼ਮਲਵ *a = l.delta + i*l.outputs + j;
            ਦਸ਼ਮਲਵ *b = net.workspace + j;
            ਦਸ਼ਮਲਵ *c = l.ਭਾਰ_updates + j*l.ਮਾਪ*l.ਮਾਪ*l.c*l.n;
            ਪੂਰਨ m = l.n;
            ਪੂਰਨ n = l.ਮਾਪ*l.ਮਾਪ*l.c;
            ਪੂਰਨ k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        ਜੇ(net.delta){
            ਲਈ(j = 0; j < locations; ++j){ 
                ਦਸ਼ਮਲਵ *a = l.weights + j*l.ਮਾਪ*l.ਮਾਪ*l.c*l.n;
                ਦਸ਼ਮਲਵ *b = l.delta + i*l.outputs + j;
                ਦਸ਼ਮਲਵ *c = net.workspace + j;

                ਪੂਰਨ m = l.ਮਾਪ*l.ਮਾਪ*l.c;
                ਪੂਰਨ n = 1;
                ਪੂਰਨ k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.ਮਾਪ,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

ਖਾਲੀ update_local_layer(local_layer l, update_ਦਲੀਲਾਂ a)
{
    ਦਸ਼ਮਲਵ learning_ਦਰ = a.learning_ਦਰ*l.learning_ਦਰ_scale;
    ਦਸ਼ਮਲਵ momentum = a.momentum;
    ਦਸ਼ਮਲਵ decay = a.decay;
    ਪੂਰਨ batch = a.batch;

    ਪੂਰਨ locations = l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h;
    ਪੂਰਨ ਮਾਪ = l.ਮਾਪ*l.ਮਾਪ*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_ਦਰ/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(ਮਾਪ, -decay*batch, l.weights, 1, l.ਭਾਰ_updates, 1);
    axpy_cpu(ਮਾਪ, learning_ਦਰ/batch, l.ਭਾਰ_updates, 1, l.weights, 1);
    scal_cpu(ਮਾਪ, momentum, l.ਭਾਰ_updates, 1);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ forward_local_layer_gpu(ਅਚਲ local_layer l, ਜਾਲ net)
{
    ਪੂਰਨ ਬਾਹਰਲਾ_h = local_ਬਾਹਰਲਾ_ਉਚਾਈ(l);
    ਪੂਰਨ ਬਾਹਰਲਾ_w = local_ਬਾਹਰਲਾ_ਚੌੜਾਈ(l);
    ਪੂਰਨ i, j;
    ਪੂਰਨ locations = ਬਾਹਰਲਾ_h * ਬਾਹਰਲਾ_w;

    ਲਈ(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    ਲਈ(i = 0; i < l.batch; ++i){
        ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼ = net.ਨਿਵੇਸ਼_gpu + i*l.w*l.h*l.c;
        im2col_gpu(ਨਿਵੇਸ਼, l.c, l.h, l.w, 
                l.ਮਾਪ, l.stride, l.pad, net.workspace);
        ਦਸ਼ਮਲਵ *output = l.output_gpu + i*l.outputs;
        ਲਈ(j = 0; j < locations; ++j){
            ਦਸ਼ਮਲਵ *a = l.weights_gpu + j*l.ਮਾਪ*l.ਮਾਪ*l.c*l.n;
            ਦਸ਼ਮਲਵ *b = net.workspace + j;
            ਦਸ਼ਮਲਵ *c = output + j;

            ਪੂਰਨ m = l.n;
            ਪੂਰਨ n = 1;
            ਪੂਰਨ k = l.ਮਾਪ*l.ਮਾਪ*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ਖਾਲੀ backward_local_layer_gpu(local_layer l, ਜਾਲ net)
{
    ਪੂਰਨ i, j;
    ਪੂਰਨ locations = l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    ਲਈ(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    ਲਈ(i = 0; i < l.batch; ++i){
        ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼ = net.ਨਿਵੇਸ਼_gpu + i*l.w*l.h*l.c;
        im2col_gpu(ਨਿਵੇਸ਼, l.c, l.h, l.w, 
                l.ਮਾਪ, l.stride, l.pad, net.workspace);

        ਲਈ(j = 0; j < locations; ++j){ 
            ਦਸ਼ਮਲਵ *a = l.delta_gpu + i*l.outputs + j;
            ਦਸ਼ਮਲਵ *b = net.workspace + j;
            ਦਸ਼ਮਲਵ *c = l.ਭਾਰ_updates_gpu + j*l.ਮਾਪ*l.ਮਾਪ*l.c*l.n;
            ਪੂਰਨ m = l.n;
            ਪੂਰਨ n = l.ਮਾਪ*l.ਮਾਪ*l.c;
            ਪੂਰਨ k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        ਜੇ(net.delta_gpu){
            ਲਈ(j = 0; j < locations; ++j){ 
                ਦਸ਼ਮਲਵ *a = l.weights_gpu + j*l.ਮਾਪ*l.ਮਾਪ*l.c*l.n;
                ਦਸ਼ਮਲਵ *b = l.delta_gpu + i*l.outputs + j;
                ਦਸ਼ਮਲਵ *c = net.workspace + j;

                ਪੂਰਨ m = l.ਮਾਪ*l.ਮਾਪ*l.c;
                ਪੂਰਨ n = 1;
                ਪੂਰਨ k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.ਮਾਪ,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

ਖਾਲੀ update_local_layer_gpu(local_layer l, update_ਦਲੀਲਾਂ a)
{
    ਦਸ਼ਮਲਵ learning_ਦਰ = a.learning_ਦਰ*l.learning_ਦਰ_scale;
    ਦਸ਼ਮਲਵ momentum = a.momentum;
    ਦਸ਼ਮਲਵ decay = a.decay;
    ਪੂਰਨ batch = a.batch;

    ਪੂਰਨ locations = l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h;
    ਪੂਰਨ ਮਾਪ = l.ਮਾਪ*l.ਮਾਪ*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_ਦਰ/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(ਮਾਪ, -decay*batch, l.weights_gpu, 1, l.ਭਾਰ_updates_gpu, 1);
    axpy_gpu(ਮਾਪ, learning_ਦਰ/batch, l.ਭਾਰ_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(ਮਾਪ, momentum, l.ਭਾਰ_updates_gpu, 1);
}

ਖਾਲੀ pull_local_layer(local_layer l)
{
    ਪੂਰਨ locations = l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h;
    ਪੂਰਨ ਮਾਪ = l.ਮਾਪ*l.ਮਾਪ*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, ਮਾਪ);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

ਖਾਲੀ ਧੱਕੋ_local_layer(local_layer l)
{
    ਪੂਰਨ locations = l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h;
    ਪੂਰਨ ਮਾਪ = l.ਮਾਪ*l.ਮਾਪ*l.c*l.n*locations;
    cuda_ਧੱਕੋ_array(l.weights_gpu, l.weights, ਮਾਪ);
    cuda_ਧੱਕੋ_array(l.biases_gpu, l.biases, l.outputs);
}
#ਜੇ_ਅੰਤ
