#ଅନ୍ତର୍ଭାଵ "local_layer.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "im2col.h"
#ଅନ୍ତର୍ଭାଵ "col2im.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ "gemm.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

ପୂର୍ଣ୍ଣସଂଖ୍ୟା local_out_height(local_layer l)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = l.h;
    ଯଦି (!l.pad) h -= l.size;
    ଅନ୍ୟଥା h -= 1;
    ଫେରନ୍ତୁ h/l.stride + 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା local_out_width(local_layer l)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = l.w;
    ଯଦି (!l.pad) w -= l.size;
    ଅନ୍ୟଥା w -= 1;
    ଫେରନ୍ତୁ w/l.stride + 1;
}

local_layer make_local_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pad, ACTIVATION activation)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
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

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_h = local_out_height(l);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_w = local_out_width(l);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା locations = out_h*out_w;
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = calloc(c*n*size*size*locations, ଆକାର(ଦଶମିକ));
    l.weight_updates = calloc(c*n*size*size*locations, ଆକାର(ଦଶମିକ));

    l.biases = calloc(l.outputs, ଆକାର(ଦଶମିକ));
    l.bias_updates = calloc(l.outputs, ଆକାର(ଦଶମିକ));

    // float scale = 1./sqrt(size*size*c);
    ଦଶମିକ scale = sqrt(2./(size*size*c));
    ପାଇଁ(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1,1);

    l.output = calloc(l.batch*out_h * out_w * n, ଆକାର(ଦଶମିକ));
    l.delta  = calloc(l.batch*out_h * out_w * n, ଆକାର(ଦଶମିକ));

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

    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ forward_local_layer(ଅଚଳ local_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_h = local_out_height(l);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_w = local_out_width(l);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା locations = out_h * out_w;

    ପାଇଁ(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    ପାଇଁ(i = 0; i < l.batch; ++i){
        ଦଶମିକ *ନିବେଶ = net.ନିବେଶ + i*l.w*l.h*l.c;
        im2col_cpu(ନିବେଶ, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        ଦଶମିକ *output = l.output + i*l.outputs;
        ପାଇଁ(j = 0; j < locations; ++j){
            ଦଶମିକ *a = l.weights + j*l.size*l.size*l.c*l.n;
            ଦଶମିକ *b = net.workspace + j;
            ଦଶମିକ *c = output + j;

            ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = l.n;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 1;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = l.size*l.size*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ଶୂନ୍ୟ backward_local_layer(local_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା locations = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ପାଇଁ(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    ପାଇଁ(i = 0; i < l.batch; ++i){
        ଦଶମିକ *ନିବେଶ = net.ନିବେଶ + i*l.w*l.h*l.c;
        im2col_cpu(ନିବେଶ, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        ପାଇଁ(j = 0; j < locations; ++j){ 
            ଦଶମିକ *a = l.delta + i*l.outputs + j;
            ଦଶମିକ *b = net.workspace + j;
            ଦଶମିକ *c = l.weight_updates + j*l.size*l.size*l.c*l.n;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = l.n;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = l.size*l.size*l.c;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        ଯଦି(net.delta){
            ପାଇଁ(j = 0; j < locations; ++j){ 
                ଦଶମିକ *a = l.weights + j*l.size*l.size*l.c*l.n;
                ଦଶମିକ *b = l.delta + i*l.outputs + j;
                ଦଶମିକ *c = net.workspace + j;

                ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = l.size*l.size*l.c;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 1;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

ଶୂନ୍ୟ update_local_layer(local_layer l, update_args a)
{
    ଦଶମିକ learning_rate = a.learning_rate*l.learning_rate_scale;
    ଦଶମିକ momentum = a.momentum;
    ଦଶମିକ decay = a.decay;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch = a.batch;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା locations = l.out_w*l.out_h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = l.size*l.size*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(size, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(size, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(size, momentum, l.weight_updates, 1);
}

#ifdef GPU

ଶୂନ୍ୟ forward_local_layer_gpu(ଅଚଳ local_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_h = local_out_height(l);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_w = local_out_width(l);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା locations = out_h * out_w;

    ପାଇଁ(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    ପାଇଁ(i = 0; i < l.batch; ++i){
        ଦଶମିକ *ନିବେଶ = net.ନିବେଶ_gpu + i*l.w*l.h*l.c;
        im2col_gpu(ନିବେଶ, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        ଦଶମିକ *output = l.output_gpu + i*l.outputs;
        ପାଇଁ(j = 0; j < locations; ++j){
            ଦଶମିକ *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
            ଦଶମିକ *b = net.workspace + j;
            ଦଶମିକ *c = output + j;

            ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = l.n;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 1;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = l.size*l.size*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ଶୂନ୍ୟ backward_local_layer_gpu(local_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା locations = l.out_w*l.out_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    ପାଇଁ(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    ପାଇଁ(i = 0; i < l.batch; ++i){
        ଦଶମିକ *ନିବେଶ = net.ନିବେଶ_gpu + i*l.w*l.h*l.c;
        im2col_gpu(ନିବେଶ, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        ପାଇଁ(j = 0; j < locations; ++j){ 
            ଦଶମିକ *a = l.delta_gpu + i*l.outputs + j;
            ଦଶମିକ *b = net.workspace + j;
            ଦଶମିକ *c = l.weight_updates_gpu + j*l.size*l.size*l.c*l.n;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = l.n;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = l.size*l.size*l.c;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        ଯଦି(net.delta_gpu){
            ପାଇଁ(j = 0; j < locations; ++j){ 
                ଦଶମିକ *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
                ଦଶମିକ *b = l.delta_gpu + i*l.outputs + j;
                ଦଶମିକ *c = net.workspace + j;

                ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = l.size*l.size*l.c;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 1;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

ଶୂନ୍ୟ update_local_layer_gpu(local_layer l, update_args a)
{
    ଦଶମିକ learning_rate = a.learning_rate*l.learning_rate_scale;
    ଦଶମିକ momentum = a.momentum;
    ଦଶମିକ decay = a.decay;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch = a.batch;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା locations = l.out_w*l.out_h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = l.size*l.size*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(size, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
    axpy_gpu(size, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(size, momentum, l.weight_updates_gpu, 1);
}

ଶୂନ୍ୟ pull_local_layer(local_layer l)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା locations = l.out_w*l.out_h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = l.size*l.size*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, size);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

ଶୂନ୍ୟ ଠେଲ_local_layer(local_layer l)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା locations = l.out_w*l.out_h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = l.size*l.size*l.c*l.n*locations;
    cuda_ଠେଲ_array(l.weights_gpu, l.weights, size);
    cuda_ଠେଲ_array(l.biases_gpu, l.biases, l.outputs);
}
#endif
