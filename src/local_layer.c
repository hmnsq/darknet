#ضمن "local_layer.h"
#ضمن "utils.h"
#ضمن "im2col.h"
#ضمن "col2im.h"
#ضمن "blas.h"
#ضمن "gemm.h"
#ضمن <stdio.h>
#ضمن <time.h>

صحيح local_out_height(local_layer l)
{
    صحيح h = l.h;
    إذا (!l.pad) h -= l.size;
    وإلا h -= 1;
    أرجع h/l.stride + 1;
}

صحيح local_out_width(local_layer l)
{
    صحيح w = l.w;
    إذا (!l.pad) w -= l.size;
    وإلا w -= 1;
    أرجع w/l.stride + 1;
}

local_layer make_local_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح size, صحيح stride, صحيح pad, ACTIVATION activation)
{
    صحيح i;
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

    صحيح out_h = local_out_height(l);
    صحيح out_w = local_out_width(l);
    صحيح locations = out_h*out_w;
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = calloc(c*n*size*size*locations, حجم(عائم));
    l.weight_updates = calloc(c*n*size*size*locations, حجم(عائم));

    l.biases = calloc(l.outputs, حجم(عائم));
    l.bias_updates = calloc(l.outputs, حجم(عائم));

    // float scale = 1./sqrt(size*size*c);
    عائم scale = sqrt(2./(size*size*c));
    لكل(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1,1);

    l.output = calloc(l.batch*out_h * out_w * n, حجم(عائم));
    l.delta  = calloc(l.batch*out_h * out_w * n, حجم(عائم));

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

    أرجع l;
}

فراغ forward_local_layer(ثابت local_layer l, network net)
{
    صحيح out_h = local_out_height(l);
    صحيح out_w = local_out_width(l);
    صحيح i, j;
    صحيح locations = out_h * out_w;

    لكل(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    لكل(i = 0; i < l.batch; ++i){
        عائم *تێخستن = net.تێخستن + i*l.w*l.h*l.c;
        im2col_cpu(تێخستن, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        عائم *output = l.output + i*l.outputs;
        لكل(j = 0; j < locations; ++j){
            عائم *a = l.weights + j*l.size*l.size*l.c*l.n;
            عائم *b = net.workspace + j;
            عائم *c = output + j;

            صحيح m = l.n;
            صحيح n = 1;
            صحيح k = l.size*l.size*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

فراغ backward_local_layer(local_layer l, network net)
{
    صحيح i, j;
    صحيح locations = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    لكل(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    لكل(i = 0; i < l.batch; ++i){
        عائم *تێخستن = net.تێخستن + i*l.w*l.h*l.c;
        im2col_cpu(تێخستن, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        لكل(j = 0; j < locations; ++j){ 
            عائم *a = l.delta + i*l.outputs + j;
            عائم *b = net.workspace + j;
            عائم *c = l.weight_updates + j*l.size*l.size*l.c*l.n;
            صحيح m = l.n;
            صحيح n = l.size*l.size*l.c;
            صحيح k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        إذا(net.delta){
            لكل(j = 0; j < locations; ++j){ 
                عائم *a = l.weights + j*l.size*l.size*l.c*l.n;
                عائم *b = l.delta + i*l.outputs + j;
                عائم *c = net.workspace + j;

                صحيح m = l.size*l.size*l.c;
                صحيح n = 1;
                صحيح k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

فراغ update_local_layer(local_layer l, update_args a)
{
    عائم learning_rate = a.learning_rate*l.learning_rate_scale;
    عائم momentum = a.momentum;
    عائم decay = a.decay;
    صحيح batch = a.batch;

    صحيح locations = l.out_w*l.out_h;
    صحيح size = l.size*l.size*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(size, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(size, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(size, momentum, l.weight_updates, 1);
}

#ifdef GPU

فراغ forward_local_layer_gpu(ثابت local_layer l, network net)
{
    صحيح out_h = local_out_height(l);
    صحيح out_w = local_out_width(l);
    صحيح i, j;
    صحيح locations = out_h * out_w;

    لكل(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    لكل(i = 0; i < l.batch; ++i){
        عائم *تێخستن = net.تێخستن_gpu + i*l.w*l.h*l.c;
        im2col_gpu(تێخستن, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        عائم *output = l.output_gpu + i*l.outputs;
        لكل(j = 0; j < locations; ++j){
            عائم *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
            عائم *b = net.workspace + j;
            عائم *c = output + j;

            صحيح m = l.n;
            صحيح n = 1;
            صحيح k = l.size*l.size*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

فراغ backward_local_layer_gpu(local_layer l, network net)
{
    صحيح i, j;
    صحيح locations = l.out_w*l.out_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    لكل(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    لكل(i = 0; i < l.batch; ++i){
        عائم *تێخستن = net.تێخستن_gpu + i*l.w*l.h*l.c;
        im2col_gpu(تێخستن, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        لكل(j = 0; j < locations; ++j){ 
            عائم *a = l.delta_gpu + i*l.outputs + j;
            عائم *b = net.workspace + j;
            عائم *c = l.weight_updates_gpu + j*l.size*l.size*l.c*l.n;
            صحيح m = l.n;
            صحيح n = l.size*l.size*l.c;
            صحيح k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        إذا(net.delta_gpu){
            لكل(j = 0; j < locations; ++j){ 
                عائم *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
                عائم *b = l.delta_gpu + i*l.outputs + j;
                عائم *c = net.workspace + j;

                صحيح m = l.size*l.size*l.c;
                صحيح n = 1;
                صحيح k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

فراغ update_local_layer_gpu(local_layer l, update_args a)
{
    عائم learning_rate = a.learning_rate*l.learning_rate_scale;
    عائم momentum = a.momentum;
    عائم decay = a.decay;
    صحيح batch = a.batch;

    صحيح locations = l.out_w*l.out_h;
    صحيح size = l.size*l.size*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(size, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
    axpy_gpu(size, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(size, momentum, l.weight_updates_gpu, 1);
}

فراغ pull_local_layer(local_layer l)
{
    صحيح locations = l.out_w*l.out_h;
    صحيح size = l.size*l.size*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, size);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

فراغ پاڵنان_local_layer(local_layer l)
{
    صحيح locations = l.out_w*l.out_h;
    صحيح size = l.size*l.size*l.c*l.n*locations;
    cuda_پاڵنان_array(l.weights_gpu, l.weights, size);
    cuda_پاڵنان_array(l.biases_gpu, l.biases, l.outputs);
}
#endif
