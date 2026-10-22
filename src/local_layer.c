#অন্তর্ভাব "local_layer.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "im2col.h"
#অন্তর্ভাব "col2im.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "gemm.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <time.h>

পূর্ণাঙ্ক local_out_height(local_layer l)
{
    পূর্ণাঙ্ক h = l.h;
    যদি (!l.pad) h -= l.size;
    অন্যথা h -= 1;
    প্রত্যাগম h/l.stride + 1;
}

পূর্ণাঙ্ক local_out_width(local_layer l)
{
    পূর্ণাঙ্ক w = l.w;
    যদি (!l.pad) w -= l.size;
    অন্যথা w -= 1;
    প্রত্যাগম w/l.stride + 1;
}

local_layer make_local_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক h, পূর্ণাঙ্ক w, পূর্ণাঙ্ক c, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক pad, ACTIVATION activation)
{
    পূর্ণাঙ্ক i;
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

    পূর্ণাঙ্ক out_h = local_out_height(l);
    পূর্ণাঙ্ক out_w = local_out_width(l);
    পূর্ণাঙ্ক locations = out_h*out_w;
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = calloc(c*n*size*size*locations, পরিমাণ(ভিন্ন));
    l.weight_updates = calloc(c*n*size*size*locations, পরিমাণ(ভিন্ন));

    l.biases = calloc(l.outputs, পরিমাণ(ভিন্ন));
    l.bias_updates = calloc(l.outputs, পরিমাণ(ভিন্ন));

    // float scale = 1./sqrt(size*size*c);
    ভিন্ন scale = sqrt(2./(size*size*c));
    প্রতি(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1,1);

    l.output = calloc(l.batch*out_h * out_w * n, পরিমাণ(ভিন্ন));
    l.delta  = calloc(l.batch*out_h * out_w * n, পরিমাণ(ভিন্ন));

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

    প্রত্যাগম l;
}

শূন্য forward_local_layer(ধ্রুব local_layer l, network net)
{
    পূর্ণাঙ্ক out_h = local_out_height(l);
    পূর্ণাঙ্ক out_w = local_out_width(l);
    পূর্ণাঙ্ক i, j;
    পূর্ণাঙ্ক locations = out_h * out_w;

    প্রতি(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    প্রতি(i = 0; i < l.batch; ++i){
        ভিন্ন *প্রবেশ = net.প্রবেশ + i*l.w*l.h*l.c;
        im2col_cpu(প্রবেশ, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        ভিন্ন *output = l.output + i*l.outputs;
        প্রতি(j = 0; j < locations; ++j){
            ভিন্ন *a = l.weights + j*l.size*l.size*l.c*l.n;
            ভিন্ন *b = net.workspace + j;
            ভিন্ন *c = output + j;

            পূর্ণাঙ্ক m = l.n;
            পূর্ণাঙ্ক n = 1;
            পূর্ণাঙ্ক k = l.size*l.size*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

শূন্য backward_local_layer(local_layer l, network net)
{
    পূর্ণাঙ্ক i, j;
    পূর্ণাঙ্ক locations = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    প্রতি(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    প্রতি(i = 0; i < l.batch; ++i){
        ভিন্ন *প্রবেশ = net.প্রবেশ + i*l.w*l.h*l.c;
        im2col_cpu(প্রবেশ, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        প্রতি(j = 0; j < locations; ++j){ 
            ভিন্ন *a = l.delta + i*l.outputs + j;
            ভিন্ন *b = net.workspace + j;
            ভিন্ন *c = l.weight_updates + j*l.size*l.size*l.c*l.n;
            পূর্ণাঙ্ক m = l.n;
            পূর্ণাঙ্ক n = l.size*l.size*l.c;
            পূর্ণাঙ্ক k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        যদি(net.delta){
            প্রতি(j = 0; j < locations; ++j){ 
                ভিন্ন *a = l.weights + j*l.size*l.size*l.c*l.n;
                ভিন্ন *b = l.delta + i*l.outputs + j;
                ভিন্ন *c = net.workspace + j;

                পূর্ণাঙ্ক m = l.size*l.size*l.c;
                পূর্ণাঙ্ক n = 1;
                পূর্ণাঙ্ক k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

শূন্য update_local_layer(local_layer l, update_args a)
{
    ভিন্ন learning_rate = a.learning_rate*l.learning_rate_scale;
    ভিন্ন momentum = a.momentum;
    ভিন্ন decay = a.decay;
    পূর্ণাঙ্ক batch = a.batch;

    পূর্ণাঙ্ক locations = l.out_w*l.out_h;
    পূর্ণাঙ্ক size = l.size*l.size*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(size, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(size, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(size, momentum, l.weight_updates, 1);
}

#ifdef GPU

শূন্য forward_local_layer_gpu(ধ্রুব local_layer l, network net)
{
    পূর্ণাঙ্ক out_h = local_out_height(l);
    পূর্ণাঙ্ক out_w = local_out_width(l);
    পূর্ণাঙ্ক i, j;
    পূর্ণাঙ্ক locations = out_h * out_w;

    প্রতি(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    প্রতি(i = 0; i < l.batch; ++i){
        ভিন্ন *প্রবেশ = net.প্রবেশ_gpu + i*l.w*l.h*l.c;
        im2col_gpu(প্রবেশ, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        ভিন্ন *output = l.output_gpu + i*l.outputs;
        প্রতি(j = 0; j < locations; ++j){
            ভিন্ন *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
            ভিন্ন *b = net.workspace + j;
            ভিন্ন *c = output + j;

            পূর্ণাঙ্ক m = l.n;
            পূর্ণাঙ্ক n = 1;
            পূর্ণাঙ্ক k = l.size*l.size*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

শূন্য backward_local_layer_gpu(local_layer l, network net)
{
    পূর্ণাঙ্ক i, j;
    পূর্ণাঙ্ক locations = l.out_w*l.out_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    প্রতি(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    প্রতি(i = 0; i < l.batch; ++i){
        ভিন্ন *প্রবেশ = net.প্রবেশ_gpu + i*l.w*l.h*l.c;
        im2col_gpu(প্রবেশ, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        প্রতি(j = 0; j < locations; ++j){ 
            ভিন্ন *a = l.delta_gpu + i*l.outputs + j;
            ভিন্ন *b = net.workspace + j;
            ভিন্ন *c = l.weight_updates_gpu + j*l.size*l.size*l.c*l.n;
            পূর্ণাঙ্ক m = l.n;
            পূর্ণাঙ্ক n = l.size*l.size*l.c;
            পূর্ণাঙ্ক k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        যদি(net.delta_gpu){
            প্রতি(j = 0; j < locations; ++j){ 
                ভিন্ন *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
                ভিন্ন *b = l.delta_gpu + i*l.outputs + j;
                ভিন্ন *c = net.workspace + j;

                পূর্ণাঙ্ক m = l.size*l.size*l.c;
                পূর্ণাঙ্ক n = 1;
                পূর্ণাঙ্ক k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

শূন্য update_local_layer_gpu(local_layer l, update_args a)
{
    ভিন্ন learning_rate = a.learning_rate*l.learning_rate_scale;
    ভিন্ন momentum = a.momentum;
    ভিন্ন decay = a.decay;
    পূর্ণাঙ্ক batch = a.batch;

    পূর্ণাঙ্ক locations = l.out_w*l.out_h;
    পূর্ণাঙ্ক size = l.size*l.size*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(size, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
    axpy_gpu(size, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(size, momentum, l.weight_updates_gpu, 1);
}

শূন্য pull_local_layer(local_layer l)
{
    পূর্ণাঙ্ক locations = l.out_w*l.out_h;
    পূর্ণাঙ্ক size = l.size*l.size*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, size);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

শূন্য ঠেলো_local_layer(local_layer l)
{
    পূর্ণাঙ্ক locations = l.out_w*l.out_h;
    পূর্ণাঙ্ক size = l.size*l.size*l.c*l.n*locations;
    cuda_ঠেলো_array(l.weights_gpu, l.weights, size);
    cuda_ঠেলো_array(l.biases_gpu, l.biases, l.outputs);
}
#endif
