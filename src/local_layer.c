#അന്തര്ഭാവ "local_layer.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "im2col.h"
#അന്തര്ഭാവ "col2im.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "gemm.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <time.h>

പൂർണ്ണസംഖ്യ local_out_height(local_layer l)
{
    പൂർണ്ണസംഖ്യ h = l.h;
    എങ്കിൽ (!l.pad) h -= l.size;
    അല്ലെങ്കിൽ h -= 1;
    തിരികെ h/l.stride + 1;
}

പൂർണ്ണസംഖ്യ local_out_width(local_layer l)
{
    പൂർണ്ണസംഖ്യ w = l.w;
    എങ്കിൽ (!l.pad) w -= l.size;
    അല്ലെങ്കിൽ w -= 1;
    തിരികെ w/l.stride + 1;
}

local_layer make_local_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ pad, ACTIVATION activation)
{
    പൂർണ്ണസംഖ്യ i;
    local_layer l = {0};
    l.type = LOCAL;

    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.batch = batch;
    l.stride = stride;
    l.size = size;
    l.pad = pad;

    പൂർണ്ണസംഖ്യ out_h = local_out_height(l);
    പൂർണ്ണസംഖ്യ out_w = local_out_width(l);
    പൂർണ്ണസംഖ്യ locations = out_h*out_w;
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = calloc(c*n*size*size*locations, വലുപ്പം(ദശാംശം));
    l.weight_updates = calloc(c*n*size*size*locations, വലുപ്പം(ദശാംശം));

    l.biases = calloc(l.outputs, വലുപ്പം(ദശാംശം));
    l.bias_updates = calloc(l.outputs, വലുപ്പം(ദശാംശം));

    // float scale = 1./sqrt(size*size*c);
    ദശാംശം scale = sqrt(2./(size*size*c));
    വേണ്ടി(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1,1);

    l.output = calloc(l.batch*out_h * out_w * n, വലുപ്പം(ദശാംശം));
    l.delta  = calloc(l.batch*out_h * out_w * n, വലുപ്പം(ദശാംശം));

    l.workspace_size = out_h*out_w*size*size*c;
    
    l.forward = forward_local_layer;
    l.backward = backward_local_layer;
    l.update = update_local_layer;

#ifdef GPU
    l.forward_gpu = forward_local_layer_gpu;
    l.backward_gpu = backward_local_layer_gpu;
    l.update_gpu = update_local_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, c*n*size*size*locations);
    l.weight_updates_gpu = cuda_make_array(l.weight_updates, c*n*size*size*locations);

    l.biases_gpu = cuda_make_array(l.biases, l.outputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, l.outputs);

    l.delta_gpu = cuda_make_array(l.delta, l.batch*out_h*out_w*n);
    l.output_gpu = cuda_make_array(l.output, l.batch*out_h*out_w*n);

#endif
    l.activation = activation;

    fprintf(stderr, "Local Layer: %d x %d x %d image, %d filters -> %d x %d x %d image\n", h,w,c,n, out_h, out_w, n);

    തിരികെ l;
}

ശൂന്യം forward_local_layer(സ്ഥിരം local_layer l, network net)
{
    പൂർണ്ണസംഖ്യ out_h = local_out_height(l);
    പൂർണ്ണസംഖ്യ out_w = local_out_width(l);
    പൂർണ്ണസംഖ്യ i, j;
    പൂർണ്ണസംഖ്യ locations = out_h * out_w;

    വേണ്ടി(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    വേണ്ടി(i = 0; i < l.batch; ++i){
        ദശാംശം *നിവേശം = net.നിവേശം + i*l.w*l.h*l.c;
        im2col_cpu(നിവേശം, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        ദശാംശം *output = l.output + i*l.outputs;
        വേണ്ടി(j = 0; j < locations; ++j){
            ദശാംശം *a = l.weights + j*l.size*l.size*l.c*l.n;
            ദശാംശം *b = net.workspace + j;
            ദശാംശം *c = output + j;

            പൂർണ്ണസംഖ്യ m = l.n;
            പൂർണ്ണസംഖ്യ n = 1;
            പൂർണ്ണസംഖ്യ k = l.size*l.size*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ശൂന്യം backward_local_layer(local_layer l, network net)
{
    പൂർണ്ണസംഖ്യ i, j;
    പൂർണ്ണസംഖ്യ locations = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    വേണ്ടി(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    വേണ്ടി(i = 0; i < l.batch; ++i){
        ദശാംശം *നിവേശം = net.നിവേശം + i*l.w*l.h*l.c;
        im2col_cpu(നിവേശം, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        വേണ്ടി(j = 0; j < locations; ++j){ 
            ദശാംശം *a = l.delta + i*l.outputs + j;
            ദശാംശം *b = net.workspace + j;
            ദശാംശം *c = l.weight_updates + j*l.size*l.size*l.c*l.n;
            പൂർണ്ണസംഖ്യ m = l.n;
            പൂർണ്ണസംഖ്യ n = l.size*l.size*l.c;
            പൂർണ്ണസംഖ്യ k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        എങ്കിൽ(net.delta){
            വേണ്ടി(j = 0; j < locations; ++j){ 
                ദശാംശം *a = l.weights + j*l.size*l.size*l.c*l.n;
                ദശാംശം *b = l.delta + i*l.outputs + j;
                ദശാംശം *c = net.workspace + j;

                പൂർണ്ണസംഖ്യ m = l.size*l.size*l.c;
                പൂർണ്ണസംഖ്യ n = 1;
                പൂർണ്ണസംഖ്യ k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

ശൂന്യം update_local_layer(local_layer l, update_args a)
{
    ദശാംശം learning_rate = a.learning_rate*l.learning_rate_scale;
    ദശാംശം momentum = a.momentum;
    ദശാംശം decay = a.decay;
    പൂർണ്ണസംഖ്യ batch = a.batch;

    പൂർണ്ണസംഖ്യ locations = l.out_w*l.out_h;
    പൂർണ്ണസംഖ്യ size = l.size*l.size*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(size, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(size, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(size, momentum, l.weight_updates, 1);
}

#ifdef GPU

ശൂന്യം forward_local_layer_gpu(സ്ഥിരം local_layer l, network net)
{
    പൂർണ്ണസംഖ്യ out_h = local_out_height(l);
    പൂർണ്ണസംഖ്യ out_w = local_out_width(l);
    പൂർണ്ണസംഖ്യ i, j;
    പൂർണ്ണസംഖ്യ locations = out_h * out_w;

    വേണ്ടി(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    വേണ്ടി(i = 0; i < l.batch; ++i){
        ദശാംശം *നിവേശം = net.നിവേശം_gpu + i*l.w*l.h*l.c;
        im2col_gpu(നിവേശം, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        ദശാംശം *output = l.output_gpu + i*l.outputs;
        വേണ്ടി(j = 0; j < locations; ++j){
            ദശാംശം *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
            ദശാംശം *b = net.workspace + j;
            ദശാംശം *c = output + j;

            പൂർണ്ണസംഖ്യ m = l.n;
            പൂർണ്ണസംഖ്യ n = 1;
            പൂർണ്ണസംഖ്യ k = l.size*l.size*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ശൂന്യം backward_local_layer_gpu(local_layer l, network net)
{
    പൂർണ്ണസംഖ്യ i, j;
    പൂർണ്ണസംഖ്യ locations = l.out_w*l.out_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    വേണ്ടി(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    വേണ്ടി(i = 0; i < l.batch; ++i){
        ദശാംശം *നിവേശം = net.നിവേശം_gpu + i*l.w*l.h*l.c;
        im2col_gpu(നിവേശം, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        വേണ്ടി(j = 0; j < locations; ++j){ 
            ദശാംശം *a = l.delta_gpu + i*l.outputs + j;
            ദശാംശം *b = net.workspace + j;
            ദശാംശം *c = l.weight_updates_gpu + j*l.size*l.size*l.c*l.n;
            പൂർണ്ണസംഖ്യ m = l.n;
            പൂർണ്ണസംഖ്യ n = l.size*l.size*l.c;
            പൂർണ്ണസംഖ്യ k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        എങ്കിൽ(net.delta_gpu){
            വേണ്ടി(j = 0; j < locations; ++j){ 
                ദശാംശം *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
                ദശാംശം *b = l.delta_gpu + i*l.outputs + j;
                ദശാംശം *c = net.workspace + j;

                പൂർണ്ണസംഖ്യ m = l.size*l.size*l.c;
                പൂർണ്ണസംഖ്യ n = 1;
                പൂർണ്ണസംഖ്യ k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

ശൂന്യം update_local_layer_gpu(local_layer l, update_args a)
{
    ദശാംശം learning_rate = a.learning_rate*l.learning_rate_scale;
    ദശാംശം momentum = a.momentum;
    ദശാംശം decay = a.decay;
    പൂർണ്ണസംഖ്യ batch = a.batch;

    പൂർണ്ണസംഖ്യ locations = l.out_w*l.out_h;
    പൂർണ്ണസംഖ്യ size = l.size*l.size*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(size, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
    axpy_gpu(size, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(size, momentum, l.weight_updates_gpu, 1);
}

ശൂന്യം pull_local_layer(local_layer l)
{
    പൂർണ്ണസംഖ്യ locations = l.out_w*l.out_h;
    പൂർണ്ണസംഖ്യ size = l.size*l.size*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, size);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

ശൂന്യം തള്ളുക_local_layer(local_layer l)
{
    പൂർണ്ണസംഖ്യ locations = l.out_w*l.out_h;
    പൂർണ്ണസംഖ്യ size = l.size*l.size*l.c*l.n*locations;
    cuda_തള്ളുക_array(l.weights_gpu, l.weights, size);
    cuda_തള്ളുക_array(l.biases_gpu, l.biases, l.outputs);
}
#endif
